/*
 * NexHardware.c
 *
 *  Created on: Jun 28, 2021
 *      Author: Andhie Setyabudi
 */

#include "NexHardware.h"

NexHardware_func NexHardware = {
		.UART_	= NULL,
		.init 	= nex_HWInit,
		.loop	= nex_HWLoop,
};


/*
 * Receive uint32_t data.
 *
 * @param number - save uint32_t data.
 * @param timeout - set timeout time.
 *
 * @retval true - success.
 * @retval false - failed.
 *
 */
bool nex_recvRetNumber(uint32_t *number, uint32_t timeout)
{
	if ( NexHardware.UART_ == NULL )
		return false;
	if ( !number )
		return false;
    uint8_t temp[8] = {0};

    if ( HAL_UART_Receive(NexHardware.UART_, temp, 8, timeout) == HAL_OK )
    {
    	if ( temp[0] == NEX_RET_NUMBER_HEAD && temp[5] == 0xFF && temp[6] == 0xFF && temp[7] == 0xFF )
		{
			*number = ((uint32_t)temp[4] << 24) | ((uint32_t)temp[3] << 16) | (temp[2] << 8) | (temp[1]);
			return true;
		}
    }
    return false;
}

/*
 * Receive string data.
 *
 * @param buffer - save string data.
 * @param len - string buffer length.
 * @param timeout - set timeout time.
 *
 * @return the length of string buffer.
 *
 */
uint16_t nex_recvRetString(char *buffer, uint16_t len, uint32_t timeout)
{
	// check for status serial UART
	if ( NexHardware.UART_ == NULL )
		return false;
	// check for buffer len
	if (!buffer || len == 0)
		return 0;

	uint16_t ret = 0;
    bool str_start_flag = false;
    uint8_t cnt_0xff = 0;
    char temp[100] = "";
    uint8_t c = 0;
    uint32_t start_ = HAL_GetTick();
    while (HAL_GetTick() - start <= timeout)
    {
        while (nexSerial.available())
        {
            c = nexSerial.read();
            if (str_start_flag)
            {
                if (0xFF == c)
                {
                    cnt_0xff++;
                    if (cnt_0xff >= 3)
                    {
                        break;
                    }
                }
                else
                {
                    temp += (char)c;
                }
            }
            else if (NEX_RET_STRING_HEAD == c)
            {
                str_start_flag = true;
            }
        }

        if (cnt_0xff >= 3)
        {
            break;
        }
    }

    ret = temp.length();
    ret = ret > len ? len : ret;
    strncpy(buffer, temp.c_str(), ret);

__return:

    dbSerialPrint("recvRetString[");
    dbSerialPrint(temp.length());
    dbSerialPrint(",");
    dbSerialPrint(temp);
    dbSerialPrintln("]");

    return ret;
}

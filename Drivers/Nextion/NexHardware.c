/*
 * NexHardware.c
 *
 *  Created on: Jun 28, 2021
 *      Author: Andhie Setyabudi
 */

#include "NexHardware.h"

NexHardware_func NexHardware = {
		.nex_serial	= NULL,
		.delay	= NULL,
		.init 	= nex_HWInit,
		.loop	= nex_HWLoop,

		.recvRetNumber	=	nex_recvRetNumber,
		.recvRetString	= 	nex_recvRetString,
		.sendCommand	= 	nex_sendCommand,
		.recvRetCommandFinished = nex_recvRetCommandFinished,
};

void nex_halt(uint32_t t)
{
	if(NexHardware.delay == NULL)
		HAL_Delay(t);
	else
		NexHardware.delay(t);
}

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
	if  ( NexHardware.nex_serial == NULL )
		return false;
	uint8_t temp[8] = {0};
	if (!number)
		return false;
	NexHardware.nex_serial->setTimeout(timeout);
	if (sizeof(temp) != NexHardware.nex_serial->readBytes((char *)temp, sizeof(temp)))
		return false;

	if (temp[0] == NEX_RET_NUMBER_HEAD && temp[5] == 0xFF && temp[6] == 0xFF && temp[7] == 0xFF)
	{
		*number = ((uint32_t)temp[4] << 24) | ((uint32_t)temp[3] << 16) | (temp[2] << 8) | (temp[1]);
		return true;
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
	if  ( NexHardware.nex_serial == NULL )
		return 0;
	if (!buffer || len == 0)
		return 0;
	uint16_t ret = 0;
	bool str_start_flag = false;
	uint8_t cnt_0xff = 0;
	char temp[MAX_BUFF_COUNT];
	uint8_t c = 0;
	long start;

	uint8_t id_tmp = 0;
	start = HAL_GetTick();
	while (HAL_GetTick() - start <= timeout)
	{
		while (NexHardware.nex_serial->available())
		{
			c = NexHardware.nex_serial->read();
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
					temp[id_tmp] = (char) c;
					id_tmp++;
//					temp+=1;
//					temp += (char)c;
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

	ret = strlen(temp);
	ret = ret > len ? len : ret;
	strncpy(buffer, temp, ret);
	return ret;
}

/*
 * Send command to Nextion.
 *
 * @param cmd - the string of command.
 */
void nex_sendCommand(const char* cmd)
{
//	if  ( NexHardware.nex_serial == NULL )
//		return;
	while (NexHardware.nex_serial->available())
	{
		NexHardware.nex_serial->read();
	}
	NexHardware.nex_serial->puts(cmd, strlen(cmd));
	USBSerial.print("%s\r\n",cmd);
	NexHardware.nex_serial->write(0xFF);
	NexHardware.nex_serial->write(0xFF);
	NexHardware.nex_serial->write(0xFF);
}


/*
 * Command is executed successfully.
 *
 * @param timeout - set timeout time.
 *
 * @retval true - success.
 * @retval false - failed.
 *
 */
bool nex_recvRetCommandFinished(uint32_t timeout)
{
	bool ret = false;
	uint8_t temp[4] = {0};

	NexHardware.nex_serial->setTimeout(timeout);
	if (4 != NexHardware.nex_serial->readBytes(temp, sizeof(temp)))
		return false;
	USBSerial.puts(temp, sizeof(temp));
	if (temp[0] == NEX_RET_CMD_FINISHED	&& temp[1] == 0xFF && temp[2] == 0xFF && temp[3] == 0xFF)
		return true;
	return false;
}

bool nex_HWInit(void)
{
    bool ret1 = false;
    bool ret2 = false;
    nex_sendCommand("");
    nex_sendCommand("bkcmd=1");
    ret1 = nex_recvRetCommandFinished(100UL);
    nex_sendCommand("page 0");
    ret2 = nex_recvRetCommandFinished(100UL);
    return ret1 && ret2;
}


void nex_HWLoop(NexObject_var *nex_listen_list[])
{


	static uint8_t __buffer[10];

	    uint16_t i;
	    uint8_t c;

	    while (NexHardware.nex_serial->available() > 0)
	    {
	    	nex_halt(10);
	        c = NexHardware.nex_serial->read();
	        USBSerial.print("%02X",c);
	        if (NEX_RET_EVENT_TOUCH_HEAD == c)
	        {
	            if (NexHardware.nex_serial->available() >= 6)
	            {
	                __buffer[0] = c;
	                for (i = 1; i < 7; i++)
	                {
	                    __buffer[i] = NexHardware.nex_serial->read();
	                    USBSerial.print("%02X",__buffer[i]);
	                }
	                __buffer[i] = 0x00;

	                if (0xFF == __buffer[4] && 0xFF == __buffer[5] && 0xFF == __buffer[6])
	                {
	                	NexObject.iterate(nex_listen_list, __buffer[1], __buffer[2], (int32_t)__buffer[3]);
	                }

	            }
	        }
	    }

//    static uint8_t __buffer[10];
//    uint8_t c;
//    if ( HAL_UART_Receive(NexHardware.UART_, &c, 1, 100) == HAL_OK )
//    {
//    	nex_halt(10);
//		if (NEX_RET_EVENT_TOUCH_HEAD == c)
//		{
//			if ( HAL_UART_Receive(NexHardware.UART_, __buffer, 10, 100) == HAL_OK )
//			{
//				if (0xFF == __buffer[3] && 0xFF == __buffer[4] && 0xFF == __buffer[5])
//
//			};
//		};
//    };
}

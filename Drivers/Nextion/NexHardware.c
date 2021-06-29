/*
 * NexHardware.c
 *
 *  Created on: Jun 28, 2021
 *      Author: Andhie Setyabudi
 */

#include "NexHardware.h"

NexHardware_func NexHardware = {
		.UART_	= NULL,
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
    char temp[MAX_BUFF_COUNT];
    memset(temp, '\0', MAX_BUFF_COUNT);
    memset(buffer, '\0', len);


    uint32_t start_ = HAL_GetTick();
    while (HAL_GetTick() - start_ <= timeout)
    {
    	HAL_UART_Receive(NexHardware.UART_, (uint8_t*)temp, MAX_BUFF_COUNT, timeout);
    	if ( strlen(temp) > 0 )
    	{
    		uint8_t loc = 0;
    		while( loc < MAX_BUFF_COUNT && temp[loc] != NULL )
    		{
    			uint8_t c = temp[loc];
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
						*buffer= (char)c;
						buffer++;
					}
				}
				else if (NEX_RET_STRING_HEAD == c)
				{
					str_start_flag = true;
				}
    			loc++;
    		}
    	}
    	if (cnt_0xff >= 3)
		{
			break;
		}
    }
    ret = (uint16_t) strlen(buffer);
    return ret;
}

/*
 * Send command to Nextion.
 *
 * @param cmd - the string of command.
 */
void nex_sendCommand(const char* cmd)
{
	size_t len = strlen(cmd);
	uint8_t buffer[len+1];
    HAL_UART_Receive(NexHardware.UART_, buffer, len+1, 100);
    sprintf(buffer,"%s",cmd);
    HAL_UART_Transmit(NexHardware.UART_, buffer, strlen(buffer), 1000UL);
    buffer[0] = 0xFF;
    buffer[1] = 0xFF;
    buffer[2] = 0xFF;
    HAL_UART_Transmit(NexHardware.UART_, buffer, 3, 1000UL);
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

    if( HAL_UART_Receive(NexHardware.UART_, temp, 4, timeout) != HAL_OK )
    	ret = false;

    if (temp[0] == NEX_RET_CMD_FINISHED && temp[1] == 0xFF && temp[2] == 0xFF && temp[3] == 0xFF)
        ret = true;
    return ret;
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
    uint8_t c;
    if ( HAL_UART_Receive(NexHardware.UART_, &c, 1, 100) == HAL_OK )
    {
    	nex_halt(10);
		if (NEX_RET_EVENT_TOUCH_HEAD == c)
		{
			if ( HAL_UART_Receive(NexHardware.UART_, __buffer, 10, 100) == HAL_OK )
			{
				if (0xFF == __buffer[3] && 0xFF == __buffer[4] && 0xFF == __buffer[5])
					NexObject.iterate(nex_listen_list, __buffer[0], __buffer[1], (int32_t)__buffer[2]);
			};
		};
    };
}

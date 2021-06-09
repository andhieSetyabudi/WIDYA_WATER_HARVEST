/*
 * E32_915T30.c
 *
 *  Created on : Jun 3, 2021
 *      Author : Andhie Setyabudi
 *      e-mail : Andhie.13@gmail.com
 *      Github : andhieSetyabudi
 */

#include "E32_915T30.h"

static uint8_t baudrate_map(uint32_t baudrate_value)
{
	uint ret = DEFAULT_DATA_RATE;
	switch( baudrate_value)
	{
		case 300 	:
			ret = DATA_RATE_300;
			break;
		case 1200 	:
			ret = DATA_RATE_1200;
			break;
		case 2400	:
			ret = DATA_RATE_2400;
			break;
		case 9600   :
			ret = DATA_RATE_9600;
			break;
		case 19200  :
			ret = DATA_RATE_19200;
			break;
		default :
			ret = 255;
			break;
	}
	return ret;
}

static EByte_status UART_SETUP(UART_HandleTypeDef* uart_line, uint8_t baud, uint8_t parity_)
{

	if ( uart_line->Instance != NULL )		// has been set by generator-code
	{
		if (uart_line->Init.WordLength == UART_WORDLENGTH_8B && uart_line->Init.StopBits != UART_STOPBITS_1 )
		{
			E32_915T30.uart_line = uart_line;
			E32_915T30.baudrate_uart = baudrate_map(uart_line->Init.BaudRate);
			switch( uart_line->Init.Parity)
			{
				case UART_PARITY_EVEN :
					E32_915T30.parity_data = PARITY_8E1;
					break;
				case UART_PARITY_ODD  :
					E32_915T30.parity_data = PARITY_8O1;
					break;
				default :
				case UART_PARITY_NONE :
					E32_915T30.parity_data = PARITY_8N1;
					break;
			}
			return EByte_OK;
		}
	}
	return EByte_ERROR;
}


Ebyte_func E32_915T30;

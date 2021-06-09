/*
 * E32_915T30.h
 *
 *  Created on : Jun 3, 2021
 *      Author : Andhie Setyabudi
 *      e-mail : Andhie.13@gmail.com
 *      Github : andhieSetyabudi
 */

#ifndef E32_915T30D_E32_915T30_H_
#define E32_915T30D_E32_915T30_H_

#include "main.h"

#ifdef STM32F4
	#include "stm32f4xx_hal.h"
#elif STM32L4
	#include "stm32l4xx_hal.h"
#endif

#include <stddef.h>

#define NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, TOTAL, ...) TOTAL
#define NUM_ARGS(...) NUM_ARGS_(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define CONCATE_(X, Y) X##Y
#define CONCATE(MACRO, NUMBER) CONCATE_(MACRO, NUMBER)
#define VA_MACRO(MACRO, ...) CONCATE(MACRO, NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

#include "stdio.h"
#include "BASIC_LOGIC.h"
#include "micro_delay.h"

/* Status Reply */
typedef enum
{
	EByte_OK       = 0x00U,
	EByte_ERROR    = 0x01U,
	EByte_BUSY     = 0x02U,
	EByte_TIMEOUT  = 0x03U
} EByte_status;

/* Baudrate list setting */
#define AUTO_BAUD		0
#define BAUD_1200		1
#define BAUD_2400		2
#define BAUD_4800		3
#define BAUD_9600		4		// as default from Ebyte
#define BAUD_19200		5
#define BAUD_38400		6
#define BAUD_57600		7
#define BAUD_115200		8

#define DEFAULT_BAUD	BAUD_9600

/* Parity data bit */
#define PARITY_8N1			0
#define PARITY_8O1			1
#define PARITY_8E1			2
#define DEFAULT_PARITY		PARITY_8N1


/* Air Data Rate (bps) */
/* The lower bps is better for longer distance */
#define DATA_RATE_300		0
#define DATA_RATE_1200		1
#define DATA_RATE_2400		2	// as default
#define DATA_RATE_4800		3
#define DATA_RATE_9600		4
#define DATA_RATE_19200		5

#define DEFAULT_DATA_RATE	DATA_RATE_2400

/* Wireless Wake Up time*/
#define WUT_250ms		0
#define WUT_500ms		1
#define WUT_750ms		2
#define WUT_1000ms		3
#define WUT_1250ms		4
#define WUT_1500ms		5
#define WUT_1750ms		6
#define WUT_2000ms		7

#define DEFAULT_WUT		WUT_250ms

/* Transmission Power */
#define PWR_30dBm		0
#define PWR_27dBm		1
#define PWR_24dBm		2
#define PWR_21dBm		3

#define DEFAULT_POWER_TRANSMISSION	PWR_30dBm

/* Option I/O pin  for  AUX, TXD, RXD*/
#define OPT_IO_OPENDRAIN	0x00
#define OPT_IO_PUSHPULL		0x01

// modes NORMAL send and recieve for example
#define MODE_NORMAL 	0			// can send and recieve
#define MODE_WAKEUP 	1			// sends a preamble to waken receiver
#define MODE_POWERDOWN 	2			// can't transmit but receive works only in wake up mode
#define MODE_PROGRAM 	3			// for programming



typedef struct {
	PORT_IO* M1_pin,
		     M2_pin,
			 AUX_pin;
}Ebyte_var;

static void __inline__ setPin(PORT_IO * var, PORT_IO *portmap)
{
	var = portmap;
}

static uint8_t __inline__ getAux(Ebyte_var var)
{
//	HAL_Delay(Delay)
	return digitalRead(var.AUX_pin);
}


static EByte_status UART_SETUP(UART_HandleTypeDef* uart_line, uint8_t baud, uint8_t parity_);
#define UART_SETUP_1(s, t, a)		UART_SETUP(a, DEFAULT_BAUD, DEFAULT_PARITY)
#define UART_SETUP_2(s,t,a,b)		UART_SETUP(a, b, DEFAULT_PARITY)
#define UART_SETUP_3(s,t,a,b,c)		UART_SETUP(a, b, c)
#define UART_SETUP(...) 			VA_MACRO(UART_SETUP_, void, void, __VA_ARGS__)


typedef struct __attribute__ ((__packed__)){
	UART_HandleTypeDef* uart_line;
	uint8_t parity_data,
			baudrate_uart,
			mode;
// basic for peripheral or I/O line
	uint32_t 		(*millis)(void);
	void 			(*halt)			(uint32_t how_long);
	EByte_status	(*initUART) 	(UART_HandleTypeDef* uart_line, uint8_t baud, uint8_t parity_);
	void 			(*setPin_M1)    (Ebyte_var *var, PORT_IO portmap);
	void 			(*setPin_M0)	(Ebyte_var *var, PORT_IO portmap);
	void 			(*setPin_AUX)	(Ebyte_var *var, PORT_IO portmap);
	EByte_status	(*initHardware)	(UART_HandleTypeDef* uart_line, uint8_t baud, uint8_t parity_,
								 	 Ebyte_var *var, PORT_IO pinM1, PORT_IO pinM0, PORT_IO pinAUX);

// basic function
	EByte_status (*setup)			(Ebyte_var* var, uint8_t rate, uint8_t wu_time, uint8_t rf_pwr, uint8_t mode_);
}Ebyte_func;

extern Ebyte_func E32_915T30;


// simplifying methods

// initialize serial / UART


// initialize Hardware ; UART and I/O pins
#define initHardware_1(s,t,a,b)			initHardware(a, DEFAULT_BAUD, DEFAULT_PARITY, b, NULL, NULL, NULL)
#define initHardware_2(s,t,a,b,c )		initHardware(a, b, DEFAULT_PARITY, c, NULL, NULL, NULL)
#define initHardware(...)				VA_MACRO(initHardware_, void, void, __VA_ARGS__)

#define setup_1(s, t, a) 		    setup(a, DEFAULT_DATA_RATE, DEFAULT_WUT, DEFAULT_POWER_TRANSMISSION, MODE_NORMAL)
#define setup_2(s, t, a, b) 	    setup(a, b, DEFAULT_WUT, DEFAULT_POWER_TRANSMISSION, MODE_NORMAL)
#define setup_3(s, t, a, b, c) 	    setup(a, b, c, DEFAULT_POWER_TRANSMISSION, MODE_NORMAL)
#define setup_4(s, t, a, b, c, d)   setup(a, b, c, d, MODE_NORMAL)
#define setup(...) 					VA_MACRO(setup_, void, void, __VA_ARGS__)


#endif /* E32_915T30D_E32_915T30_H_ */



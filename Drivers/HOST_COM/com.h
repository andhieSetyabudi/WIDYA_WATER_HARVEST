/*
 * com.h
 *
 *  Created on: Jun 15, 2021
 *      Author: andhie setyabudi
 */

#ifndef HOST_COM_COM_H_
#define HOST_COM_COM_H_

/* USER CODE BEGIN Header */
#define UART1_Start 			0x53
#define UART1_End 				0x5C

//Power CMD
#define UART1_CMD_SLEEP 		0x78
#define UART1_CMD_RESTART 		0x87
#define UART1_CMD_MAIN_ON 		0x58

//Manufacturing CMD
#define UART1_CMD_SN_RD 		0xC5
#define UART1_CMD_UUID_RD 		0x86
#define UART1_CMD_SN_WR 		0x5C
#define UART1_CMD_FIRMWARE_VER 	0x35
#define UART1_CMD_FIRMWARE 		0x3A		// transfer firmwae
#define UART1_CMD_CALIB_RD 		0x56		// read all calibration result
#define UART1_CMD_SESSION_START 0x75
#define UART1_CMD_SESSION_END 	0x57


#endif /* HOST_COM_COM_H_ */

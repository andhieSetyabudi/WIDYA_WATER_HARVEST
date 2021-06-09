/*
 * micro_delay.h
 *
 *  Created on : Jun 3, 2021
 *      Author : Andhie Setyabudi
 *      e-mail : Andhie.13@gmail.com
 *      Github : andhieSetyabudi
 */

#ifndef BASIC_IO_MICRO_DELAY_H_
#define BASIC_IO_MICRO_DELAY_H_

#include "main.h"
#include "stdio.h"
#include "stm32_hal_legacy.h"

#ifndef DWT_DELAY_EX
	#define DWT_DELAY_EX 1
#endif
/* Status Reply */
typedef enum
{
	DWTDelay_OK       = 0x00U,
	DWTDelay_ERROR    = 0x01U,
} DWTdelay_status;

DWTdelay_status  delay_microsInit(void);
void delay_microSeconds(uint32_t us);

#endif /* BASIC_IO_MICRO_DELAY_H_ */

/*
 * soft_i2c.h
 *
 *  Created on : Jun 3, 2021
 *      Author : Andhie Setyabudi
 *      e-mail : Andhie.13@gmail.com
 *      Github : andhieSetyabudi
 */

#ifndef SOFT_I2C_SOFT_I2C_H_
#define SOFT_I2C_SOFT_I2C_H_

#include "main.h"

#ifdef STM32F4
	#include "stm32f4xx_hal.h"
#elif STM32L4
	#include "stm32l4xx_hal.h"
#endif

#include "stdio.h"
#include "stddef.h"
#include "BASIC_LOGIC.h"
#include "micro_delay.h"


#define NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, TOTAL, ...) TOTAL
#define NUM_ARGS(...) NUM_ARGS_(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define CONCATE_(X, Y) X##Y
#define CONCATE(MACRO, NUMBER) CONCATE_(MACRO, NUMBER)
#define VA_MACRO(MACRO, ...) CONCATE(MACRO, NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

#endif /* SOFT_I2C_SOFT_I2C_H_ */

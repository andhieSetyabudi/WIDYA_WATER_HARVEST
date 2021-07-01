/*
 * HX710B.h
 *
 *  Created on : Jun 3, 2021
 *      Author : Andhie Setyabudi
 *      e-mail : Andhie.13@gmail.com
 *      Github : andhieSetyabudi
 */

#ifndef HX710B_HX710B_H_
#define HX710B_HX710B_H_

#include "main.h"

#ifdef STM32F4
	#include "stm32f4xx_hal.h"
#elif STM32L4
	#include "stm32l4xx_hal.h"
#endif


#include "stdio.h"
#include "stddef.h"
#include "BASIC_LOGIC.h"


#define NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, TOTAL, ...) TOTAL
#define NUM_ARGS(...) NUM_ARGS_(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define CONCATE_(X, Y) X##Y
#define CONCATE(MACRO, NUMBER) CONCATE_(MACRO, NUMBER)
#define VA_MACRO(MACRO, ...) CONCATE(MACRO, NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

#define HX710B_hlt			asm("NOP")
#define HX710B_timeout_		500U

#define MODE_DIFF1		25		// data rate 10Hz
#define MODE_TEMP		26		// data rate 40Hz
#define MODE_DIFF2		27		// data rate 40Hz	-> would be default

/* Status Reply */
typedef enum
{
	HX710B_OK       = 0x00U,
	HX710B_ERROR    = 0x01U,
	HX710B_BUSY     = 0x02U,
	HX710B_TIMEOUT  = 0x03U
} HX710B_status;

typedef struct {
	PORT_IO SCI, DO;
	uint8_t mode;
}HX710B_var;




static uint8_t HX710BIsReady(HX710B_var *var, uint32_t timeout_);

#define HX710BIsReady_1(s,t,a)		HX710BIsReady(a,HX710B_timeout_)
#define HX710BIsReady_2(s,t,a,b)	HX710BIsReady(a,b)
#define HX710BIsReady(...)			VA_MACRO(HX710BIsReady_,void, void, __VA_ARGS__)

static HX710B_status HX710BRead(HX710B_var *var, int * data, uint32_t timeout_);
#define HX710BRead_1(s,t,a)		HX710BRead(a,NULL,HX710B_timeout_)
#define HX710BRead_2(s,t,a,b)	HX710BRead(a,b,HX710B_timeout_)
#define HX710BRead_3(s,t,a,b,c)	HX710BRead(a,b,c)
#define HX710BRead(...)			VA_MACRO(HX710BRead_, void, void,__VA_ARGS__)

static HX710B_status HX710BSetMode(HX710B_var *var, uint8_t mode, uint8_t timeout_);
#define HX710BSetMode_1(s,t,a)		HX710BSetMode(a,MODE_DIFF2,HX710B_timeout_)
#define HX710BSetMode_2(s,t,a,b)	HX710BSetMode(a,b,HX710B_timeout_)
#define HX710BSetMode_3(s,t,a,b,c)	HX710BSetMode(a,b,c)
#define HX710BSetMode(...)			VA_MACRO(HX710BSetMode_, void, void,__VA_ARGS__)

static HX710B_status HX710BInit(HX710B_var* var, PORT_IO SCI, PORT_IO SDO, uint8_t mode_);
#define HX710BInit_3(s,t,a,b,c)		HX710BInit(a,b,c,MODE_DIFF2)
#define HX710BInit_4(s,t,a,b,c,d)	HX710BInit(a,b,c,d)
#define HX710BInit(...)			VA_MACRO(HX710BInit_, void, void,__VA_ARGS__)

typedef struct __attribute__ ((__packed__)){
	void (*halt)				(uint32_t time_halt);
	HX710B_status	(*init) 	(HX710B_var* var, PORT_IO SCI, PORT_IO SDO, uint8_t mode_);
	HX710B_status 	(*setMode)  (HX710B_var *var, uint8_t mode, uint8_t timeout_);
	HX710B_status	(*read)		(HX710B_var *var, uint32_t * data, uint32_t timeout_);
	HX710B_status	(*isReady)	(HX710B_var *var, uint32_t timeout_);
}HX710B_funct;

//#define init_3(s,t,a,b,c)		init(a,b,c,MODE_DIFF2)
//#define init_4(s,t,a,b,c,d)		init(a,b,c,d)
//#define init(...)				VA_MACRO(init_, void, void,__VA_ARGS__)

extern HX710B_funct HX710B;


#endif /* HX710B_HX710B_H_ */

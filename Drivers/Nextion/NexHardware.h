/*
 * NexHardware.h
 *
 *  Created on: Jun 28, 2021
 *      Author: Andhie Setyabudi
 */

#ifndef NEXTION_NEXHARDWARE_H_
#define NEXTION_NEXHARDWARE_H_

#include "main.h"
#include "NexObject.h"
#include "stdbool.h"
#include "stddef.h"

#define NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, TOTAL, ...) TOTAL
#define NUM_ARGS(...) NUM_ARGS_(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define CONCATE_(X, Y) X##Y
#define CONCATE(MACRO, NUMBER) CONCATE_(MACRO, NUMBER)
#define VA_MACRO(MACRO, ...) CONCATE(MACRO, NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

#define NEX_RET_CMD_FINISHED            	(0x01)
#define NEX_RET_EVENT_LAUNCHED          	(0x88)
#define NEX_RET_EVENT_UPGRADED          	(0x89)
#define NEX_RET_EVENT_TOUCH_HEAD            (0x65)
#define NEX_RET_EVENT_POSITION_HEAD         (0x67)
#define NEX_RET_EVENT_SLEEP_POSITION_HEAD   (0x68)
#define NEX_RET_CURRENT_PAGE_ID_HEAD        (0x66)
#define NEX_RET_STRING_HEAD                 (0x70)
#define NEX_RET_NUMBER_HEAD                 (0x71)
#define NEX_RET_INVALID_CMD             	(0x00)
#define NEX_RET_INVALID_COMPONENT_ID    	(0x02)
#define NEX_RET_INVALID_PAGE_ID         	(0x03)
#define NEX_RET_INVALID_PICTURE_ID      	(0x04)
#define NEX_RET_INVALID_FONT_ID         	(0x05)
#define NEX_RET_INVALID_BAUD            	(0x11)
#define NEX_RET_INVALID_VARIABLE        	(0x1A)
#define NEX_RET_INVALID_OPERATION       	(0x1B)
//
//typedef struct{
//	UART_HandleTypeDef* UART_;
//}NexHardware_var;


/**
 * Init Nextion.
 *
 * @return true if success, false for failure.
 */
static bool nex_HWInit(void);

/**
 * Listen touch event and calling callbacks attached before.
 *
 * Supports push and pop at present.
 *
 * @param nex_listen_list - index to Nextion Components list.
 * @return none.
 *
 * @warning This function must be called repeatedly to response touch events
 *  from Nextion touch panel. Actually, you should place it in your loop function.
 */
static void nex_HWLoop(NexObject_var *nex_listen_list[]);


/**
 * @}
 */

static bool nex_recvRetNumber(uint32_t *number, uint32_t timeout);
static uint16_t nex_recvRetString(char *buffer, uint16_t len, uint32_t timeout);
static void sendCommand(const char* cmd);
static bool recvRetCommandFinished(uint32_t timeout);

typedef struct{
	UART_HandleTypeDef* UART_;
	bool (*init)	(void);
	void (*loop)	(NexObject_var *nex_listen_list[]);

}NexHardware_func;

extern NexHardware_func NexHardware;

#endif /* NEXTION_NEXHARDWARE_H_ */

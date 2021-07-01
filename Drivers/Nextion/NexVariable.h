/*
 * NexVariable.h
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 *      andhie.13@gmail.com
 */

#ifndef NEXTION_NEXVARIABLE_H_
#define NEXTION_NEXVARIABLE_H_

#include "NexHardware.h"
#include "NexButton.h"

/**
* Get val attribute of component
*
* @param number - buffer storing data retur
* @return the length of the data
*/
static uint32_t NexVariable_getValue(NexObject_var *obj, uint32_t *number);

/**
* Set val attribute of component
*
* @param number - To set up the data
* @return true if success, false for failure
*/
static bool NexVariable_setValue(NexObject_var *obj, uint32_t number);

typedef struct{
	void (*create)		(NexObject_var *obj, uint8_t pid, uint8_t cid, const char *name);
	uint16_t (*getText)	(NexObject_var *obj, char *buffer, uint16_t len);
	bool (*setText)		(NexObject_var *obj, uint32_t number);
	uint32_t (*getValue)(NexObject_var *obj, uint32_t *number);

}NexVariable_func;

extern NexVariable_func NexVariable;

#endif /* NEXTION_NEXVARIABLE_H_ */

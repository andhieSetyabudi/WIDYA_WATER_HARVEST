/*
 * NexPage.h
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 */

#ifndef NEXTION_NEXPAGE_H_
#define NEXTION_NEXPAGE_H_

#include "NexObject.h"
#include "NexHardware.h"
#include "NexButton.h"

bool NexPage_show(NexObject_var * var);

typedef struct{
	void (*create)	(NexObject_var *obj, uint8_t pid, uint8_t cid, const char *name);
	bool (*show)    (NexObject_var * var);
}NexPage_func;

extern NexPage_func NexPage;


#endif /* NEXTION_NEXPAGE_H_ */

/*
 * NexObject.c
 *
 *  Created on: Jun 28, 2021
 *      Author: Andhie Setyabudi
 */

#include "NexObject.h"


NexObject_funct NexObject = {
		.create		=nex_createObject,
		.getObjPid 	= nex_getObjPid,
		.getObjCid 	= nex_getObjCid,
		.getObjName = nex_getObjName,
};

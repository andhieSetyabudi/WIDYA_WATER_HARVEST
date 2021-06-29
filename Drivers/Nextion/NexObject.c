/*
 * NexObject.c
 *
 *  Created on: Jun 28, 2021
 *      Author: Andhie Setyabudi
 */

#include "NexObject.h"



void nex_attachPush(NexObject_var * this, NexTouchEventCb push, void *ptr)
{
    this->__cb_push = push;
    this->__cbpush_ptr = ptr;
}

void nex_detachPush(NexObject_var * this)
{
    this->__cb_push = NULL;
    this->__cbpush_ptr = NULL;
}

void nex_attachPop(NexObject_var * this, NexTouchEventCb pop, void *ptr)
{
    this->__cb_pop = pop;
    this->__cbpop_ptr = ptr;
}

void nex_detachPop(NexObject_var * this)
{
    this->__cb_pop = NULL;
    this->__cbpop_ptr = NULL;
}

void nex_push(NexObject_var * this)
{
    if (this->__cb_push != NULL)
    {
        this->__cb_push(this->__cbpush_ptr);
    }
}

void nex_pop(NexObject_var * this)
{
    if (this->__cb_pop != NULL)
    {
    	this->__cb_pop(this->__cbpop_ptr);
    }
}

void nex_iterate(NexObject_var **list, uint8_t pid, uint8_t cid, int32_t event)
{
	NexObject_var *e = NULL;
    uint16_t i = 0;

    if (NULL == list)
    {
        return;
    }

    for(i = 0; (e = list[i]) != NULL; i++)
    {
        if (e->__pid == pid && e->__cid == cid)
        {
            if (NEX_EVENT_PUSH == event)
            {
            	nex_push(e);
//                e->push();
            }
            else if (NEX_EVENT_POP == event)
            {
            	nex_pop(e);
//                e->pop();
            }

            break;
        }
    }
}



NexObject_funct NexObject = {
		.create		=nex_createObject,
		.getObjPid 	= nex_getObjPid,
		.getObjCid 	= nex_getObjCid,
		.getObjName = nex_getObjName,

		.attachPush	=	nex_attachPush,
		.detachPush = 	nex_detachPush,
		.attachPop	= 	nex_attachPop,
		.detachPop	= 	nex_detachPop,
		.push		= 	nex_push,
		.pop		= 	nex_pop,
		.iterate	= 	nex_iterate,
};

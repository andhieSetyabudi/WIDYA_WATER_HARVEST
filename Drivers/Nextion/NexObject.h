/*
 * NexObject.h
 *
 *  Created on: Jun 25, 2021
 *      Author: Andhie Setyabudi
 */
#pragma once
#ifndef NEXTION_NEXOBJECT_H_
#define NEXTION_NEXOBJECT_H_

#include "stdio.h"
#include "stdint.h"
#include "stddef.h"

/**
 * Push touch event occuring when your finger or pen coming to Nextion touch pannel.
 */
#define NEX_EVENT_PUSH  (0x01)

/**
 * Pop touch event occuring when your finger or pen leaving from Nextion touch pannel.
 */
#define NEX_EVENT_POP   (0x00)

typedef void (*NexTouchEventCb)(void *ptr);


typedef struct{
	uint8_t __pid; /* Page ID */
	uint8_t __cid; /* Component ID */
	const char *__name; /* An unique name */
	NexTouchEventCb __cb_push;
	void *__cbpush_ptr;
	NexTouchEventCb __cb_pop;
	void *__cbpop_ptr;
}NexObject_var;


static __inline__ void nex_createObject (NexObject_var * this, uint8_t pid, uint8_t cid, const char *name)
{
    this->__pid 	= pid;
    this->__cid 	= cid;
    this->__name 	= name;
    this->__cb_pop 	= NULL;
    this->__cb_push = NULL;
    this->__cbpop_ptr  = NULL;
    this->__cbpush_ptr = NULL;
}

static __inline__ uint8_t nex_getObjPid(NexObject_var this)
{
    return this.__pid;
}

static __inline__ uint8_t nex_getObjCid(NexObject_var this)
{
    return this.__cid;
}

static __inline__ const char* nex_getObjName(NexObject_var this)
{
    return this.__name;
}

static void nex_attachPush(NexObject_var * this, NexTouchEventCb push, void *ptr);
static void nex_detachPush(NexObject_var * this);
static void nex_attachPop(NexObject_var * this, NexTouchEventCb pop, void *ptr);
static void nex_detachPop(NexObject_var * this);
static void nex_push(NexObject_var * this);
static void nex_pop(NexObject_var * this);
static void nex_iterate(NexObject_var **list, uint8_t pid, uint8_t cid, int32_t event);

typedef struct{
	void (*create) 				(NexObject_var * this, uint8_t pid, uint8_t cid, const char *name);
	uint8_t (*getObjPid)		(NexObject_var this);
	uint8_t (*getObjCid)		(NexObject_var this);
	const char* (*getObjName) 	(NexObject_var this);

	// nexTouch function
	void (*attachPush)			(NexObject_var * this, NexTouchEventCb push, void *ptr);
	void (*detachPush)			(NexObject_var * this);
	void (*attachPop)			(NexObject_var * this, NexTouchEventCb pop, void *ptr);
	void (*detachPop)			(NexObject_var * this);
	void (*push)				(NexObject_var * this);
	void (*pop)					(NexObject_var * this);
	void (*iterate)				(NexObject_var **list, uint8_t pid, uint8_t cid, int32_t event);

}NexObject_funct;
extern NexObject_funct NexObject;


#endif /* NEXTION_NEXOBJECT_H_ */

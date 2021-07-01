/*
 * NexVariable.c
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 */

#include "NexVariable.h"

uint32_t NexVariable_getValue(NexObject_var *obj, uint32_t *number)
{
	size_t len_ = 0;
	len_ = strlen(obj->__name)+10;
	char cmd[len_];
	sprintf(cmd,"get %s.val", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

bool NexVariable_setValue(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	utoa(number, buf, 10);
	size_t len_ = strlen(obj->__name)+20;
	char cmd[len_];
	sprintf(cmd,"%s.val=\"%s\"", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(1000UL);
}

NexVariable_func NexVariable = {
		.create	  = NexButton_create,
		.getText  = NexButton_getText,
		.setText  = NexButton_setText,
		.getValue = NexVariable_getValue,
		.setValue = NexVariable_setValue,
};

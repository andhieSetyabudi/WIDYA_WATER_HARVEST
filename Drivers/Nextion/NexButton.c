/*
 * NexButton.c
 *
 *  Created on: Jun 29, 2021
 *      Author: Andhie Setyabudi
 */

#include "NexButton.h"
#include "string.h"

void NexButton_create(NexObject_var *obj, uint8_t pid, uint8_t cid, const char *name)
{
	NexObject.create(obj, pid, cid, name);
}


uint16_t NexButton_getText(NexObject_var *obj, char *buffer, uint16_t len)
{
	size_t len_ = 0;
	len_ = strlen(obj->__name)+10;
	char cmd[len_];
	sprintf(cmd,"get %s.txt", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetString(buffer, len, 100UL);
}

bool NexButton_setText(NexObject_var *obj, const char *buffer)
{
	size_t len_ = strlen(obj->__name)+(size_t)8+strlen(buffer);
	char cmd[len_];
	sprintf(cmd,"%s.txt\"%s\"", obj->__name, buffer);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

uint32_t NexButton_Get_background_color_bco(NexObject_var *obj, uint32_t *number)
{
	size_t len_ = strlen(obj->__name)+10;
	char cmd[len_];
	sprintf(cmd,"get %s.bco", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

bool NexButton_Set_background_color_bco(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name) + 20;
	char cmd[len_];
// create bco
	utoa(number, buf, 10);
	sprintf(cmd, "%s.bco=%u", obj->__name, buf);
	NexHardware.sendCommand(cmd);

// create ref
	sprintf(cmd, "ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

uint32_t NexButton_Get_press_background_color_bco2(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+12] = "";
	sprintf(cmd, "get %s.bco2", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number,100UL);
}

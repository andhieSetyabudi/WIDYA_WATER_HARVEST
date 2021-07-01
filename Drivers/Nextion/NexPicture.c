/*
 * NexPicture.c
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 */

#include "NexPicture.h"

bool NexPicture_Get_background_image_pic(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+12];
	sprintf(cmd,"get %s.pic", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

bool NexPicture_Set_background_image_pic(NexObject_var *obj, uint32_t number)
{
    char buf[10] = {0};
    size_t len_ = strlen(obj->__name)+20;
    char cmd[len_];
    // create .pic
    utoa(number, buf, 10);
    sprintf(cmd, "%s.pic=%s", obj->__name, buf);
    NexHardware.sendCommand(cmd);
    return NexHardware.recvRetCommandFinished(100UL);
}

bool NexPicture_getPic(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+12];
	sprintf(cmd,"get %s.pic", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

bool NexPicture_setPic(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name)+20;
	char cmd[len_];
	// create .pic
	utoa(number, buf, 10);
	sprintf(cmd, "%s.pic=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}


NexPicture_func NexPicture={
		.create		=	NexButton_create,
		.Get_background_image	=   NexPicture_Get_background_image_pic,
		.Set_background_image	= 	NexPicture_Set_background_image_pic,
		.getPic					= 	NexPicture_getPic,
		.setPic					= 	NexPicture_setPic,
};


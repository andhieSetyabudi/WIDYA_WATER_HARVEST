/*
 * NexButton.c
 *
 *  Created on: Jun 29, 2021
 *      Author: Andhie Setyabudi
 */

#include "NexButton.h"
#include "string.h"
#include "stdlib.h"

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
	sprintf(cmd, "%s.bco=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd,'\0', len_);
// create ref
	sprintf(cmd, "ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

uint32_t NexButton_Get_press_background_color_bco2(NexObject_var *obj, uint32_t *number)
{
	size_t len_ = strlen(obj->__name) + 12;
	char cmd[len_];
	sprintf(cmd, "get %s.bco2", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number,100UL);
}

bool NexButton_Set_press_background_color_bco2(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name)+20;
	char cmd[len_];

	// create bco2
	utoa(number, buf, 10);
	sprintf(cmd,"%s.bco=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd, '\0', len_);
	// create ref
	sprintf(cmd,"ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

uint32_t NexButton_Get_font_color_pco(NexObject_var *obj, uint32_t *number)
{
	size_t len_ = strlen(obj->__name) + 12;
	char cmd[len_];
	sprintf(cmd, "get %s.pco", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

bool NexButton_Set_font_color_pco(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name) + 20;
	char cmd[len_];

	// create pco
	utoa(number, buf, 10);
	sprintf(cmd, "%s.pco=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd, '\0', len_);
	// create ref
	sprintf(cmd, "ref %s",obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

uint32_t NexButton_Get_press_font_color_pco2(NexObject_var *obj, uint32_t *number)
{
	size_t len_ = strlen(obj->__name)+12;
	char cmd[len_];
	sprintf(cmd, "get %s.pco2", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

bool NexButton_Set_press_font_color_pco2(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name)+20;
	char cmd[len_];
	// create pco2
	utoa(number, buf, 10);
	sprintf(cmd,"%s.pco2=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd,'\0',len_);
	// create ref
	sprintf(cmd, "ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

uint32_t NexButton_Get_place_xcen(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+12];
	sprintf(cmd,"get %s.xcen", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

bool NexButton_Set_place_xcen(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name) + 20;
	char cmd[len_];
	// create xcen
	utoa(number, buf, 10);
	sprintf(cmd, "%s.xcen=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd, '\0',len_);
	// create ref
	sprintf(cmd, "ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

uint32_t NexButton_Get_place_ycen(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+12];
	sprintf(cmd,"get %s.ycen", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

bool NexButton_Set_place_ycen(NexObject_var *obj, uint32_t number)
{
	 char buf[10] = {0};
	 size_t len_ = strlen(obj->__name) + 20;
	 char cmd[len_];
	 //create ycen
	 utoa(number, buf, 10);
	 sprintf(cmd, "%s.ycen=%s", obj->__name,buf);
	 NexHardware.sendCommand(cmd);
	 memset(cmd, '\0', len_);
	 //create ref
	 sprintf(cmd,"ref %s", obj->__name);
	 NexHardware.sendCommand(cmd);
	 return NexHardware.recvRetCommandFinished(100UL);
}

uint32_t NexButton_getFont(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+12];
	sprintf(cmd,"get %s.font", obj->__name);
	NexHardware.sendCommand(cmd);
    return NexHardware.recvRetNumber(number, 100UL);
}

bool NexButton_setFont(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name)+20;
	char cmd[len_];
	// create .font
	utoa(number, buf, 10);
	sprintf(cmd,"%s.font=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd,'\0', len_);
	// create ref
	sprintf(cmd, "ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

uint32_t NexButton_Get_background_cropi_picc(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+12];
	sprintf(cmd,"get %s.picc", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

bool NexButton_Set_background_crop_picc(NexObject_var *obj, uint32_t number)
{
	size_t len_ = strlen(obj->__name)+20;
	char cmd[len_];
    char buf[10] = {0};
    // create .picc
    utoa(number, buf, 10);
    sprintf(cmd,"%s.picc=%s", obj->__name, buf);
    NexHardware.sendCommand(cmd);
    memset(cmd,'\0', len_);
    // create ref
    sprintf(cmd,"ref %s", obj->__name);
    NexHardware.sendCommand(cmd);
    return NexHardware.recvRetCommandFinished(100UL);
}

uint32_t NexButton_Get_press_background_crop_picc2(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+12];
	sprintf(cmd, "get %s.picc2", obj->__name);
	NexHardware.sendCommand(cmd);
    return NexHardware.recvRetNumber(number, 100UL);
}

bool NexButton_Set_press_background_crop_picc2(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name)+20;
	char cmd[len_];
	// create .picc2
	utoa(number, buf, 10);
	sprintf(cmd, "%s.picc2=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd, '\0', len_);
	// create ref
	sprintf(cmd, "ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

uint32_t NexButton_Get_background_image_pic(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+12];
	sprintf(cmd,"get %s.pic", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

bool NexButton_Set_background_image_pic(NexObject_var *obj, uint32_t number)
{
    char buf[10] = {0};
    size_t len_ = strlen(obj->__name)+20;
    char cmd[len_];
    // create .pic
    utoa(number, buf, 10);
    sprintf(cmd, "%s.pic=%s", obj->__name, buf);
    NexHardware.sendCommand(cmd);
    memset(cmd, '\0', len_);
    // create ref
    sprintf(cmd, "ref %s", obj->__name);
    NexHardware.sendCommand(cmd);
    return NexHardware.recvRetCommandFinished(100UL);
}

uint32_t NexButton_Get_press_background_image_pic2(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+12];
	sprintf(cmd,"get %s.pic2", obj->__name);
	NexHardware.sendCommand(cmd);
    return NexHardware.recvRetNumber(number, 100UL);
}


bool NexButton_Set_press_background_image_pic2(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name)+20;
	char cmd[len_];
	// create .pic2
	utoa(number, buf, 10);
	sprintf(cmd,"%s.pic2=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd,'\0',len_);
	// create ref
	sprintf(cmd, "ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}


NexButton_func NexButton={
		.create 	= NexButton_create,
		.getText	= NexButton_getText,
		.setText	= NexButton_setText,

};

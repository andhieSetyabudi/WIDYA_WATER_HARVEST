/*
 * NexNumber.c
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 */


#include "NexNumber.h"

/**
 * Get lenth attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t NexNumber_Get_number_length(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+12];
	sprintf(cmd,"get %s.lenth", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

/**
 * Set lenth attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool NexNumber_Set_number_length(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name) + 20;
	char cmd[len_];
// create bco
	utoa(number, buf, 10);
	sprintf(cmd, "%s.lenth=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd,'\0', len_);
// create ref
	sprintf(cmd, "ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

NexNumber_func NexNumber={
		.create 	= NexButton_create,
		.getValue	= NexSlider_getValue,
		.setValue	= NexSlider_setValue,
		.Get_background_color		=  NexButton_Get_background_color_bco,
		.Set_background_color		=  NexButton_Set_background_color_bco,
		.Get_font_color				=  NexButton_Get_font_color_pco,
		.Set_font_color				=  NexButton_Set_font_color_pco,
		.Get_place_xcen				=  NexButton_Get_place_xcen,
		.Set_place_xcen				=  NexButton_Set_place_xcen,
		.Get_place_ycen				=  NexButton_Get_place_ycen,
		.Set_place_ycen				=  NexButton_Set_place_ycen,
		.getFont					=  NexButton_getFont,
		.setFont					=  NexButton_setFont,
		.Get_number_length			=  NexNumber_Get_number_length,
		.Set_number_length			=  NexNumber_Set_number_length,
		.Get_background_crop		=  NexButton_Get_background_crop_picc,
		.Set_background_crop		=  NexButton_Set_background_crop_picc,
		.Get_background_image		=  NexButton_Get_background_image_pic,
		.Set_background_image		=  NexButton_Set_background_image_pic,
};

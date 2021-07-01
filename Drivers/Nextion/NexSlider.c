/*
 * NexSlider.c
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 */

#include "NexSlider.h"

/**
 * Get the value of slider.
 *
 * @param number - an output parameter to save the value of slider.
 *
 * @retval true - success.
 * @retval false - failed.
 */
bool NexSlider_getValue(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+10];
	sprintf(cmd,"get %s.val", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
};

/**
 * Set the value of slider.
 *
 * @param number - the value of slider.
 *
 * @retval true - success.
 * @retval false - failed.
 */
bool NexSlider_setValue(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	char cmd[strlen(obj->__name) + 20];
// create .val
	utoa(number, buf, 10);
	sprintf(cmd, "%s.val=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
};

/**
 * Get wid attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t NexSlider_Get_pointer_thickness_wid(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+10];
	sprintf(cmd,"get %s.wid", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

/**
 * Set wid attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool NexSlider_Set_pointer_thickness_wid(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name) + 20;
	char cmd[len_];
// create .wid
	utoa(number, buf, 10);
	sprintf(cmd, "%s.wid=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd,'\0', len_);
// create ref
	sprintf(cmd, "ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

/**
 * Get hig attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t NexSlider_Get_cursor_height_hig(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+10];
	sprintf(cmd,"get %s.hig", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

/**
 * Set hig attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool NexSlider_Set_cursor_height_hig(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name) + 20;
	char cmd[len_];
// create .hig
	utoa(number, buf, 10);
	sprintf(cmd, "%s.hig=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd,'\0', len_);
// create ref
	sprintf(cmd, "ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

/**
 * Get maxval attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t NexSlider_getMaxval(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+14];
	sprintf(cmd,"get %s.maxval", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

/**
 * Set maxval attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool NexSlider_setMaxval(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name) + 20;
	char cmd[len_];
// create .maxval
	utoa(number, buf, 10);
	sprintf(cmd, "%s.maxval=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd,'\0', len_);
// create ref
	sprintf(cmd, "ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

/**
 * Get minval attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t NexSlider_getMinval(NexObject_var *obj, uint32_t *number)
{
	char cmd[strlen(obj->__name)+14];
	sprintf(cmd,"get %s.minval", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetNumber(number, 100UL);
}

/**
 * Set minval attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool NexSlider_setMinval(NexObject_var *obj, uint32_t number)
{
	char buf[10] = {0};
	size_t len_ = strlen(obj->__name) + 20;
	char cmd[len_];
// create .minval
	utoa(number, buf, 10);
	sprintf(cmd, "%s.minval=%s", obj->__name, buf);
	NexHardware.sendCommand(cmd);
	memset(cmd,'\0', len_);
// create ref
	sprintf(cmd, "ref %s", obj->__name);
	NexHardware.sendCommand(cmd);
	return NexHardware.recvRetCommandFinished(100UL);
}

NexSlider_func NexSlider={
		.create		=	NexButton_create,
		.getValue	= 	NexSlider_getValue,
		.setValue	=	NexSlider_setValue,
		.Get_background_color_bco	= NexButton_Get_background_color_bco,
		.Set_background_color_bco	= NexButton_Set_background_color_bco,
		.Get_font_color_pco			= NexButton_Get_font_color_pco,
		.Set_font_color_pco			= NexButton_Set_font_color_pco,
		.Get_pointer_thickness		= NexSlider_Get_pointer_thickness_wid,
		.Set_pointer_thickness		= NexSlider_Set_pointer_thickness_wid,
		.Get_cursor_height			= NexSlider_Get_cursor_height_hig,
		.Set_cursor_height			= NexSlider_Set_cursor_height_hig,
		.getMaxVal					= NexSlider_getMaxval,
		.setMaxVal					= NexSlider_setMaxval,
		.getMinVal					= NexSlider_getMinval,
		.setMinVal					= NexSlider_setMinval,
};

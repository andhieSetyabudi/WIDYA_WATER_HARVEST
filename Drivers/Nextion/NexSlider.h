/*
 * NexSlider.h
 *
 *  Created on: Jun 30, 2021
 *      Author: Andhie Setyabudi
 */

#ifndef NEXTION_NEXSLIDER_H_
#define NEXTION_NEXSLIDER_H_

#include "NexHardware.h"
#include "NexButton.h"

/**
 * Get the value of slider.
 *
 * @param number - an output parameter to save the value of slider.
 *
 * @retval true - success.
 * @retval false - failed.
 */
bool NexSlider_getValue(NexObject_var *obj, uint32_t *number);

/**
 * Set the value of slider.
 *
 * @param number - the value of slider.
 *
 * @retval true - success.
 * @retval false - failed.
 */
bool NexSlider_setValue(NexObject_var *obj, uint32_t number);

/**
 * Get wid attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t NexSlider_Get_pointer_thickness_wid(NexObject_var *obj, uint32_t *number);

/**
 * Set wid attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool NexSlider_Set_pointer_thickness_wid(NexObject_var *obj, uint32_t number);

/**
 * Get hig attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t NexSlider_Get_cursor_height_hig(NexObject_var *obj, uint32_t *number);

/**
 * Set hig attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool NexSlider_Set_cursor_height_hig(NexObject_var *obj, uint32_t number);

/**
 * Get maxval attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t NexSlider_getMaxval(NexObject_var *obj, uint32_t *number);

/**
 * Set maxval attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool NexSlider_setMaxval(NexObject_var *obj, uint32_t number);

/**
 * Get minval attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t NexSlider_getMinval(NexObject_var *obj, uint32_t *number);

/**
 * Set minval attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool NexSlider_setMinval(NexObject_var *obj, uint32_t number);


typedef struct{
	void (*create)		(NexObject_var *obj, uint8_t pid, uint8_t cid, const char *name);
	bool (*getValue)	(NexObject_var *obj, uint32_t *number);
	bool (*setValue)	(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_background_color_bco)		(NexObject_var *obj, uint32_t *number);
	bool (*Set_background_color_bco)			(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_font_color_pco)				(NexObject_var *obj, uint32_t *number);
	bool (*Set_font_color_pco)					(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_pointer_thickness)			(NexObject_var *obj, uint32_t *number);
	bool (*Set_pointer_thickness)				(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_cursor_height)				(NexObject_var *obj, uint32_t *number);
	bool (*Set_cursor_height)					(NexObject_var *obj, uint32_t number);
	uint32_t (*getMaxVal)						(NexObject_var *obj, uint32_t *number);
	bool (*setMaxVal)							(NexObject_var *obj, uint32_t number);
	uint32_t (*getMinVal)						(NexObject_var *obj, uint32_t *number);
	bool (*setMinVal)							(NexObject_var *obj, uint32_t number);
}NexSlider_func;

extern NexSlider_func NexSlider;



#endif /* NEXTION_NEXSLIDER_H_ */

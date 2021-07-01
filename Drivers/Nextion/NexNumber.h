/*
 * NexNumber.h
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi Wibowo
 */

#ifndef NEXTION_NEXNUMBER_H_
#define NEXTION_NEXNUMBER_H_

#include "NexHardware.h"
#include "NexButton.h"
#include "NexVariable.h"
#include "NexSlider.h"


/**
 * Get lenth attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t NexNumber_Get_number_length(NexObject_var *obj, uint32_t *number);

/**
 * Set lenth attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool NexNumber_Set_number_length(NexObject_var *obj, uint32_t number);



typedef struct{
	void (*create) 		(NexObject_var *obj, uint8_t pid, uint8_t cid, const char *name);

	uint32_t (*getValue)(NexObject_var *obj, uint32_t *number);
	bool (*setValue)	(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_background_color)		(NexObject_var *obj, uint32_t *number);
	bool (*Set_background_color)			(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_font_color)				(NexObject_var *obj, uint32_t *number);
	bool (*Set_font_color)					(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_place_xcen)				(NexObject_var *obj, uint32_t *number);
	bool (*Set_place_xcen)					(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_place_ycen)				(NexObject_var *obj, uint32_t *number);
	bool (*Set_place_ycen)					(NexObject_var *obj, uint32_t number);

	uint32_t (*getFont)						(NexObject_var *obj, uint32_t *number);
	bool (*setFont)							(NexObject_var *obj, uint32_t number);

	uint32_t (*Get_number_length)			(NexObject_var *obj, uint32_t *number);
	bool (*Set_number_length)				(NexObject_var *obj, uint32_t number);

	uint32_t (*Get_background_crop)			(NexObject_var *obj, uint32_t *number);
	bool (*Set_background_crop)				(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_background_image)		(NexObject_var *obj, uint32_t *number);
	bool (*Set_background_image)			(NexObject_var *obj, uint32_t number);
}NexNumber_func;

extern NexNumber_func NexNumber;
#endif /* NEXTION_NEXNUMBER_H_ */

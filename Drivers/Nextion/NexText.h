/*
 * NexText.h
 *
 *  Created on: Jun 29, 2021
 *      Author: Andhie setyabudi
 */

#ifndef NEXTION_NEXTEXT_H_
#define NEXTION_NEXTEXT_H_

#include "NexHardware.h"
#include "NexButton.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"


typedef struct{
	void(*create)	(NexObject_var *obj, uint8_t pid, uint8_t cid, const char *name);
	uint16_t (*getText)	(NexObject_var *obj, char *buffer, uint16_t len);
	bool (*setText)		(NexObject_var *obj, const char *buffer);
	uint32_t (*Get_background_color_bco)		(NexObject_var *obj, uint32_t *number);
	bool (*Set_background_color_bco)			(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_font_color_pco)				(NexObject_var *obj, uint32_t *number);
	bool (*Set_font_color_pco)					(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_place_xcen)					(NexObject_var *obj, uint32_t *number);
	bool (*Set_place_xcen)						(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_place_ycen)					(NexObject_var *obj, uint32_t *number);
	bool (*Set_place_ycen)						(NexObject_var *obj, uint32_t number);
	uint32_t (*getFont)							(NexObject_var *obj, uint32_t *number);
	bool (*setFont)								(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_background_crop_picc)		(NexObject_var *obj, uint32_t *number);
	bool (*Set_background_crop_picc)			(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_background_image_pic)		(NexObject_var *obj, uint32_t *number);
	bool (*Set_background_image_pic)			(NexObject_var *obj, uint32_t number);
}NexText_func;

extern NexText_func NexText;

#endif /* NEXTION_NEXTEXT_H_ */

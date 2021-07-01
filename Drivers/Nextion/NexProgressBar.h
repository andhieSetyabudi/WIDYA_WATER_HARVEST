/*
 * NexProgressBar.h
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 */

#ifndef NEXTION_NEXPROGRESSBAR_H_
#define NEXTION_NEXPROGRESSBAR_H_

#include "NexHardware.h"
#include "NexObject.h"
#include "NexSlider.h"

typedef struct{
	void (*create)		(NexObject_var *obj, uint8_t pid, uint8_t cid, const char *name);
	bool (*getValue)	(NexObject_var *obj, uint32_t *number);
	bool (*setValue)	(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_background_color_bco)		(NexObject_var *obj, uint32_t *number);
	bool (*Set_background_color_bco)			(NexObject_var *obj, uint32_t number);
	uint32_t (*Get_font_color_pco)				(NexObject_var *obj, uint32_t *number);
	bool (*Set_font_color_pco)					(NexObject_var *obj, uint32_t number);
}NexProgressBar_func;

extern NexProgressBar_func NexProgressBar;

#endif /* NEXTION_NEXPROGRESSBAR_H_ */

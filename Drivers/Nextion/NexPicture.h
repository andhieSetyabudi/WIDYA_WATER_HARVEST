/*
 * NexPicture.h
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 */

#ifndef NEXTION_NEXPICTURE_H_
#define NEXTION_NEXPICTURE_H_

#include "NexButton.h"

    /**
     * Get picture's number.
     *
     * @param number - an output parameter to save picture number.
     *
     * @retval true - success.
     * @retval false - failed.
     */
static bool NexPicture_Get_background_image_pic(NexObject_var *obj, uint32_t *number);

/**
 * Set picture's number.
 *
 * @param number -the picture number.
 *
 * @retval true - success.
 * @retval false - failed.
 */
static bool NexPicture_Set_background_image_pic(NexObject_var *obj, uint32_t number);

/**
 * Get picture's number.
 *
 * @param number - an output parameter to save picture number.
 *
 * @retval true - success.
 * @retval false - failed.
 */
static bool NexPicture_getPic(NexObject_var *obj, uint32_t *number);

/**
 * Set picture's number.
 *
 * @param number -the picture number.
 *
 * @retval true - success.
 * @retval false - failed.
 */
static bool NexPicture_setPic(NexObject_var *obj, uint32_t number);

typedef struct{
	void (*create)		(NexObject_var *obj, uint8_t pid, uint8_t cid, const char *name);
	bool (*Get_background_image)	(NexObject_var *obj, uint32_t *number);
	bool (*Set_background_image)	(NexObject_var *obj, uint32_t number);
	bool (*getPic)					(NexObject_var *obj, uint32_t *number);
	bool (*setPic)					(NexObject_var *obj, uint32_t number);

}NexPicture_func;

extern NexPicture_func NexPicture;
#endif /* NEXTION_NEXPICTURE_H_ */

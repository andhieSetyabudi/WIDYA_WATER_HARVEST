/*
 * NexText.c
 *
 *  Created on: Jun 29, 2021
 *      Author: Andhie Setyabudi
 */

#include "NexText.h"

NexText_func NexText =
{
		.create 		= NexButton_create,
		.getText		= NexButton_getText,
		.setText		= NexButton_setText,
		.Get_background_color_bco		= NexButton_Get_background_color_bco,
		.Set_background_color_bco		= NexButton_Set_background_color_bco,
		.Get_font_color_pco				= NexButton_Get_font_color_pco,
		.Set_font_color_pco				= NexButton_Set_font_color_pco,
		.Get_place_xcen					= NexButton_Get_place_xcen,
		.Set_place_xcen					= NexButton_Set_place_xcen,
		.Get_place_ycen					= NexButton_Get_place_ycen,
		.Set_place_ycen					= NexButton_Set_place_ycen,
		.getFont						= NexButton_getFont,
		.setFont						= NexButton_setFont,
		.Get_background_crop_picc		= NexButton_Get_background_crop_picc,
		.Set_background_crop_picc		= NexButton_Set_background_crop_picc,
		.Get_background_image_pic		= NexButton_Get_background_image_pic,
		.Set_background_image_pic		= NexButton_Set_background_image_pic,
};

/*
 * NexProgressBar.c
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 */


#include "NexProgressBar.h"

NexProgressBar_func NexProgressBar={
		.create		= NexButton_create,
		.getValue	= 	NexSlider_getValue,
		.setValue	=	NexSlider_setValue,
		.Get_background_color_bco	= NexButton_Get_background_color_bco,
		.Set_background_color_bco	= NexButton_Set_background_color_bco,
		.Get_font_color_pco			= NexButton_Get_font_color_pco,
		.Set_font_color_pco			= NexButton_Set_font_color_pco,
};

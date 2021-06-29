/*
 * NexButton.h
 *
 *  Created on: Jun 29, 2021
 *      Author: Andhie Setyabudi
 */

#pragma once
#ifndef NEXTION_NEXBUTTON_H_
#define NEXTION_NEXBUTTON_H_

#include "NexHardware.h"

static void NexButton_create(NexObject_var *obj, uint8_t pid, uint8_t cid, const char *name);

/**
 * Get text attribute of component.
 *
 * @param buffer - buffer storing text returned.
 * @param len - length of buffer.
 * @return The real length of text returned.
 */
static uint16_t NexButton_getText(NexObject_var *obj, char *buffer, uint16_t len);

/**
 * Set text attribute of component.
 *
 * @param buffer - text buffer terminated with '\0'.
 * @return true if success, false for failure.
 */
static bool NexButton_setText(NexObject_var *obj, const char *buffer);

/**
 * Get bco attribute of component
 *
 * @param number - buffer storing data return
 * @return the length of the data
 */
static uint32_t NexButton_Get_background_color_bco(NexObject_var *obj, uint32_t *number);

/**
 * Set bco attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
static bool NexButton_Set_background_color_bco(NexObject_var *obj, uint32_t number);

/**
 * Get bco2 attribute of component
 *
 * @param number - buffer storing data return
 * @return the length of the data
 */
static uint32_t NexButton_Get_press_background_color_bco2(NexObject_var *obj, uint32_t *number);

/**
 * Set bco2 attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_press_background_color_bco2(uint32_t number);

/**
 * Get pco attribute of component
 *
 * @param number - buffer storing data return
 * @return the length of the data
 */
uint32_t Get_font_color_pco(uint32_t *number);

/**
 * Set pco attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_font_color_pco(uint32_t number);

/**
 * Get pco2 attribute of component
 *
 * @param number - buffer storing data return
 * @return the length of the data
 */
uint32_t Get_press_font_color_pco2(uint32_t *number);

/**
 * Set pco2 attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_press_font_color_pco2(uint32_t number);

/**
 * Get xcen attribute of component
 *
 * @param number - buffer storing data return
 * @return the length of the data
 */
uint32_t Get_place_xcen(uint32_t *number);

/**
 * Set xcen attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_place_xcen(uint32_t number);

/**
 * Get ycen attribute of component
 *
 * @param number - buffer storing data return
 * @return the length of the data
 */
uint32_t Get_place_ycen(uint32_t *number);

/**
 * Set ycen attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_place_ycen(uint32_t number);

/**
 * Get font attribute of component
 *
 * @param number - buffer storing data return
 * @return the length of the data
 */
uint32_t getFont(uint32_t *number);

/**
 * Set font attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool setFont(uint32_t number);

/**
 * Get picc attribute of component
 *
 * @param number - buffer storing data return
 * @return the length of the data
 */
uint32_t Get_background_cropi_picc(uint32_t *number);

/**
 * Set picc attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_background_crop_picc(uint32_t number);

/**
 * Get picc2 attribute of component
 *
 * @param number - buffer storing data return
 * @return the length of the data
 */
uint32_t Get_press_background_crop_picc2(uint32_t *number);

/**
 * Set picc2 attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_press_background_crop_picc2(uint32_t number);

/**
 * Get pic attribute of component
 *
 * @param number - buffer storing data return
 * @return the length of the data
 */
uint32_t Get_background_image_pic(uint32_t *number);

/**
 * Set pic attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_background_image_pic(uint32_t number);

/**
 * Get pic2 attribute of component
 *
 * @param number - buffer storing data return
 * @return the length of the data
 */
uint32_t Get_press_background_image_pic2(uint32_t *number);

/**
 * Set pic2 attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_press_background_image_pic2(uint32_t number);

typedef struct {
		void (*create) (NexObject_var *obj, uint8_t pid, uint8_t cid, const char *name);
}NexButton_func;


#endif /* NEXTION_NEXBUTTON_H_ */

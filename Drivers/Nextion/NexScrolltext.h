/*
 * NexScrolltext.h
 *
 *  Created on: Jul 1, 2021
 *      Author: Andhie Setyabudi
 */

#ifndef NEXTION_NEXSCROLLTEXT_H_
#define NEXTION_NEXSCROLLTEXT_H_

#include "NexHardware.h"
#include "NexButton.h"

/**
 * Get text attribute of component.
 *
 * @param buffer - buffer storing text returned.
 * @param len - length of buffer.
 * @return The real length of text returned.
 */
uint16_t getText(char *buffer, uint16_t len);

/**
 * Set text attribute of component.
 *
 * @param buffer - text buffer terminated with '\0'.
 * @return true if success, false for failure.
 */
bool setText(const char *buffer);

/**
 * Get bco attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t Get_background_color_bco(uint32_t *number);

/**
 * Set bco attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_background_color_bco(uint32_t number);

/**
 * Get pco attribute of component
 *
 * @param number - buffer storing data retur
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
 * Get xcen attribute of component
 *
 * @param number - buffer storing data retur
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
 * @param number - buffer storing data retur
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
 * @param number - buffer storing data retur
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
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t Get_background_crop_picc(uint32_t *number);

/**
 * Set picc attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_background_crop_picc(uint32_t number);

/**
 * Get pic attribute of component
 *
 * @param number - buffer storing data retur
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
 * Get dir attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t Get_scroll_dir(uint32_t *number);

/**
 * Set dir attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_scroll_dir(uint32_t number);

/**
 * Get dis attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t Get_scroll_distance(uint32_t *number);

/**
 * Set dis attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_scroll_distance(uint32_t number);

/**
 * Get tim attribute of component
 *
 * @param number - buffer storing data retur
 * @return the length of the data
 */
uint32_t Get_cycle_tim(uint32_t *number);

/**
 * Set tim attribute of component
 *
 * @param number - To set up the data
 * @return true if success, false for failure
 */
bool Set_cycle_tim(uint32_t number);

bool enable(void);
bool disable(void);

#endif /* NEXTION_NEXSCROLLTEXT_H_ */

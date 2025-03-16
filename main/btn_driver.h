
#pragma once
#ifndef __BTN_DRIVER_H__
#define __BTN_DRIVER_H__

#include <stdbool.h>
#include "freertos/FreeRTOS.h" //iram_attr

/*******************************************************
 *                extern variables
 *******************************************************/
volatile extern bool btn_ok_pressed;
volatile extern bool btn_nok_pressed;

/*******************************************************
 *                functions headers
 *******************************************************/

/**
* @brief configure btns gpio (btn_ok and btn_nok)
*/
void btns_init();

/**
 * @brief callback executed when intr (rising_edge) happens on pin 8
 */
void  IRAM_ATTR btn_ok_isr();


/**
 * @brief callback executed when intr (rising_edge) happens on pin 9
 */
void  IRAM_ATTR btn_nok_isr();

/**
 * @brief while(1) : logs when (btn_ok_pressed == true) or (btn_nok_pressed == true)
 */
void btn_log_task();




#endif // __BTN_DRIVER_H__
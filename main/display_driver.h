
#pragma once
#ifndef __DISPLAY_DRIVER_H__
#define __DISPLAY_DRIVER_H__

#include "scan_app.h"
#include "display_driver.h"
#include "lists_lib.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/lock.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "esp_lcd_ili9341.h"


/*******************************************************
 *                functions headers
 *******************************************************/

static bool notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *data, void * user_ctx);
static void lvgl_flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map);
static void lvgl_port_update_callback(lv_display_t *disp);
static void increase_lvgl_tick(void * arg);
static void lvgl_port_task(void * arg);

void display_init();

void update_label_ok_nok();




#endif // __DISPLAY_DRIVER_H__
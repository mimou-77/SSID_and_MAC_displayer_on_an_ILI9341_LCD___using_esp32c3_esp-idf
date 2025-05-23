
#pragma once
#ifndef __SCAN_APP_H__
#define __SCAN_APP_H__

#include <stdint.h>
#include "esp_wifi.h"


/*******************************************************
 *                types
 *******************************************************/
typedef struct 
{
    uint8_t * supla_device_ssid ;
    char * supla_device_mac;
} supla_ssid_mac_t;

extern uint8_t * supla_device_ssid ;
extern char supla_device_mac[18] ; //will be lowercase ; defined in main.c, modified by scan_app.c
extern char char_supla_device_ssid[21];


/*******************************************************
 *                functions headers
 *******************************************************/
char wifi_scan();


















#endif // __SCAN_APP_H__
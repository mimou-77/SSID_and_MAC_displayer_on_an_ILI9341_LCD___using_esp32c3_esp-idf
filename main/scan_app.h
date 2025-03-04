
#pragma once
#ifndef __SCAN_APP_H__
#define __SCAN_APP_H__

#include <stdint.h>


/*******************************************************
 *                types
 *******************************************************/
typedef struct 
{
    uint8_t * supla_device_ssid ;
    char * supla_device_mac;
} supla_ssid_mac_t;



/*******************************************************
 *                functions headers
 *******************************************************/
supla_ssid_mac_t wifi_scan();

















#endif // __SCAN_APP_H__
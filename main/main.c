#include "scan_app.h"
#include "lists_lib.h"
#include "display_driver.h"
#include "lists_lib.h"

#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "esp_netif.h"

 
static const char * TAG = "main";

uint8_t * supla_device_ssid = NULL ;
char supla_device_mac[18] = {0} ; //scan_app.c modifies it
char char_supla_device_ssid[21] = {0} ;


//declared extern in lists_lib.h ; modified by auto_classify_ok_nok ; used by display_driver.c/update_label_ok_nok
volatile char device_ok = 0;
volatile char device_nok = 0;


int app_main(void)
{
    supla_device_mac[17] = '\0' ; //to avoid conflicts when supla_device_mac used as char *

    char found = 0 ;
    esp_err_t err = ESP_OK;
    
    err = nvs_flash_init();
    //if flash mem is full, erase flash and reinit ; same if new version of flash mem 
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK( err );
        err = nvs_flash_init();
    }

    
    
 
    if(!(wifi_scan())) //if no supla device found
    {
        ESP_LOGI(TAG, "no supla device found ; exiting");
    }
    else //if supla device found
    {
        found = 1; 
    }


    //check for debug purposes
    if (supla_device_ssid == NULL)
    {
        ESP_LOGI(TAG, "supla_device_ssid = NULL");
        
    }
    

    if(found)
    {
        for (int i = 0; i < 23; i++)
        {
            char_supla_device_ssid[i] = (char)(supla_device_ssid[i]);
        }
    
        //check for debug purposes
        ESP_LOGI(TAG, "from main: uint8_t  ssid = %s", supla_device_ssid); //incorrect result (bad characters)
        ESP_LOGI(TAG, "from main: char ssid = %s", char_supla_device_ssid);
    }

    
    display_init();

    init_spiffs(); //init spiffs + call auto_classify
 

    // à modifier pour pas while(1); update label of ui only once 
    while(1)
    {   update_label_ok_nok();
        
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

}

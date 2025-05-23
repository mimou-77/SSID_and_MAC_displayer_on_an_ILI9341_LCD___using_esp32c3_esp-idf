#include "scan_app.h"
#include "lists_lib.h"
#include "btn_driver.h"
#include "display_driver.h"
#include "lists_lib.h"

#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"

#include "esp_netif.h"

 
static const char * TAG = "main";

uint8_t * supla_device_ssid = NULL ;
char supla_device_mac[18] = {0} ; //scan_app.c modifies it
char char_supla_device_ssid[23] = {0} ;


// volatile bool btn_ok_pressed = false;
// volatile bool btn_nok_pressed = false;
volatile bool device_ok = false;
volatile bool device_nok = false;


int app_main(void)
{
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

    //uses buttons to classify device as ok or nok ↓
    //btns_init();

    init_spiffs();

    //automatically classifies device as ok or nok ↓
    auto_classify_ok_nok();

    //connect to ssid/pw for the csv files transfer
    


    while(1)
    {   update_label_ok_nok();
        
        //uses buttons to classify device as ok or nok ↓
        //update_lists();

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

}

#include "scan_app.h"
#include "lists_lib.h"
#include "btn_driver.h"
#include "display_driver.h"
#include "lists_lib.h"

#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"

 
static const char * TAG = "main";

uint8_t * supla_device_ssid = NULL ;
char supla_device_mac[18] = {0} ;
char char_supla_device_ssid[21] = {0} ;


volatile bool btn_ok_pressed = false;
volatile bool btn_nok_pressed = false;


void app_main(void)
{

    esp_err_t err = ESP_OK;
    
    err = nvs_flash_init();
    //if flash mem is full, erase flash and reinit ; same if new version of flash mem 
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK( err );
        err = nvs_flash_init();
    }

    btns_init();
    
    wifi_scan();

    if (supla_device_ssid == NULL)
    {
        ESP_LOGI(TAG, "supla_device_ssid = NULL");
        
    }
    

    for (int i = 0; i < 21; i++)
    {
        char_supla_device_ssid[i] = (char)(supla_device_ssid[i]);
    }
    

    ESP_LOGI(TAG, "from main: uint8_t  ssid = %s", supla_device_ssid); //incorrect result (bad characters)
    ESP_LOGI(TAG, "from main: char ssid = %s", char_supla_device_ssid);
    
    display_init();

    init_spiffs();

    while(1)
    {   update_label_ok_nok();
        update_lists();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

}

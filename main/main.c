#include "scan_app.h"
#include "display_driver.h"

#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"

 
static const char * TAG = "main";
 
 

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

    wifi_scan();
    
    

    //display_init(supla_ssid, supla_mac);

}

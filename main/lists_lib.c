#include "lists_lib.h"
#include "btn_driver.h"//to use btn_ok_pressed et btn_nok_pressed
#include "esp_spiffs.h"
#include "scan_app.h"
#include "esp_log.h"

static const char *TAG = "lists_lib";


void init_spiffs()
{
    esp_vfs_spiffs_conf_t conf =
    {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 3,
        .format_if_mount_failed = true,
    };

    esp_err_t err = esp_vfs_spiffs_register(&conf);

    size_t total = 0, used = 0;
    err = esp_spiffs_info(NULL, &total, &used); //get total and used
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "failed to get spiffs infos %s", esp_err_to_name(err));
    }
    else
    {
        ESP_LOGI(TAG, "partition size: total = %d, used = %d", total, used);
    }
}

void update_lists()
{
    if(btn_ok_pressed) //add device mac to ok_list
    {
        FILE * ok_file = fopen("/spiffs/ok_list.csv", "a"); //mode append
        if (ok_file == NULL)
        {
            ESP_LOGI(TAG, "failed to create ok_list.csv");
            return;
        } 

        fprintf(ok_file, supla_device_mac);
        fclose(ok_file);
    }

    if(btn_nok_pressed) //add device mac to ok_list
    {
        FILE * nok_file = fopen("/spiffs/nok_list.csv", "a"); //mode append
        if (nok_file == NULL)
        {
            ESP_LOGI(TAG, "failed to create nok_list.csv");
            return;
        } 

        fprintf(nok_file, supla_device_mac);
        fclose(nok_file);
    }
}
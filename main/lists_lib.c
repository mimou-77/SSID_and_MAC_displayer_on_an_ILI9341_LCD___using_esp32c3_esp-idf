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

    //changed
    // //delete content of ok_list.csv and nok_list.csv
    // FILE * ok_file_d = fopen("/spiffs/ok_list.csv", "w");
    // fclose(ok_file_d);
    // ESP_LOGI(TAG, "file content of ok_lists.csv deleted");
    // FILE * nok_file_d = fopen("/spiffs/nok_list.csv", "w");
    // fclose(nok_file_d);
    // ESP_LOGI(TAG, "file content of nok_lists.csv deleted");
    
    //show content of ok_list.csv and nok_list.csv
    FILE * ok_file_l = fopen("/spiffs/ok_list.csv", "r");
        if (ok_file_l == NULL)
        {
            ESP_LOGI(TAG, "failed to open file: %s", "ok_file_l");
            return;
        }
    char line_ok[256]; //1 line of the file = 256 characters (max length)
    while(fgets(line_ok, sizeof(line_ok), ok_file_l) != NULL)
    {
        ESP_LOGI(TAG, "csv line of ok_list.csv : %s", line_ok);
    }
    fclose(ok_file_l);
    FILE * nok_file_l = fopen("/spiffs/nok_list.csv", "r");
    if (nok_file_l == NULL)
    {
        ESP_LOGI(TAG, "failed to open file: %s", "nok_file_l");
        return;
    }
    char line_nok[256]; //1 line of the file = 256 characters (max length)
    while(fgets(line_nok, sizeof(line_nok), nok_file_l) != NULL)
    {
        ESP_LOGI(TAG, "csv line of ok_list.csv : %s", line_nok);
    }
    fclose(nok_file_l);
        
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

        fprintf(ok_file, "%s\n", supla_device_mac);
        fclose(ok_file);

        //log file content (file = ok_list.csv)
        FILE * ok_file_l = fopen("/spiffs/ok_list.csv", "r");
        if (ok_file_l == NULL)
        {
            ESP_LOGI(TAG, "failed to open file: %s", "ok_file_l");
            return;
        }
        
        char line[256]; //1 line of the file = 256 characters (max length)
        int i = 0;
        while(fgets(line, sizeof(line), ok_file_l) != NULL)
        {
            ESP_LOGI(TAG, "ok_list.csv line %d: %s", i, line);
            i++;
        }
        fclose(ok_file_l);
    }

    if(btn_nok_pressed) //add device mac to ok_list
    {
        FILE * nok_file = fopen("/spiffs/nok_list.csv", "a"); //mode append
        if (nok_file == NULL)
        {
            ESP_LOGI(TAG, "failed to create nok_list.csv");
            return;
        } 

        fprintf(nok_file, "%s\n", supla_device_mac);
        fclose(nok_file);

        //log file content (file = nok_list.csv)
        FILE * nok_file_l = fopen("/spiffs/nok_list.csv", "r");
        if (nok_file_l == NULL)
        {
            ESP_LOGI(TAG, "failed to open file: %s", "nok_file_l");
            return;
        }
        
        char line[256]; //1 line of the file = 256 characters (max length)
        int i = 0;
        while(fgets(line, sizeof(line), nok_file_l) != NULL)
        {
            ESP_LOGI(TAG, "nok_list.csv line %d: %s", i, line);
            i++;
        }
        fclose(nok_file_l);
    }
}
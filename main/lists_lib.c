#include "lists_lib.h"
#include "btn_driver.h"//to use btn_ok_pressed et btn_nok_pressed
#include "esp_spiffs.h"
#include "scan_app.h"
#include "esp_log.h"
#include "string.h"

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
    

    //check if ok_list.csv and nok_list.csv exist, if not, create them
    FILE * ok_file_c = fopen("/spiffs/ok_list.csv", "a+");
    fclose(ok_file_c);
    
    FILE * nok_file_c = fopen("/spiffs/nok_list.csv", "a+");
    fclose(nok_file_c);
    


    //show content of ok_list.csv and nok_list.csv
    FILE * ok_file_l = fopen("/spiffs/ok_list.csv", "r");
        if (ok_file_l == NULL)
        {
            ESP_LOGI(TAG, "failed to open file: %s", "ok_file_l");
            return;
        }
    char line_ok[256]; //1 line of the file = 256 characters (max length)
    ESP_LOGI(TAG, "ok_list.csv : ");
    while(fgets(line_ok, sizeof(line_ok), ok_file_l) != NULL)
    {
        ESP_LOGI("", "          %s", line_ok);
    }
    fclose(ok_file_l);
    FILE * nok_file_l = fopen("/spiffs/nok_list.csv", "r");
    if (nok_file_l == NULL)
    {
        ESP_LOGI(TAG, "failed to open file: %s", "nok_file_l");
        return;
    }
    char line_nok[256]; //1 line of the file = 256 characters (max length)
    ESP_LOGI(TAG, "nok_list.csv : ");
    while(fgets(line_nok, sizeof(line_nok), nok_file_l) != NULL)
    {
        ESP_LOGI("","          %s", line_nok);
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

        // before adding the device mac to ok_list, check if it's already there
        char device_in_ok_list = 0; 
        char line[64]; //buffer where to store the line read from the file
        FILE * ok_file_c = fopen("/spiffs/ok_list.csv", "r");
        while (fgets(line, 64, ok_file_c) != NULL) //fgets reads a new line, returns next line, return NULL if we reached EOF
        {
            if (strstr(line, supla_device_mac) != NULL) //we found device mac in a line of the csv file
            {
                device_in_ok_list = 1 ;
                ESP_LOGI(TAG, "device already in ok_list.csv"); 
            }
            
        }
        fclose(ok_file_c);
        
        if (!device_in_ok_list) //write device mac in ok_list.csv
        {   
            FILE * ok_file = fopen("/spiffs/ok_list.csv", "a"); //mode append
            if (ok_file == NULL)
            {
                ESP_LOGI(TAG, "failed to create ok_list.csv");
                return;
            } 
        
            fprintf(ok_file, "%s\n", supla_device_mac);
            fclose(ok_file);
        }
        

            
        // //log file content (file = ok_list.csv) when ok_btn pressed (whether device mac added or not) 
        // FILE * ok_file_l = fopen("/spiffs/ok_list.csv", "r");
        // if (ok_file_l == NULL)
        // {
        //     ESP_LOGI(TAG, "failed to open file: %s", "ok_file_l");
        //     return;
        // }
        
        // char line_log[256]; //1 line of the file = 256 characters (max length)
        // int i = 0;
        // while(fgets(line_log, sizeof(line_log), ok_file_l) != NULL)
        // {
        //     ESP_LOGI(TAG, "ok_list.csv line %d: %s", i, line_log);
        //     i++;
        // }
        // fclose(ok_file_l);
    }

    if(btn_nok_pressed) //add device mac to nok_list
    {
        //check if device mac is already in nok_list.csv
        char device_in_nok_list = 0; 
        char line[64]; //buffer where to store the line read from the file
        FILE * nok_file_c = fopen("/spiffs/nok_list.csv", "r");
        while (fgets(line, 64, nok_file_c) != NULL) //fgets reads a new line, returns next line, return NULL if we reached EOF
        {
            if (strstr(line, supla_device_mac) != NULL) //we found device mac in a line of the csv file
            {
                device_in_nok_list = 1 ;
                ESP_LOGI(TAG, "device already in nok_list.csv"); 
            }
            
        }
        fclose(nok_file_c);

        if (!device_in_nok_list) //if device mac isn't in the nok_list, add it to nok_list
        {
            FILE * nok_file = fopen("/spiffs/nok_list.csv", "a"); //mode append
            if (nok_file == NULL)
            {
                ESP_LOGI(TAG, "failed to create nok_list.csv");
                return;
            } 

            fprintf(nok_file, "%s\n", supla_device_mac);
            fclose(nok_file);
        }
        


        // //log file content (file = nok_list.csv) when nok_btn pressed (whether device mac added or not) 
        // FILE * nok_file_l = fopen("/spiffs/nok_list.csv", "r");
        // if (nok_file_l == NULL)
        // {
        //     ESP_LOGI(TAG, "failed to open file: %s", "nok_file_l");
        //     return;
        // }
        
        // char line_log[256]; //1 line of the file = 256 characters (max length)
        // int i = 0;
        // while(fgets(line_log, sizeof(line_log), nok_file_l) != NULL)
        // {
        //     ESP_LOGI(TAG, "nok_list.csv line %d: %s", i, line_log);
        //     i++;
        // }
        // fclose(nok_file_l);
    }
}
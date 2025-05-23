#include "lists_lib.h"
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


    //check if ok_list.csv and nok_list.csv exist, if not, create them
    FILE * ok_file_c = fopen("/spiffs/ok_list.csv", "a+");
    fclose(ok_file_c);
    
    FILE * nok_file_c = fopen("/spiffs/nok_list.csv", "a+");
    fclose(nok_file_c);

    // //
    // //
    // //for debug purposes //temporary: delete contents of  ok_lists.csv and nok_lists.csv
    // ok_file_c = fopen("/spiffs/ok_list.csv","w");
    // fclose(ok_file_c);
    // nok_file_c = fopen("/spiffs/nok_list.csv", "w");
    // fclose(nok_file_c);


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

    //automatically classify device as ok or nok ↓
    auto_classify_ok_nok();

}


//automatically classifies device as ok or nok ↓
void auto_classify_ok_nok()
{

    // ↓↓↓ defined in main.c ; extern in scan_app.h ; modified by scan_app.h : ↓↓↓
    // extern char supla_device_mac[18] ; //will be lowercase ; defined in main.c, modified by scan_app.c
    // extern char char_supla_device_ssid[23];

    //device ok if last part of ssid = mac && last character(ssid) = last character(mac)+1
    //check if last part of ssid = mac && last character(ssid) = last character(mac)+1
    // 10 -> 21 of ssid ; remove ":" fom mac => mac_an ; 0 -> 11 of mac_an_u
    //////////////////////////////////////////////////////////////////////////////////

    //remove ':' fom mac => mac_an 
    char mac_an[13];//12 characters +'\0' = 13
        
    int j = 0;
    for (int i = 0; i < strlen(supla_device_mac); i++)
    {
        if (supla_device_mac[i] != ':')
        {
            mac_an[j] = supla_device_mac[i];
            j++;
        }
    }
    mac_an[j] = '\0';
    // ESP_LOGI(TAG, "check: supla_device_mac = %s", supla_device_mac);
    // ESP_LOGI(TAG, "check: mac_an = %s", mac_an);

    //uppercase mac_an -> mac_an_u
    char mac_an_u[13];
    for (int i = 0; i < strlen(mac_an); i++)
    {
        if(!((mac_an[i] >= '0') && (mac_an[i] <= '9'))) //if not a number (if letter)
                {   
                    mac_an_u[i] =  mac_an[i] - 32;
                }  
                else //if number
                {
                    mac_an_u[i] =  mac_an[i];
                }
    }
    

    //if R2L || R2S
    if (strstr(char_supla_device_ssid, "R2L") || strstr(char_supla_device_ssid, "R2S"))
    {
        //compare ssid[10->20] with mac_an_u[0->10] and (ssid[21]+1) with mac_an_u[11]

        // substring ssid_10_20 = ssid[10->20]
        char ssid_10_20[12]; //10+'\0'
        strncpy(ssid_10_20, char_supla_device_ssid+10, 11);
        ssid_10_20[11] = '\0';
        //substring mac_an_u_0_10
        char mac_an_u_0_10[12];
        strncpy(mac_an_u_0_10, mac_an_u, 11);
        mac_an_u_0_10[11] = '\0';

        //mac_u : mac uppercase
        char mac_u[18];
        for (int i = 0; i < strlen(supla_device_mac); i++)
        {
            if(!((supla_device_mac[i] >= '0') && (supla_device_mac[i] <= '9'))) //if not a number (if letter or ':')
                    {
                        if (supla_device_mac[i] == ':') //if ':'
                        {
                            mac_u[i] =  supla_device_mac[i];
                        }
                        else //if letter
                        {
                            mac_u[i] =  supla_device_mac[i] - 32; 
                        }
                        
                    }  
            else //if number
                {
                    mac_u[i] =  supla_device_mac[i];
                }
        }
        mac_u[17] = '\0';

        // //check
        // ESP_LOGI(TAG, "mac_u = %s\n", mac_u);
        

        //compare ssid_10_20 with mac_an_u_0_10 && (ssid[21]+1) with mac_an_u[11]
        if ((strcmp(ssid_10_20, mac_an_u_0_10) == 0) && (mac_an_u[11] == (char_supla_device_ssid[21]+1))) //= => correct OK
        {
            ESP_LOGI(TAG, "device OK");
            add_to_ok_list(mac_u);
        }
        else
        {
            ESP_LOGI(TAG, "device NOK");
            add_to_nok_list(mac_u);
        }
        
    }
    

    //if R0 || R1
    if (strstr(char_supla_device_ssid, "R0") || strstr(char_supla_device_ssid, "R1"))
    {
        // substring ssid_9_20 = ssid[9->19]
        char ssid_9_19[11];
        strncpy(ssid_9_19, char_supla_device_ssid+9, 10);
        ssid_9_19[10] = '\0';
        //substring mac_an_u_0_10
        char mac_an_u_0_10[12];
        strncpy(mac_an_u_0_10, mac_an_u, 11);
        mac_an_u_0_10[11] = '\0';

        //mac_u : mac uppercase
        char mac_u[18];
        for (int i = 0; i < strlen(supla_device_mac); i++)
        {
            if(!((supla_device_mac[i] >= '0') && (supla_device_mac[i] <= '9'))) //if not a number (if letter or ':')
                    {
                        if (supla_device_mac[i] == ':') //':'
                        {
                            mac_u[i] =  supla_device_mac[i];
                        }
                        else //letter
                        {
                            mac_u[i] =  supla_device_mac[i] - 32; 
                        }
                    }  
            else //if number
                {
                        mac_u[i] =  supla_device_mac[i];
                }
        }
        mac_u[17] = '\0';

        //compare ssid_9_19 with mac_an_u_0_10 && (ssid[20]+1) with mac_an_u[11]
        if ((strcmp(ssid_9_19, mac_an_u_0_10) == 0) && (mac_an_u[11] == (char_supla_device_ssid[20]+1))) //= => correct OK
        {
            ESP_LOGI(TAG, "device OK");
            add_to_ok_list(mac_u);
        }
        else
        {
            ESP_LOGI(TAG, "device NOK");
            add_to_nok_list(mac_u);
        }
    }
}



/**
 * @brief check if device mac is in ok_list.csv , if not, write it there 
 * @param _mac_u 
 */
void add_to_ok_list(char * _mac_u) //_mac_u ends with '\0' naturally => no problem in using strstr
{
    char device_in_ok_list = 0;
    char line[64]; //buffer where to store the line read from the file
    FILE * ok_file_c = fopen("/spiffs/ok_list.csv", "r");
    
    // before adding the device mac to ok_list, check if it's already there
    while (fgets(line, 64, ok_file_c) != NULL) //fgets reads a new line, returns next line, return NULL if we reached EOF
        {
            if (strstr(line, _mac_u) != NULL) //we found device mac in a line of the ok csv file
            {
                device_in_ok_list = 1 ;
                ESP_LOGI(TAG, "device already in ok_list.csv");
                device_ok = 1; 
            }
            
        }
    fclose(ok_file_c);

    if (!device_in_ok_list) //write device mac in ok_list.csv
    {   
        FILE * ok_file = fopen("/spiffs/ok_list.csv", "a"); //mode append
        if (ok_file == NULL)
        {
            ESP_LOGI(TAG, "failed to open ok_list.csv");
            return;
        } 
        
        fprintf(ok_file, "%s\n", _mac_u);
        fclose(ok_file);
        device_ok = 1; //extern in lists_lib.h ; used in display_driver.c to update ui
        ESP_LOGI(TAG, "device added to ok_list.csv");
    }
    
}


/**
 * @brief check if device mac is in nok_list.csv , if not, write it there 
 * @param _mac_u 
 */
void add_to_nok_list(char * _mac_u) //_mac_u ends with '\0' naturally => no problem in using strstr
{
    char device_in_nok_list = 0;
    char line[64]; //buffer where to store the line read from the file
    FILE * nok_file_c = fopen("/spiffs/nok_list.csv", "r");
    
    // before adding the device mac to nok_list, check if it's already there
    while (fgets(line, 64, nok_file_c) != NULL) //fgets reads a new line, returns next line, return NULL if we reached EOF
        {
            if (strstr(line, _mac_u) != NULL) //we found device mac in a line of the ok csv file
            {
                device_in_nok_list = 1 ;
                ESP_LOGI(TAG, "device already in nok_list.csv");
                device_nok = 1;  
            }
   
        }
    fclose(nok_file_c);

    if (!device_in_nok_list) //write device mac in nok_list.csv
    {   
        FILE * nok_file = fopen("/spiffs/nok_list.csv", "a"); //mode append
        if (nok_file == NULL)
        {
            ESP_LOGI(TAG, "failed to open nok_list.csv");
            return;
        } 
        
        fprintf(nok_file, "%s\n", _mac_u);
        fclose(nok_file);
        device_nok = 1; //extern in lists_lib.h ; used in display_driver.c to update ui
        ESP_LOGI(TAG, "device added to nok_list.csv");
    }
    
}





#include "scan_app.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "string.h" //to use memset


#define DEFAULT_SCAN_LIST_SIZE 10 //size of the array where scanned APs will be stored
//CONFIG_EXAMPLE_SCAN_LIST_SIZE = 10


/*******************************************************
 *                global variables
 *******************************************************/
static const char *TAG = "scan_app";

extern uint8_t * supla_device_ssid ;
extern char supla_device_mac[18] ;



/*******************************************************
 *                functions implementations
 *******************************************************/
/**
 * @brief scans wifi and returns a structure containing the supla device ssid and mac
 * @return 
 */
void wifi_scan()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    uint16_t aps_nbr = DEFAULT_SCAN_LIST_SIZE; //size of the array where some of the scanned APs will be stored = max nbr of APs we're willing to retrive
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE]; //array where the aps_nbr APs will be stored 
    uint16_t ap_count = 0; //total APs scanned
    memset(ap_info, 0, sizeof(ap_info)); //initialize elts of ap_info to 0

    //while(1) {
    esp_wifi_scan_start(NULL, true);
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count)); //nbr of scanned APs we found
    esp_wifi_scan_get_ap_records(&aps_nbr, ap_info); //get aps_nbr scanned APs and store them in the array ap_info 
    ESP_LOGI(TAG, "total APs scanned = %u, size of APs list shown = %u", ap_count, aps_nbr);

    for (int i = 0; i < aps_nbr; i++)
    {
        // ESP_LOGI(TAG, "SSID \t\t%s", ap_info[i].ssid); //ssid of the AP ; in our cas : SSID contains the MAC addr
        char mac_str[18]; //addr mac = chaine de 17 caracteres: xx:xx:xx:xx:xx:xx + \0
        //formatage de la chaine mac_str et assignation de valeurs
        sprintf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x",
                ap_info[i].bssid[0], ap_info[i].bssid[1], ap_info[i].bssid[2], ap_info[i].bssid[3], ap_info[i].bssid[4], ap_info[i].bssid[5]);
        // ESP_LOGI(TAG, "MAC \t\t%s", mac_str); //mac addr of the AP

        
        if (strstr((char *)(ap_info[i].bssid), "SOMEF") != NULL)
        {
            
            supla_device_ssid = ap_info[i].ssid;

            for (int j = 0; j < 6; j++)
            {
                supla_device_ssid[j] = (ap_info[i].ssid)[j];
            }
            
            for (int k = 0; k < 18; k++)
            {
                supla_device_mac[k] = mac_str[k];
            }

            ESP_LOGI(TAG, "SUPLA device found : SSID = %s", supla_device_ssid);
            ESP_LOGI(TAG, "                     MAC = %s", supla_device_mac);
        }
        
    }

    // supla_ssid_mac_t supla_ssid_mac =
    //     {
    //         .supla_device_ssid = supla_device_ssid,
    //         .supla_device_mac = supla_device_mac
    //     };
    //}//end while(1)
    
}
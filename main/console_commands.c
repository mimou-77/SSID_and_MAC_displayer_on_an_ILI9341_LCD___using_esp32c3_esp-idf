
#include "esp_system.h" //ESP_ERROR_CHECK(err) ?
#include "esp_log.h"
#include "esp_console.h"

#define CONFIG_CONSOLE_MAX_COMMAND_LINE_LENGTH  1024


/*-----------------------------------------------------------------------------------------------*/
/* registered console commands :                                                                 */
/*                                                                 */
/*                                                                                               */
/*-----------------------------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------------------------*/
/* global variables                                                                             */
/*-----------------------------------------------------------------------------------------------*/

static const char *TAG = "console_commands";

/*-----------------------------------------------------------------------------------------------*/
/* functions implementations                                                                             */
/*-----------------------------------------------------------------------------------------------*/


void init_console()
{
    esp_console_repl_t *repl = NULL; //Read-Eval-Print Loop
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();


    repl_config.prompt = "AP_displayer_console >>> ";
    repl_config.max_cmdline_length = CONFIG_CONSOLE_MAX_COMMAND_LINE_LENGTH;

    ESP_LOGI(TAG, "Command history disabled");

    esp_console_register_help_command(); //help command : lists all registered commands
    
    // register_system_common(); //help , restart (device), free (amount of free heap), heap (heap stats), version, gpio
    //                           //, tasks (running freertos tasks) 


    #if defined(CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG) //other options: CONFIG_ESP_CONSOLE_UART_DEFAULT
                                                    //, CONFIG_ESP_CONSOLE_USB_CDC
    esp_console_dev_usb_serial_jtag_config_t hw_config = ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_serial_jtag(&hw_config, &repl_config, &repl));
    #endif

    ESP_ERROR_CHECK(esp_console_start_repl(repl));
}

#include "esp_system.h" //ESP_ERROR_CHECK(err) ?
#include "esp_log.h"
#include "esp_console.h"
#include "esp_spiffs.h"

#include "console_commands.h"


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

/**
 * @brief configure console repl, register commands 
 */
void init_console()
{
    esp_console_repl_t *repl = NULL; //Read-Eval-Print Loop
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();


    repl_config.prompt = "AP_displayer_console >>> ";
    repl_config.max_cmdline_length = CONFIG_CONSOLE_MAX_COMMAND_LINE_LENGTH;

    ESP_LOGI(TAG, "Command history disabled");

    esp_console_register_help_command(); //help command : lists all registered commands
    register_restart();
    register_ok_list();
    register_nok_list();



    #if defined(CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG) //other options: CONFIG_ESP_CONSOLE_UART_DEFAULT
                                                    //, CONFIG_ESP_CONSOLE_USB_CDC
    esp_console_dev_usb_serial_jtag_config_t hw_config = ESP_CONSOLE_DEV_USB_SERIAL_JTAG_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_console_new_repl_usb_serial_jtag(&hw_config, &repl_config, &repl));
    #endif

    ESP_ERROR_CHECK(esp_console_start_repl(repl));
}


/**
 * @brief register the command: restart
 * 
 */
static void register_restart()
{
    const esp_console_cmd_t cmd = 
    {
        .command = "restart",
        .help = "software reset of the chip",
        .hint = NULL, //possible arguments shown when u type the cmd : cmd [arg1] [arg2]
        .func = &restart,

    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );

}

/**
 * @brief called from register_restart()
 * @param argc 
 * @param argv 
 * @return 
 */
static int restart(int argc, char ** argv)
{
    ESP_LOGI(TAG, "restarting");
    esp_restart(); //from esp_system.c
}



/**
 * @brief register the command: ok_list
 * 
 */
static void register_ok_list()
{
    const esp_console_cmd_t cmd = 
    {
        .command = "ok_list",
        .help = "show list of ok devices (content of ok_list.csv)",
        .hint = NULL, //possible arguments shown when u type the cmd : cmd [arg1] [arg2]
        .func = &ok_list,

    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );

}

/**
 * @brief called from register_ok_list()
 * @param argc 
 * @param argv 
 * @return 
 */
static int ok_list(int argc, char ** argv)
{
    ESP_LOGI(TAG, "ok devices :");
    
    //show content of ok_list.csv
    FILE * ok_file_l = fopen("/spiffs/ok_list.csv", "r");
        if (ok_file_l == NULL)
        {
            ESP_LOGI(TAG, "failed to open file: %s", "ok_file_l");
            return 1;
        }
    char line_ok[256]; //1 line of the file = 256 characters (max length)
    ESP_LOGI("", "ok_list.csv : ");
    while(fgets(line_ok, sizeof(line_ok), ok_file_l) != NULL)
    {
        ESP_LOGI("", "          %s", line_ok);
    }
    fclose(ok_file_l);

    return 0;
}



/**
 * @brief register the command: nok_list
 * 
 */
static void register_nok_list()
{
    const esp_console_cmd_t cmd = 
    {
        .command = "nok_list",
        .help = "show list of nok_devices (content of nok_list.csv)",
        .hint = NULL, //possible arguments shown when u type the cmd : cmd [arg1] [arg2]
        .func = &nok_list,

    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );

}

/**
 * @brief called from register_nok_list()
 * @param argc 
 * @param argv 
 * @return 
 */
static int nok_list(int argc, char ** argv)
{
    ESP_LOGI(TAG, "nok devices:");

    //show content of ok_list.csv
    FILE * nok_file_l = fopen("/spiffs/nok_list.csv", "r");
        if (nok_file_l == NULL)
        {
            ESP_LOGI(TAG, "failed to open file: %s", "nok_file_l");
            return 1;
        }
    char line_nok[256]; //1 line of the file = 256 characters (max length)
    ESP_LOGI("", "nok_list.csv : ");
    while(fgets(line_nok, sizeof(line_nok), nok_file_l) != NULL)
    {
        ESP_LOGI("", "          %s", line_nok);
    }
    fclose(nok_file_l);

    return 0;
    
}






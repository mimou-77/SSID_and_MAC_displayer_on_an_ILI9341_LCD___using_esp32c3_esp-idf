
#include "esp_system.h" //ESP_ERROR_CHECK(err) ?
#include "esp_log.h"
#include "esp_console.h"
#include "esp_spiffs.h"
#include "lists_lib.h"

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
    register_last_added_device();



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




/**
 * @brief register the command: last_added_device
 * 
 */
static void register_last_added_device()
{
    const esp_console_cmd_t cmd = 
    {
        .command = "last_added_device",
        .help = "last added device to ok_list of nok_list",
        .hint = NULL, //possible arguments shown when u type the cmd : cmd [arg1] [arg2]
        .func = &last_added_device,

    };
    ESP_ERROR_CHECK( esp_console_cmd_register(&cmd) );

}

/**
 * @brief called from register_last_added_device()
 * @param argc 
 * @param argv 
 * @return 
 */
static int last_added_device(int argc, char ** argv)
{       FILE * last_in_file_p = fopen("/spiffs/last_device_in.csv", "r");

    //read last_device_in_ok_list from file (not from extern variable) //file contains 1 caracter: 0 or 1
    if (last_in_file_p == NULL)
    {
        ESP_LOGI(TAG, "failed to open file: %s", "last_device_in.csv");
        return 1;
    }
    // file could be empty
    char c = fgetc(last_in_file_p); //retrive content of last_device_in.csv
    fclose(last_in_file_p);

    //where to store the last line of ok_list.csv or nok_list.csv
    char line[64];
    //check where last added device in (ok_list.csv or nok_list.csv)
    if (c) //c=1 => last added device is in ok_list.csv 
    {
        //read last line of ok_list.csv
        //
        //
        //file format : "line1\nline2\nEOF"
        //
        //
        FILE * ok_file = fopen("/spiffs/ok_list.csv", "r");
        if (ok_file == NULL)
        {
            ESP_LOGE(TAG, "Failed to open /spiffs/ok_list.csv for reading.");
            return 1;
        }
        // Get file size
        fseek(ok_file, 0, SEEK_END); //moves file pointer to position 0 from EOF 
        //ftell : nbr of bytes from beginning of the file to file pointer
        //get file size (nbr of bytes from beginning of file to end of file)
        long file_size = ftell(ok_file);
        if (file_size == 0) //Empty file
        { 
            ESP_LOGI(TAG, "/spiffs/ok_list.csv is empty. No added devices");
            fclose(ok_file);
            return 0;
        }
        long i = file_size - 1; //last character in file is EOF, start from last character before EOF and move backwards until u find '\n'
        int j = 0;
        while (i>=0)
        {
            fseek(ok_file, i, SEEK_SET); //move pointer to position EOF-1 : i position from beginning of file
            c = fgetc(ok_file); //read charcter at current position : i
            if (c == '\n')
            {
                j++;//j : nbr of '\n'
                if(j == 2)
                {
                    break; //break while
                }
            }
            i--;
        }
        //i is now at the position of the 2nd '\n'
        // 2 cases: file contains 1 line or file contains multiple lines
        // if 1 line:
        if (j < 2)
        {
            //1 line, starts at beginning of file
            //go to beginning of file
            fseek(ok_file, 0, SEEK_SET);
        }
        // if multiple lines
        else
        {
            //multiple lines, i at the 2nd '\n' => move file pointer to i+1
            fseek(ok_file, i + 1, SEEK_SET); 
        }

        //put the last line in line[64]
        fgets(line, sizeof(line), ok_file); //naturally the last line ends with '\0'
        ESP_LOGI(TAG, "last added device %s", line);
        
    }   
    else
    {
        //c != 1 => c = 0 => last added device is in nok_list.csv OR no device added
        
        FILE * nok_file = fopen("/spiffs/nok_list.csv", "r");
        if (nok_file == NULL)
        {
            ESP_LOGE(TAG, "Failed to open /spiffs/nok_list.csv for reading.");
            return 1;
        }

        // Get file size
        fseek(nok_file, 0, SEEK_END); //moves file pointer to position 0 from EOF 

        //ftell : nbr of bytes from beginning of the file to file pointer
        //get file size (nbr of bytes from beginning of file to end of file)
        long file_size = ftell(nok_file);
        if (file_size == 0) //Empty file
        { 
            ESP_LOGI(TAG, "/spiffs/nok_list.csv is empty. No added devices");
            fclose(nok_file);
            return 0;
        }
        long i = file_size - 1; //last character in file is EOF, start from last character before EOF and move backwards until u find '\n'
        int j = 0;
        while (i>=0)
        {
            fseek(nok_file, i, SEEK_SET); //move pointer to position EOF-1 : i position from beginning of file
            c = fgetc(nok_file); //read charcter at current position : i
            if (c == '\n')
            {
                j++;//j : nbr of '\n'
                if(j == 2)
                {
                    break; //break while
                }
            }
            i--;
        }
        //i is now at the position of the 2nd '\n'
        // 2 cases: file contains 1 line or file contains multiple lines
        // if 1 line:
        if (j < 2)
        {
            //1 line, starts at beginning of file
            //go to beginning of file
            fseek(nok_file, 0, SEEK_SET);
        }
        // if multiple lines
        else
        {
            //multiple lines, i at the 2nd '\n' => move file pointer to i+1
            fseek(nok_file, i + 1, SEEK_SET); 
        }

        //put the last line in line[64]
        fgets(line, sizeof(line), nok_file); //naturally the last line ends with '\0'
        ESP_LOGI(TAG, "last added device %s", line);
        

        fclose(nok_file);

    }

    return 0;
}



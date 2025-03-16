#include "btn_driver.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h" //iram_attr
#include "freertos/task.h"//xtaskcreate


/*******************************************************
 *                global variables
 *******************************************************/
static const char *TAG = "btn_driver";


/*******************************************************
 *                functions implementations
 *******************************************************/
void btns_init()
{

    gpio_install_isr_service(0);
    gpio_reset_pin(7);
    const gpio_config_t btn_ok_gpio_cfg =
    {
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = 1ULL << 7,
        .pull_down_en = GPIO_PULLDOWN_ONLY,
        .intr_type = GPIO_INTR_POSEDGE,
    };
    gpio_config(&btn_ok_gpio_cfg);
    gpio_intr_enable(7);
    gpio_isr_handler_add(7, btn_ok_isr , NULL);

    gpio_reset_pin(9);
    const gpio_config_t btn_nok_gpio_cfg =
    {
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = 1ULL << 9,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_POSEDGE,
    };
    gpio_config(&btn_nok_gpio_cfg);
    gpio_intr_enable(9);
    gpio_isr_handler_add(9, btn_nok_isr , NULL);

    ESP_LOGI(TAG, "btn_pins config done");

    xTaskCreate(btn_log_task, "btn_log_task", 2048, NULL, 10, NULL);
}


void  IRAM_ATTR btn_ok_isr()
{
    btn_ok_pressed = true;
}

void  IRAM_ATTR btn_nok_isr()
{
    btn_nok_pressed = true;
}


void btn_log_task()
{
    while(1)
    {
        if(btn_ok_pressed)
        {
            vTaskDelay(500/portTICK_PERIOD_MS);
            btn_ok_pressed = false;
            ESP_LOGI(TAG, "btn_ok pressed");
        }
        else if (btn_nok_pressed)
        {
            vTaskDelay(500/portTICK_PERIOD_MS);
            btn_ok_pressed = false;
            ESP_LOGI(TAG, "btn_nok pressed");
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
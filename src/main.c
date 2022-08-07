
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define BUILTIN_LED 2

void app_main()
{
    ESP_LOGI("E AI CARAIO", "");

    gpio_set_direction(BUILTIN_LED, GPIO_MODE_OUTPUT);

    for (;;)
    {
        gpio_set_level(BUILTIN_LED, 1);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        gpio_set_level(BUILTIN_LED, 0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
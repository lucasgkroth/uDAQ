
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#define BUILTIN_LED 2
#define BUF_SIZE (1024)
#define ECHO_TASK_STACK_SIZE 4096
#define LOG_TAG "UART"

void echo_task()
{
    ESP_LOGI("E AI CARAIO", "");

    gpio_set_direction(BUILTIN_LED, GPIO_MODE_OUTPUT);

    const uart_port_t uart_num = UART_NUM_0;

    // Configure UART parameters
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 122,
    };

    ESP_ERROR_CHECK(uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, ESP_INTR_FLAG_EDGE));
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(uart_num, GPIO_NUM_1, GPIO_NUM_3, GPIO_NUM_22, GPIO_NUM_19));

    // Configure a temporary buffer for the incoming data
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);

    // Send a constant buffer
    uint8_t *buffer[5] = {0x54, 0x45, 0x53, 0x54, 0x45};

    for (;;)
    {

        // Write data to the UART
        uart_write_bytes(uart_num, (char *)buffer, 5);

        // Read data from the UART
        int len = uart_read_bytes(uart_num, data, (BUF_SIZE - 1), 500 / portTICK_PERIOD_MS);

        if (len)
        {
            gpio_set_level(BUILTIN_LED, 1);
            data[len] = '\0';
            ESP_LOGI(LOG_TAG, "%s", data);
            ESP_LOGI("LEN", "%d", len);
        }
        gpio_set_level(BUILTIN_LED, 0);
    }
}

void app_main(void)
{
    xTaskCreate(echo_task, "uart_echo_task", ECHO_TASK_STACK_SIZE, NULL, 10, NULL);
}
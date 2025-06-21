// Copyright 2024 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <string.h>

#include <esp_amp_platform.h>

#include "app_priv.h"
#include "simple_uart.h"
#include "queue.h"

static const char *TAG = "app_driver";

#define RX_GPIO_NUM     (gpio_num_t)GPIO_NUM_4
#define TX_GPIO_NUM     (gpio_num_t)GPIO_NUM_5
#define RTS_GPIO_NUM    (gpio_num_t)(-1)
#define CTS_GPIO_NUM    (gpio_num_t)(-1)

#define RX_BUFFER_SIZE  0x40
#define TX_BUFFER_SIZE  50

// #define UART_PORT_NUM    LP_UART_NUM_0
#define UART_PORT_NUM    UART_NUM_1

int app_driver_uart_echo(void)
{
    static int count = 0;

    /* creating the tx buffer */
    // char tx_buffer[TX_BUFFER_SIZE];
    // int tx_len = snprintf(tx_buffer, TX_BUFFER_SIZE, "%d: Hello", ++count);

    // /* checking for buffer overflow */
    // if (tx_len >= 50) {
    //     printf("%s: rx buffer overflow, please increase the rx buffer size", TAG);
    // }

    /* transmiting tx_buiffer via uart port */
    // esp_err_t ret = simple_uart_write_bytes(UART_PORT_NUM, tx_buffer, tx_len, 500);
    // if (ret != ESP_OK) {
    //     printf("%s: Error in transmiting data\n", TAG);
    //     esp_amp_platform_delay_ms(200);
    //     return -1;
    // }

    /* delay of 10 ms */
    esp_amp_platform_delay_ms(10);

    /* receviing data from the rx uart port */
    char rx_buffer[RX_BUFFER_SIZE];
    int rx_len = simple_uart_read_bytes(UART_PORT_NUM, rx_buffer, RX_BUFFER_SIZE, 10);

    if (rx_len > 0) {
        Queue q;
        /* printing the received data */
        printf("%s: Received UART Data: %d\n", TAG, rx_len);
        for (int i = 0; i < rx_len; i++)
        {
            // printf("%02X", rx_buffer[i]);
            q.enqueue(rx_buffer[i]);
        }
        printf("\n");

        for (int i = 0; i < rx_len; i++)
        {
            printf("%02X\n", q.dequeue());
        }
        // q.display();
    }

    // while(simple_uart_availaible(UART_PORT_NUM)) {
    //     char buf = simple_uart_read_byte(UART_PORT_NUM);
    //     printf("%02X", buf);
    // }
    // printf("\n");

    /* delay of 1 sec */
    // esp_amp_platform_delay_ms(1000);

    return 0;
}

int app_driver_init()
{
    printf("%s: Initializing driver\n", TAG);

    /* uart driver config */
    static uart_cfg_t cfg = {
        .uart_pin_cfg = {
            .tx_io_num = TX_GPIO_NUM,
            .rx_io_num = RX_GPIO_NUM,
            .rts_io_num = RTS_GPIO_NUM,
            .cts_io_num = CTS_GPIO_NUM,
        },
        /* Default UART protocol config */
        .uart_proto_cfg = {
            .baud_rate = 256000,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .rx_flow_ctrl_thresh = 0,
        },
    };

    /* initialise the uart drvier */
    esp_err_t err = simple_uart_init(UART_PORT_NUM, cfg);
    if (err != ESP_OK) {
        printf("%s: Failed to initialise the UART driver\n", TAG);
        return -1;
    }
    printf("%s: UART driver initialized successfully\n", TAG);

    return 0;
}

int app_driver_feature_update()
{
    printf("%s: Feature update\n", TAG);
    /* Add code to handle feature updates for the device */
    /* Appropriate arguments might need to be passed to the function */

    return 0;
}

int app_driver_event_handler(low_code_event_t *event)
{
    printf("%s: Received event: %d\n", TAG, event->event_type);

    switch (event->event_type) {
        case LOW_CODE_EVENT_SETUP_MODE_START:
            printf("%s: Setup mode started\n", TAG);
            break;
        case LOW_CODE_EVENT_SETUP_MODE_END:
            printf("%s: Setup mode ended\n", TAG);
            break;
        case LOW_CODE_EVENT_SETUP_DEVICE_CONNECTED:
            printf("%s: Device connected during setup\n", TAG);
            break;
        case LOW_CODE_EVENT_SETUP_STARTED:
            printf("%s: Setup process started\n", TAG);
            break;
        case LOW_CODE_EVENT_SETUP_SUCCESSFUL:
            printf("%s: Setup process successful\n", TAG);
            break;
        case LOW_CODE_EVENT_SETUP_FAILED:
            printf("%s: Setup process failed\n", TAG);
            break;
        case LOW_CODE_EVENT_NETWORK_CONNECTED:
            printf("%s: Network connected\n", TAG);
            break;
        case LOW_CODE_EVENT_NETWORK_DISCONNECTED:
            printf("%s: Network disconnected\n", TAG);
            break;
        case LOW_CODE_EVENT_OTA_STARTED:
            printf("%s: OTA update started\n", TAG);
            break;
        case LOW_CODE_EVENT_OTA_STOPPED:
            printf("%s: OTA update stopped\n", TAG);
            break;
        case LOW_CODE_EVENT_READY:
            printf("%s: Device is ready\n", TAG);
            break;
        case LOW_CODE_EVENT_IDENTIFICATION_START:
            printf("%s: Identification started\n", TAG);
            break;
        case LOW_CODE_EVENT_IDENTIFICATION_STOP:
            printf("%s: Identification stopped\n", TAG);
            break;
        case LOW_CODE_EVENT_TEST_MODE_LOW_CODE:
            printf("%s: Low code test mode is triggered for subtype: %d\n", TAG, (int)*((int*)(event->event_data)));
            break;
        case LOW_CODE_EVENT_TEST_MODE_COMMON:
            printf("%s: common test mode triggered\n", TAG);
            break;
        case LOW_CODE_EVENT_TEST_MODE_BLE:
            printf("%s: ble test mode triggered\n", TAG);
            break;
        case LOW_CODE_EVENT_TEST_MODE_SNIFFER:
            printf("%s: sniffer test mode triggered\n", TAG);
            break;
        default:
            printf("%s: Unhandled event type: %d\n", TAG, event->event_type);
            break;
    }

    return 0;
}

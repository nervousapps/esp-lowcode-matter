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
#ifndef _app_driver_h
#define _app_driver_h

#include <stdio.h>
#include <string>

#include <system.h>
#include <low_code.h>

#include <light_driver.h>

#include <esp_log.h>

#include "app_priv.h"

#define BUTTON_GPIO_NUM ((gpio_num_t)10)

#define WS2812_CTRL_IO ((gpio_num_t)8)

#define LP_UART_PORT_NUM LP_UART_NUM_0

static const char *TAG = "app_driver";

int app_driver_init()
{
    /* Enable intterupts for LD2410 */
    system_enable_software_interrupt();

    printf("%s: Initializing light driver\n", TAG);
    light_driver_config_t light_cfg = {
        .device_type = LIGHT_DEVICE_TYPE_WS2812,
        .channel_comb = LIGHT_CHANNEL_COMB_3CH_RGB,
        .io_conf = {
            .ws2812_io = {
                .ctrl_io = WS2812_CTRL_IO,
            },
        },
        .min_brightness = 0,
        .max_brightness = 100,
    };
    light_driver_init(&light_cfg);
    light_driver_set_power(true);
    light_driver_set_hue(100);
    light_driver_set_saturation(100);
    light_driver_set_brightness(100);

    occupancy_state = system_digital_read(10);

    // printf("%s: Initializing driver\n", TAG);

    // /* uart driver config */
    // static uart_cfg_t ld2410_uart_cfg = {
    //     .uart_pin_cfg = {
    //         .tx_io_num = TX_GPIO_NUM,
    //         .rx_io_num = RX_GPIO_NUM,
    //         .rts_io_num = RTS_GPIO_NUM,
    //         .cts_io_num = CTS_GPIO_NUM,
    //     },
    //     /* Default UART protocol config */
    //     .uart_proto_cfg = {
    //         .baud_rate = LD2410_BAUD_RATE,
    //         .data_bits = UART_DATA_8_BITS,
    //         .parity = UART_PARITY_DISABLE,
    //         .stop_bits = UART_STOP_BITS_1,
    //         .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    //         .rx_flow_ctrl_thresh = 0,
    //     },
    // };

    // /* initialise the uart drvier */
    // esp_err_t err = simple_uart_init(LP_UART_PORT_NUM, ld2410_uart_cfg);
    // if (err != ESP_OK)
    // {
    //     printf("%s: Failed to initialise the UART driver\n", TAG);
    // }

    printf("%s: App driver init\n", TAG);
    return 0;
}

int app_driver_set_light_state(bool state)
{
    printf("%s: Setting light state: %s\n", TAG, state ? "ON" : "OFF");
    return light_driver_set_power(state);
}

int app_driver_set_light_brightness(uint8_t brightness)
{
    brightness = brightness * 100 / 255;
    printf("%s: Setting light brightness: %d\n", TAG, brightness);
    return light_driver_set_brightness(brightness);
}

int app_driver_set_light_hue(uint8_t hue)
{
    hue = hue * 360 / 255;
    printf("%s: Setting light hue: %d\n", TAG, hue);
    return light_driver_set_hue(hue);
}

int app_driver_set_light_saturation(uint8_t saturation)
{
    saturation = saturation * 100 / 255;
    printf("%s: Setting light saturation: %d\n", TAG, saturation);
    return light_driver_set_saturation(saturation);
}

int app_driver_set_light_temperature(uint16_t temperature)
{
    temperature = 1000000 / temperature;
    printf("%s: Setting light temperature: %d\n", TAG, temperature);
    return light_driver_set_temperature(temperature);
}

int app_driver_event_handler(low_code_event_t *event)
{
    /* Get the events. Approriate indicators should be shown to the user based on the event. */
    printf("%s: Received event: %d\n", TAG, event->event_type);
    light_effect_config_t effect_config = {
        .type = LIGHT_EFFECT_INVALID,
        .mode = LIGHT_WORK_MODE_COLOR,
        .max_brightness = 100,
        .min_brightness = 10};

    ESP_LOGI("", "App driver event : %d", event->event_type);

    /* Handle the events */
    switch (event->event_type)
    {
    case LOW_CODE_EVENT_SETUP_MODE_START:
        printf("%s: Setup mode started\n", TAG);
        /* Start Indication */
        effect_config.type = LIGHT_EFFECT_BLINK;
        light_driver_effect_start(&effect_config, 2000, 120000);
        break;
    case LOW_CODE_EVENT_SETUP_MODE_END:
        printf("%s: Setup mode ended\n", TAG);
        /* Stop Indication */
        light_driver_effect_stop();
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
        printf("%s: Low code test mode is triggered for subtype: %d\n", TAG, (int)*((int *)(event->event_data)));
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

uint8_t occupancy_value = 0;
bool set_occupancy(bool occupancy)
{
    if (occupancy)
    {
        occupancy_value = 1;
    }
    else
    {
        occupancy_value = 0;
    }
    /* Update the occupancy feature */
    low_code_feature_data_t update_data = {
        .details = {
            .endpoint_id = 2,
            .low_level{
                .matter{
                    .cluster_id = 1030, /*!< Matter cluster ID */
                    .attribute_id = 0   /*!< Matter attribute ID */
                }}},
        .value = {.type = LOW_CODE_VALUE_TYPE_UNSIGNED_INTEGER, .value_len = sizeof(uint8_t), .value = (uint8_t *)&occupancy_value},
    };
    low_code_feature_update_to_system(&update_data);
    ESP_LOGI("", "Set occupancy to : %d", occupancy_value);
    return true;
}

bool set_illuminance(int illuminance)
{
    /* Update the illuminance feature */
    low_code_feature_data_t update_data = {
        .details = {
            .endpoint_id = 2,
            .low_level{
                .matter{
                    .cluster_id = 1024, /*!< Matter cluster ID */
                    .attribute_id = 0   /*!< Matter attribute ID */
                }}},
        .value = {.type = LOW_CODE_VALUE_TYPE_UNSIGNED_INTEGER, .value_len = sizeof(uint8_t), .value = (uint8_t *)&illuminance},
    };
    low_code_feature_update_to_system(&update_data);
    ESP_LOGI("", "Set illuminance to : %d", illuminance);
    return true;
}

int get_output_pin()
{
    return system_digital_read(10);
}

void sensor_loop(MyLD2410 ld2410_sensor)
{
    ESP_LOGI("", "In sensor loop");
    // // char c = simple_uart_read_byte(LP_UART_PORT_NUM);
    // // const char *hexDigits = "0123456789ABCDEF";
    // // std::string hexStr;
    // // hexStr += hexDigits[(c >> 4) & 0x0F];
    // // hexStr += hexDigits[c & 0x0F];
    // // printf("%s : ###################### %s\n", TAG, hexStr.c_str());
    MyLD2410::Response resp = ld2410_sensor.check();
    ESP_LOGI("", "Check : %d", resp);
    if ((resp == MyLD2410::Response::DATA))
    {
        if (ld2410_sensor.presenceDetected())
        {
            ESP_LOGI("", "Presence detected");
        }
        else
        {
            ESP_LOGI("", "Presence not detected");
        }
    }
}

#endif

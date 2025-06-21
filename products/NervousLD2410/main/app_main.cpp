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
#ifndef _app_main_c
#define _app_main_c


#include <stdio.h>

#include <system.h>
#include <low_code.h>

#include <esp_log.h>

#include "lp_sw_timer.h"

#include "app_priv.h"

static const char *TAG = "app_main";

MyLD2410 ld2410_sensor = MyLD2410(UART_NUM_1, true);

void simulate_occupancy(lp_sw_timer_handle_t timer, void *args) {
    occupancy_state = !occupancy_state;
    if(set_occupancy(occupancy_state)){
        ESP_LOGI("", "Simulate occupancy to : %d", occupancy_state);
    }
    // int output_pin = get_output_pin();

    // ESP_LOGI("", "Output pin : %d", output_pin);

    // sensor_loop(ld2410_sensor);
}

void check_sensor(lp_sw_timer_handle_t timer, void *args) {
    sensor_loop(ld2410_sensor);
}

static void setup()
{
    /* Register callbacks */
    low_code_register_callbacks(feature_update_from_system, event_from_system);

    /* Initialize driver */
    app_driver_init();

    // lp_sw_timer_config_t timer_simulate_occupancy_cfg = {
    //     .periodic = true,
    //     .timeout_ms = 1000,
    //     .handler = simulate_occupancy,
    //     .arg = &occupancy_state,
    // };
    // lp_sw_timer_handle_t timer_simulate_occupancy = lp_sw_timer_create(&timer_simulate_occupancy_cfg);

    // lp_sw_timer_start(timer_simulate_occupancy);

    // system_set_pin_mode(10, INPUT);

    ld2410_sensor.begin();

    bool is_enhance = ld2410_sensor.enhancedMode(true);
    if(is_enhance)
        printf("############ In enhance mode");
    else
        printf("############ Failed enhance mode");

    lp_sw_timer_config_t timer_check_sensor_cfg = {
        .periodic = true,
        .timeout_ms = 10,
        .handler = check_sensor,
        .arg = &occupancy_state,
    };
    lp_sw_timer_handle_t timer_check_sensor = lp_sw_timer_create(&timer_check_sensor_cfg);

    lp_sw_timer_start(timer_check_sensor);
}

static void loop()
{
    /* The corresponding callbacks are called if data is received from system */
    low_code_get_feature_update_from_system();
    low_code_get_event_from_system();
}

int feature_update_from_system(low_code_feature_data_t *data)
{
    /* Get the device feature updates */
    uint16_t endpoint_id = data->details.endpoint_id;
    uint32_t feature_id = data->details.feature_id;

    // printf("Endpoint id : %d", endpoint_id);
    // ESP_LOGI("", "Endpoint id : %d", endpoint_id);
    // ESP_LOGI("", "Feature id : %u", (unsigned int)feature_id);

    if (endpoint_id == 1) {
        if (feature_id == LOW_CODE_FEATURE_ID_POWER) {  // Power
            bool power_value = *(bool *)data->value.value;
            printf("%s: Feature update: power: %d\n", TAG, power_value);
            app_driver_set_light_state(power_value);
        } else if (feature_id == LOW_CODE_FEATURE_ID_BRIGHTNESS) {  // Brightness
            uint8_t brightness = *(uint8_t *)data->value.value;
            printf("%s: Feature update: brightness: %d\n", TAG, brightness);
            app_driver_set_light_brightness(brightness);
        } else if (feature_id == LOW_CODE_FEATURE_ID_COLOR_TEMPERATURE) {  // Color temperature
            uint16_t color_temp = *(uint16_t *)data->value.value;
            printf("%s: Feature update: color temperature: %d\n", TAG, color_temp);
            app_driver_set_light_temperature(color_temp);
        } else if (feature_id == LOW_CODE_FEATURE_ID_HUE) {  // Hue
            uint8_t hue = *(uint8_t *)data->value.value;
            printf("%s: Feature update: hue: %d\n", TAG, hue);
            app_driver_set_light_hue(hue);
        } else if (feature_id == LOW_CODE_FEATURE_ID_SATURATION) {  // Saturation
            uint8_t saturation = *(uint8_t *)data->value.value;
            printf("%s: Feature update: saturation: %d\n", TAG, saturation);
            app_driver_set_light_saturation(saturation);
        }
    }

    

    return 0;
}

int event_from_system(low_code_event_t *event)
{
    /* Handle the events from low_code_event_type_t */
    return app_driver_event_handler(event);
}

extern "C" int main()
{
    printf("%s: Starting low code\n", TAG);

    /* Pre-Initializations: This should be called first and should always be present */
    system_setup();
    setup();

    /* Loop */
    while (1) {
        system_loop();
        loop();
    }
    return 0;
}

#endif
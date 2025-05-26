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

// #pragma once
#ifndef _app_priv_h
#define _app_priv_h

#include <stdint.h>
#include <low_code.h>

#include "lp_sw_timer.h"
#include "simple_uart.h"
// #include "ld2410.h"
#include "MyLD2410.h"

/* Driver functions */
int app_driver_init();
int app_driver_set_light_state(bool state);
int app_driver_set_light_brightness(uint8_t brightness);
int app_driver_set_light_temperature(uint16_t temperature);
int app_driver_set_light_hue(uint8_t hue);
int app_driver_set_light_saturation(uint8_t saturation);
bool set_occupancy(bool occupancy);
bool set_illuminance(int illuminance);
int get_output_pin();
static bool occupancy_state = false;

void sensor_loop(MyLD2410 ld2410_sensor);

void simulate_occupancy(lp_sw_timer_handle_t timer, void *args);

/* Events handler */
int app_driver_event_handler(low_code_event_t *event);

/* Callbacks from system */
int feature_update_from_system(low_code_feature_data_t *data);
int event_from_system(low_code_event_t *event);

#endif

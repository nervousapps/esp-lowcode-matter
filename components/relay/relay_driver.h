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

/**
 * @file relay_driver.h
 * @brief GPIO-based relay control driver
 *
 * This component provides functionality to control relay modules through GPIO pins.
 * It supports basic relay operations like initialization and power control.
 */

#pragma once

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the relay driver for a specific GPIO
 *
 * This function initializes the relay driver for a given GPIO pin.
 * It configures the GPIO as an output for relay control.
 *
 * @param gpio_num The GPIO number to use for relay control
 */
void relay_driver_init(int gpio_num);

/**
 * @brief Set the power state of the relay
 *
 * This function controls the power state of the relay connected to the specified GPIO.
 *
 * @param gpio_num The GPIO number connected to the relay
 * @param power true to turn the relay on, false to turn it off
 */
void relay_driver_set_power(int gpio_num, bool power);

#ifdef __cplusplus
}
#endif


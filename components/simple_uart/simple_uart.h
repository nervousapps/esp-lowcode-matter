/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include <hal/uart_types.h>

#include <lp_core_uart.h>
#include <esp_err.h>

typedef lp_core_uart_cfg_t uart_cfg_t;

/**
 * @brief initialise the UART peripheral
 *
 * @param uart_port     uart port to use
 * @param cfg           lp_core_uart_cfg_t configuration for uart
 *
 * @return esp_err_t    ESP_OK when successful
 *
 */
esp_err_t simple_uart_init(uart_port_t uart_num, uart_cfg_t cfg);

/**
 * @brief Read data from the UART port
 *
 * This function will read data from the Rx FIFO. If a timeout value is configured, then this function will timeout once the number of CPU cycles expire.
 *
 * @param uart_num      UART port number
 * @param buf           data buffer address
 * @param size          data length to send
 * @param timeout       Operation timeout in CPU cycles. Set to -1 to wait forever.
 *
 * @return              - (-1) Error
 *                      - OTHERS (>=0) The number of bytes read from the Rx FIFO
 */
int simple_uart_read_bytes(uart_port_t uart_num, void *buf, size_t size, int32_t timeout);

/**
 * @brief Write data to the UART port
 *
 * This function will write data to the Tx FIFO. If a timeout value is configured, this function will timeout once the number of CPU cycles expire.
 *
 * @param uart_num      UART port number
 * @param src           data buffer address
 * @param size          data length to send
 * @param timeout       Operation timeout in CPU cycles. Set to -1 to wait forever.
 *
 * @return esp_err_t    ESP_OK when successful
 */
esp_err_t simple_uart_write_bytes(uart_port_t uart_num, const void *src, size_t size, int32_t timeout);


int simple_uart_availaible(uart_port_t uart_num);

int simple_uart_read_byte(uart_port_t uart_num);

#ifdef __cplusplus
}
#endif

#include <stdint.h>
#include <stdbool.h>
#include "myserial.h"
#include "ulp_lp_core_uart.h"
#include "ulp_lp_core_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LP_UART_PORT_NUM LP_UART_NUM_0

uint8_t* serial_read(void)
{
        uint8_t data[1024] = {0};
        int len = 0;

        /* Read data from the LP_UART */
        len = lp_core_uart_read_bytes(LP_UART_PORT_NUM, data, (sizeof(data) - 1), 10);

        return data;
}

bool serial_write(uint8_t data, int len)
{
        esp_err_t ret;
        /* Write data back to the LP_UART */
        ret = lp_core_uart_write_bytes(LP_UART_PORT_NUM, (const char *)data, len, 500);
        if (ret != ESP_OK) {
                /* Error in writing. Bail */
                return false;
        }
        return true;
}

#ifdef __cplusplus
}
#endif
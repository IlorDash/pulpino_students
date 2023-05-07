
#define __riscv__
#define LED_DELAY 10

//#include <spi.h>
#include <gpio.h>
#include <uart.h>
//#include <utils.h>
#include <pulpino.h>

#include "cipher.h"


int main() {
    uart_set_cfg(0, 27); // 115200 baud UART, no parity (50MHz CPU)

    set_pin_function(30, FUNC_GPIO);
    set_gpio_pin_direction(30, DIR_OUT);

    set_pin_function(29, FUNC_GPIO);
    set_gpio_pin_direction(29, DIR_OUT);

    set_pin_function(28, FUNC_GPIO);
    set_gpio_pin_direction(28, DIR_OUT);

    set_gpio_pin_value(30, 0);

    cipher_init();

    uint8_t src_data[CIPHER_DATA_BYTES_NUM];
    uint8_t dst_data[CIPHER_DATA_BYTES_NUM];

    while (1) {
        set_gpio_pin_value(30, 1);
        for (int i = 0; i < CIPHER_DATA_BYTES_NUM; i++) {
            src_data[i] = (uint8_t)uart_getchar();
        }
        set_gpio_pin_value(30, 0);
        if (!cipher_data((uint32_t*)src_data, (uint32_t*)dst_data)) {
            uart_send(dst_data, CIPHER_DATA_BYTES_NUM);
            uart_wait_tx_done();
        } else {
            const char err_msg[] = "Error!\n";
            uart_send(err_msg, strlen(err_msg));
            uart_wait_tx_done();
        }
    }
}

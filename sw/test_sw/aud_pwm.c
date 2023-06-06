#include "aud_pwm.h"
#include <uart.h>

volatile struct AUD_PWM_APB *aud_pwm;

void aud_pwm_start(void)
{
    // aud_pwm->START = 0;
    // for (int i = 0; i < 100; i++)
    // {
    // }
    uart_send("6) aud START=0\n", 15);
    uart_wait_tx_done();
    aud_pwm->START = 0xffffffff;
    uart_send("7) aud START=1\n", 15);
    uart_wait_tx_done();
}

void aud_pwm_reset(void)
{
    aud_pwm->RST = 0xffffffff;
    for (int i = 0; i < 100; i++)
    {
    }
    aud_pwm->RST = 0;
}

void aud_pwm_init(void)
{
    uart_send("1) FOO\n", 7);
    uart_wait_tx_done();
    CGREG |= (1 << CGAUD_PWM);

    uart_send("2) FOO\n", 7);
    uart_wait_tx_done();
    aud_pwm = (struct AUD_PWM_APB *)(AUD_PWM_BASE_ADDR);
    uart_send("3) FOO\n", 7);
    uart_wait_tx_done();

    aud_pwm->RST = 0xffffffff;
    uart_send("4) FOO\n", 7);
    uart_wait_tx_done();

    for (int i = 0; i < 100; i++)
    {
    }

    aud_pwm->RST = 0;
    uart_send("5) FOO\n", 5);
    uart_wait_tx_done();

    aud_pwm->START = 0;
}

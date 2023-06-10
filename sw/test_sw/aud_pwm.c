#include "aud_pwm.h"
#include <uart.h>

volatile struct AUD_PWM_APB *aud_pwm;

void aud_pwm_start(void)
{
    aud_pwm->START = 0xffffffff;
}

void aud_pwm_stop(void)
{
    aud_pwm->START = 0;
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
    CGREG |= (1 << CGAUD_PWM);

    aud_pwm = (struct AUD_PWM_APB *)(AUD_PWM_BASE_ADDR);

    aud_pwm->RST = 0xffffffff;

    for (int i = 0; i < 100; i++)
    {
    }
    aud_pwm->RST = 0;
    aud_pwm->START = 0;
}

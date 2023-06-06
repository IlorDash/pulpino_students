#include "aud_pwm.h"

volatile struct AUD_PWM_APB *aud_pwm;

void aud_pwm_start(void)
{
    aud_pwm->START = 0;
    for (int i = 0; i < 100; i++)
    {
    }
    aud_pwm->RST = 1;
}

void aud_pwm_reset(void)
{
    aud_pwm->RST = 1;
    for (int i = 0; i < 100; i++)
    {
    }
    aud_pwm->RST = 0;
}

void aud_pwm_init(void)
{
    CGREG |= (1 << CGAUD_PWM);
    aud_pwm = (struct AUD_PWM_APB *)(AUD_PWM_BASE_ADDR);
    aud_pwm->RST = 1;
    for (int i = 0; i < 100; i++)
    {
    }
    aud_pwm->RST = 0;
    aud_pwm->START = 0;
}

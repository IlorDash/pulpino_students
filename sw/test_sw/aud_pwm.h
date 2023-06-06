#ifndef _AUD_PWM_H
#define _AUD_PWM_H

#include <pulpino.h>
#include <stdint.h>

__attribute__((packed)) struct AUD_PWM_APB
{
    uint32_t START;   // 0x0
    uint32_t RST; // 0x4
};

void aud_pwm_init(void);
void aud_pwm_start();
void aud_pwm_reset();

#endif

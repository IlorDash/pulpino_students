#ifndef _SEG7_CONTROL_H
#define _SEG7_CONTROL_H

#include <pulpino.h>
#include <stdint.h>

#define NUMS_TO_DISP_NUM 3

__attribute__((packed)) struct SEG7_CONTROL_APB
{
    uint32_t NUM;   // 0x0
    uint32_t RESET; // 0x4
};

void seg7_control_init(void);
int seg7_control_disp_nums(int8_t nums[]);

#endif

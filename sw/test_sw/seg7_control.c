#include "seg7_control.h"

volatile struct SEG7_CONTROL_APB *seg7_control;

void seg7_control_set_nums(int8_t nums[])
{
    int nums_in_octets = 0;
    for (int i = 0; i < NUMS_TO_DISP_NUM; i++)
    {
        if (nums[i] < 0)
        {
            nums[i] *= -1;
            while (nums[i] / 10)
            {
                nums[i] /= 10;
            }
            // Left shift by 24, 12, 0 bits in order to leave empty octets for spaces
            nums_in_octets |= (0xf0 | (nums[i] & 0xf)) << (8 * (NUMS_TO_DISP_NUM - i) - 4 * i);
            printf("-) %d %x; %x\n", nums[i], (nums[i] & 0xf), nums_in_octets);
            continue;
        }

        while (nums[i] / 100)
        {
            nums[i] /= 10;
        }
        // Left shift by 24, 12, 0 bits in order to leave empty octets for spaces
        nums_in_octets |= ((((nums[i] / 10) & 0xf) << 4) | ((nums[i] % 10) & 0xf)) << (8 * (NUMS_TO_DISP_NUM - i) - 4 * i);
        printf("+) %d; %x\n", nums[i], nums_in_octets);
    }

    seg7_control->NUM = nums_in_octets;
}

void seg7_control_init(void)
{
    CGREG |= (1 << CGSPI_SEG7_CONTROL);
    seg7_control = (struct SEG7_CONTROL_APB *)(SEG7_CONTROL_BASE_ADDR);
    seg7_control->RESET = 1;
}

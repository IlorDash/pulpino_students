#include <spi_accel.h>

#define DATA_READY_TIMEOUT 100000000

volatile struct SPI_ACCEL_APB *spi_accel;

int spi_accel_get_data(struct spi_accel_data *data)
{
    int cntr = 0;
    while ((!spi_accel->DATA_READY) && (cntr < DATA_READY_TIMEOUT))
    {
        // wait some time
#ifdef __riscv__
        asm volatile("nop");
#else
        asm volatile("l.nop");
#endif
        cntr++;
    }

    if (cntr == DATA_READY_TIMEOUT)
    {
        return -1;
    }

    data->data_x = spi_accel->DATA_X;
    data->data_y = spi_accel->DATA_Y;
    data->data_z = spi_accel->DATA_Z;
    return 0;
}

void spi_accel_init(void)
{
    CGREG |= (1 << CGSPI_ACCEL);
    spi_accel = (struct SPI_ACCEL_APB *)(SPI_ACCEL_BASE_ADDR);
}
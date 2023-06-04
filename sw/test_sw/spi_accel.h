#ifndef _SPI_ACCEL_H
#define _SPI_ACCEL_H

#define DATA_HEX_NUM 4
#define RANGE 2 // in g

#include <pulpino.h>
#include <stdint.h>

__attribute__((packed)) struct SPI_ACCEL_APB
{
    uint32_t DATA_X; // 0x0
    uint32_t DATA_Y; // 0x4
    uint32_t DATA_Z; // 0x8

    uint32_t DATA_READY; // 0xc
};

struct spi_accel_regs
{
    uint32_t data_x;
    uint32_t data_y;
    uint32_t data_z;
};

struct spi_accel_real_data
{
    uint16_t x;
    uint16_t y;
    uint16_t z;
};

// Cipher registers
#define SPI_ACCEL_DATA_X 0x0
#define SPI_ACCEL_DATA_Y 0x4
#define SPI_ACCEL_DATA_Z 0x8
#define SPI_ACCEL_DATA_READY 0xc

void spi_accel_init(void);
int spi_accel_get_data(struct spi_accel_regs *data);

#endif

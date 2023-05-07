#ifndef _CIPHER_H
#define _CIPHER_H

#include <stdint.h>

#define CIPHER_DATA_BYTES_NUM 16

__attribute__((packed)) struct CIPHER_APB {
    uint32_t RST;   // 0x0
    uint32_t REQ;   // 0x4
    uint32_t ACK;   // 0x8

    uint32_t VALID; // 0xc
    uint32_t BUSY;  // 0x10

    uint32_t DATA_IN[4];    // 0x14 - 0x20
    uint32_t DATA_OUT[4];   // 0x24 - 0x30
};

// Cipher registers
#define CIPHER_ADDR_RST 0x0
#define CIPHER_ADDR_REQ 0x4
#define CIPHER_ADDR_ACK 0x8
#define CIPHER_ADDR_VALID 0xc
#define CIPHER_ADDR_BUSY 0x10
#define CIPHER_ADDR_DATA_IN_0 0x14
#define CIPHER_ADDR_DATA_IN_1 0x18
#define CIPHER_ADDR_DATA_IN_2 0x1c
#define CIPHER_ADDR_DATA_IN_3 0x20
#define CIPHER_ADDR_DATA_OUT_0 0x24
#define CIPHER_ADDR_DATA_OUT_1 0x28
#define CIPHER_ADDR_DATA_OUT_2 0x2c
#define CIPHER_ADDR_DATA_OUT_3 0x30

void cipher_init(void);
int cipher_data(const uint32_t* src, uint32_t* dst);

#endif

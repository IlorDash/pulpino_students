#include <pulpino.h>
#include <gpio.h>
#include "cipher.h"

#define MAX_WAIT_CYCLES 100

volatile struct CIPHER_APB* cipher;

int wait_cipher_free(int wait_cycles) {
    int cur_cycle = wait_cycles;
    while ((cur_cycle != 0) && (cipher->BUSY != 0)) {
        cur_cycle--;
    }
    return (cur_cycle != 0) ? 1 : 0;
}

int wait_cipher_valid(int wait_cycles) {
    int cur_cycle = wait_cycles;
    while ((cur_cycle != 0) && (cipher->VALID == 0)) {
        cur_cycle--;
    }
    return (cur_cycle != 0) ? 1 : 0;
}

void req_cipher() {
    cipher->REQ = 1;
    int i = 0;
    while(i < 2){   // wait 2 clocks as it made in lab 2 testbench
        i++;
    }
    cipher->REQ = 0;
}

void ack_cipher() {
    cipher->ACK = 1;
    int i = 0;
    while(i < 2){   // wait 2 clocks as it made in lab 2 testbench
        i++;
    }
    cipher->ACK = 0;
}

int cipher_data(const uint32_t* src, uint32_t* dst) {

    for (int i = 0; i < CIPHER_DATA_BYTES_NUM / 4; i++) {
        cipher->DATA_IN[i] = src[i];
    }

    // wait for previous request finished
    if (!wait_cipher_free(MAX_WAIT_CYCLES)) {
        cipher->RST = 0;
        return -1;
    }
    req_cipher();

    if (!wait_cipher_valid(MAX_WAIT_CYCLES)) {
        cipher->RST = 0;
        return -1;
    }

    for (int i = 0; i < CIPHER_DATA_BYTES_NUM / 4; i++) {
        dst[i] = cipher->DATA_OUT[i];
    }
    ack_cipher();

    return 0;
}

void cipher_init(void) {
    CGREG |= (1 << CGCIPHER);
    cipher = (struct CIPHER_APB*)(CIPHER_BASE_ADDR);
    cipher->RST = 1;
}

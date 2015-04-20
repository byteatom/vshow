#include "endian.h"

int endian(void) {
    union {
        uint32_t value;
        uint8_t data[sizeof(uint32_t)];
    } number;

    number.data[0] = 0x00;
    number.data[1] = 0x01;
    number.data[2] = 0x02;
    number.data[3] = 0x03;

    switch (number.value) {
    case UINT32_C(0x00010203):
        return ENDIAN_BIG;
    case UINT32_C(0x03020100):
        return ENDIAN_LITTLE;
    case UINT32_C(0x02030001):
        return ENDIAN_BIG_WORD;
    case UINT32_C(0x01000302):
        return ENDIAN_LITTLE_WORD;
    default:
        return ENDIAN_UNKNOWN;
    }
}

int ENDIAN = endian();

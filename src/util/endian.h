#ifndef MISC_H
#define MISC_H

#include "stdint.h"

enum {
  ENDIAN_UNKNOWN,
  ENDIAN_BIG,
  ENDIAN_LITTLE,
  ENDIAN_BIG_WORD,   /* Middle-endian, Honeywell 316 style */
  ENDIAN_LITTLE_WORD /* Middle-endian, PDP-11 style */
};

extern int ENDIAN;

#define swap2(x) \
    ((((uint16_t)(x) >> 8) & 0x00FF) | \
    (((uint16_t)(x)  << 8) & 0xFF00))

#define swap4(x) \
    ((((uint32_t)(x) >> 24) & 0x000000FF) | \
    (((uint32_t)(x)  >>  8) & 0x0000FF00) | \
    (((uint32_t)(x)  <<  8) & 0x00FF0000) | \
    (((uint32_t)(x)  << 24) & 0xFF000000))

#define hton2(x) (ENDIAN == ENDIAN_LITTLE ? swap2(x) : x )
#define hton4(x) (ENDIAN == ENDIAN_LITTLE ? swap4(x) : x )
#define ntoh2(x) hton2(x)
#define ntoh4(x) hton4(x)

#endif // MISC_H


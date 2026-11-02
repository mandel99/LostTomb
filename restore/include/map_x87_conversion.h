#ifndef RESTORE_MAP_X87_CONVERSION_H
#define RESTORE_MAP_X87_CONVERSION_H
#include <stdint.h>
/* Explicit input ABI for the value consumed from native ST0. Result contains
   signed64 two's-complement bits. All x87 exceptions must be masked in the
   compared domain. This helper neither reads nor changes hardware FP state. */
uint64_t restore_map_trunc80(const uint8_t raw80[10]);
#endif

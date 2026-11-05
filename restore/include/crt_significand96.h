#ifndef RESTORE_CRT_SIGNIFICAND96_H
#define RESTORE_CRT_SIGNIFICAND96_H
#include <stdint.h>

/* Original MSVC i386 cdecl interfaces; no incoming scratch registers needed.
   All addresses wrap at32 bits and bit arithmetic retains signed IDIV rules. */
uint32_t restore_crt96_increment_bit(uint32_t *words,int32_t bit);
uint32_t restore_crt96_round_and_clear_tail(uint32_t *words,int32_t bit);
uint32_t FUN_00568925(int32_t words,int32_t bit);
uint32_t FUN_0056896e(int32_t words,int32_t bit);
uint32_t FUN_005689c4(int32_t words,int32_t bit);
uint32_t FUN_00569e0c(uint32_t left,uint32_t right,uint32_t *result);
#endif

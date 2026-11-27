#ifndef PHARAOH_RUNTIME_CRT_X87_MATH_H
#define PHARAOH_RUNTIME_CRT_X87_MATH_H

#include <stdint.h>

/* These live x87 entries require MSVC Win32. quiet_nan and set_exponent
 * retain/produce the actual ST0 value; their C signatures do not round or
 * pop it into binary64. quiet_nan's scalar result is the complete Native EAX. */
uint32_t pharaoh_crt_x87_quiet_nan_live(uint32_t classification);
void pharaoh_crt_x87_set_exponent_live(uint64_t value,
    int32_t exponent);
int32_t pharaoh_crt_x87_clear_status(void);
void pharaoh_crt_x87_generate_exceptions(uint32_t exceptions);

#endif

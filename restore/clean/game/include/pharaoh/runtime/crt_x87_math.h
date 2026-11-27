#ifndef PHARAOH_RUNTIME_CRT_X87_MATH_H
#define PHARAOH_RUNTIME_CRT_X87_MATH_H

#include <stdint.h>

/* These live x87 entries require MSVC Win32. quiet_nan, set_exponent and frexp
 * retain/produce the actual ST0 value; their C signatures do not round or
 * pop it into binary64. quiet_nan's scalar result is the complete Native EAX. */
uint32_t pharaoh_crt_x87_quiet_nan_live(uint32_t classification);
void pharaoh_crt_x87_set_exponent_live(uint64_t value,
    int32_t exponent);
int32_t pharaoh_crt_x87_clear_status(void);
void pharaoh_crt_x87_generate_exceptions(uint32_t exceptions);

void pharaoh_crt_x87_frexp_live(uint64_t value, int32_t *exponent);
uint32_t pharaoh_crt_x87_handle_masked_error(uint32_t exceptions,
    uint64_t *result, uint32_t control);

/* Native shared DWORD: FSTCW saves the low WORD; normalization writes
 * the upper WORD. No incoming upper garbage becomes a C API input. */
typedef struct PharaohCrtX87ControlSlot {
    uint16_t original;
    uint16_t upper;
} PharaohCrtX87ControlSlot;

/* The scalar normalization result represents Native EDX. C parents retain
 * their actual classification across this call, as Native preserves EAX. */
uint32_t pharaoh_crt_x87_normalize_control_live(PharaohCrtX87ControlSlot *slot);
int32_t pharaoh_crt_x87_status_wait(void);
int32_t pharaoh_crt_x87_control_word(uint32_t value, uint32_t mask);

#endif

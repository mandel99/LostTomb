#ifndef PHARAOH_RUNTIME_CRT_MATH_DISPATCH_H
#define PHARAOH_RUNTIME_CRT_MATH_DISPATCH_H

#include <stdint.h>

typedef struct PharaohCrtMathExceptionRecord {
    int32_t type;
    const char *name;
    uint64_t arg1;
    uint64_t arg2;
    uint64_t result;
} PharaohCrtMathExceptionRecord;

/* Whole0056971F. Globals605490/1266430 are the actual shared mode/error
 * cells. The mode remains live until the Native post-control-load gate. */
void pharaoh_crt_math_dispatch(uint32_t operation,
    PharaohCrtMathExceptionRecord *record, const uint16_t *saved_control,
    const volatile uint32_t *math_hook_mode, int32_t *error_code);

#endif

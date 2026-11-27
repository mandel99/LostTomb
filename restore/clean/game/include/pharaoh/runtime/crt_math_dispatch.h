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

/* Native567517/56760B have shared-stack/register entry contracts. These C
 * forms expose their logical inputs and retain the actual live ST0 result. */
void pharaoh_crt_math_one_arg_error_live(int32_t type, const char *name,
    uint32_t operation, uint64_t operand, const uint16_t *saved_control,
    const volatile uint32_t *math_hook_mode, int32_t *error_code);
void pharaoh_crt_math_finish_live(const uint16_t *saved_control,
    uint64_t operand, uint32_t operation, const char *name,
    const volatile uint32_t *math_hook_mode, int32_t *error_code);

#endif

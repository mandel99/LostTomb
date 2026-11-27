#ifndef PHARAOH_RUNTIME_CRT_ASIN_H
#define PHARAOH_RUNTIME_CRT_ASIN_H

#include <stdint.h>

/* Actual process cells12663C0/605490/1266430; no default state is invented. */
typedef struct PharaohCrtMathEnvironment {
    const volatile uint32_t *bypass_errors;
    const volatile uint32_t *hook_mode;
    int32_t *error_code;
} PharaohCrtMathEnvironment;

/* Native562D00/562D1D consume live ST0 and retain its extended result.
 * Core classification is the actual5675E8 result for the binary64 snapshot. */
void pharaoh_crt_asin_live(const PharaohCrtMathEnvironment *environment);
void pharaoh_crt_asin_core_live(uint32_t classification, uint64_t operand,
    const PharaohCrtMathEnvironment *environment);

#endif

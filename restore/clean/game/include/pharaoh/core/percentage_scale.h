#ifndef PHARAOH_CORE_PERCENTAGE_SCALE_H
#define PHARAOH_CORE_PERCENTAGE_SCALE_H

#include <stdint.h>

typedef enum PharaohPercentageScaleStatus {
    PHARAOH_PERCENTAGE_SCALE_VALUE,
    PHARAOH_PERCENTAGE_SCALE_SIGNED_DIVIDE_OVERFLOW
} PharaohPercentageScaleStatus;

typedef struct PharaohPercentageScaleResult {
    PharaohPercentageScaleStatus status;
    int32_t value;
} PharaohPercentageScaleResult;

/* Original entry 0x004CDE60. The product wraps to 32 bits before signed
   division. A zero denominator returns zero; x86 IDIV overflow is reported
   explicitly because the native instruction raises a divide exception. */
PharaohPercentageScaleResult pharaoh_scale_value_to_percent(
    int32_t value, int32_t denominator);

#endif

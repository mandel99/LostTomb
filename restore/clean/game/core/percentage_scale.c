#include "pharaoh/core/percentage_scale.h"

#include <limits.h>

static int64_t signed_dword_value(uint32_t bits)
{
    if ((bits & UINT32_C(0x80000000)) != 0) {
        return (int64_t)bits - INT64_C(0x100000000);
    }
    return bits;
}

PharaohPercentageScaleResult pharaoh_scale_value_to_percent(
    int32_t value, int32_t denominator)
{
    PharaohPercentageScaleResult result = {
        PHARAOH_PERCENTAGE_SCALE_VALUE, 0
    };

    if (denominator == 0) {
        return result;
    }

    const uint32_t product_bits = (uint32_t)value * UINT32_C(100);
    const int64_t signed_product = signed_dword_value(product_bits);
    const int64_t quotient = signed_product / denominator;

    if (quotient < INT32_MIN || quotient > INT32_MAX) {
        result.status = PHARAOH_PERCENTAGE_SCALE_SIGNED_DIVIDE_OVERFLOW;
        return result;
    }

    result.value = (int32_t)quotient;
    return result;
}

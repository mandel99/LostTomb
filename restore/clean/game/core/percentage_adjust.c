#include "pharaoh/core/percentage_adjust.h"

static int64_t signed_dword_value(uint32_t bits)
{
    if ((bits & UINT32_C(0x80000000)) != 0) {
        return (int64_t)bits - INT64_C(0x100000000);
    }
    return bits;
}

int32_t pharaoh_adjust_by_percentage(int32_t value, int32_t percentage)
{
    const uint32_t product_bits = (uint32_t)value * (uint32_t)percentage;
    const int64_t signed_product = signed_dword_value(product_bits);

    return (int32_t)(signed_product / 100);
}

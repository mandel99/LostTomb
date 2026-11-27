#include "pharaoh/core/integer_clamp.h"

int32_t pharaoh_int32_divide_round_up(int32_t value, int32_t divisor)
{
    int32_t result;
    if (divisor == 0) return 0;
    result = value / divisor;
    if (value % divisor != 0) ++result;
    return result;
}

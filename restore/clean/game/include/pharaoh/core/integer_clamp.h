#ifndef PHARAOH_CORE_INTEGER_CLAMP_H
#define PHARAOH_CORE_INTEGER_CLAMP_H

#include <stdint.h>

int32_t pharaoh_int32_clamp(int32_t value, int32_t minimum, int32_t maximum);
int32_t pharaoh_int32_divide_round_up(int32_t value, int32_t divisor);
void pharaoh_int32_add_clamped_0_100(int32_t *value, int32_t delta);
int32_t pharaoh_hundreds_max_index(const int32_t values[4]);

#endif

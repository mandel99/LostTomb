#ifndef PHARAOH_CORE_X87_NUMERIC_H
#define PHARAOH_CORE_X87_NUMERIC_H

#include <stdint.h>

#pragma pack(push,1)
typedef struct PharaohFloat80 {
    uint64_t mantissa;
    uint16_t exponent_sign;
} PharaohFloat80;
#pragma pack(pop)
_Static_assert(sizeof(PharaohFloat80)==10,"native TBYTE");

/* Typed memory input replaces the native live ST(0) argument. The current
 * thread's real x87 control word remains an inherited machine input. */
int64_t pharaoh_x87_truncate80(const PharaohFloat80 *value);
int32_t pharaoh_x87_scaled_progress(int32_t stage, const double *step);
int32_t pharaoh_x87_tomb_percent(int32_t sum, int32_t count);
int32_t pharaoh_x87_integer_ratio(int32_t numerator, int32_t denominator);
int32_t pharaoh_x87_floodplain_fertility(uint32_t fertility);
int32_t pharaoh_x87_i32_multiply_pair(int32_t value,const double *first,const double *second);
int32_t pharaoh_x87_i32_divide_multiply(int32_t value,const double *divisor,const double *scale);

extern const double pharaoh_library_progress_step;
extern const double pharaoh_abu_simbel_progress_step;

int32_t pharaoh_x87_scaled_integer_ratio(int32_t numerator,int32_t denominator,const double *scale);

uint32_t pharaoh_x87_route_length(int32_t delta_x,int32_t delta_y);
uint32_t pharaoh_x87_route_ratio(int32_t delta,uint32_t length_bits);
void pharaoh_x87_route_coordinate(int32_t origin,uint32_t ratio_bits,
    int32_t distance,PharaohFloat80 *value);
uint32_t pharaoh_x87_route_compare(int32_t integer,uint32_t float_bits);

#endif

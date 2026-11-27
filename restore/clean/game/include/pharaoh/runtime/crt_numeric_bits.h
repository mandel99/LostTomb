#ifndef PHARAOH_RUNTIME_CRT_NUMERIC_BITS_H
#define PHARAOH_RUNTIME_CRT_NUMERIC_BITS_H

#include <stdint.h>

#pragma pack(push, 1)
typedef struct PharaohCrtDoubleBits {
    uint32_t low;
    uint32_t high;
} PharaohCrtDoubleBits;

typedef struct PharaohCrtExtended80Bits {
    uint32_t significand_low;
    uint32_t significand_high;
    uint16_t sign_exponent;
} PharaohCrtExtended80Bits;

typedef struct PharaohCrtFloatFormatBindings {
    uintptr_t convert_primary;
    uintptr_t format_a;
    uintptr_t assign;
    uintptr_t format_b;
    uintptr_t format_c;
    uintptr_t convert_secondary;
} PharaohCrtFloatFormatBindings;
#pragma pack(pop)

void pharaoh_crt_shift_right_96(uint32_t words[3], int32_t bit_count);
void pharaoh_crt_double_to_extended80(
    PharaohCrtExtended80Bits *destination,
    const PharaohCrtDoubleBits *source);
uint64_t pharaoh_crt_unsigned_divide_64(
    uint64_t dividend, uint64_t divisor);
uint64_t pharaoh_crt_unsigned_remainder_64(
    uint64_t dividend, uint64_t divisor);
uint32_t pharaoh_crt_x87_control_to_flags(uint32_t control_word);
uint32_t pharaoh_crt_flags_to_x87_control(uint32_t flags);
uint16_t pharaoh_crt_default_x87_control_word(uint32_t argument);
uint32_t pharaoh_crt_double_exponent_class(uint32_t high_word);
int32_t pharaoh_crt_x87_status_word(int16_t status_word);
int32_t pharaoh_crt_update_x87_control_word(
    int16_t original_control,
    uint32_t requested_bits,
    uint32_t mask,
    uint16_t *updated_control);
void pharaoh_crt_float_format_bindings_initialize(
    PharaohCrtFloatFormatBindings *bindings,
    uintptr_t convert,
    uintptr_t format_a,
    uintptr_t assign,
    uintptr_t format_b,
    uintptr_t format_c);
void pharaoh_crt_copy_96(uint32_t destination[3], const uint32_t source[3]);
void pharaoh_crt_clear_96(uint32_t words[3]);
uint32_t pharaoh_crt_is_zero_96(const uint32_t words[3]);
uint32_t pharaoh_crt_tail_is_zero_96(
    const uint32_t words[3], int32_t bit_index);
uint32_t pharaoh_crt_add_u32_with_carry(
    uint32_t left, uint32_t right, uint32_t *sum);
void pharaoh_crt_shift_left_one_96(uint32_t words[3]);
void pharaoh_crt_shift_right_one_96(uint32_t words[3]);

#endif

#include "pharaoh/runtime/crt_numeric_bits.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohCrtDoubleBits) == 8,
               "native double bit container must be 8 bytes");
_Static_assert(sizeof(PharaohCrtExtended80Bits) == 10 &&
               offsetof(PharaohCrtExtended80Bits, sign_exponent) == 8,
               "native x87 extended container must be 10 bytes");

/* Complete zero-callee C translation of Pharaoh.exe 0x00568A92. */
void pharaoh_crt_shift_right_96(uint32_t words[3], int32_t bit_count)
{
    const int32_t word_count = bit_count / 32;
    const uint32_t bit_shift = (uint32_t)(bit_count % 32) & 31u;
    const uint32_t mask = bit_shift == 0u ? 0u :
        (UINT32_C(1) << bit_shift) - 1u;
    uint32_t carry = 0;
    int index;

    for (index = 0; index < 3; ++index) {
        const uint32_t word = words[index];
        words[index] = (word >> bit_shift) | carry;
        carry = bit_shift == 0u ? 0u :
            (word & mask) << (32u - bit_shift);
    }

    for (index = 2; index >= 0; --index) {
        if (index < word_count)
            words[index] = 0;
        else
            words[index] = words[index - word_count];
    }
}

/* Complete zero-callee C translation of Pharaoh.exe 0x00568DEC. */
void pharaoh_crt_double_to_extended80(
    PharaohCrtExtended80Bits *destination,
    const PharaohCrtDoubleBits *source)
{
    const uint16_t top = (uint16_t)(source->high >> 16);
    const uint16_t sign = top & UINT16_C(0x8000);
    const uint32_t source_exponent = (top & UINT16_C(0x7ff0)) >> 4;
    const uint32_t fraction_high = source->high & UINT32_C(0x000fffff);
    uint32_t significand_high = UINT32_C(0x80000000);
    uint32_t exponent;

    if (source_exponent == 0u) {
        if (fraction_high == 0u && source->low == 0u) {
            destination->significand_low = 0;
            destination->significand_high = 0;
            destination->sign_exponent = 0;
            return;
        }
        exponent = UINT32_C(0x3c01);
        significand_high = 0;
    } else if (source_exponent == UINT32_C(0x7ff)) {
        exponent = UINT32_C(0x7fff);
    } else {
        exponent = source_exponent + UINT32_C(0x3c00);
    }

    significand_high |= (source->low >> 21) | (fraction_high << 11);
    destination->significand_low = source->low << 11;
    destination->significand_high = significand_high;

    while ((significand_high & UINT32_C(0x80000000)) == 0u) {
        significand_high = (destination->significand_low >> 31) |
            (significand_high << 1);
        destination->significand_low <<= 1;
        destination->significand_high = significand_high;
        --exponent;
    }
    destination->sign_exponent = sign | (uint16_t)exponent;
}

/* Complete zero-callee logical C translations of the Visual C++ helpers at
 * Pharaoh.exe 0x00569250 (__aulldiv) and 0x005692C0 (__aullrem). */
uint64_t pharaoh_crt_unsigned_divide_64(
    uint64_t dividend, uint64_t divisor)
{
    return dividend / divisor;
}

uint64_t pharaoh_crt_unsigned_remainder_64(
    uint64_t dividend, uint64_t divisor)
{
    return dividend % divisor;
}

/* Complete zero-callee C translation of Pharaoh.exe 0x0056880A. */
uint32_t pharaoh_crt_x87_control_to_flags(uint32_t control_word)
{
    uint32_t flags = 0;
    if ((control_word & 0x0001u) != 0) flags |= 0x00010u;
    if ((control_word & 0x0004u) != 0) flags |= 0x00008u;
    if ((control_word & 0x0008u) != 0) flags |= 0x00004u;
    if ((control_word & 0x0010u) != 0) flags |= 0x00002u;
    if ((control_word & 0x0020u) != 0) flags |= 0x00001u;
    if ((control_word & 0x0002u) != 0) flags |= 0x80000u;
    switch (control_word & 0x0c00u) {
    case 0x0400u: flags |= 0x00100u; break;
    case 0x0800u: flags |= 0x00200u; break;
    case 0x0c00u: flags |= 0x00300u; break;
    default: break;
    }
    if ((control_word & 0x0300u) == 0)
        flags |= 0x20000u;
    else if ((control_word & 0x0300u) == 0x0200u)
        flags |= 0x10000u;
    if ((control_word & 0x1000u) != 0) flags |= 0x40000u;
    return flags;
}

/* Complete zero-callee C translation of Pharaoh.exe 0x0056889C. */
uint32_t pharaoh_crt_flags_to_x87_control(uint32_t flags)
{
    uint32_t control_word = (flags & 0x00010u) != 0;
    if ((flags & 0x00008u) != 0) control_word |= 0x0004u;
    if ((flags & 0x00004u) != 0) control_word |= 0x0008u;
    if ((flags & 0x00002u) != 0) control_word |= 0x0010u;
    if ((flags & 0x00001u) != 0) control_word |= 0x0020u;
    if ((flags & 0x80000u) != 0) control_word |= 0x0002u;
    switch (flags & 0x00300u) {
    case 0x00100u: control_word |= 0x0400u; break;
    case 0x00200u: control_word |= 0x0800u; break;
    case 0x00300u: control_word |= 0x0c00u; break;
    default: break;
    }
    if ((flags & 0x30000u) == 0)
        control_word |= 0x0300u;
    else if ((flags & 0x30000u) == 0x10000u)
        control_word |= 0x0200u;
    if ((flags & 0x40000u) != 0) control_word |= 0x1000u;
    return control_word;
}

/* Complete zero-callee logical C translation of Pharaoh.exe 0x00567575.
 * Native writes this value into the high word of its stack argument and loads
 * it with FLDCW. */
uint16_t pharaoh_crt_default_x87_control_word(uint32_t argument)
{
    return (uint16_t)((argument & UINT32_C(0x300)) | UINT32_C(0x7f));
}

/* Complete zero-callee C translation of Pharaoh.exe 0x005675E8. */
uint32_t pharaoh_crt_double_exponent_class(uint32_t high_word)
{
    const uint32_t exponent = high_word & UINT32_C(0x7ff00000);
    return exponent == UINT32_C(0x7ff00000) ? high_word : exponent;
}

/* Complete zero-callee logical C translation of Pharaoh.exe 0x00568532.
 * FSTSW supplies the live hardware word; MOVSX determines the full result. */
int32_t pharaoh_crt_x87_status_word(int16_t status_word)
{
    return (int32_t)status_word;
}

/* Complete zero-callee logical C translation of Pharaoh.exe 0x0056854F.
 * The original control word is returned sign-extended while FLDCW consumes
 * the separately exposed updated value. */
int32_t pharaoh_crt_update_x87_control_word(
    int16_t original_control,
    uint32_t requested_bits,
    uint32_t mask,
    uint16_t *updated_control)
{
    const uint32_t original = (uint16_t)original_control;
    *updated_control = (uint16_t)((original & ~mask) |
        (requested_bits & mask));
    return (int32_t)original_control;
}

/* Complete zero-callee C translation of Pharaoh.exe 0x005618AE. The native
 * routine binds the same conversion callback into its first and last slots. */
void pharaoh_crt_float_format_bindings_initialize(
    PharaohCrtFloatFormatBindings *bindings,
    uintptr_t convert,
    uintptr_t format_a,
    uintptr_t assign,
    uintptr_t format_b,
    uintptr_t format_c)
{
    bindings->convert_primary = convert;
    bindings->format_a = format_a;
    bindings->assign = assign;
    bindings->format_b = format_b;
    bindings->format_c = format_c;
    bindings->convert_secondary = convert;
}

/* Complete zero-callee C translation of Pharaoh.exe 0x00568A50. */
void pharaoh_crt_copy_96(uint32_t destination[3], const uint32_t source[3])
{
    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
}

/* Complete zero-callee C translation of Pharaoh.exe 0x00568A6B. */
void pharaoh_crt_clear_96(uint32_t words[3])
{
    words[0] = 0;
    words[1] = 0;
    words[2] = 0;
}

/* Complete zero-callee C translation of Pharaoh.exe 0x00568A77. */
uint32_t pharaoh_crt_is_zero_96(const uint32_t words[3])
{
    return (uint32_t)(words[0] == 0 && words[1] == 0 && words[2] == 0);
}

/* Complete zero-callee C translation of Pharaoh.exe 0x00568925. */
uint32_t pharaoh_crt_tail_is_zero_96(
    const uint32_t words[3], int32_t bit_index)
{
    const int32_t word_index = bit_index / 32;
    const uint32_t shift = (31u - (uint32_t)(bit_index % 32)) & 31u;
    int32_t index;

    if ((words[word_index] & ~(UINT32_MAX << shift)) != 0)
        return 0;
    for (index = word_index + 1; index < 3; ++index) {
        if (words[index] != 0)
            return 0;
    }
    return 1;
}

/* Complete zero-callee C translation of Pharaoh.exe 0x00569E0C. */
uint32_t pharaoh_crt_add_u32_with_carry(
    uint32_t left, uint32_t right, uint32_t *sum)
{
    const uint32_t value = left + right;
    *sum = value;
    return (uint32_t)(value < left || value < right);
}

/* Complete zero-callee C translation of Pharaoh.exe 0x00569E8B. */
void pharaoh_crt_shift_left_one_96(uint32_t words[3])
{
    const uint32_t low = words[0];
    const uint32_t middle = words[1];
    words[0] = low << 1;
    words[1] = (middle << 1) | (low >> 31);
    words[2] = (words[2] << 1) | (middle >> 31);
}

/* Complete zero-callee C translation of Pharaoh.exe 0x00569EB9. */
void pharaoh_crt_shift_right_one_96(uint32_t words[3])
{
    const uint32_t middle = words[1];
    words[1] = (middle >> 1) | (words[2] << 31);
    words[2] >>= 1;
    words[0] = (words[0] >> 1) | (middle << 31);
}

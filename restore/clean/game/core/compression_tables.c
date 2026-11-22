#include "pharaoh/core/compression_tables.h"

uint32_t pharaoh_compression_crc_update(
    const uint8_t *input, const uint32_t *length, const uint32_t *state,
    const uint32_t table[256])
{
    uint32_t remaining = *length;
    uint32_t crc = *state;
    while (remaining-- != 0u) {
        crc = (crc >> 8) ^ table[(crc ^ *input++) & UINT32_C(0xff)];
    }
    return crc;
}

void pharaoh_decompression_build_decode_table(
    int32_t count, const uint8_t *lengths, const uint8_t *starts,
    uint8_t table[256])
{
    while (--count >= 0) {
        const uint32_t step = UINT32_C(1) << (lengths[count] & 31u);
        uint32_t index = starts[count];
        do {
            table[index] = (uint8_t)count;
            index += step;
        } while (index < 256u);
    }
}

#include "pharaoh/core/memory_fill.h"

#define PHARAOH_MEMORY_FILL_BYTE_PATTERN UINT32_C(0x01010101)

/* 0040f8f0 writes complete DWORDs first, then the trailing bytes. */
void pharaoh_memory_fill_bytes(uint32_t *destination, uint32_t byte_count,
    uint8_t value)
{
    const uint32_t dword_count = byte_count >> 2;
    const uint32_t dword_value =
        (uint32_t)value * PHARAOH_MEMORY_FILL_BYTE_PATTERN;

    for (uint32_t index = 0; index < dword_count; ++index) {
        destination[index] = dword_value;
    }

    uint8_t *tail = (uint8_t *)(void *)(destination + dword_count);
    const uint32_t tail_bytes = byte_count & UINT32_C(3);
    for (uint32_t index = 0; index < tail_bytes; ++index) {
        tail[index] = value;
    }
}

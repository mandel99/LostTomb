#ifndef PHARAOH_CORE_MEMORY_FILL_H
#define PHARAOH_CORE_MEMORY_FILL_H

#include <stdint.h>

void pharaoh_memory_fill_bytes(uint32_t *destination, uint32_t byte_count,
    uint8_t value);

#endif

#include "crt_stream_constructor.h"

uint32_t restore_stream_ios_construct(uint32_t self)
{
    restore_stream_write32(self, 0x570534u);
    restore_stream_write32(self + 4u, 0u);
    restore_stream_write32(self + 8u, 4u);
    restore_stream_write32(self + 12u, 0u);
    restore_stream_write32(self + 16u, 0u);
    restore_stream_write32(self + 32u, 0u);
    restore_stream_write32(self + 36u, 0u);
    restore_stream_write32(self + 40u, 6u);
    *(volatile uint8_t *)(uintptr_t)(self + 44u) = 0x20u;
    restore_stream_write32(self + 48u, 0u);
    restore_stream_write32(self + 28u, 0u);
    return self;
}

#ifndef RESTORE_CRT_STREAM_CONSTRUCTOR_H
#define RESTORE_CRT_STREAM_CONSTRUCTOR_H

#include <stdint.h>
#include <windows.h>

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error Original stream member entries require MSVC i386.
#endif

/* All offsets below are byte offsets into the original 32-bit layouts.
   The first word of each stream subobject is a virtual-base table pointer;
   its second DWORD is the signed displacement, represented by wrapping
   address arithmetic here. It is not a vtable function slot. */
static __inline uint32_t restore_stream_read32(uint32_t address)
{
    return *(volatile const uint32_t *)(uintptr_t)address;
}

static __inline void restore_stream_write32(uint32_t address, uint32_t value)
{
    *(volatile uint32_t *)(uintptr_t)address = value;
}

static __inline uint32_t restore_stream_virtual_base(uint32_t self)
{
    uint32_t table = restore_stream_read32(self);
    return self + restore_stream_read32(table + 4u);
}

uint32_t restore_stream_ios_construct(uint32_t self);
void restore_stream_ios_destroy(uint32_t self);
uint32_t restore_stream_ios_initialize(uint32_t self, uint32_t buffer);
uint32_t restore_stream_buffer_construct(uint32_t self);
uint32_t restore_stream_filebuf_construct(uint32_t self);
uint32_t restore_stream_ostream_construct(uint32_t self, uint32_t buffer,
    uint32_t construct_virtual_base);
uint32_t restore_stream_istream_construct(uint32_t self, uint32_t buffer,
    uint32_t construct_virtual_base);
void restore_stream_istream_destroy(uint32_t virtual_base_argument);
uint32_t restore_stream_iostream_construct(uint32_t self, uint32_t buffer,
    uint32_t construct_virtual_base);
uint32_t restore_stream_fstream_construct(uint32_t self,
    uint32_t construct_virtual_base);

/* Minimal member-call edge: receiver in ECX, flags on the stack, actual
   first virtual function address. The deleting destructor owns RET 4. */
void restore_stream_virtual_delete(uint32_t buffer, uint32_t flags);

void *FUN_00561a1d(uint32_t bytes);
void FUN_00561a2b(void *allocation);

#endif

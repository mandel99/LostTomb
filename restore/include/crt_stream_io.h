#ifndef RESTORE_CRT_STREAM_IO_H
#define RESTORE_CRT_STREAM_IO_H

#include "crt_stream_constructor.h"

uint32_t restore_stream_filebuf_grow(uint32_t self);
uint32_t restore_stream_filebuf_overflow(uint32_t self, uint32_t character);
uint32_t restore_stream_filebuf_underflow(uint32_t self);
uint32_t restore_stream_filebuf_seek(uint32_t self, uint32_t offset,
    uint32_t origin, uint32_t mode);
uint32_t restore_stream_ostream_character(uint32_t self, uint32_t character);
uint32_t restore_stream_buffer_put(uint32_t self, uint32_t character);
uint32_t restore_stream_ostream_integer(uint32_t self, uint32_t value);
uint32_t restore_stream_ostream_prepare(uint32_t self);
void restore_stream_ostream_suffix(uint32_t self);
uint32_t restore_stream_ostream_string(uint32_t self, uint32_t text);
uint32_t restore_stream_ostream_flush(uint32_t self);
uint32_t restore_stream_ostream_format(uint32_t self, uint32_t prefix,
    uint32_t text);
uint32_t restore_stream_buffer_write(uint32_t self, uint32_t source,
    uint32_t count);
uint32_t restore_stream_buffer_read(uint32_t self, uint32_t destination,
    uint32_t count);
uint32_t restore_stream_buffer_allocate(uint32_t self);
uint32_t restore_stream_buffer_set(uint32_t self, uint32_t first,
    uint32_t last, uint32_t owned);
uint32_t restore_stream_buffer_putback(uint32_t self, uint32_t character);
uint32_t restore_stream_buffer_putback_direct(uint32_t self,
    uint32_t character);

/* Four omitted native vtable endpoints, authenticated separately. */
uint32_t restore_stream_filebuf_set_growth(uint32_t self, uint32_t growth);
uint32_t restore_stream_buffer_setbuf(uint32_t self, uint32_t buffer,
    uint32_t count);
uint32_t restore_stream_buffer_default_seek(void);
uint32_t restore_stream_buffer_seek_position(uint32_t self, uint32_t offset,
    uint32_t mode);

/* Numeric original code tokens resolve to actual compiled canonical entries.
   Unknown values retain their identity for external compiled callbacks. */
uint32_t restore_stream_io_resolve(uint32_t token);

/* Minimal native ABI edges; all slots are BYTE offsets. */
uint32_t restore_stream_io_virtual0(uint32_t self, uint32_t slot);
uint32_t restore_stream_io_virtual1(uint32_t self, uint32_t slot, uint32_t a);
uint32_t restore_stream_io_virtual2(uint32_t self, uint32_t slot, uint32_t a,
    uint32_t b);
uint32_t restore_stream_io_virtual3(uint32_t self, uint32_t slot, uint32_t a,
    uint32_t b, uint32_t c);
uint32_t restore_stream_io_allocate_call(uint32_t target, uint32_t bytes,
    uint32_t live_ecx);
void restore_stream_io_release_call(uint32_t target, uint32_t buffer);

#endif

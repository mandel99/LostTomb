#ifndef RESTORE_CRT_STREAM_DESTRUCTOR_H
#define RESTORE_CRT_STREAM_DESTRUCTOR_H

#include "crt_stream_constructor.h"

uint32_t restore_stream_ios_scalar_delete(uint32_t self, uint32_t flags);
uint32_t restore_stream_filebuf_scalar_delete(uint32_t self, uint32_t flags);
uint32_t restore_stream_buffer_scalar_delete(uint32_t self, uint32_t flags);
void restore_stream_filebuf_destroy(uint32_t self);
void restore_stream_buffer_destroy(uint32_t self);
uint32_t restore_stream_buffer_pending(uint32_t self);
void restore_stream_ostream_destroy(uint32_t virtual_base_argument);
void restore_stream_iostream_destroy(uint32_t virtual_base_argument);
void restore_stream_fstream_destroy(uint32_t virtual_base_argument);
void restore_stream_fstream_complete_destroy(uint32_t self);

uint32_t restore_stream_fstream_scalar_delete(uint32_t virtual_base_argument,
    uint32_t flags);
uint32_t restore_stream_ostream_scalar_delete(uint32_t virtual_base_argument,
    uint32_t flags);
uint32_t restore_stream_istream_scalar_delete(uint32_t virtual_base_argument,
    uint32_t flags);
uint32_t restore_stream_iostream_scalar_delete(uint32_t virtual_base_argument,
    uint32_t flags);

/* Release callback: cdecl stack buffer plus the native live ECX=buffer. */
void restore_stream_release_call(uint32_t target, uint32_t buffer);

/* These explicit inputs are original parent EBP addresses, not C frames. */
void restore_stream_cleanup_56b7d0(uint32_t native_frame);
void restore_stream_cleanup_56e17c(uint32_t native_frame);
void restore_stream_cleanup_56e190(uint32_t native_frame);
void restore_stream_cleanup_56e1c0(uint32_t native_frame);
void restore_stream_cleanup_56e1e4(uint32_t native_frame);
void restore_stream_cleanup_56e208(uint32_t native_frame);
void restore_stream_cleanup_56e222(uint32_t native_frame);
void restore_stream_cleanup_56e238(uint32_t native_frame);

#endif

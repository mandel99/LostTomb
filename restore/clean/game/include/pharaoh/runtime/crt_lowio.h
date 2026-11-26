#ifndef PHARAOH_RUNTIME_CRT_LOWIO_H
#define PHARAOH_RUNTIME_CRT_LOWIO_H

#include <stdint.h>

typedef struct PharaohCrtLowioRecord {
    uint32_t handle;
    uint8_t flags;
    uint8_t reserved_05[3];
} PharaohCrtLowioRecord;

typedef struct PharaohCrtLowioState {
    PharaohCrtLowioRecord **blocks;
    uint32_t capacity;
    uint32_t standard_handle_mode;
    uint32_t *dos_error;
    int32_t *error_number;
} PharaohCrtLowioState;

typedef struct PharaohCrtFileStream {
    uint8_t reserved_00[0x10];
    uint32_t descriptor;
} PharaohCrtFileStream;

_Static_assert(sizeof(PharaohCrtLowioRecord) == 8,
    "native CRT lowio record size");

/* Native entry 0x005669C6. */
uint32_t pharaoh_crt_get_os_handle(
    PharaohCrtLowioState *state, uint32_t descriptor);
uint32_t pharaoh_crt_set_os_handle(PharaohCrtLowioState *state,
    uint32_t descriptor, uint32_t handle_bits);
uint32_t pharaoh_crt_stream_descriptor(const PharaohCrtFileStream *stream);

#endif

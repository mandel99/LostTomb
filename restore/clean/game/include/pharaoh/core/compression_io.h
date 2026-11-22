#ifndef PHARAOH_CORE_COMPRESSION_IO_H
#define PHARAOH_CORE_COMPRESSION_IO_H

#include <stdint.h>

#include "pharaoh/core/pk_explode.h"

typedef struct PharaohCodecBuffer {
    uint8_t *data;
    uint32_t position;
    uint32_t capacity;
} PharaohCodecBuffer;

typedef void (*PharaohCodecError)(void *user_data, const char *message);

typedef struct PharaohCodecContext {
    PharaohCodecBuffer encode_input;
    PharaohCodecBuffer encoded;
    PharaohCodecBuffer decoded;
    uint32_t mode;
    uint32_t crc;
    uint32_t encoded_size;
    uint32_t error;
    const uint32_t *crc_table;
    PharaohCodecError report_error;
    void *error_user_data;
} PharaohCodecContext;

uint32_t pharaoh_codec_read(void *output, const uint32_t *requested,
    void *user_data);
void pharaoh_codec_write(const void *data, const uint32_t *length,
    void *user_data);

typedef int32_t (*PharaohCompressedChunkRead)(void *user_data,
    uint32_t file, void *destination, uint32_t byte_count);

typedef struct PharaohCompressedChunkState {
    uint8_t *scratch_input;
    uint8_t *scratch;
    int32_t chunk_header;
    uint32_t requested_bytes;
    uint32_t checksum;
    PharaohCompressedChunkRead read;
    void *read_user_data;
    const PharaohPkExplodeTables *explode_tables;
    const uint32_t *crc_table;
    PharaohPkExplodeError report_error;
    void *error_user_data;
} PharaohCompressedChunkState;

uint32_t pharaoh_compressed_chunk_read(PharaohCompressedChunkState *state,
    uint32_t file, void *output, uint32_t byte_count);

#endif

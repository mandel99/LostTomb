#include "pharaoh/core/compression_io.h"
#include "pharaoh/core/compression_tables.h"

#include <string.h>

static const char OUT_OF_BUFFER[] = "ERR:COMP2 Out of buffer space.";
static const char CORRUPT_BUFFER[] = "ERR:COMP1 Corrupt.";

/* Complete typed callback translation of Pharaoh.exe 0x0055BDB0. */
uint32_t pharaoh_codec_read(void *output, const uint32_t *requested,
    void *user_data)
{
    PharaohCodecContext *context = user_data;
    PharaohCodecBuffer *source;
    uint32_t copied;
    if (context->error == 1) return 0;
    source = context->mode == 1 ? &context->encode_input : &context->encoded;
    copied = source->position >= source->capacity ? 0 :
        source->capacity - source->position;
    if (copied > *requested) copied = *requested;
    if (copied != 0)
        memcpy(output, source->data + source->position, copied);
    source->position += copied;
    if (context->mode == 1)
        context->crc = pharaoh_compression_crc_update(output, &copied,
            &context->crc, context->crc_table);
    return copied;
}

/* Complete typed callback translation of Pharaoh.exe 0x0055BE50. */
void pharaoh_codec_write(const void *data, const uint32_t *length,
    void *user_data)
{
    PharaohCodecContext *context = user_data;
    PharaohCodecBuffer *target;
    if (context->error == 1) return;
    if (context->mode == 1) {
        context->encoded_size += *length;
        target = &context->encoded;
    } else {
        target = &context->decoded;
    }
    if (target->position >= target->capacity) {
        context->report_error(context->error_user_data, OUT_OF_BUFFER);
        context->error = 1;
        return;
    }
    if (target->capacity - target->position < *length) {
        context->report_error(context->error_user_data, CORRUPT_BUFFER);
        context->error = 1;
        return;
    }
    memcpy(target->data + target->position, data, *length);
    target->position += *length;
    if (context->mode == 2)
        context->crc = pharaoh_compression_crc_update(data, length,
            &context->crc, context->crc_table);
}

/* Complete typed translation of Pharaoh.exe 0x0055BCF0. The native globals
 * holding the scratch pointer, header, requested size and CRC are explicit
 * state. As in the executable, any nonzero low-level read result is accepted. */
uint32_t pharaoh_compressed_chunk_read(PharaohCompressedChunkState *state,
    uint32_t file, void *output, uint32_t byte_count)
{
    int32_t result;

    state->scratch = state->scratch_input;
    if ((int32_t)byte_count > 1200000) return 0;
    state->requested_bytes = byte_count;

    result = state->read(state->read_user_data, file,
        &state->chunk_header, sizeof(state->chunk_header));
    if (result == 0) return 0;

    if ((uint32_t)state->chunk_header == UINT32_C(0x80000000)) {
        result = state->read(state->read_user_data, file, output, byte_count);
        if (result == 0) return 0;
    } else if (state->chunk_header == 0) {
        memset(output, 0, byte_count);
    } else {
        result = state->read(state->read_user_data, file, state->scratch,
            (uint32_t)state->chunk_header);
        if (result == 0) return 0;
        if (pharaoh_pk_decompress(&state->checksum, state->scratch,
                (uint32_t)state->chunk_header, output, byte_count,
                state->explode_tables, state->crc_table,
                state->report_error, state->error_user_data) == 0)
            return 0;
    }
    return 1;
}

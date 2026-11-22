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

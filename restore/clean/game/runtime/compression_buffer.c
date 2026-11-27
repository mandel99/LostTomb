#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "pharaoh/core/compression_io.h"
#include "pharaoh/core/pk_implode.h"

/* The encoder's mutable callback ABI forwards to the shared, complete C
 * owners of0055BDB0/0055BE50. No original-code entrypoints are retained. */
static uint32_t implode_read(uint8_t *buffer, uint32_t *size, void *user_data)
{
    return pharaoh_codec_read(buffer, size, user_data);
}

static void implode_write(uint8_t *buffer, uint32_t *size, void *user_data)
{
    pharaoh_codec_write(buffer, size, user_data);
}

/* Whole0055BF10, reused from src/Unit02641.c and the old mixed/src unit.
 * Keep the movable, zeroed allocation and both allocation failure paths. */
uint32_t pharaoh_pk_compress(uint32_t *checksum, uint32_t *encoded_size,
    const void *input, void *output, uint32_t size,
    const uint32_t *mode, const uint32_t *dictionary,
    const uint32_t crc_table[256], PharaohCodecError report_error,
    void *error_user_data)
{
    static const char COMPRESS_ERROR[] =
        "ERR:COMP Error occurred while compressing.";
    HGLOBAL memory = GlobalAlloc(0x42, 0x8dd8);
    PharaohPkImplodeState *work;
    PharaohCodecContext context = {0};
    uint32_t result = 1;
    if (memory == NULL) return 0;
    work = GlobalLock(memory);
    if (work == NULL) {
        GlobalFree(memory);
        return 0;
    }
    context.encode_input.data = (uint8_t *)(uintptr_t)input;
    context.encode_input.capacity = size;
    context.encoded.data = output;
    context.encoded.capacity = size;
    context.mode = 1;
    context.crc = UINT32_MAX;
    context.crc_table = crc_table;
    context.report_error = report_error;
    context.error_user_data = error_user_data;
    if (pharaoh_pk_implode(implode_read, implode_write, work, &context,
            mode, dictionary) == 0 && context.error == 0) {
        *checksum = ~context.crc;
        *encoded_size = context.encoded_size;
    } else {
        report_error(error_user_data, COMPRESS_ERROR);
        result = 0;
    }
    GlobalUnlock(memory);
    GlobalFree(memory);
    return result;
}

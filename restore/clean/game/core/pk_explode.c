#include "pharaoh/core/pk_explode.h"
#include "pharaoh/core/compression_io.h"
#include "pharaoh/core/compression_tables.h"

#include <stdlib.h>
#include <string.h>

enum { PK_EXPLODE_DECODE_ERROR = 0x306 };

static uint32_t shift_right_x86(uint32_t value, uint32_t count)
{
    return value >> (count & 31u);
}

static uint32_t shift_left_x86(uint32_t value, uint32_t count)
{
    return value << (count & 31u);
}

/* Complete indirect-callback C translation of Pharaoh.exe 0x0055D080. */
int32_t pharaoh_pk_explode_consume_bits(
    PharaohPkExplodeContext *context, uint32_t bit_count)
{
    uint32_t remaining = context->bits_remaining;
    if (bit_count <= remaining) {
        context->bits_remaining = remaining - bit_count;
        context->bit_buffer = shift_right_x86(context->bit_buffer, bit_count);
        return 0;
    }

    context->bit_buffer = shift_right_x86(context->bit_buffer, remaining);
    if (context->input_index == context->input_size) {
        context->input_index = 0x800;
        context->input_size = context->read(context->input_buffer,
            &context->input_index, context->read_user_data);
        if (context->input_size == 0) return 1;
        context->input_index = 0;
    }

    context->bit_buffer |=
        (uint32_t)context->input_buffer[context->input_index++] << 8;
    context->bit_buffer = shift_right_x86(context->bit_buffer,
        bit_count - remaining);
    context->bits_remaining = remaining - bit_count + 8;
    return 0;
}

/* Complete one-callee C translation of Pharaoh.exe 0x0055CE50. */
uint32_t pharaoh_pk_explode_decode_next_token(
    PharaohPkExplodeContext *context)
{
    uint32_t token;
    if ((context->bit_buffer & 1u) == 0) {
        if (pharaoh_pk_explode_consume_bits(context, 1) != 0)
            return PK_EXPLODE_DECODE_ERROR;
        token = context->bit_buffer & 0xffu;
        if (context->literal_mode == 0) {
            if (pharaoh_pk_explode_consume_bits(context, 8) != 0)
                return PK_EXPLODE_DECODE_ERROR;
        } else {
            if (token == 0) {
                if (pharaoh_pk_explode_consume_bits(context, 8) != 0)
                    return PK_EXPLODE_DECODE_ERROR;
                token = context->literal_code_direct[
                    context->bit_buffer & 0xffu];
            } else {
                token = context->length_code[token];
                if (token == 0xffu) {
                    if ((context->bit_buffer & 0x3fu) == 0) {
                        if (pharaoh_pk_explode_consume_bits(context, 6) != 0)
                            return PK_EXPLODE_DECODE_ERROR;
                        token = context->length_code_6[
                            context->bit_buffer & 0x7fu];
                    } else {
                        if (pharaoh_pk_explode_consume_bits(context, 4) != 0)
                            return PK_EXPLODE_DECODE_ERROR;
                        token = context->length_code_4[
                            context->bit_buffer & 0xffu];
                    }
                }
            }
            if (pharaoh_pk_explode_consume_bits(context,
                    context->length_bit_count[token]) != 0)
                return PK_EXPLODE_DECODE_ERROR;
        }
    } else {
        uint32_t extra_bits, extra_value;
        if (pharaoh_pk_explode_consume_bits(context, 1) != 0)
            return PK_EXPLODE_DECODE_ERROR;
        token = context->literal_code[context->bit_buffer & 0xffu];
        if (pharaoh_pk_explode_consume_bits(context,
                context->literal_bit_count[token]) != 0)
            return PK_EXPLODE_DECODE_ERROR;
        extra_bits = context->literal_extra_bit_count[token];
        if (extra_bits != 0) {
            extra_value = (shift_left_x86(1u, extra_bits) - 1u) &
                context->bit_buffer;
            if (pharaoh_pk_explode_consume_bits(context, extra_bits) != 0 &&
                token + extra_value != 0x10eu)
                return PK_EXPLODE_DECODE_ERROR;
            token = context->literal_base[token] + extra_value;
        }
        token += 0x100u;
    }
    return token;
}

/* Complete one-callee C translation of Pharaoh.exe 0x0055D000. */
uint32_t pharaoh_pk_explode_get_copy_offset(
    PharaohPkExplodeContext *context, uint32_t length)
{
    uint32_t offset = context->distance_code[context->bit_buffer & 0xffu];
    if (pharaoh_pk_explode_consume_bits(context,
            context->distance_extra_bit_count[offset]) != 0)
        return 0;
    if (length == 2) {
        offset = (offset << 2) | (context->bit_buffer & 3u);
        if (pharaoh_pk_explode_consume_bits(context, 2) != 0) return 0;
    } else {
        offset = shift_left_x86(offset, context->distance_bit_count) |
            (context->distance_mask & context->bit_buffer);
        if (pharaoh_pk_explode_consume_bits(context,
                context->distance_bit_count) != 0)
            return 0;
    }
    return offset + 1;
}

/* Complete three-callee C translation of Pharaoh.exe 0x0055CD50. */
uint32_t pharaoh_pk_explode_data(PharaohPkExplodeContext *context)
{
    uint32_t token;
    context->output_position = 0x1000;
    for (;;) {
        token = pharaoh_pk_explode_decode_next_token(context);
        if (token <= 0x304u) {
            if (token < 0x100u) {
                context->output_buffer[context->output_position++] =
                    (uint8_t)token;
            } else {
                uint32_t length = token - 0xfeu;
                uint32_t offset = pharaoh_pk_explode_get_copy_offset(
                    context, length);
                uint8_t *destination;
                uint8_t *source;
                if (offset == 0) {
                    token = PK_EXPLODE_DECODE_ERROR;
                    break;
                }
                destination = context->output_buffer +
                    context->output_position;
                source = destination - offset;
                context->output_position += length;
                do {
                    *destination++ = *source++;
                } while (--length != 0);
            }
            if (context->output_position > 0x1fffu) {
                uint32_t size = 0x1000;
                context->write(context->output_buffer + 0x1000,
                    &size, context->read_user_data);
                memmove(context->output_buffer,
                    context->output_buffer + 0x1000,
                    context->output_position - 0x1000);
                context->output_position -= 0x1000;
            }
            continue;
        }
        break;
    }

    {
        uint32_t size = context->output_position - 0x1000;
        context->write(context->output_buffer + 0x1000,
            &size, context->read_user_data);
    }
    return token;
}

static void build_length_decode_tables(PharaohPkExplodeContext *context,
    const uint16_t code_table[256])
{
    for (int32_t symbol = 255; symbol >= 0; --symbol) {
        uint8_t length = context->length_bit_count[symbol];
        const uint16_t code = code_table[symbol];
        if (length < 9u) {
            for (uint32_t index = code; index < 0x100u;
                 index += 1u << (length & 31u))
                context->length_code[index] = (uint8_t)symbol;
        } else if ((code & 0xffu) == 0) {
            length = (uint8_t)(length - 8u);
            context->length_bit_count[symbol] = length;
            for (uint32_t index = code >> 8; index < 0x100u;
                 index += 1u << (length & 31u))
                context->literal_code_direct[index] = (uint8_t)symbol;
        } else {
            context->length_code[code & 0xffu] = 0xffu;
            if ((code & 0x3fu) == 0) {
                length = (uint8_t)(length - 6u);
                context->length_bit_count[symbol] = length;
                for (uint32_t index = code >> 6; index < 0x80u;
                     index += 1u << (length & 31u))
                    context->length_code_6[index] = (uint8_t)symbol;
            } else {
                length = (uint8_t)(length - 4u);
                context->length_bit_count[symbol] = length;
                for (uint32_t index = code >> 4; index < 0x100u;
                     index += 1u << (length & 31u))
                    context->length_code_4[index] = (uint8_t)symbol;
            }
        }
    }
}

/* Complete four-callee C translation of Pharaoh.exe 0x0055CBD0. */
uint8_t pharaoh_pk_explode(PharaohPkExplodeRead read,
    PharaohPkExplodeWrite write, PharaohPkExplodeContext *context,
    void *user_data, const PharaohPkExplodeTables *tables)
{
    context->read = read;
    context->write = write;
    context->read_user_data = user_data;
    context->input_index = 0x800;
    context->input_size = read(context->input_buffer,
        &context->input_index, user_data);
    if (context->input_size < 5) return 3;

    context->literal_mode = context->input_buffer[0];
    context->distance_bit_count = context->input_buffer[1];
    context->bit_buffer = context->input_buffer[2];
    context->bits_remaining = 0;
    context->input_index = 3;
    if (context->distance_bit_count <= 3 ||
        context->distance_bit_count >= 7)
        return 1;
    context->distance_mask = 0xffffu >>
        (16u - context->distance_bit_count);

    if (context->literal_mode != 0) {
        if (context->literal_mode != 1) return 2;
        memcpy(context->length_bit_count, tables->length_bit_count, 0x100);
        build_length_decode_tables(context, tables->length_code);
    }
    memcpy(context->literal_bit_count, tables->literal_bit_count, 0x10);
    pharaoh_decompression_build_decode_table(0x10,
        context->literal_bit_count, tables->literal_code_start,
        context->literal_code);
    memcpy(context->literal_extra_bit_count,
        tables->literal_extra_bit_count, 0x10);
    memcpy(context->literal_base, tables->literal_base, 0x20);
    memcpy(context->distance_extra_bit_count,
        tables->distance_extra_bit_count, 0x40);
    pharaoh_decompression_build_decode_table(0x40,
        context->distance_extra_bit_count, tables->distance_code_start,
        context->distance_code);

    return pharaoh_pk_explode_data(context) == PK_EXPLODE_DECODE_ERROR ? 4 : 0;
}

/* Complete allocation/callback parent translation of Pharaoh.exe 0x0055C000. */
uint32_t pharaoh_pk_decompress(uint32_t *checksum, const void *input,
    uint32_t input_size, void *output, uint32_t output_capacity,
    const PharaohPkExplodeTables *tables, const uint32_t crc_table[256],
    PharaohPkExplodeError report_error, void *error_user_data)
{
    static const char UNCOMPRESS_ERROR[] =
        "ERR:COMP Error uncompressing.";
    PharaohPkExplodeContext *work = calloc(1, sizeof(*work));
    PharaohCodecContext context = {0};
    uint32_t result = 1;
    uint8_t status;
    if (work == NULL) return 0;

    context.encoded.data = (uint8_t *)(uintptr_t)input;
    context.encoded.capacity = input_size;
    context.decoded.data = output;
    context.decoded.capacity = output_capacity;
    context.mode = 2;
    context.crc = 0xffffffffu;
    context.crc_table = crc_table;
    context.report_error = report_error;
    context.error_user_data = error_user_data;
    status = pharaoh_pk_explode(pharaoh_codec_read, pharaoh_codec_write,
        work, &context, tables);
    if (status == 0 && context.error == 0) {
        *checksum = ~context.crc;
    } else {
        report_error(error_user_data, UNCOMPRESS_ERROR);
        result = 0;
    }
    free(work);
    return result;
}

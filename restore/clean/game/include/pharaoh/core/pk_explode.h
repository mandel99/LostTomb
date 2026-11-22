#ifndef PHARAOH_CORE_PK_EXPLODE_H
#define PHARAOH_CORE_PK_EXPLODE_H

#include <stdint.h>

typedef uint32_t (*PharaohPkExplodeRead)(uint8_t *buffer,
    const uint32_t *requested_size, void *user_data);
typedef void (*PharaohPkExplodeWrite)(const uint8_t *buffer,
    const uint32_t *size, void *user_data);

typedef struct PharaohPkExplodeContext {
    uint32_t literal_mode;
    uint32_t distance_bit_count;
    uint32_t distance_mask;
    uint32_t bit_buffer;
    uint32_t bits_remaining;
    uint32_t input_index;
    uint32_t input_size;
    void *read_user_data;
    PharaohPkExplodeRead read;
    PharaohPkExplodeWrite write;
    uint32_t output_position;
    uint8_t output_buffer[0x2204];
    uint8_t input_buffer[0x800];
    uint8_t distance_code[0x100];
    uint8_t literal_code[0x100];
    uint8_t length_code[0x100];
    uint8_t length_code_4[0x100];
    uint8_t length_code_6[0x80];
    uint8_t literal_code_direct[0x100];
    uint8_t length_bit_count[0x140];
    uint8_t distance_extra_bit_count[0x40];
    uint8_t literal_bit_count[0x10];
    uint8_t literal_extra_bit_count[0x10];
    uint16_t literal_base[0x10];
} PharaohPkExplodeContext;

typedef struct PharaohPkExplodeTables {
    const uint8_t *distance_extra_bit_count;
    const uint8_t *distance_code_start;
    const uint8_t *literal_extra_bit_count;
    const uint16_t *literal_base;
    const uint8_t *literal_bit_count;
    const uint8_t *literal_code_start;
    const uint8_t *length_bit_count;
    const uint16_t *length_code;
} PharaohPkExplodeTables;

typedef void (*PharaohPkExplodeError)(void *user_data,
    const char *message);

uint32_t pharaoh_pk_explode_decode_next_token(
    PharaohPkExplodeContext *context);
uint32_t pharaoh_pk_explode_data(PharaohPkExplodeContext *context);
uint32_t pharaoh_pk_explode_get_copy_offset(
    PharaohPkExplodeContext *context, uint32_t length);
int32_t pharaoh_pk_explode_consume_bits(
    PharaohPkExplodeContext *context, uint32_t bit_count);
uint8_t pharaoh_pk_explode(PharaohPkExplodeRead read,
    PharaohPkExplodeWrite write, PharaohPkExplodeContext *context,
    void *user_data, const PharaohPkExplodeTables *tables);
uint32_t pharaoh_pk_decompress(uint32_t *checksum, const void *input,
    uint32_t input_size, void *output, uint32_t output_capacity,
    const PharaohPkExplodeTables *tables, const uint32_t crc_table[256],
    PharaohPkExplodeError report_error, void *error_user_data);

#endif

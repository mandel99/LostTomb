#ifndef PHARAOH_CORE_COMPRESSION_TABLES_H
#define PHARAOH_CORE_COMPRESSION_TABLES_H

#include <stdint.h>
#include <stddef.h>

typedef uint32_t (*PharaohPkReadFunction)(
    uint8_t *buffer, uint32_t *size, void *user_data);
typedef void (*PharaohPkWriteFunction)(
    uint8_t *buffer, uint32_t *size, void *user_data);

typedef struct PharaohPkImplodeState {
    uint32_t distance;
    uint32_t output_bytes;
    uint32_t output_bits;
    uint32_t dictionary_bits;
    uint32_t dictionary_mask;
    uint32_t compression_type;
    uint32_t dictionary_bytes;
    uint8_t distance_bits[64];
    uint8_t distance_codes[64];
    uint8_t character_bits[774];
    uint16_t character_codes[774];
    uint16_t reserved_09ae;
    void *user_data;
    PharaohPkReadFunction read;
    PharaohPkWriteFunction write;
    int16_t match_next[516];
    uint32_t reserved_0dc4;
    uint16_t hash_to_index[2304];
    uint16_t hash_to_index_end;
    uint8_t output[2050];
} PharaohPkImplodeState;

uint32_t pharaoh_compression_crc_update(
    const uint8_t *input, const uint32_t *length, const uint32_t *state,
    const uint32_t table[256]);
void pharaoh_decompression_build_decode_table(
    int32_t count, const uint8_t *lengths, const uint8_t *starts,
    uint8_t table[256]);

void pharaoh_compression_fill_bytes(
    uint32_t *destination, uint8_t value, uint32_t byte_count);
void pharaoh_compression_build_pair_index(
    uint8_t *state, const uint8_t *begin, const uint8_t *end);
void pharaoh_decompression_build_symbol_tables(
    uint8_t *state, const uint16_t symbol_codes[256]);
int32_t pharaoh_compression_find_longest_match(
    uint8_t *state, const uint8_t *current);
void pharaoh_compression_flush_full_output(PharaohPkImplodeState *state);

#endif

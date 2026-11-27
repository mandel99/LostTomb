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
    uint8_t work[0x2204];
    uint16_t hash_offsets[0x2204];
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
    PharaohPkImplodeState *state, const uint8_t *begin, const uint8_t *end);
int32_t pharaoh_compression_find_longest_match(
    PharaohPkImplodeState *state, const uint8_t *current);
void pharaoh_compression_flush_full_output(PharaohPkImplodeState *state);

/* Native32-bit compression state. Pair indexing borrows a nonempty range
 * inside work with a readable lookahead byte at end. Matching requires the
 * genuine sorted offsets, dictionary size and Native lookahead capacity.
 * The write procedure is an actual relocated C callback. */
_Static_assert(sizeof(void *)!=4 ||
    offsetof(PharaohPkImplodeState,user_data)==0x9b0,"Native PK user");
_Static_assert(sizeof(void *)!=4 ||
    offsetof(PharaohPkImplodeState,write)==0x9b8,"Native PK writer");
_Static_assert(sizeof(void *)!=4 ||
    offsetof(PharaohPkImplodeState,match_next)==0x9bc,"Native PK prefix");
_Static_assert(sizeof(void *)!=4 ||
    offsetof(PharaohPkImplodeState,hash_to_index)==0xdc8,"Native PK buckets");
_Static_assert(sizeof(void *)!=4 ||
    offsetof(PharaohPkImplodeState,output)==0x1fca,"Native PK output");
_Static_assert(sizeof(void *)!=4 ||
    offsetof(PharaohPkImplodeState,work)==0x27cc,"Native PK work");
_Static_assert(sizeof(void *)!=4 ||
    offsetof(PharaohPkImplodeState,hash_offsets)==0x49d0,"Native PK offsets");
_Static_assert(sizeof(void *)!=4 || sizeof(PharaohPkImplodeState)==0x8dd8,
    "Native PK allocation");

#endif

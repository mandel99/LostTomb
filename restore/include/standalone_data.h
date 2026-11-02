#ifndef RESTORE_STANDALONE_DATA_H
#define RESTORE_STANDALONE_DATA_H
#include <stddef.h>
#include <stdint.h>
typedef struct restore_data_segment {
    const char *name;
    uintptr_t address;
    size_t size;
    const unsigned char *initial;
    size_t initial_size;
    uint32_t initial_fnv1a;
    int writable;
} restore_data_segment;
extern const restore_data_segment restore_data_segments[];
extern const size_t restore_data_segment_count;
extern const uintptr_t restore_data_arena_address;
extern const size_t restore_data_arena_size;
/* Recreates only non-executable data; it never maps a PE image or .text. */
int restore_data_open(void);
void restore_data_close(void);
#endif

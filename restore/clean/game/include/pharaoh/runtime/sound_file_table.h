#ifndef PHARAOH_RUNTIME_SOUND_FILE_TABLE_H
#define PHARAOH_RUNTIME_SOUND_FILE_TABLE_H

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohSoundNameEntry {
    void *buffer;
    uint32_t owns_buffer;
} PharaohSoundNameEntry;

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4200) /* C flexible-array member */
#endif
typedef struct PharaohSoundNameArray {
    int32_t allocation_count;
    PharaohSoundNameEntry entries[];
} PharaohSoundNameArray;
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

typedef struct PharaohSoundFileTable {
    int32_t capacity;
    int32_t count;
    PharaohSoundNameArray *storage;
} PharaohSoundFileTable;

typedef struct PharaohSoundFileMemoryOperations {
    void *user_data;
    PharaohSoundNameArray *(*allocate_array)(void *user_data, int32_t count);
    void (*release_array)(void *user_data, PharaohSoundNameArray *array);
    void (*release_buffer)(void *user_data, void *buffer);
    char *(*duplicate_string)(void *user_data, const char *string);
} PharaohSoundFileMemoryOperations;

/* Original address 0x00560280. */
PharaohSoundNameEntry *pharaoh_sound_name_entry_construct(
    PharaohSoundNameEntry *entry);

/* Original address 0x005602B0. */
void pharaoh_sound_name_entry_destroy(PharaohSoundNameEntry *entry,
    const PharaohSoundFileMemoryOperations *memory);

/* Original address 0x0055DBC0. live_table_bytes models the native shared
   sound-table allocation counter passed in from application state. */
PharaohSoundFileTable *pharaoh_sound_file_table_construct(
    PharaohSoundFileTable *table, uint32_t *live_table_bytes);

/* Original address 0x0055DBF8. */
void pharaoh_sound_file_table_destroy(PharaohSoundFileTable *table,
    uint32_t *live_table_bytes,
    const PharaohSoundFileMemoryOperations *memory);

/* Original address 0x0055DC18. */
void pharaoh_sound_file_table_clear(PharaohSoundFileTable *table,
    const PharaohSoundFileMemoryOperations *memory);

/* Original address 0x0055DC63. */
int32_t pharaoh_sound_file_table_resize(PharaohSoundFileTable *table,
    int32_t requested_capacity,
    const PharaohSoundFileMemoryOperations *memory);

/* Original address 0x0055DD3F. Returns an existing matching slot or appends
   the path. copy_string is retained in the entry ownership field verbatim. */
int32_t pharaoh_sound_file_table_add(PharaohSoundFileTable *table,
    const char *path, int32_t copy_string,
    const PharaohSoundFileMemoryOperations *memory);

#endif

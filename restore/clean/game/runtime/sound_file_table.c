#include "pharaoh/runtime/sound_file_table.h"

#include <string.h>

PharaohSoundNameEntry *pharaoh_sound_name_entry_construct(
    PharaohSoundNameEntry *entry)
{
    entry->buffer = NULL;
    entry->owns_buffer = 0u;
    return entry;
}

void pharaoh_sound_name_entry_destroy(PharaohSoundNameEntry *entry,
    const PharaohSoundFileMemoryOperations *memory)
{
    if (entry->owns_buffer != 0u && entry->buffer != NULL) {
        memory->release_buffer(memory->user_data, entry->buffer);
        entry->buffer = NULL;
    }
}

PharaohSoundFileTable *pharaoh_sound_file_table_construct(
    PharaohSoundFileTable *table, uint32_t *live_table_bytes)
{
    *live_table_bytes += 12u;
    table->storage = NULL;
    table->count = 0;
    table->capacity = 0;
    return table;
}

void pharaoh_sound_file_table_destroy(PharaohSoundFileTable *table,
    uint32_t *live_table_bytes,
    const PharaohSoundFileMemoryOperations *memory)
{
    *live_table_bytes -= 12u;
    pharaoh_sound_file_table_clear(table, memory);
}

void pharaoh_sound_file_table_clear(PharaohSoundFileTable *table,
    const PharaohSoundFileMemoryOperations *memory)
{
    PharaohSoundNameArray *array = table->storage;

    if (array != NULL) {
        int32_t index = array->allocation_count;
        while (index > 0) {
            --index;
            pharaoh_sound_name_entry_destroy(&array->entries[index], memory);
        }
        memory->release_array(memory->user_data, array);
        table->storage = NULL;
    }
}

int32_t pharaoh_sound_file_table_resize(PharaohSoundFileTable *table,
    int32_t requested_capacity,
    const PharaohSoundFileMemoryOperations *memory)
{
    PharaohSoundNameArray *array;
    int32_t index;

    if (requested_capacity < 1) {
        return 0;
    }

    pharaoh_sound_file_table_clear(table, memory);
    array = memory->allocate_array(memory->user_data, requested_capacity);
    if (array == NULL) {
        table->storage = NULL;
        return 0;
    }

    array->allocation_count = requested_capacity;
    for (index = 0; index < requested_capacity; ++index) {
        pharaoh_sound_name_entry_construct(&array->entries[index]);
    }

    table->storage = array;
    if (table->storage == NULL) {
        return 0;
    }

    table->capacity = requested_capacity;
    table->count = 0;
    return 1;
}

int32_t pharaoh_sound_file_table_add(PharaohSoundFileTable *table,
    const char *path, int32_t copy_string,
    const PharaohSoundFileMemoryOperations *memory)
{
    int32_t slot = table->count;
    int32_t index;
    PharaohSoundNameArray *array;
    PharaohSoundNameEntry *entry;

    if (table->count >= table->capacity) {
        return -1;
    }

    array = table->storage;
    if (array == NULL) {
        return -1;
    }

    for (index = 0; index < table->count; ++index) {
        entry = &array->entries[index];
        if (entry->buffer != NULL &&
            strcmp((const char *)entry->buffer, path) == 0) {
            return index;
        }
    }

    entry = &array->entries[slot];
    if (copy_string == 0) {
        entry->buffer = (void *)path;
    } else {
        entry->buffer = memory->duplicate_string(memory->user_data, path);
    }
    entry->owns_buffer = (uint32_t)copy_string;
    table->count = slot + 1;
    return slot;
}

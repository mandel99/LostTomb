#ifdef NDEBUG
#undef NDEBUG
#endif

#include "pharaoh/runtime/sound_file_table.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct SoundFileTestMemory {
    bool fail_next_array_allocation;
    int32_t arrays_allocated;
    int32_t arrays_released;
    int32_t buffers_released;
} SoundFileTestMemory;

static PharaohSoundNameArray *allocate_array(void *user_data, int32_t count)
{
    SoundFileTestMemory *state = user_data;
    size_t bytes;
    PharaohSoundNameArray *array;

    if (state->fail_next_array_allocation) {
        state->fail_next_array_allocation = false;
        return NULL;
    }
    bytes = offsetof(PharaohSoundNameArray, entries) +
        (size_t)count * sizeof(PharaohSoundNameEntry);
    array = malloc(bytes);
    if (array != NULL) {
        ++state->arrays_allocated;
    }
    return array;
}

static void release_array(void *user_data, PharaohSoundNameArray *array)
{
    SoundFileTestMemory *state = user_data;
    ++state->arrays_released;
    free(array);
}

static void release_buffer(void *user_data, void *buffer)
{
    SoundFileTestMemory *state = user_data;
    ++state->buffers_released;
    free(buffer);
}

int main(void)
{
    SoundFileTestMemory state = {false, 0, 0, 0};
    const PharaohSoundFileMemoryOperations memory = {
        &state, allocate_array, release_array, release_buffer, NULL
    };
    PharaohSoundFileTable table;
    PharaohSoundNameEntry standalone;
    uint32_t live_table_bytes = 0;
    char *owned_buffer;
    char borrowed_buffer;

    assert(pharaoh_sound_file_table_construct(&table, &live_table_bytes) ==
        &table);
    assert(live_table_bytes == 12u);
    assert(table.storage == NULL && table.count == 0 && table.capacity == 0);

    assert(pharaoh_sound_file_table_resize(&table, 3, &memory) == 1);
    assert(table.capacity == 3 && table.count == 0);
    assert(table.storage->allocation_count == 3);
    for (int32_t index = 0; index < 3; ++index) {
        assert(table.storage->entries[index].buffer == NULL);
        assert(table.storage->entries[index].owns_buffer == 0u);
    }

    owned_buffer = malloc(4);
    assert(owned_buffer != NULL);
    table.storage->entries[0].buffer = owned_buffer;
    table.storage->entries[0].owns_buffer = 1u;
    table.storage->entries[1].buffer = &borrowed_buffer;
    table.storage->entries[1].owns_buffer = 0u;
    table.count = 2;
    assert(pharaoh_sound_file_table_resize(&table, 2, &memory) == 1);
    assert(state.buffers_released == 1);
    assert(state.arrays_released == 1);
    assert(table.capacity == 2 && table.count == 0);
    assert(table.storage->allocation_count == 2);

    {
        PharaohSoundNameArray *old_storage = table.storage;
        const int32_t old_capacity = table.capacity;
        table.count = 1;
        assert(pharaoh_sound_file_table_resize(&table, 0, &memory) == 0);
        assert(table.storage == old_storage && table.capacity == old_capacity);
        assert(table.count == 1);
        assert(state.arrays_released == 1);
    }

    state.fail_next_array_allocation = true;
    assert(pharaoh_sound_file_table_resize(&table, 4, &memory) == 0);
    assert(table.storage == NULL);
    assert(table.capacity == 2 && table.count == 1);
    assert(state.arrays_released == 2);

    assert(pharaoh_sound_name_entry_construct(&standalone) == &standalone);
    assert(standalone.buffer == NULL && standalone.owns_buffer == 0u);
    standalone.buffer = malloc(4);
    assert(standalone.buffer != NULL);
    standalone.owns_buffer = 1u;
    pharaoh_sound_name_entry_destroy(&standalone, &memory);
    assert(standalone.buffer == NULL && standalone.owns_buffer == 1u);
    assert(state.buffers_released == 2);

    pharaoh_sound_file_table_destroy(&table, &live_table_bytes, &memory);
    assert(live_table_bytes == 0);
    return 0;
}

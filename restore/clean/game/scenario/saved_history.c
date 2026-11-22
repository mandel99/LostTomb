#include "pharaoh/scenario/saved_history.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohSavedHistory) == 820 &&
    offsetof(PharaohSavedHistory, values) == 20,
    "native history snapshot layout");
_Static_assert(sizeof(void *) != 4 || sizeof(PharaohDynamicQueueSlot) == 28,
    "native dynamic history stride");

void pharaoh_history_release(PharaohDynamicQueueHeader *h,
    const PharaohHistoryMemory *memory)
{
    if (h->items != NULL) {
        memory->release(memory->user_data, h->items);
        h->items = NULL;
    }
    h->capacity = 0;
    h->tail = 0;
    h->count = 0;
    h->head = 1;
}

uint8_t pharaoh_history_reserve(PharaohDynamicQueueHeader *h,
    int32_t capacity, const PharaohHistoryMemory *memory)
{
    if (capacity <= h->capacity) return 1;
    pharaoh_history_release(h, memory);
    h->capacity = capacity;
    const uint32_t bytes = (uint32_t)capacity * 4u;
    h->items = memory->allocate(memory->user_data, bytes);
    const uint32_t words = bytes >> 2;
    for (uint32_t i = 0; i < words; ++i) h->items[i] = 0;
    h->head = 1;
    h->tail = 0;
    h->count = 0;
    return 1;
}

void pharaoh_history_import(PharaohDynamicQueueHeader *h,
    PharaohSavedHistory snapshot, const PharaohHistoryMemory *memory)
{
    (void)pharaoh_history_reserve(h, snapshot.capacity, memory);
    h->count = snapshot.count;
    /* The flag is one byte of the native object's first DWORD. */
    ((uint8_t *)&h->reserved_00)[0] = snapshot.flag;
    h->head = snapshot.head;
    h->tail = snapshot.tail;
}

void pharaoh_history_export(PharaohDynamicQueueHeader *h,
    PharaohSavedHistory *snapshot)
{
    if (h->capacity > 200) h->capacity = 200;
    (void)pharaoh_dynamic_queue_wrap_index(h, &h->head);
    (void)pharaoh_dynamic_queue_wrap_index(h, &h->tail);
    (void)pharaoh_dynamic_queue_wrap_index(h, &h->count);
    snapshot->count = h->count;
    snapshot->capacity = h->capacity;
    snapshot->head = h->head;
    snapshot->tail = h->tail;
    snapshot->flag = ((const uint8_t *)&h->reserved_00)[0];
    if (h->items != NULL) {
        const uint32_t words = ((uint32_t)h->capacity * 4u) >> 2;
        for (uint32_t i = 0; i < words; ++i)
            snapshot->values[i] = h->items[i];
    } else {
        for (uint32_t i = 0; i < 200; ++i) snapshot->values[i] = 0;
    }
}

void pharaoh_saved_histories_transfer(PharaohDynamicQueueSlot histories[10],
    PharaohSavedHistory snapshots[10], uint8_t load,
    const PharaohHistoryMemory *memory)
{
    for (uint32_t i = 0; i < 10; ++i) {
        if (load != 0)
            pharaoh_history_import(&histories[i].queue, snapshots[i], memory);
        else pharaoh_history_export(&histories[i].queue, &snapshots[i]);
    }
}

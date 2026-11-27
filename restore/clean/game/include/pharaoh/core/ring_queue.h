#ifndef PHARAOH_CORE_RING_QUEUE_H
#define PHARAOH_CORE_RING_QUEUE_H

#include <stdbool.h>
#include <stdint.h>

enum { PHARAOH_RING_QUEUE_CAPACITY = 50 };

typedef struct PharaohRingQueue {
    int32_t state;
    int32_t head;
    int32_t tail;
    int32_t capacity;
    int32_t reserved;
    int32_t items[PHARAOH_RING_QUEUE_CAPACITY];
} PharaohRingQueue;

typedef struct PharaohDynamicQueueHeader {
    int32_t reserved_00;
    int32_t head;
    int32_t tail;
    int32_t count;
    int32_t capacity;
    int32_t *items;
} PharaohDynamicQueueHeader;

typedef struct PharaohDynamicQueueSlot {
    PharaohDynamicQueueHeader queue;
    uint32_t reserved_18;
} PharaohDynamicQueueSlot;

_Static_assert(sizeof(PharaohRingQueue) == 220,
    "The native ring queue prefix is 220 bytes");

/* Original addresses 0x004F6520, 0x004F6560, 0x004F6570,
 * 0x004F65B0, 0x004F65F0 and 0x004F6650. */
bool pharaoh_ring_queue_initialize(PharaohRingQueue *queue);
void pharaoh_ring_queue_clear(PharaohRingQueue *queue);
bool pharaoh_ring_queue_contains(const PharaohRingQueue *queue, int32_t value);
bool pharaoh_ring_queue_push(PharaohRingQueue *queue, const int32_t *value);
int32_t pharaoh_ring_queue_next(const PharaohRingQueue *queue, int32_t *cursor);
bool pharaoh_ring_queue_is_full(const PharaohRingQueue *queue);

/* Complete C translation of Pharaoh.exe 0x004F6A10. */
bool pharaoh_dynamic_queue_wrap_index(
    volatile PharaohDynamicQueueHeader *queue, volatile int32_t *index);

enum { PHARAOH_FERRY_QUEUE_COUNT = 50 };
typedef struct PharaohFerryQueueRecord {
    PharaohRingQueue queue;
    int32_t building_id;
} PharaohFerryQueueRecord;

int32_t pharaoh_ring_queue_size(const PharaohRingQueue *queue);
int32_t pharaoh_ring_queue_pop(PharaohRingQueue *queue);

#endif

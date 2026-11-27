#include "pharaoh/core/ring_queue.h"

static int32_t pharaoh_ring_queue_count(const PharaohRingQueue *queue)
{
    const uint32_t difference =
        (uint32_t)queue->tail - (uint32_t)queue->head;

    if ((difference & UINT32_C(0x80000000)) != 0u) {
        return (int32_t)((uint32_t)queue->capacity + difference);
    }
    return (int32_t)difference;
}

bool pharaoh_ring_queue_initialize(PharaohRingQueue *queue)
{
    int32_t index;

    queue->head = 0;
    queue->tail = 0;
    queue->capacity = PHARAOH_RING_QUEUE_CAPACITY;
    for (index = 0; index < PHARAOH_RING_QUEUE_CAPACITY; ++index) {
        queue->items[index] = 0;
    }
    return true;
}

void pharaoh_ring_queue_clear(PharaohRingQueue *queue)
{
    queue->tail = 0;
    queue->head = 0;
}

bool pharaoh_ring_queue_contains(const PharaohRingQueue *queue, int32_t value)
{
    int32_t index = queue->head;

    while (index != queue->tail) {
        if (queue->items[index] == value) {
            return true;
        }
        index = (int32_t)((uint32_t)index + 1u);
        if (index >= queue->capacity) {
            index = 0;
        }
    }
    return false;
}

bool pharaoh_ring_queue_push(PharaohRingQueue *queue, const int32_t *value)
{
    const int32_t tail = queue->tail;
    int32_t next = (int32_t)((uint32_t)tail + 1u);

    if (next >= queue->capacity) {
        next = 0;
    }
    if (next == queue->head) {
        return false;
    }
    queue->items[tail] = *value;
    queue->tail = next;
    return true;
}

int32_t pharaoh_ring_queue_next(const PharaohRingQueue *queue, int32_t *cursor)
{
    int32_t index = *cursor;
    int32_t value = 0;

    if (index != queue->tail) {
        value = queue->items[index];
        index = (int32_t)((uint32_t)index + 1u);
        *cursor = index;
        /* The original permits cursor to alias a queue header word. */
        if (index >= queue->capacity) {
            *cursor = 0;
        }
    }
    return value;
}

bool pharaoh_ring_queue_is_full(const PharaohRingQueue *queue)
{
    return pharaoh_ring_queue_count(queue) == queue->capacity - 1;
}

bool pharaoh_dynamic_queue_wrap_index(
    volatile PharaohDynamicQueueHeader *queue, volatile int32_t *index)
{
    int32_t capacity;
    int32_t value;

    if (queue->capacity == 0)
        return false;

    value = *index;
    while (value < 0) {
        value = (int32_t)((uint32_t)*index + (uint32_t)queue->capacity);
        *index = value;
    }

    capacity = queue->capacity;
    if (*index >= capacity) {
        do {
            value = (int32_t)((uint32_t)*index - (uint32_t)capacity);
            *index = value;
            capacity = queue->capacity;
        } while (value >= capacity);
    }
    return true;
}

int32_t pharaoh_ring_queue_size(const PharaohRingQueue *queue)
{
    const uint32_t difference =
        (uint32_t)queue->tail - (uint32_t)queue->head;

    if ((difference & UINT32_C(0x80000000)) != 0u) {
        return (int32_t)((uint32_t)queue->capacity + difference);
    }
    return (int32_t)difference;
}

int32_t pharaoh_ring_queue_pop(PharaohRingQueue *queue)
{
    return pharaoh_ring_queue_next(queue, &queue->head);
}

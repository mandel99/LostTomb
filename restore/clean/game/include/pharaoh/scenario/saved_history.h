#ifndef PHARAOH_SCENARIO_SAVED_HISTORY_H
#define PHARAOH_SCENARIO_SAVED_HISTORY_H

#include "pharaoh/core/ring_queue.h"

typedef struct PharaohSavedHistory {
    int32_t count, capacity, head, tail;
    uint8_t flag, reserved_11[3];
    int32_t values[200];
} PharaohSavedHistory;

typedef struct PharaohHistoryMemory {
    void *user_data;
    void *(*allocate)(void *user_data, uint32_t bytes);
    void (*release)(void *user_data, void *allocation);
} PharaohHistoryMemory;

/* Native 004F6760, 004F6700, 004F67E0 and 004F6810. Import consumes the
 * snapshot by value in the EXE and restores metadata only, not values[]. */
void pharaoh_history_release(PharaohDynamicQueueHeader *history,
    const PharaohHistoryMemory *memory);
uint8_t pharaoh_history_reserve(PharaohDynamicQueueHeader *history,
    int32_t capacity, const PharaohHistoryMemory *memory);
void pharaoh_history_import(PharaohDynamicQueueHeader *history,
    PharaohSavedHistory snapshot, const PharaohHistoryMemory *memory);
void pharaoh_history_export(PharaohDynamicQueueHeader *history,
    PharaohSavedHistory *snapshot);
/* Native 004D0A40: ten histories, byte-sized load/save selector. */
void pharaoh_saved_histories_transfer(PharaohDynamicQueueSlot histories[10],
    PharaohSavedHistory snapshots[10], uint8_t load,
    const PharaohHistoryMemory *memory);

#endif

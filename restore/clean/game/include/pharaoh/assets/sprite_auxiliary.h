#ifndef PHARAOH_ASSETS_SPRITE_AUXILIARY_H
#define PHARAOH_ASSETS_SPRITE_AUXILIARY_H

#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_SPRITE_AUXILIARY_CHILD_OFFSET = 0x114,
    PHARAOH_SPRITE_AUXILIARY_FIRST_LIST_OFFSET = 0x118,
    PHARAOH_SPRITE_AUXILIARY_RECORDS_OFFSET = 0x11c,
    PHARAOH_SPRITE_AUXILIARY_FREE_LIST_OFFSET = 0x120,
    PHARAOH_SPRITE_AUXILIARY_RECORD_BYTES = 0x18,
    PHARAOH_SPRITE_AUXILIARY_RECORD_ALLOCATION_HEADER_BYTES = 4,
    PHARAOH_SPRITE_AUXILIARY_CHILD_DISPOSE_FLAGS = 3
};

typedef struct PharaohSpriteAuxiliary PharaohSpriteAuxiliary;

typedef struct PharaohSpriteAuxiliaryRecord {
    uint32_t reserved_0;
    PharaohSpriteAuxiliary *owner;
    void *allocation;
    uint32_t reserved_12;
    int32_t index;
    uint32_t reserved_20;
} PharaohSpriteAuxiliaryRecord;

typedef struct PharaohSpriteAuxiliaryAllocationList {
    int32_t *next_free_record;
    uint32_t reserved_4;
    void *allocation;
} PharaohSpriteAuxiliaryAllocationList;

struct PharaohSpriteAuxiliary {
    uint8_t native_prefix[PHARAOH_SPRITE_AUXILIARY_CHILD_OFFSET];
    void *child_object;
    PharaohSpriteAuxiliaryAllocationList *first_list;
    uint8_t *records;
    PharaohSpriteAuxiliaryAllocationList *free_record_list;
};

typedef char PharaohSpriteAuxiliaryChildOffset[
    offsetof(PharaohSpriteAuxiliary, child_object) ==
        PHARAOH_SPRITE_AUXILIARY_CHILD_OFFSET ? 1 : -1];
typedef char PharaohSpriteAuxiliaryRecordsOffset[
    offsetof(PharaohSpriteAuxiliary, records) ==
        PHARAOH_SPRITE_AUXILIARY_RECORDS_OFFSET ? 1 : -1];
typedef char PharaohSpriteAuxiliaryFreeStackOffset[
    offsetof(PharaohSpriteAuxiliary, free_record_list) ==
        PHARAOH_SPRITE_AUXILIARY_FREE_LIST_OFFSET ? 1 : -1];
typedef char PharaohSpriteAuxiliaryNativeSize[
    sizeof(PharaohSpriteAuxiliary) == 0x124 ? 1 : -1];
typedef char PharaohSpriteAuxiliaryRecordNativeSize[
    sizeof(PharaohSpriteAuxiliaryRecord) ==
        PHARAOH_SPRITE_AUXILIARY_RECORD_BYTES ? 1 : -1];
typedef char PharaohSpriteAuxiliaryRecordOwnerOffset[
    offsetof(PharaohSpriteAuxiliaryRecord, owner) == 4 ? 1 : -1];
typedef char PharaohSpriteAuxiliaryRecordAllocationOffset[
    offsetof(PharaohSpriteAuxiliaryRecord, allocation) == 8 ? 1 : -1];
typedef char PharaohSpriteAuxiliaryRecordIndexOffset[
    offsetof(PharaohSpriteAuxiliaryRecord, index) == 0x10 ? 1 : -1];
typedef char PharaohSpriteAuxiliaryListAllocationOffset[
    offsetof(PharaohSpriteAuxiliaryAllocationList, allocation) == 8 ? 1 : -1];
typedef char PharaohSpriteAuxiliaryListNativeSize[
    sizeof(PharaohSpriteAuxiliaryAllocationList) == 0x0c ? 1 : -1];

typedef struct PharaohSpriteAuxiliaryReleaseOperations {
    void *user_data;
    void (*dispose_child)(void *user_data, void *child, int32_t flags);
    void (*release_private_heap_block)(void *user_data, void *allocation);
    void **shared_allocation_slot;
    uint32_t *shared_allocation_units;
} PharaohSpriteAuxiliaryReleaseOperations;

/* Native 0x0055D8B0 destroys nested members in field order. */
void pharaoh_sprite_auxiliary_destroy(
    PharaohSpriteAuxiliary *auxiliary,
    const PharaohSpriteAuxiliaryReleaseOperations *operations);

/* Typed C translation of 0x0055D960. The native routine captures the original
   slot value, destroys that object, releases the captured allocation, then
   clears the slot even if destruction changed it. */
void pharaoh_sprite_auxiliary_release_slot(
    PharaohSpriteAuxiliary **slot,
    const PharaohSpriteAuxiliaryReleaseOperations *operations);

/* Native 0x0055D990 returns one record to the auxiliary object's free stack.
   Negative indices are ignored; valid nonnegative indices are a caller
   precondition, as the original performs no bounds check. */
uint32_t pharaoh_sprite_auxiliary_return_record(
    PharaohSpriteAuxiliary *auxiliary, int32_t record_index);

/* Native 0x0055DB90 releases a record's private allocation and returns it to
   its owner when the stored owner and index qualify. */
void pharaoh_sprite_auxiliary_destroy_record(
    PharaohSpriteAuxiliaryRecord *record,
    const PharaohSpriteAuxiliaryReleaseOperations *operations);

#endif

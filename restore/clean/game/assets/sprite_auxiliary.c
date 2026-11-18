#include "pharaoh/assets/sprite_auxiliary.h"

#if defined(_MSC_VER) && defined(_M_IX86)
#include <excpt.h>
#endif

static void destroy_record_array(PharaohSpriteAuxiliaryRecord *records,
    uint32_t record_count,
    const PharaohSpriteAuxiliaryReleaseOperations *operations)
{
    for (uint32_t remaining = record_count; remaining != 0; --remaining) {
        pharaoh_sprite_auxiliary_destroy_record(&records[remaining - 1],
            operations);
    }
}

void pharaoh_sprite_auxiliary_release_slot(
    PharaohSpriteAuxiliary **slot,
    const PharaohSpriteAuxiliaryReleaseOperations *operations)
{
    PharaohSpriteAuxiliary *const saved_auxiliary = *slot;

    if (saved_auxiliary != 0) {
        pharaoh_sprite_auxiliary_destroy(saved_auxiliary, operations);
        operations->release_private_heap_block(operations->user_data,
            saved_auxiliary);
        *slot = 0;
    }
}

uint32_t pharaoh_sprite_auxiliary_return_record(
    PharaohSpriteAuxiliary *auxiliary, int32_t record_index)
{
    if (record_index >= 0) {
        const uint32_t record_offset = (uint32_t)record_index *
            PHARAOH_SPRITE_AUXILIARY_RECORD_BYTES;
        uint8_t *const record = auxiliary->records + record_offset;
        int32_t *free_record_top =
            auxiliary->free_record_list->next_free_record - 1;

        auxiliary->free_record_list->next_free_record = free_record_top;
        *free_record_top = (int32_t)(uintptr_t)record;
    }

    return 0;
}

void pharaoh_sprite_auxiliary_destroy_record(
    PharaohSpriteAuxiliaryRecord *record,
    const PharaohSpriteAuxiliaryReleaseOperations *operations)
{
    void *const allocation = record->allocation;

    if (allocation != 0) {
        operations->release_private_heap_block(operations->user_data,
            allocation);
        record->allocation = 0;
    }

    if (record->owner != 0 && record->index >= 0) {
        (void)pharaoh_sprite_auxiliary_return_record(record->owner,
            record->index);
    }
}

void pharaoh_sprite_auxiliary_destroy(
    PharaohSpriteAuxiliary *auxiliary,
    const PharaohSpriteAuxiliaryReleaseOperations *operations)
{
    void *const child = auxiliary->child_object;

    if (child != 0) {
        operations->dispose_child(operations->user_data, child,
            PHARAOH_SPRITE_AUXILIARY_CHILD_DISPOSE_FLAGS);
        auxiliary->child_object = 0;
    }

    PharaohSpriteAuxiliaryAllocationList *const first_list =
        auxiliary->first_list;
    if (first_list != 0) {
        operations->release_private_heap_block(operations->user_data,
            first_list->allocation);
        operations->release_private_heap_block(operations->user_data,
            first_list);
        auxiliary->first_list = 0;
    }

    uint8_t *const record_bytes = auxiliary->records;
    if (record_bytes != 0) {
        uint8_t *const allocation_base = record_bytes -
            PHARAOH_SPRITE_AUXILIARY_RECORD_ALLOCATION_HEADER_BYTES;
        const uint32_t record_count =
            *(const uint32_t *)(const void *)allocation_base;
        PharaohSpriteAuxiliaryRecord *const records =
            (PharaohSpriteAuxiliaryRecord *)(void *)record_bytes;

#if defined(_MSC_VER) && defined(_M_IX86)
        __try {
            destroy_record_array(records, record_count, operations);
        }
        __finally {
            if (AbnormalTermination()) {
                destroy_record_array(records, record_count, operations);
            }
        }
#else
        destroy_record_array(records, record_count, operations);
#endif

        operations->release_private_heap_block(operations->user_data,
            allocation_base);
        auxiliary->records = 0;
    }

    PharaohSpriteAuxiliaryAllocationList *const free_record_list =
        auxiliary->free_record_list;
    if (free_record_list != 0) {
        operations->release_private_heap_block(operations->user_data,
            free_record_list->allocation);
        operations->release_private_heap_block(operations->user_data,
            free_record_list);
        auxiliary->free_record_list = 0;
    }

    void *const shared_allocation = *operations->shared_allocation_slot;
    if (shared_allocation != 0) {
        operations->release_private_heap_block(operations->user_data,
            shared_allocation);
        *operations->shared_allocation_slot = 0;
        *operations->shared_allocation_units = 0;
    }
}

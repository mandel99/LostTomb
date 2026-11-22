#include "pharaoh/runtime/chunk_io.h"

#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(void *) != 4 || sizeof(PharaohChunkDescriptor) == 20,
    "native chunk descriptor layout changed");
_Static_assert(sizeof(void *) != 4 || sizeof(PharaohChunkMigrationRecord) == 16,
    "native chunk migration layout changed");

static uint32_t wrapped_product(int32_t left, int32_t right)
{
    return (uint32_t)left * (uint32_t)right;
}

static void zero_bytes(void *destination, uint32_t byte_count)
{
    memset(destination, 0, byte_count);
}

static void read_chunk(const PharaohChunkIoContext *context, uint32_t codec,
    int32_t file, void *destination, uint32_t byte_count)
{
    PharaohChunkRead read = codec == 0 ? context->read_raw :
        context->read_compressed;
    (void)read(context->user_data, file, destination, byte_count);
}

static void migrate_records(const PharaohChunkMigrationRecord *migration,
    uint8_t *records, int32_t count, int32_t stride, int32_t existed)
{
    while (count-- > 0) {
        migration->apply(records, existed);
        records += stride;
    }
}

/* Complete typed translation of Pharaoh.exe 0x004D0E10. It preserves the
 * native version-selection rule, unchecked read results, 32-bit products,
 * backward record reshaping and the two migration orderings. */
uint8_t pharaoh_chunk_read_migrate(const PharaohChunkIoContext *context,
    int32_t index, PharaohChunkDescriptor *descriptors, int32_t file)
{
    PharaohChunkDescriptor *current = &descriptors[index];
    const PharaohChunkDescriptor *stored = &context->stored_descriptors[index];
    const PharaohChunkMigrationRecord *migration = current->migrations;
    int32_t migration_index = 0;
    int32_t needs_shape_change = 0;

    if (context->stored_schema_present != 0 &&
        current->destination != context->stored_descriptor_storage &&
        (current->element_size != stored->element_size ||
         current->element_count != stored->element_count)) {
        needs_shape_change = 1;
    }

    if (migration != NULL) {
        while (migration[migration_index].version != 0 &&
               migration[migration_index].version < context->file_version)
            ++migration_index;
        if (migration[migration_index].version == 0 && !needs_shape_change)
            migration = NULL;
        else
            migration += migration_index;
    }

    if (migration == NULL && !needs_shape_change) {
        read_chunk(context, current->codec, file, current->destination,
            wrapped_product(current->element_count, current->element_size));
        goto version_check;
    }

    {
        const int32_t old_count = stored->element_count;
        const int32_t old_size = stored->element_size;
        const uint32_t old_bytes = wrapped_product(old_count, old_size);
        const int32_t current_bytes = (int32_t)wrapped_product(
            current->element_count, current->element_size);

        if (current_bytes < (int32_t)old_bytes) {
            uint8_t *temporary = context->allocate(context->user_data,
                old_bytes);
            if (temporary == NULL) return 0;
            zero_bytes(temporary, old_bytes);
            read_chunk(context, stored->codec, file, temporary, old_bytes);

            if (migration != NULL && current->element_size < old_size) {
                migrate_records(migration, temporary, old_count, old_size, 1);
                if (old_count < current->element_count)
                    migrate_records(migration,
                        temporary + old_count * old_size,
                        current->element_count - old_count, old_size, 0);
            }

            for (int32_t record = current->element_count - 1;
                 record >= 0; --record) {
                uint8_t *destination = (uint8_t *)current->destination +
                    record * current->element_size;
                context->move(destination, temporary + record * old_size,
                    (uint32_t)old_size);
                if (old_size < current->element_size)
                    zero_bytes(destination + old_size,
                        (uint32_t)(current->element_size - old_size));
            }

            if (migration != NULL && old_size <= current->element_size) {
                migrate_records(migration, temporary, old_count, old_size, 1);
                if (old_count < current->element_count)
                    migrate_records(migration,
                        temporary + old_count * old_size,
                        current->element_count - old_count, old_size, 0);
            }
            if (migration == NULL && old_count < current->element_count) {
                zero_bytes((uint8_t *)current->destination +
                    old_count * current->element_size,
                    wrapped_product(current->element_count - old_count,
                        current->element_size));
            }
            context->free(context->user_data, temporary);
        } else {
            uint8_t *destination = current->destination;
            zero_bytes(destination, old_bytes);
            read_chunk(context, stored->codec, file, destination, old_bytes);

            for (int32_t record = old_count - 1; record >= 0; --record) {
                uint8_t *expanded = destination +
                    record * current->element_size;
                context->move(expanded, destination + record * old_size,
                    (uint32_t)old_size);
                if (old_size < current->element_size)
                    zero_bytes(expanded + old_size,
                        (uint32_t)(current->element_size - old_size));
            }

            if (migration == NULL) {
                if (old_count < current->element_count)
                    zero_bytes(destination + old_count * current->element_size,
                        wrapped_product(current->element_count - old_count,
                            current->element_size));
            } else {
                migrate_records(migration, destination, old_count,
                    current->element_size, 1);
                if (old_count < current->element_count)
                    migrate_records(migration,
                        destination + old_count * current->element_size,
                        current->element_count - old_count,
                        current->element_size, 0);
            }
        }
    }

version_check:
    if (context->file_version < 0x73) {
        context->set_message(context->user_data, 5, 0x18);
        context->set_message(context->user_data, 5, 0x19);
        context->show_error(context->user_data, context->error_x,
            context->error_y, 0);
        return 0;
    }
    return 1;
}

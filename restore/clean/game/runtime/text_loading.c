#include "pharaoh/runtime/text_loading.h"

#include <string.h>

static void replace_allocation(const PharaohTextFileOperations *operations,
                               void *user_data, void **allocation,
                               uint32_t byte_count)
{
    if (*allocation != NULL) {
        operations->release(user_data, *allocation);
        *allocation = NULL;
    }
    *allocation = operations->allocate(user_data, byte_count);
}

int pharaoh_load_text_catalogs(const PharaohTextLoadContext *context,
                               const char *primary_filename,
                               const char *media_filename)
{
    const PharaohTextFileOperations *operations = context->operations;
    PharaohTextCatalog *catalog = context->catalog;
    PharaohTextFile *file = operations->open(context->user_data,
        primary_filename, PHARAOH_TEXT_FILE_OPEN_FLAGS);
    if (file == NULL) {
        return 0;
    }

    const uint32_t primary_size = operations->size(context->user_data, file);
    const uint32_t primary_payload_size =
        primary_size - PHARAOH_TEXT_PRIMARY_FIXED_BYTES;
    replace_allocation(operations, context->user_data, &catalog->primary_text,
        primary_payload_size);
    (void)operations->read_text(context->user_data, file,
        catalog->primary_header, PHARAOH_TEXT_PRIMARY_HEADER_BYTES);
    (void)operations->read_text(context->user_data, file,
        catalog->primary_index, PHARAOH_TEXT_PRIMARY_INDEX_BYTES);
    (void)operations->read_text(context->user_data, file,
        catalog->primary_text, primary_payload_size);
    operations->close(context->user_data, file);

    file = operations->open(context->user_data, media_filename,
        PHARAOH_TEXT_FILE_OPEN_FLAGS);
    if (file == NULL) {
        return 0;
    }

    const uint32_t media_size = operations->size(context->user_data, file);
    const uint32_t media_payload_size =
        media_size - PHARAOH_TEXT_MEDIA_INDEX_BYTES;
    replace_allocation(operations, context->user_data, &catalog->media_text,
        media_payload_size);
    (void)operations->read_text(context->user_data, file,
        catalog->media_index, PHARAOH_TEXT_MEDIA_INDEX_BYTES);
    (void)operations->read_text(context->user_data, file,
        catalog->media_text, media_payload_size);
    operations->close(context->user_data, file);
    return 1;
}

/* Native MM index startsE209A0; title/subtitle/body areE209FC/E20A00/E20A04.
 * The index stores relative file offsets, never serialized process pointers. */
PharaohTextMediaMessage pharaoh_text_media_message(
    const PharaohTextCatalog *catalog,int32_t message_id)
{
    int32_t offsets[3];
    memcpy(offsets,catalog->media_index+0x5c+(uint32_t)message_id*80,
        sizeof(offsets));
    const char *const text=catalog->media_text;
    return (PharaohTextMediaMessage){text+offsets[0],text+offsets[1],text+offsets[2]};
}

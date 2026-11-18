#include "pharaoh/runtime/file_writing.h"

int32_t pharaoh_file_write_all(const PharaohFileWriteContext *context,
                               const char *path, const void *buffer,
                               uint32_t byte_count)
{
    const PharaohFileWriteOperations *const operations =
        context->operations;
    const int32_t descriptor = operations->open(context->user_data, path,
        PHARAOH_FILE_WRITE_OPEN_FLAGS, PHARAOH_FILE_WRITE_PERMISSIONS);

    if (descriptor == -1) {
        return 0;
    }

    (void)operations->write(context->user_data, descriptor, buffer,
        byte_count);
    (void)operations->close(context->user_data, descriptor);
    return 1;
}

#include "pharaoh/runtime/file_size.h"

int32_t pharaoh_file_size(const PharaohFileSizeContext *context,
                          const char *filename)
{
    const PharaohFileSizeOperations *const operations = context->operations;
    const int32_t file_handle = operations->open_file(context->user_data,
        filename, PHARAOH_FILE_OPEN_FLAGS_NATIVE_8000);
    if (file_handle == -1) {
        return 0;
    }

    const int32_t size = operations->get_file_size(context->user_data,
        file_handle);
    (void)operations->close_file(context->user_data, file_handle);
    return size;
}

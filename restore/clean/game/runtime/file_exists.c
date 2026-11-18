#include "pharaoh/runtime/file_exists.h"

int32_t pharaoh_file_exists(const PharaohFileSizeContext *files,
                            const PharaohFileDirectoryResetContext *directory,
                            const char *filename)
{
    pharaoh_file_directory_reset(directory);

    const PharaohFileSizeOperations *const operations = files->operations;
    const int32_t file_handle = operations->open_file(files->user_data,
        filename, PHARAOH_FILE_OPEN_FLAGS_NATIVE_8000);
    if (file_handle < 0) {
        return 0;
    }

    (void)operations->close_file(files->user_data, file_handle);
    return 1;
}

#ifndef PHARAOH_RUNTIME_FILE_READ_PART_H
#define PHARAOH_RUNTIME_FILE_READ_PART_H

#include <stdint.h>

#include "pharaoh/runtime/file_directory_reset.h"
#include "pharaoh/runtime/file_media_root.h"
#include "pharaoh/runtime/file_size.h"
#include "pharaoh/runtime/text_loading.h"

typedef struct PharaohFileReadPartOperations {
    int32_t (*seek_file)(void *user_data, int32_t file_handle,
                         int32_t offset, int32_t origin);
    int32_t (*read_file)(void *user_data, int32_t file_handle,
                         void *destination, uint32_t byte_count);
} PharaohFileReadPartOperations;

typedef struct PharaohFileReadPartContext {
    const PharaohFileSizeContext *files;
    const PharaohFileReadPartOperations *operations;
    void *user_data;
    PharaohFileMediaRootContext *media_root;
    const PharaohFileDirectoryResetContext *directory_reset;
    PharaohTextSelection *retry_text_selection;
    /* Persistent native C:\\ buffer: bytes 1 and 2 are initialized to ":\\". */
    char *media_prefixed_path;
} PharaohFileReadPartContext;

/* Native entry 0x004CE480. Tries the path, the selected media drive, the
   optional 555\\ folder, then the media-root fallback before restoring the
   startup directory. The media-prefixed path is persistent caller storage. */
int32_t pharaoh_file_read_part(const PharaohFileReadPartContext *context,
                               const char *path, void *destination,
                               int32_t byte_count, int32_t offset);

#endif

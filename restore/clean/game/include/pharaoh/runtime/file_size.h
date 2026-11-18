#ifndef PHARAOH_RUNTIME_FILE_SIZE_H
#define PHARAOH_RUNTIME_FILE_SIZE_H

#include <stdint.h>

enum { PHARAOH_FILE_OPEN_FLAGS_NATIVE_8000 = 0x8000 };

typedef struct PharaohFileSizeOperations {
    int32_t (*open_file)(void *user_data, const char *filename,
                         uint32_t native_flags);
    int32_t (*get_file_size)(void *user_data, int32_t file_handle);
    int32_t (*close_file)(void *user_data, int32_t file_handle);
} PharaohFileSizeOperations;

typedef struct PharaohFileSizeContext {
    const PharaohFileSizeOperations *operations;
    void *user_data;
} PharaohFileSizeContext;

/* Native entry 0x004CED50. The caller supplies the file backend used by the
   executable's open, length, and close helpers. */
int32_t pharaoh_file_size(const PharaohFileSizeContext *context,
                          const char *filename);

#endif

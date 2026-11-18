#ifndef PHARAOH_RUNTIME_FILE_WRITING_H
#define PHARAOH_RUNTIME_FILE_WRITING_H

#include <stdint.h>

enum {
    PHARAOH_FILE_WRITE_OPEN_FLAGS = 0x8301,
    PHARAOH_FILE_WRITE_PERMISSIONS = 0x0180
};

typedef struct PharaohFileWriteOperations {
    int32_t (*open)(void *user_data, const char *path, uint32_t flags,
                    uint32_t permissions);
    int32_t (*write)(void *user_data, int32_t descriptor,
                     const void *buffer, uint32_t byte_count);
    int32_t (*close)(void *user_data, int32_t descriptor);
} PharaohFileWriteOperations;

typedef struct PharaohFileWriteContext {
    const PharaohFileWriteOperations *operations;
    void *user_data;
} PharaohFileWriteContext;

/* Typed translation of native 0x004CED90. Write and close return values are
   intentionally ignored, matching the executable's caller-visible result. */
int32_t pharaoh_file_write_all(const PharaohFileWriteContext *context,
                               const char *path, const void *buffer,
                               uint32_t byte_count);

#endif

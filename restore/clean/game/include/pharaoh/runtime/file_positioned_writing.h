#ifndef PHARAOH_RUNTIME_FILE_POSITIONED_WRITING_H
#define PHARAOH_RUNTIME_FILE_POSITIONED_WRITING_H

#include <stdint.h>

#include "pharaoh/runtime/file_writing.h"

enum {
    PHARAOH_FILE_POSITIONED_WRITE_OPEN_FLAGS = 0x8101,
    PHARAOH_FILE_POSITIONED_WRITE_PERMISSIONS = 0x0180,
    PHARAOH_FILE_SEEK_FROM_END = 2
};

typedef struct PharaohFilePositionedWriteOperations {
    int32_t (*seek)(void *user_data, int32_t descriptor,
                    int32_t offset, int32_t origin);
} PharaohFilePositionedWriteOperations;

typedef struct PharaohFilePositionedWriteContext {
    const PharaohFileWriteContext *file_writer;
    const PharaohFilePositionedWriteOperations *operations;
} PharaohFilePositionedWriteContext;

/* Native entry 0x004CE920. A failed seek returns zero and leaves the open
   descriptor unclosed, matching the executable's behavior. */
int32_t pharaoh_file_write_part(
    const PharaohFilePositionedWriteContext *context, const char *path,
    const void *buffer, int32_t byte_count, int32_t offset);

/* Native entry 0x004CE980. Seeks to end with origin 2 before writing; a
   failed seek returns zero without closing the descriptor. */
int32_t pharaoh_file_append(
    const PharaohFilePositionedWriteContext *context, const char *path,
    const void *buffer, int32_t byte_count);

#endif

#ifndef PHARAOH_RUNTIME_FILE_LOADING_H
#define PHARAOH_RUNTIME_FILE_LOADING_H

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohReadOnlyFile PharaohReadOnlyFile;

typedef struct PharaohFileReadOperations {
    PharaohReadOnlyFile *(*open)(void *user_data, const char *filename);
    size_t (*size)(void *user_data, PharaohReadOnlyFile *file);
    size_t (*read)(void *user_data, void *destination, size_t element_size,
                   size_t element_count, PharaohReadOnlyFile *file);
    int32_t (*seek)(void *user_data, PharaohReadOnlyFile *file,
                    int32_t offset, int32_t origin);
    void (*close)(void *user_data, PharaohReadOnlyFile *file);
} PharaohFileReadOperations;

typedef struct PharaohFileReadContext {
    const PharaohFileReadOperations *operations;
    void *user_data;
} PharaohFileReadContext;

int pharaoh_load_raw_file(const PharaohFileReadContext *context,
                          const char *filename, void *destination);

#endif

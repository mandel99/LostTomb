#ifndef PHARAOH_RUNTIME_FILE_LOADING_H
#define PHARAOH_RUNTIME_FILE_LOADING_H

#include <stddef.h>

typedef struct PharaohReadOnlyFile PharaohReadOnlyFile;

typedef struct PharaohFileReadOperations {
    PharaohReadOnlyFile *(*open)(void *user_data, const char *filename);
    size_t (*size)(void *user_data, PharaohReadOnlyFile *file);
    size_t (*read)(void *user_data, void *destination, size_t element_size,
                   size_t element_count, PharaohReadOnlyFile *file);
    void (*close)(void *user_data, PharaohReadOnlyFile *file);
} PharaohFileReadOperations;

typedef struct PharaohFileReadContext {
    const PharaohFileReadOperations *operations;
    void *user_data;
} PharaohFileReadContext;

int pharaoh_load_raw_file(const PharaohFileReadContext *context,
                          const char *filename, void *destination);

#endif

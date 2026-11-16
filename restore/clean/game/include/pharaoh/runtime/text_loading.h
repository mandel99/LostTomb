#ifndef PHARAOH_RUNTIME_TEXT_LOADING_H
#define PHARAOH_RUNTIME_TEXT_LOADING_H

#include <stdint.h>

enum {
    PHARAOH_TEXT_FILE_OPEN_FLAGS = 0x8000,
    PHARAOH_TEXT_PRIMARY_HEADER_BYTES = 0x1c,
    PHARAOH_TEXT_PRIMARY_INDEX_BYTES = 8000,
    PHARAOH_TEXT_PRIMARY_FIXED_BYTES = 0x1f5c,
    PHARAOH_TEXT_MEDIA_INDEX_BYTES = 0x13898
};

typedef struct PharaohTextFile PharaohTextFile;

typedef struct PharaohTextFileOperations {
    PharaohTextFile *(*open)(void *user_data, const char *filename,
                             uint32_t open_flags);
    uint32_t (*size)(void *user_data, PharaohTextFile *file);
    int32_t (*read_text)(void *user_data, PharaohTextFile *file,
                         void *destination, uint32_t byte_count);
    void (*close)(void *user_data, PharaohTextFile *file);
    void *(*allocate)(void *user_data, uint32_t byte_count);
    void (*release)(void *user_data, void *allocation);
} PharaohTextFileOperations;

typedef struct PharaohTextCatalog {
    uint8_t primary_header[PHARAOH_TEXT_PRIMARY_HEADER_BYTES];
    uint8_t primary_index[PHARAOH_TEXT_PRIMARY_INDEX_BYTES];
    void *primary_text;
    uint8_t media_index[PHARAOH_TEXT_MEDIA_INDEX_BYTES];
    void *media_text;
} PharaohTextCatalog;

typedef struct PharaohTextLoadContext {
    const PharaohTextFileOperations *operations;
    void *user_data;
    PharaohTextCatalog *catalog;
} PharaohTextLoadContext;

int pharaoh_load_text_catalogs(const PharaohTextLoadContext *context,
                               const char *primary_filename,
                               const char *media_filename);

#endif

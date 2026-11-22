#ifndef PHARAOH_RUNTIME_CHUNK_IO_H
#define PHARAOH_RUNTIME_CHUNK_IO_H

#include <stdint.h>

typedef void (*PharaohChunkMigration)(void *record, int32_t existed);

typedef struct PharaohChunkMigrationRecord {
    uint32_t reserved_00;
    uint32_t reserved_04;
    int32_t version;
    PharaohChunkMigration apply;
} PharaohChunkMigrationRecord;

typedef struct PharaohChunkDescriptor {
    uint32_t codec;
    void *destination;
    int32_t element_size;
    int32_t element_count;
    const PharaohChunkMigrationRecord *migrations;
} PharaohChunkDescriptor;

typedef int32_t (*PharaohChunkRead)(void *user_data, int32_t file,
    void *destination, uint32_t byte_count);
typedef void *(*PharaohChunkAllocate)(void *user_data, uint32_t byte_count);
typedef void (*PharaohChunkFree)(void *user_data, void *allocation);
typedef void (*PharaohChunkMove)(void *destination, const void *source,
    uint32_t byte_count);
typedef void (*PharaohChunkSetMessage)(void *user_data, int32_t group,
    int32_t id);
typedef void (*PharaohChunkShowError)(void *user_data, uint32_t x,
    uint32_t y, int32_t argument);

typedef struct PharaohChunkIoContext {
    int32_t file_version;
    int32_t stored_schema_present;
    const PharaohChunkDescriptor *stored_descriptors;
    const void *stored_descriptor_storage;
    PharaohChunkRead read_raw;
    PharaohChunkRead read_compressed;
    PharaohChunkAllocate allocate;
    PharaohChunkFree free;
    PharaohChunkMove move;
    PharaohChunkSetMessage set_message;
    PharaohChunkShowError show_error;
    void *user_data;
    uint32_t error_x;
    uint32_t error_y;
} PharaohChunkIoContext;

uint8_t pharaoh_chunk_read_migrate(const PharaohChunkIoContext *context,
    int32_t index, PharaohChunkDescriptor *descriptors, int32_t file);

#endif

#ifndef PHARAOH_RUNTIME_STATUS_LOG_APPEND_H
#define PHARAOH_RUNTIME_STATUS_LOG_APPEND_H

#include <stdint.h>

typedef struct PharaohStatusLogAppendOperations {
    void *user_data;
    int32_t (*open_file)(void *user_data, const char *path,
        uint32_t flags, uint32_t permission_mode);
    uint32_t (*seek_file)(void *user_data, int32_t descriptor,
        int32_t offset, uint32_t origin);
    int32_t (*write_file)(void *user_data, int32_t descriptor,
        const void *bytes, uint32_t byte_count);
    int32_t (*flush_file)(void *user_data, int32_t descriptor);
    int32_t (*close_file)(void *user_data, int32_t descriptor);
} PharaohStatusLogAppendOperations;

typedef struct PharaohStatusLogAppendState {
    /* Native 0x00E92E48: zero selects truncate/create; success sets it to one. */
    uint32_t status_file_initialized;
} PharaohStatusLogAppendState;

/* Native entry 0x004CE860. `bytes` is the already formatted buffer supplied
   by 0x004CE6E0; count is a byte length, not a string length. */
void pharaoh_status_log_append(PharaohStatusLogAppendState *state,
    const uint8_t *bytes, uint32_t byte_count,
    const PharaohStatusLogAppendOperations *operations);

#endif

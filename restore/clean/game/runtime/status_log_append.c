#include "pharaoh/runtime/status_log_append.h"

enum {
    STATUS_LOG_OPEN_CREATE_TRUNCATE = 0x8301,
    STATUS_LOG_OPEN_EXISTING = 0x8101,
    STATUS_LOG_PERMISSION_MODE = 0x0180,
    STATUS_LOG_SEEK_FROM_END = 2
};

static const char status_log_filename[] = "status.txt";

void pharaoh_status_log_append(PharaohStatusLogAppendState *state,
    const uint8_t *bytes, uint32_t byte_count,
    const PharaohStatusLogAppendOperations *operations)
{
    const uint32_t flags = state->status_file_initialized == 0 ?
        STATUS_LOG_OPEN_CREATE_TRUNCATE : STATUS_LOG_OPEN_EXISTING;
    const int32_t descriptor = operations->open_file(operations->user_data,
        status_log_filename, flags, STATUS_LOG_PERMISSION_MODE);

    if (descriptor == -1) {
        return;
    }

    if (state->status_file_initialized != 0) {
        /* The native caller ignores seek failure and still attempts the write. */
        (void)operations->seek_file(operations->user_data, descriptor, 0,
            STATUS_LOG_SEEK_FROM_END);
    }

    if (byte_count != 0) {
        (void)operations->write_file(operations->user_data, descriptor,
            bytes, byte_count);
    }
    (void)operations->flush_file(operations->user_data, descriptor);
    (void)operations->close_file(operations->user_data, descriptor);
    state->status_file_initialized = 1;
}

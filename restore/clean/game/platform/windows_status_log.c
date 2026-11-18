#define _CRT_SECURE_NO_WARNINGS
#include <io.h>
#include <string.h>

#include "pharaoh/platform/windows_status_log.h"
#include "pharaoh/runtime/status_log_timestamp.h"

static int32_t open_file(void *user_data, const char *path, uint32_t flags,
    uint32_t permission_mode)
{
    (void)user_data;
    return _open(path, (int)flags, (int)permission_mode);
}

static uint32_t seek_file(void *user_data, int32_t descriptor,
    int32_t offset, uint32_t origin)
{
    (void)user_data;
    return (uint32_t)_lseek(descriptor, offset, (int)origin);
}

static int32_t write_file(void *user_data, int32_t descriptor,
    const void *bytes, uint32_t byte_count)
{
    (void)user_data;
    return _write(descriptor, bytes, byte_count);
}

static int32_t flush_file(void *user_data, int32_t descriptor)
{
    (void)user_data;
    return _commit(descriptor);
}

static int32_t close_file(void *user_data, int32_t descriptor)
{
    (void)user_data;
    return _close(descriptor);
}

void pharaoh_windows_status_log_append_operations_initialize(
    PharaohStatusLogAppendOperations *operations)
{
    memset(operations, 0, sizeof(*operations));
    operations->open_file = open_file;
    operations->seek_file = seek_file;
    operations->write_file = write_file;
    operations->flush_file = flush_file;
    operations->close_file = close_file;
}

void pharaoh_windows_status_log_format_operations_initialize(
    PharaohStatusLogFormatOperations *operations,
    const PharaohStatusLogAppendOperations *append_operations)
{
    memset(operations, 0, sizeof(*operations));
    operations->refresh_timestamp_text =
        pharaoh_status_log_refresh_timestamp_text;
    operations->append_operations = append_operations;
}

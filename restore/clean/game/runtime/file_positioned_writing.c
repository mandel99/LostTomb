#include "pharaoh/runtime/file_positioned_writing.h"

static int32_t open_for_positioned_write(
    const PharaohFilePositionedWriteContext *context, const char *path)
{
    const PharaohFileWriteContext *const file_writer = context->file_writer;
    return file_writer->operations->open(file_writer->user_data, path,
        PHARAOH_FILE_POSITIONED_WRITE_OPEN_FLAGS,
        PHARAOH_FILE_POSITIONED_WRITE_PERMISSIONS);
}

static int32_t write_after_seek(
    const PharaohFilePositionedWriteContext *context, int32_t descriptor,
    const void *buffer, int32_t byte_count, int32_t seek_offset,
    int32_t seek_origin)
{
    if (context->operations->seek(context->file_writer->user_data,
            descriptor, seek_offset, seek_origin) == -1) {
        return 0;
    }

    const PharaohFileWriteContext *const file_writer = context->file_writer;
    const int32_t result = file_writer->operations->write(
        file_writer->user_data, descriptor, buffer, (uint32_t)byte_count);
    (void)file_writer->operations->close(file_writer->user_data, descriptor);
    return result;
}

int32_t pharaoh_file_write_part(
    const PharaohFilePositionedWriteContext *context, const char *path,
    const void *buffer, int32_t byte_count, int32_t offset)
{
    const int32_t descriptor = open_for_positioned_write(context, path);
    if (descriptor == -1) {
        return 0;
    }

    return write_after_seek(context, descriptor, buffer, byte_count,
        offset, 0);
}

int32_t pharaoh_file_append(
    const PharaohFilePositionedWriteContext *context, const char *path,
    const void *buffer, int32_t byte_count)
{
    const int32_t descriptor = open_for_positioned_write(context, path);
    if (descriptor == -1) {
        return 0;
    }

    return write_after_seek(context, descriptor, buffer, byte_count, 0,
        PHARAOH_FILE_SEEK_FROM_END);
}

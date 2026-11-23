#include "pharaoh/scenario/saved_game_io.h"
#include <string.h>

static int32_t raw_read(void *data,int32_t file,void *out,uint32_t bytes)
{
    PharaohSavedGameIo *io=data;
    return io->external.read_raw(io->external.user_data,file,out,bytes);
}
static int32_t codec_read(void *data,uint32_t file,void *out,uint32_t bytes)
{
    return raw_read(data,(int32_t)file,out,bytes);
}
static int32_t compressed_read(void *data,int32_t file,void *out,uint32_t bytes)
{
    PharaohSavedGameIo *io=data;
    return (int32_t)pharaoh_compressed_chunk_read(&io->compressed,
        (uint32_t)file,out,bytes);
}
static void *allocate(void *data,uint32_t bytes)
{
    PharaohSavedGameIo *io=data;
    return io->external.allocate(io->external.user_data,bytes);
}
static void release(void *data,void *allocation)
{
    PharaohSavedGameIo *io=data;
    io->external.free(io->external.user_data,allocation);
}
static void message(void *data,int32_t group,int32_t id)
{
    PharaohSavedGameIo *io=data;
    io->external.set_message(io->external.user_data,group,id);
}
static void error(void *data,uint32_t x,uint32_t y,int32_t argument)
{
    PharaohSavedGameIo *io=data;
    io->external.show_error(io->external.user_data,x,y,argument);
}
void pharaoh_saved_game_io_bind(PharaohSavedGameIo *io,
    PharaohSavedGameReadContext *read,void *const destinations[181],
    const PharaohChunkMigrationRecord *messages,
    const PharaohChunkMigrationRecord *events,
    const PharaohChunkIoContext *external,
    const PharaohCompressedChunkState *compressed)
{
    io->external=*external;
    io->chunks=*external;
    io->chunks.stored_descriptors=io->stored;
    io->chunks.stored_descriptor_storage=io->stored;
    io->chunks.read_raw=raw_read;
    io->chunks.read_compressed=compressed_read;
    io->chunks.allocate=allocate;
    io->chunks.free=release;
    io->chunks.set_message=message;
    io->chunks.show_error=error;
    io->chunks.user_data=io;
    io->compressed=*compressed;
    io->compressed.read=codec_read;
    io->compressed.read_user_data=io;
    pharaoh_saved_game_schema_bind(io->schema,destinations,messages,events);
    io->schema[4].destination=&io->chunks.file_version;
    io->schema[5].destination=&io->chunks.stored_schema_present;
    io->schema[6].destination=io->stored;
    read->chunk_io=&io->chunks;
    read->descriptors=io->schema;
}

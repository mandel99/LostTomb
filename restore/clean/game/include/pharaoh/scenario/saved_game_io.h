#ifndef PHARAOH_SCENARIO_SAVED_GAME_IO_H
#define PHARAOH_SCENARIO_SAVED_GAME_IO_H
#include "pharaoh/scenario/saved_game_state_read.h"
#include "pharaoh/scenario/saved_game_schema.h"
#include "pharaoh/core/compression_io.h"

typedef struct PharaohSavedGameIo {
    PharaohChunkIoContext chunks;
    PharaohChunkIoContext external;
    PharaohChunkDescriptor schema[181], stored[300];
    PharaohCompressedChunkState compressed;
} PharaohSavedGameIo;

/* The181 destinations are live scene buffers. Header chunks4/5/6 are bound
 * to the reader's own version, schema flag and stored descriptor table.
 * Serialized destination pointers are never used as scene buffer addresses. */
void pharaoh_saved_game_io_bind(PharaohSavedGameIo *io,
    PharaohSavedGameReadContext *read,void *const destinations[181],
    const PharaohChunkMigrationRecord *messages,
    const PharaohChunkMigrationRecord *events,
    const PharaohChunkIoContext *external,
    const PharaohCompressedChunkState *compressed);
#endif

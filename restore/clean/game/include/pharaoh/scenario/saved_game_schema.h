#ifndef PHARAOH_SCENARIO_SAVED_GAME_SCHEMA_H
#define PHARAOH_SCENARIO_SAVED_GAME_SCHEMA_H
#include "pharaoh/runtime/chunk_io.h"

/* Exact 181 native chunk shapes. Version/count destinations must bind the
 * corresponding PharaohChunkIoContext members; chunk6 is 300 stored records.
 * Caller supplies both real migration tables, terminated by version zero. */
void pharaoh_saved_game_schema_bind(PharaohChunkDescriptor descriptors[181],
    void *const destinations[181],
    const PharaohChunkMigrationRecord *message_migrations,
    const PharaohChunkMigrationRecord *event_migrations);
#endif

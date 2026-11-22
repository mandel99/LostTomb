#ifndef PHARAOH_SCENARIO_MISSION_SCHEMA_H
#define PHARAOH_SCENARIO_MISSION_SCHEMA_H

#include "pharaoh/runtime/chunk_io.h"
#include "pharaoh/simulation/flood_settings.h"
#include "pharaoh/rendering/monument_spot_animation.h"

typedef struct PharaohMissionSchemaContext {
    PharaohChunkDescriptor *saved; /* 181 records */
    PharaohChunkDescriptor *map;   /* 24 records */
    PharaohFloodplainSettings *floodplain_settings;
    void *service_visit_grid; /* 51984 elements of4 bytes */
    PharaohMonumentSpotAnimation *sun_temple_spots; /* 13 records */
    void *routing_fields[16]; /* native descending D0568C..D05650 */
    void *saved_tail[15]; /* destinations166..180, in native order */
    void *map_enemy_table; /* 36 records of8 bytes */
    void *grass_growth_grid; /* 51984 bytes */
} PharaohMissionSchemaContext;

/* Native CRT entries004D1340 ->004D1350 and004D4A50 ->004D4A60.
 * Supply the static base schemas first. These retain saved147.codec and
 * map21.codec exactly; the original constructors do not overwrite them. */
void pharaoh_saved_game_schema_start(const PharaohMissionSchemaContext *c);
void pharaoh_saved_game_schema_finish(const PharaohMissionSchemaContext *c);
void pharaoh_map_schema_start(const PharaohMissionSchemaContext *c);
void pharaoh_map_schema_finish(const PharaohMissionSchemaContext *c);

#endif

#include "pharaoh/scenario/mission_schema.h"

#include <stddef.h>

static void set_descriptor(PharaohChunkDescriptor *d, uint32_t codec,
    void *destination, int32_t size, int32_t count)
{
    d->codec = codec;
    d->destination = destination;
    d->element_size = size;
    d->element_count = count;
    d->migrations = NULL;
}

/* Completed C algorithm reused from MissionSchema.c; no legacy-save repair
 * shim is part of these native constructor entries. */
void pharaoh_saved_game_schema_finish(const PharaohMissionSchemaContext *c)
{
    static const struct { uint32_t codec; int32_t size, count; } tail[15] = {
        {0,1,26}, {0,1,15}, {1,1,51984}, {0,1,1}, {1,1,51984},
        {0,24,10}, {0,24,18}, {0,4,1}, {0,4,1}, {0,12,1},
        {0,24,27}, {0,24,27}, {0,24,15}, {0,24,56}, {0,24,75}
    };
    c->saved[147].destination = c->floodplain_settings;
    c->saved[147].element_size = 36;
    c->saved[147].element_count = 1;
    c->saved[147].migrations = NULL;
    set_descriptor(&c->saved[148], 1, c->service_visit_grid, 4, 51984);
    set_descriptor(&c->saved[149], 0, c->sun_temple_spots, 24, 13);
    for (uint32_t i = 0; i < 16; ++i)
        set_descriptor(&c->saved[150 + i], 0, c->routing_fields[i], 4, 1);
    for (uint32_t i = 0; i < 15; ++i)
        set_descriptor(&c->saved[166 + i], tail[i].codec, c->saved_tail[i],
            tail[i].size, tail[i].count);
}

void pharaoh_saved_game_schema_start(const PharaohMissionSchemaContext *c)
{
    pharaoh_saved_game_schema_finish(c);
}

void pharaoh_map_schema_finish(const PharaohMissionSchemaContext *c)
{
    c->map[21].destination = c->floodplain_settings;
    c->map[21].element_size = 36;
    c->map[21].element_count = 1;
    c->map[21].migrations = NULL;
    set_descriptor(&c->map[22], 0, c->map_enemy_table, 8, 36);
    set_descriptor(&c->map[23], 1, c->grass_growth_grid, 1, 51984);
}

void pharaoh_map_schema_start(const PharaohMissionSchemaContext *c)
{
    pharaoh_map_schema_finish(c);
}

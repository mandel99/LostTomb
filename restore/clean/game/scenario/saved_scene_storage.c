#include "pharaoh/scenario/saved_scene_storage.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

_Static_assert(sizeof(PharaohBuildingTypeRecord) == 264 &&
    sizeof(PharaohWalkerQueryRecord) == 388 &&
    sizeof(PharaohScenarioPlayerRuntime) == 18904 &&
    sizeof(PharaohSavedHistory) == 820 &&
    sizeof(PharaohFloodplainSettings) == 36,
    "saved scene record shapes changed");
_Static_assert(sizeof(void *) != 4 ||
    (sizeof(PharaohEditorRoute) == 324 &&
     sizeof(PharaohMonumentSpotAnimation) == 24),
    "saved scene pointer record shapes changed");

static void *allocate(void *user_data, uint32_t bytes)
{ (void)user_data; return malloc(bytes); }
static void release(void *user_data, void *allocation)
{ (void)user_data; free(allocation); }

static uint32_t align_payload(uint32_t offset)
{
    union PayloadAlignment { long double real; uint64_t integer; void *pointer; };
    const uint32_t alignment = _Alignof(union PayloadAlignment);
    return (offset + alignment - 1u) / alignment * alignment;
}

static int arena_chunk(uint32_t index)
{ return index > 6 && index != 174 && index != 175; }

static void bind_views(PharaohSavedSceneStorage *s)
{
    void **const d = s->destinations;
    s->view = (PharaohSavedSceneViews){
        .images=d[7], .edges=d[8], .building_ids=d[9], .terrain=d[10],
        .canal_water=d[11], .walker_ids=d[12], .properties=d[13],
        .random=d[15], .height_values=d[16], .walkers=d[21],
        .players=d[28], .current_player=d[31], .buildings=d[32],
        .city_orientation=d[33], .empire_cities=d[50], .metadata=d[75],
        .storages=d[113], .history_snapshots=d[140], .empire_objects=d[142],
        .routes=d[143], .meadow_value=d[135], .variation_tag=d[144],
        .floodplain_settings=d[147], .service_visits=d[148],
        .game_is_replay=d[169], .fertility=d[170],
        .work_spots={d[171],d[172],d[146],d[149]}
    };
}

int32_t pharaoh_saved_scene_storage_open(PharaohSavedSceneStorage *s,
    PharaohSettingsFile *settings, const PharaohSavedSceneMemory *memory)
{
    PharaohChunkDescriptor shape[PHARAOH_SAVED_SCENE_CHUNKS];
    uint32_t bytes = 0;
    memset(s, 0, sizeof(*s));
    s->memory = memory != NULL ? *memory :
        (PharaohSavedSceneMemory){NULL,allocate,release};
    s->settings = settings;
    pharaoh_saved_game_schema_bind(shape, s->destinations, NULL, NULL);
    for (uint32_t i=0; i<PHARAOH_SAVED_SCENE_CHUNKS; ++i)
        if (arena_chunk(i)) {
            bytes = align_payload(bytes);
            bytes += (uint32_t)shape[i].element_size *
                (uint32_t)shape[i].element_count;
        }
    s->allocation = s->memory.allocate(s->memory.user_data, bytes);
    if (s->allocation == NULL) return 0;
    s->allocation_bytes = bytes;
    memset(s->allocation, 0, bytes);
    bytes = 0;
    for (uint32_t i=0; i<PHARAOH_SAVED_SCENE_CHUNKS; ++i)
        if (arena_chunk(i)) {
            bytes = align_payload(bytes);
            s->destinations[i] = (uint8_t *)s->allocation + bytes;
            bytes += (uint32_t)shape[i].element_size *
                (uint32_t)shape[i].element_count;
        }
    s->destinations[0] = &settings->mission_id;
    s->destinations[1] = &settings->campaign;
    s->destinations[2] = &settings->previous_campaign_link;
    s->destinations[3] = &settings->campaign_link;
    s->destinations[174] = &settings->difficulty;
    s->destinations[175] = &s->mission_records.started;
    bind_views(s);
    PharaohMonumentSpotAnimation *const spots[9] = {
        s->destinations[171], s->destinations[172], s->destinations[146],
        s->destinations[149], s->destinations[176], s->destinations[179],
        s->destinations[177], s->destinations[178], s->destinations[180]
    };
    pharaoh_monument_scene_data_initialize(&s->monument_data, spots);
    return 1;
}

void pharaoh_saved_scene_storage_close(PharaohSavedSceneStorage *s)
{
    if (s->allocation != NULL)
        s->memory.release(s->memory.user_data, s->allocation);
    memset(s, 0, sizeof(*s));
}

void pharaoh_saved_scene_bind_reader(PharaohSavedSceneStorage *s,
    PharaohSavedGameIo *io, PharaohSavedGameReadContext *read,
    const PharaohChunkMigrationRecord *messages,
    const PharaohChunkMigrationRecord *events,
    const PharaohChunkIoContext *external,
    const PharaohCompressedChunkState *compressed)
{
    pharaoh_saved_game_io_bind(io, read, s->destinations,
        messages, events, external, compressed);
    s->destinations[4] = &io->chunks.file_version;
    s->destinations[5] = &io->chunks.stored_schema_present;
    s->destinations[6] = io->stored;
    read->settings = s->settings;
    read->buildings = s->view.buildings;
    read->players = s->view.players;
    read->current_player = s->view.current_player;
    read->routes = s->view.routes;
    for (uint32_t i=0; i<4; ++i) read->work_spots[i] = s->view.work_spots[i];
    read->allowed_buildings = (uint16_t *)(void *)(s->view.metadata + 0x408);
    read->predator_variant = (uint16_t *)(void *)(s->view.metadata + 0x2bc);
    read->allowed_buildings_tail = s->view.metadata + 0x462;
    read->animation_set_bytes = s->view.metadata + 0x3e4;
    read->floodplain_settings = s->view.floodplain_settings;
    read->service_visit_grid = s->view.service_visits;
    read->game_is_replay = s->view.game_is_replay;
    read->frogs_countdown = s->destinations[105];
    read->pyramid_speedup_countdown = s->destinations[106];
    read->map_sweep_total = s->destinations[107];
    read->locusts_countdown = s->destinations[40];
    read->mummies_max = s->destinations[173];
    read->hailstorm_countdown = s->destinations[46];
    read->map_sweep_countdown = s->destinations[131];
    read->pyramid_speedup_active = &s->pyramid_speedup_active;
}

void pharaoh_saved_scene_bind_upgrades(PharaohSavedSceneStorage *s,
    PharaohSavedGameUpgradeBindings *b)
{
    b->walkers = s->view.walkers;
    b->formations = s->destinations[24];
    b->name_counters = s->destinations[104];
    b->empire_cities = s->view.empire_cities;
    b->empire_objects = s->view.empire_objects;
    b->storages = s->view.storages;
    b->messages = s->destinations[77];
    b->event_count = s->destinations[136];
    b->events = (void *)((uint8_t *)s->destinations[136] + 4);
    b->resource_prices = (int32_t (*)[2])s->destinations[53] + 31;
    b->saved_pairs = s->destinations[130];
    b->history_snapshots = s->view.history_snapshots;
    b->mission_records = &s->mission_records;
    b->small_obelisk_spots = s->monument_data.small_obelisk;
    b->large_obelisk_spots = s->monument_data.large_obelisk;
    b->monument_scripts = pharaoh_monument_animation_scripts;
}

void pharaoh_saved_scene_bind_work_spots(PharaohSavedSceneStorage *s,
    PharaohSavedGameSpotBindings *b)
{
    b->caesareum = s->monument_data.caesareum;
    b->lighthouse = s->monument_data.lighthouse;
    b->library = s->monument_data.library;
    b->abu_simbel = s->monument_data.abu_simbel;
    b->royal_tombs = s->monument_data.royal_groups;
    b->scripts = pharaoh_monument_animation_scripts;
}

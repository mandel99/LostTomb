#include "pharaoh/scenario/saved_scene_reader.h"
#include "pharaoh/core/pk_explode_data.h"

#include <string.h>

static const PharaohChunkMigrationRecord message_migrations[2] = {
    {44,1000,0x88,NULL},{0,0,0,NULL}
};
static const PharaohChunkMigrationRecord event_migrations[2] = {
    {15628,1,0x82,NULL},{0,0,0,NULL}
};

/* The player index is loaded by the reader. Resolve this destination when
 * the upgrade actually executes, rather than capturing the previous player. */
static void reset_monuments(void *data)
{
    PharaohSavedSceneReader *const r = data;
    PharaohSavedGameUpgradeBindings *const b = &r->upgrades;
    int32_t *const completed = (int32_t *)(void *)((uint8_t *)
        &r->read.players[*r->read.current_player] + 0x2988);
    pharaoh_monuments_reset_runtime_fields(r->read.buildings,
        *b->pyramid_style, b->pyramid_parts, b->mastaba_parts, completed);
}

void pharaoh_saved_scene_reader_bind(PharaohSavedSceneReader *r,
    PharaohSavedSceneStorage *s, const PharaohSavedSceneReaderInputs *in)
{
    memset(r, 0, sizeof(*r));
    r->scene = s;
    r->read.operations = &r->operations;
    r->history_memory = (PharaohHistoryMemory){in->io.user_data,
        in->io.allocate, in->io.free};
    r->upgrades = (PharaohSavedGameUpgradeBindings){
        .read=&r->read, .default_resource_prices=in->default_resource_prices,
        .pyramid_style=in->pyramid_style, .pyramid_parts=in->pyramid_parts,
        .mastaba_parts=in->mastaba_parts, .histories=r->histories,
        .history_memory=&r->history_memory, .crt_random=in->random
    };
    pharaoh_saved_scene_bind_upgrades(s, &r->upgrades);
    pharaoh_saved_game_bind_upgrade_steps(&r->operations, &r->upgrades);
    r->operations.upgrade[PHARAOH_SAVE_V92_MONUMENTS] = reset_monuments;
    r->operations.upgrade_user_data[PHARAOH_SAVE_V92_MONUMENTS] = r;
    r->spots.random = in->random;
    pharaoh_saved_scene_bind_work_spots(s, &r->spots);
    pharaoh_saved_game_bind_work_spot_steps(&r->operations, &r->spots);
    r->routes = (PharaohSavedRouteBindings){in->io.user_data,
        in->io.allocate, in->io.free,
        (const PharaohRouteMapObject *)(const void *)s->view.empire_objects,
        in->route_images, in->route_image_count};
    pharaoh_saved_game_bind_route_samples(&r->operations, &r->routes);
    PharaohCompressedChunkState codec = in->compressed;
    codec.explode_tables = &pharaoh_pk_explode_tables;
    codec.crc_table = pharaoh_compression_crc_table;
    pharaoh_saved_scene_bind_reader(s, &r->io, &r->read,
        message_migrations, event_migrations, &in->io, &codec);
}

void pharaoh_saved_scene_reader_close(PharaohSavedSceneReader *r)
{
    if (r->scene == NULL) return;
    for (uint32_t i=0; i<50; ++i) {
        PharaohEditorRoute *const route = &r->scene->view.routes[i];
        if (route->samples != NULL) {
            r->routes.release(r->routes.user_data, route->samples);
            route->samples = NULL;
        }
        route->capacity = 0;
        route->sample_count = 0;
    }
    for (uint32_t i=0; i<10; ++i)
        pharaoh_history_release(&r->histories[i].queue, &r->history_memory);
    memset(r, 0, sizeof(*r));
}

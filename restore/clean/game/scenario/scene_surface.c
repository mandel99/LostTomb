#include "pharaoh/scenario/scene_surface.h"

static void rebuild_map_tiles(void *user_data)
{
    pharaoh_map_refresh_all_tiles(user_data);
}

void pharaoh_saved_game_bind_surface_step(PharaohSavedGameInitializeOperations *operations,
    PharaohMapSurfaceContext *surface)
{
    operations->step[PHARAOH_INIT_MAP_TILES]=(PharaohSavedGameStepBinding){
        surface,rebuild_map_tiles};
}

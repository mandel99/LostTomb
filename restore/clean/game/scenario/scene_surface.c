#include "pharaoh/scenario/scene_surface.h"
#include "pharaoh/map/moisture.h"

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

static void revalidate_water_images(void *user_data)
{
    pharaoh_map_revalidate_water_images(user_data);
}

void pharaoh_saved_game_bind_moisture_step(PharaohSavedGameInitializeOperations *operations,
    PharaohMapSurfaceContext *surface)
{
    operations->step[PHARAOH_INIT_FLOOD_REVALIDATION]=(PharaohSavedGameStepBinding){
        surface,revalidate_water_images};
}

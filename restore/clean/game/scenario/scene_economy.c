#include "pharaoh/scenario/scene_economy.h"

static void object_images(void *user_data)
{
    const PharaohSceneEconomy *c = user_data;
    pharaoh_empire_refresh_object_images(c->images);
}

static void city_images(void *user_data)
{
    const PharaohSceneEconomy *c = user_data;
    pharaoh_empire_refresh_city_image_dimensions(c->images);
}

static void resources(void *user_data)
{
    const PharaohSceneEconomy *c = user_data;
    pharaoh_city_resource_determine_available(c->resources);
}

void pharaoh_saved_game_bind_economy_steps(
    PharaohSavedGameInitializeOperations *ops, PharaohSceneEconomy *c)
{
    ops->step[PHARAOH_INIT_EMPIRE_IMAGES] =
        (PharaohSavedGameStepBinding){c, object_images};
    ops->step[PHARAOH_INIT_EMPIRE_CITY_IMAGES] =
        (PharaohSavedGameStepBinding){c, city_images};
    ops->step[PHARAOH_INIT_RESOURCES] =
        (PharaohSavedGameStepBinding){c, resources};
}

#ifndef PHARAOH_SCENARIO_SAVED_GAME_INITIALIZE_H
#define PHARAOH_SCENARIO_SAVED_GAME_INITIALIZE_H

#include "pharaoh/scenario/saved_game_state_read.h"
#include "pharaoh/ui/briefing.h"

enum PharaohSavedGameInitializeStep {
    PHARAOH_INIT_MAP_GRID,       /* 0046F7B0 */
    PHARAOH_INIT_BUILDING_MODEL, /* 00540C90 */
    PHARAOH_INIT_FIGURE_MODEL,   /* 00540ED0 */
    PHARAOH_INIT_TAX_MODEL,      /* 00541160 */
    PHARAOH_INIT_MONUMENT_SCRIPTS, /* 004F4520 */
    PHARAOH_INIT_EXTRA_SCRIPTS,  /* 0053D970 */
    PHARAOH_INIT_ENEMY_PROPERTIES, /* 004BC720 */
    PHARAOH_INIT_SHRUBS,         /* 004771C0 */
    PHARAOH_INIT_FLAG,           /* 004E7E20 */
    PHARAOH_INIT_BUILDING_COUNTS, /* 004677C0 */
    PHARAOH_INIT_CULTURE_COVERAGE, /* 00468170 */
    PHARAOH_INIT_OBELISK_SCRIPTS, /* 004E59A0 */
    PHARAOH_INIT_BUILDING_TILES, /* 00485F70 */
    PHARAOH_INIT_FLOOD_REVALIDATION, /* 00486E70 */
    PHARAOH_INIT_LEGACY_FARMS,   /* 00489840 */
    PHARAOH_INIT_MAP_TILES,      /* 004DEA50 */
    PHARAOH_INIT_MAP_BOUNDS,     /* 004DDF90 */
    PHARAOH_INIT_CAMERA_BOUNDS,  /* 004DE080 */
    PHARAOH_INIT_ROUTE_RESET,    /* 00517A20 */
    PHARAOH_INIT_WATER_ROUTES,   /* 005178B0 */
    PHARAOH_INIT_CITIZEN_ROUTES, /* 00517370 */
    PHARAOH_INIT_NONCITIZEN_ROUTES, /* 00517660 */
    PHARAOH_INIT_WALL_ROUTES,    /* 005178E0 */
    PHARAOH_INIT_CLEAN_ROUTES,   /* 004B32C0 */
    PHARAOH_INIT_ROAD_NETWORK,   /* 00468860 */
    PHARAOH_INIT_KINGDOM_ACCESS, /* 00457AF0 */
    PHARAOH_INIT_GRANARY_STOCKS, /* 0045E040 */
    PHARAOH_INIT_BUILDING_MENU,  /* 00404350 */
    PHARAOH_INIT_SIDEBAR,        /* 004051A0 */
    PHARAOH_INIT_MESSAGE_AREAS,  /* 004E1830 */
    PHARAOH_INIT_CITY_SOUND,     /* 0041D730 */
    PHARAOH_INIT_STATE_PAIR,     /* 0041DF60 */
    PHARAOH_INIT_GARDENS,        /* 00476750 */
    PHARAOH_INIT_ROTATED_BUILDINGS, /* 0046D840 */
    PHARAOH_INIT_DESTINATIONS,   /* 004BC7A0 */
    PHARAOH_INIT_TRANSPORT_COUNTS, /* 004BCA90 */
    PHARAOH_INIT_EMPIRE_IMAGES,  /* 00442660 */
    PHARAOH_INIT_EMPIRE_CITY_IMAGES, /* 004426C0 */
    PHARAOH_INIT_REPAIR_GRANARIES, /* 004D5AD0 */
    PHARAOH_INIT_FIND_D1_ANCHOR,  /* 004D5BC0 */
    PHARAOH_INIT_EXPORT_KEEP,    /* 00443330 */
    PHARAOH_INIT_RESOURCES,      /* 005596D0 */
    PHARAOH_INIT_HIGHEST_BUILDING, /* 0045AA30 */
    PHARAOH_INIT_MONUMENT_SLOTS, /* 004F61E0 */
    PHARAOH_INIT_FERRY_PASSENGERS, /* 004A3560 */
    PHARAOH_INIT_STEP_COUNT
};

typedef struct PharaohSavedGameGeometry {
    int32_t width, height, grid_start, grid_border;
} PharaohSavedGameGeometry;

typedef struct PharaohSavedGameStepBinding {
    void *user_data;
    PharaohSavedGameStep apply;
} PharaohSavedGameStepBinding;

typedef struct PharaohSavedGameInitializeOperations {
    void *user_data;
    PharaohSavedGameStepBinding step[PHARAOH_INIT_STEP_COUNT];
    uint8_t (*map_inside)(void *user_data, int32_t x, int32_t y);
    void (*build_floods)(void *user_data, int32_t x0, int32_t y0,
        int32_t x1, int32_t y1);
    void (*calculate_lookup)(void *user_data, int32_t orientation,
        int32_t force);
    int32_t (*count_warship_wharves)(void *user_data);
} PharaohSavedGameInitializeOperations;

typedef struct PharaohSavedGameInitializeContext {
    void *map_image_grid; /* 0x32c40 bytes */
    const int16_t *scenario_climate;
    int32_t *current_climate;
    const PharaohSavedGameGeometry *scenario_geometry;
    PharaohSavedGameGeometry *geometry;
    const int32_t *diamond_x_min, *diamond_y_min;
    int16_t *invasion_x, *invasion_y; /* 16 words each */
    int32_t *city_view_orientation, *map_view_orientation;
    int32_t *file_version;
    uint8_t *flood_revalidation_pending;
    int32_t *removal_undo, *city_overlay, *previous_city_overlay;
    PharaohBriefingState *briefing;
    PharaohScenarioPlayerRuntime *players;
    const int32_t *current_player;
    int32_t *message_scroll_position, *message_list_scroll;
    uint8_t *ui_flag;
    uint32_t *health_recompute_distances;
    int32_t *warship_wharf_count;
    const int32_t *scenario_mode;
    int32_t *custom_value_78d828;
    const PharaohSavedGameInitializeOperations *operations;
} PharaohSavedGameInitializeContext;

/* Complete typed reuse of Unit01828.c, native 0x004D52A0. Child operations
 * require their real C contexts; the prior schema-recovery shim is excluded. */
void pharaoh_saved_game_initialize(PharaohSavedGameInitializeContext *context);

#endif

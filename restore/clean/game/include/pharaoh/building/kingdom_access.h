#ifndef PHARAOH_BUILDING_KINGDOM_ACCESS_H
#define PHARAOH_BUILDING_KINGDOM_ACCESS_H

#include "pharaoh/map/building_access_queries.h"
#include "pharaoh/map/access_routing.h"
#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/scenario/scene_routing.h"
#include "pharaoh/runtime/city_view.h"

enum PharaohBuildingAccessMonument {
    PHARAOH_ACCESS_SUN_TEMPLE, PHARAOH_ACCESS_LIBRARY,
    PHARAOH_ACCESS_ENTRANCE, PHARAOH_ACCESS_CAESAREUM,
    PHARAOH_ACCESS_LIGHTHOUSE, PHARAOH_ACCESS_MAUSOLEUM,
    PHARAOH_ACCESS_SMALL_TOMB, PHARAOH_ACCESS_MEDIUM_TOMB,
    PHARAOH_ACCESS_LARGE_TOMB, PHARAOH_ACCESS_GRAND_TOMB,
    PHARAOH_ACCESS_MONUMENT_COUNT
};
typedef struct PharaohBuildingAccessPosition {
    void *user_data;
    uint8_t (*find)(void *user_data, int32_t building, int32_t *x, int32_t *y);
} PharaohBuildingAccessPosition;

typedef struct PharaohBuildingAccessOperations {
    void *user_data;
    PharaohBuildingAccessPosition monument[PHARAOH_ACCESS_MONUMENT_COUNT];
    void (*displace_house)(void *, int32_t player, int32_t x, int32_t y,
        int32_t population); /* 00459020 */
    void (*post_problem)(void *, int32_t message, int32_t a, int32_t b); /* 004E2970 */
    void (*post_message)(void *, int32_t message, int32_t a, int32_t tile,
        int32_t b, int32_t c); /* 004E0CF0 */
    void (*remove_special)(void *, int32_t building); /* 004C03D0 */
    void (*remove_footprint)(void *, int32_t building, int32_t x, int32_t y,
        int32_t width, int32_t height); /* 0047C2B0 */
    void (*clear_farm_tiles)(void *, int32_t building, int32_t kind); /* 0043FD00 */
    void (*explosion_cloud)(void *, int32_t x, int32_t y, int32_t size,
        int32_t mode); /* 0048C0E0 */
    void (*clear_parts)(void *, int32_t building, int32_t mode); /* 004695D0 */
    void (*refresh_water)(void *, int32_t x0, int32_t y0, int32_t x1,
        int32_t y1, int32_t mode); /* 00516F90 */
    void (*refresh_citizen)(void *, int32_t x0, int32_t y0, int32_t x1,
        int32_t y1); /* 005173A0 */
} PharaohBuildingAccessOperations;

typedef struct PharaohBuildingAccessContext {
    PharaohBuildingTypeRecord *buildings;
    const int32_t *highest_id, *current_player;
    PharaohScenarioPlayerRuntime *players;
    PharaohRoadAccessMap *access;
    const PharaohSpecialEntryMap *pyramid;
    PharaohAccessRepairMap *repair;
    PharaohSceneRouting *routes;
    PharaohMapSurfaceContext *surface;
    PharaohCityViewContext *view;
    uint32_t *event_popup, *removal_undo;
    const PharaohBuildingAccessOperations *operations;
} PharaohBuildingAccessContext;

/* Native00458270 and00457AF0, with mandatory typed child boundaries above.
 * Binding them requires their real C children; no IL or fallback is supplied. */
void pharaoh_building_remove_highest_serial(PharaohBuildingAccessContext *c);
void pharaoh_building_update_kingdom_access(PharaohBuildingAccessContext *c);
void pharaoh_saved_game_bind_kingdom_access_step(
    PharaohSavedGameInitializeOperations *ops, PharaohBuildingAccessContext *c);

/* Complete two-child native00430CD0. */
void pharaoh_city_view_focus_tile(PharaohCityViewContext *view, int32_t tile,
    int32_t *result_x, int32_t *result_y);

#endif

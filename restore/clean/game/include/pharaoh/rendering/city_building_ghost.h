#ifndef PHARAOH_RENDERING_CITY_BUILDING_GHOST_H
#define PHARAOH_RENDERING_CITY_BUILDING_GHOST_H

#include "pharaoh/building/construction_preview.h"
#include "pharaoh/map/construction_routes.h"
#include "pharaoh/population/food_supply_query.h"
#include "pharaoh/editor/validation.h"
#include "pharaoh/rendering/image_draw.h"
#include "pharaoh/rendering/upper_sprite_draw.h"
#include "pharaoh/runtime/city_view.h"

typedef struct PharaohGhostDefinition {
    int32_t size;
    uint32_t reserved_04,image_group;
    int32_t image_offset;
    uint32_t reserved_10;
} PharaohGhostDefinition; /* Actual5D06DC,20-byte rows. */
typedef struct PharaohGhostImageGroup { int32_t first_image; uint32_t reserved; }
    PharaohGhostImageGroup; /* ActualF55BB4,8-byte rows. */
typedef struct PharaohGhostMapObject { uint8_t reserved[24],state; }
    PharaohGhostMapObject; /* Actual D05720 pointer grid observes only BYTE+18. */
typedef struct PharaohGhostScreenOffset { int32_t x,y; } PharaohGhostScreenOffset;
typedef struct PharaohGhostDrawQueueEntry { int32_t image,x,y; uint32_t reserved; }
    PharaohGhostDrawQueueEntry; /* Actual70E140,16-byte rows. */

typedef struct PharaohGhostCall0 { void *context; void (*draw)(void *); } PharaohGhostCall0;
typedef struct PharaohGhostCall1 { void *context; void (*draw)(void *,int32_t); } PharaohGhostCall1;
typedef struct PharaohGhostCall2 { void *context; void (*draw)(void *,int32_t,int32_t); } PharaohGhostCall2;
typedef struct PharaohGhostCall3 { void *context; void (*draw)(void *,int32_t,int32_t,int32_t); } PharaohGhostCall3;
typedef struct PharaohGhostQuery1 { void *context; int32_t (*query)(void *,int32_t); } PharaohGhostQuery1;

/* Required genuine C providers for the listed Native children. Each context
 * is that provider's actual typed object/aggregate. No token is executed as a
 * code address, and a missing callback never draws or succeeds by default. */
typedef struct PharaohCityBuildingGhostOperations {
    PharaohGhostCall0 preview_43d300,preview_43d720,preview_43e090;
    PharaohGhostCall0 preview_441490,preview_441f80,preview_4bfcd0;
    PharaohGhostCall0 preview_4debe0,preview_4edbb0,preview_541c00;
    PharaohGhostCall1 preview_43df10,preview_43f5b0,preview_4c0f30,preview_4c2f50;
    PharaohGhostCall2 preview_43dd70;
    PharaohGhostCall3 bridge_probe_47f200;
    PharaohGhostQuery1 road_canal_image_51c460; /* full signed EAX:1/15/16/36/42 */
    PharaohGhostCall0 member_40d870; /* actual receiver6FD5D8 */
    PharaohGhostCall0 member_401420; /*6DE650*/
    PharaohGhostCall0 member_416f90; /*6FD368*/
    PharaohGhostCall0 member_5456e0; /*6F9320*/
    PharaohGhostCall0 member_558b80; /*6F5648*/
    PharaohGhostCall0 member_555e20; /*6DE9E0*/
    PharaohGhostCall0 member_54f410; /*6E9188*/
    PharaohGhostCall0 member_4e6610; /*6DE548*/
    void *keyboard_context;
    int16_t (*key_state)(void *,int32_t virtual_key); /* real GetAsyncKeyState */
} PharaohCityBuildingGhostOperations;

typedef struct PharaohCityBuildingGhostContext {
    const int32_t *tool,*native_605994,*native_e38dcc,*editor;
    const uint8_t *placement_byte; /*605A5C*/
    uint8_t *special_tool; /*6DE514, stored before all entry gates*/
    const int32_t *selected_tile,*tile_x,*tile_y; /*BCA300/304/308*/
    const int32_t *orientation,*view_origin_x,*view_origin_y,*map_origin; /*BCA2E0/E8/EC/D8*/
    const PharaohMapGridGeometry *geometry;
    const PharaohMapTerrainView *map;
    const int16_t *walker_ids; /*B350DC signedWORD walker grid*/
    const uint8_t *property_flags,*tile_direction; /*B285CC/ADC36C*/
    const int32_t *image_ids; /*B7482C DWORD grid*/
    const PharaohGhostMapObject *const *objects; /*D05720 pointer grid*/
    const PharaohConstructionCommitCity *cities; /*same actualEA1870,49D8 bank*/
    const PharaohConstructionPreviewCity *preview_cities; /*same bank,+4600 view*/
    const uint32_t *current_player;
    const PharaohConstructionRequirements *requirements; /*6059B4..CF DWORDs*/
    const uint32_t *terrain_rule_token; /*605A48 DWORD; zero or405B40/60/80*/
    PharaohConstructionTerrainRule terrain_predicate; /*actual C body for selected token*/
    const int32_t *native_6059e4,*native_bc9724,*native_605a28,*native_576144,*native_705c6c;
    int32_t *variant; /*BC9714, Native R-key toggle stores a DWORD*/
    PharaohBridgePlacementState *bridge; /*BC96F4*/
    const PharaohGhostDefinition *definitions;
    const PharaohGhostImageGroup *image_groups;
    const int32_t *footprint_counts; /*5CFCD4, DWORD counts indexed by size*/
    const int32_t (*footprint_offsets)[36]; /*5CFE10,4x144 bytes*/
    const PharaohGhostScreenOffset *screen_offsets; /*5CFCF0,8-byte rows*/
    const int32_t *half_height,*bridge_height,*half_width; /*70E44C/448/45C*/
    const int32_t *group_f56c94,*group_f56c34,*group_f56b84,*group_f56c44,*group_f56be4;
    const int32_t *group_f56d34,*group_f564dc,*group_f56764,*group_f56544,*group_f56b24;
    const int32_t *group_f56d54,*group_f56d5c,*group_f56d64,*group_f566ac,*group_f566b4,*group_f5653c;
    uint16_t *tint; /*70E120 WORD, aliases image context tint*/
    uint32_t *draw_count; /*70E13C DWORD*/
    PharaohGhostDrawQueueEntry *draw_queue;
    PharaohImageDrawContext *images;
    const PharaohUpperSpriteDrawContext *upper;
    const PharaohCityViewContext *view;
    const PharaohConstructionRouteQueries *routes;
    PharaohMapSurfaceContext *surface;
    const PharaohMapImageContext *image_match;
    PharaohMapAreaCursor *area;
    const PharaohFoodSupplyQueryContext *food;
    PharaohOccupancyState *occupancy;
    const PharaohEditorTerrainKindContext *editor_terrain;
    const PharaohCityBuildingGhostOperations *operations;
} PharaohCityBuildingGhostContext;

enum PharaohCityBuildingGhostStatus {
    PHARAOH_BUILDING_GHOST_READY,
    PHARAOH_BUILDING_GHOST_MISSING_PROVIDER,
    PHARAOH_BUILDING_GHOST_UNKNOWN_TERRAIN_RULE
};
typedef struct PharaohCityBuildingGhostResult {
    enum PharaohCityBuildingGhostStatus status;
    uint32_t pending_address; /* diagnostic only, never executed */
} PharaohCityBuildingGhostResult;

/* Whole43B630 C control/data algorithm. Logical Native result is void; result
 * reports binding availability only. Supply ordinary real game geometry:
 * orientation0/2/4/6, size0..6, valid bordered grids and actual queue capacity.
 * No stack garbage input or synthetic unsupported-orientation output exists.
 * Every context aliases the same live map/image/scalar state. */
PharaohCityBuildingGhostResult pharaoh_city_building_ghost_support(
    const PharaohCityBuildingGhostContext *);
PharaohCityBuildingGhostResult pharaoh_city_building_ghost(
    const PharaohCityBuildingGhostContext *);

/* Host must preflight the active tool before entering the void city renderer.
 * A pending provider is an integration stop, not a successful empty footprint.
 * Rejected unsupported bindings do not execute the Native controller. */
#endif

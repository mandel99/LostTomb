#ifndef PHARAOH_BUILDING_CONSTRUCTION_LOAD_H
#define PHARAOH_BUILDING_CONSTRUCTION_LOAD_H

#include "pharaoh/building/allocation.h"
#include "pharaoh/building/temple_images.h"
#include "pharaoh/commerce/resource_supply.h"
#include "pharaoh/core/crt_random.h"
#include "pharaoh/core/ring_queue.h"
#include "pharaoh/map/terrain_queries.h"
#include "pharaoh/map/venue_tiles.h"
#include "pharaoh/monuments/image_updates.h"
#include "pharaoh/monuments/mausoleum_images.h"
#include "pharaoh/monuments/pyramid_images.h"
#include "pharaoh/rendering/monument_spot_animation.h"

/* Native player record view. These fields share the scenario/player backing
 * store; this is not a second collection of construction state. */
typedef struct PharaohConstructionCity {
    uint8_t native_0000[0x2788];
    int16_t palace_x, palace_y;
    int32_t palace_grid_offset, palace_building_id;
    uint8_t native_2794[0x433a-0x2794];
    int16_t sphinx_stage;
    uint8_t native_433c[0x4486-0x433c];
    int16_t palace_placed;
    uint8_t native_4488[0x44a4-0x4488];
    int16_t recruiter_x, recruiter_y;
    int32_t recruiter_grid_offset, recruiter_building_id;
    uint8_t native_44b0[0x45f4-0x44b0];
    int16_t distribution_center_x, distribution_center_y;
    int32_t distribution_center_grid_offset, distribution_center_building_id;
    int32_t distribution_center_placed;
    uint8_t native_4604[0x472c-0x4604];
    int32_t temple_complex_building_id;
    uint8_t native_4730[0x4814-0x4730];
    int16_t festival_square_placed;
    uint8_t native_4816[0x499c-0x4816];
    int32_t mansion_placed;
    uint8_t native_49a0[0x49d0-0x49a0];
    int32_t military_academy_placed;
    uint8_t native_49d4[4];
} PharaohConstructionCity;

typedef struct PharaohConstructionFormation {
    uint8_t native_00[2], legion_id, native_03;
    int16_t walker_type;
    uint8_t native_06[0x7a-6];
    int16_t experience;
    uint8_t native_7c[0x90-0x7c];
} PharaohConstructionFormation;

typedef union PharaohConstructionDword {
    uint32_t value;
    struct { uint16_t low, high; } words;
    struct { uint8_t low, upper[3]; } bytes;
} PharaohConstructionDword;

typedef struct PharaohConstructionSavedLegion {
    PharaohConstructionDword legion_id, experience, troop_count;
} PharaohConstructionSavedLegion;

_Static_assert(sizeof(PharaohConstructionCity)==0x49d8 &&
    offsetof(PharaohConstructionCity,palace_x)==0x2788 &&
    offsetof(PharaohConstructionCity,sphinx_stage)==0x433a &&
    offsetof(PharaohConstructionCity,palace_placed)==0x4486 &&
    offsetof(PharaohConstructionCity,recruiter_x)==0x44a4 &&
    offsetof(PharaohConstructionCity,distribution_center_x)==0x45f4 &&
    offsetof(PharaohConstructionCity,temple_complex_building_id)==0x472c &&
    offsetof(PharaohConstructionCity,festival_square_placed)==0x4814 &&
    offsetof(PharaohConstructionCity,mansion_placed)==0x499c &&
    offsetof(PharaohConstructionCity,military_academy_placed)==0x49d0,
    "native construction player fields");
_Static_assert(sizeof(PharaohConstructionFormation)==0x90 &&
    offsetof(PharaohConstructionFormation,walker_type)==4 &&
    offsetof(PharaohConstructionFormation,experience)==0x7a &&
    sizeof(PharaohConstructionSavedLegion)==12,
    "native construction fort and saved legion records");

/* Required native children without a clean owner. Each placement callback
 * owns the corresponding monument receiver; no substitute rendering is implied.
 * Storage/queue results are byte results in this caller, not full EAX APIs. */
typedef struct PharaohConstructionLoadOperations {
    void *user_data;
    void (*farm_update_image)(void *,int32_t building,uint32_t is_farm); /*00488E60*/
    uint8_t (*storage_allocate)(void *,int32_t building); /*0046B100*/
    void (*temple_complex_create)(void *,int32_t building,int32_t x,int32_t y,
        uint32_t owner,int32_t kind,uint32_t creating); /*004C3420*/
    uint8_t (*ferry_queue_allocate)(void *,uint32_t building); /*0046EFD0*/
    void (*water_determine_orientation_size2)(void *,int32_t x,int32_t y,
        int32_t rotate_with_view,int32_t water_lift); /*0047CC70*/
    int32_t (*legion_create)(void *,int32_t building); /*004B6E10*/
    void (*legion_add_soldiers)(void *,int32_t count,int32_t formation,
        int32_t x,int32_t y); /*00472790*/
    void (*sphinx_create)(void *,int32_t building); /*004F4F80*/
    void (*obelisk_create)(void *,int32_t building); /*004E57F0*/
    void (*obelisk_update_image)(void *,int32_t building,int32_t x,int32_t y); /*004E5840*/
    void (*sun_temple_create)(void *,int32_t x,int32_t y,uint32_t building); /*005417B0*/
    void (*abu_simbel_place)(void *,int32_t x,int32_t y,int32_t building,bool skip); /*004015A0*/
    void (*library_place)(void *,int32_t x,int32_t y,int32_t building,bool skip); /*0040DA90*/
    void (*caesareum_place)(void *,int32_t x,int32_t y,int32_t building,bool skip); /*00417180*/
    void (*small_tomb_place)(void *,int32_t x,int32_t y,int32_t building,bool skip); /*00545C30*/
    void (*medium_tomb_place)(void *,int32_t x,int32_t y,int32_t building,bool skip); /*00559130*/
    void (*large_tomb_place)(void *,int32_t x,int32_t y,int32_t building,bool skip); /*00556480*/
    void (*grand_tomb_place)(void *,int32_t x,int32_t y,int32_t building,bool skip); /*0054FBA0*/
    void (*lighthouse_place)(void *,int32_t x,int32_t y,int32_t building,bool skip); /*004E6840*/
    void (*mausoleum_place)(void *,int32_t x,int32_t y,int32_t building,uint8_t mode); /*004DF9D0*/
    void (*pyramid_create_parts)(void *,uint32_t owner,int32_t x,int32_t y,
        uint32_t size,int32_t building); /*004EE780*/
    void (*clear_building_rectangle)(void *,int32_t building,int32_t x,int32_t y,
        int32_t width,int32_t height); /*0047C2B0*/
    /* Shared provider group; NULL retains user_data for existing adapters. */
    void *support_user_data;
} PharaohConstructionLoadOperations;

typedef struct PharaohConstructionLoadContext {
    PharaohMapSurfaceContext *surface;
    PharaohConstructionCity *cities;
    PharaohConstructionFormation *formations;
    const PharaohBuildingImageProperty *properties;
    const uint32_t *current_player;
    const uint32_t *water_orientation,*water_image_offset;
    uint32_t *dock_count,*warship_wharf_count;
    int32_t *ferry_state,*first_ferry;
    PharaohConstructionSavedLegion *saved_legions; /*three rows*/
    uint32_t *monument_selections; /*six contiguous DWORDs, native6059D4*/
    const char *const *temple_scripts;
    PharaohFerryQueueRecord *ferry_queues;
    PharaohBuildingAllocationContext *allocation;
    PharaohMapBuildingTilesContext *building_tiles;
    const PharaohHouseImageData *house_images;
    PharaohVenueTileContext *venue_tiles;
    PharaohWaterOrientationMap *water_orientation_map;
    PharaohBuildingFootprintMap *water_footprint;
    PharaohWaterLiftImageState *water_lift_images;
    PharaohMapAreaCursor *terrain_cursor;
    const PharaohResourceAvailabilityContext *resources;
    const PharaohSpriteLibraryLoadContext *libraries;
    const char *const (*sphinx_stage_names)[3];
    const char *sphinx_fixed_library;
    PharaohTempleImageContext *temple_images;
    PharaohPyramidImageContext *pyramid_images;
    PharaohMausoleumImageContext *mausoleum_images;
    PharaohMonumentImageContext *monument_images[PHARAOH_ROTATED_MONUMENT_COUNT];
    PharaohMonumentImageContext *sun_temple_images;
    const PharaohMonumentAnimationRegistry *animations;
    const PharaohMonumentAnimScript *animation_scripts;
    int32_t animation_script_count;
    PharaohCrtRandom *random;
    PharaohConstructionCancelState *cancel;
    const PharaohConstructionLoadOperations *operations;
} PharaohConstructionLoadContext;

/*00470990. All contexts and children selected by the native kind/path are
 * required. Construction/player/surface views must alias the same live state. */
void pharaoh_building_construction_add_to_map(PharaohConstructionLoadContext *c,
    uint32_t owner,int32_t kind,int32_t building,int32_t x,int32_t y,
    int32_t size,uint32_t creating,uint32_t mode);
#endif

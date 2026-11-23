#ifndef PHARAOH_MAP_ROTATED_BUILDINGS_H
#define PHARAOH_MAP_ROTATED_BUILDINGS_H

#include "pharaoh/map/water_building_images.h"
#include "pharaoh/map/venue_tiles.h"
#include "pharaoh/scenario/saved_game_initialize.h"

typedef struct PharaohBuildingImageProperty {
    uint8_t native_00[8];
    int32_t image_group;
    uint8_t native_0c[8];
} PharaohBuildingImageProperty;

typedef struct PharaohBuildingImageBinding {
    void *user_data;
    void (*draw)(void *user_data,int32_t building);
} PharaohBuildingImageBinding;

enum PharaohRotatedMonument {
    /* Native kinds225/227/228/230 use receivers006FD5D8/006FD368/
     * 006DE548/006DE650 respectively; the shared4017A0 method alone does
     * not distinguish Caesareum from Abu Simbel. */
    PHARAOH_ROTATED_LIBRARY, PHARAOH_ROTATED_CAESAREUM,
    PHARAOH_ROTATED_LIGHTHOUSE, PHARAOH_ROTATED_ABU_SIMBEL,
    PHARAOH_ROTATED_SMALL_TOMB, PHARAOH_ROTATED_MEDIUM_TOMB,
    PHARAOH_ROTATED_LARGE_TOMB, PHARAOH_ROTATED_GRAND_TOMB,
    PHARAOH_ROTATED_MONUMENT_COUNT
};

typedef struct PharaohRotatedBuildingOperations {
    void *user_data;
    PharaohBuildingImageBinding monument[PHARAOH_ROTATED_MONUMENT_COUNT];
    void (*gatehouse)(void *,int32_t building); /* 004BFB30 */
    void (*temple_complex)(void *,int32_t building,int32_t image_offset); /* 004C0D50 */
    void (*sun_temple)(void *,int32_t building); /* 00541920 */
    void (*mausoleum)(void *,int32_t building); /* 004DFD90 */
    void (*bandstand_adjacent)(void *,int32_t building,int32_t x,int32_t y); /* 00440540 */
    void (*pyramid)(void *,int32_t building); /* 004EE4A0 */
    /* NULL retains the common user_data for existing operation providers. */
    void *gatehouse_user_data, *temple_complex_user_data, *sun_temple_user_data;
    void *mausoleum_user_data;
    void *bandstand_user_data, *pyramid_user_data;
} PharaohRotatedBuildingOperations;

typedef struct PharaohRotatedBuildingContext {
    PharaohMapSurfaceContext *surface;
    PharaohBuildingFootprintMap *water_footprint;
    PharaohWaterLiftImageState *water_lift;
    PharaohVenueTileContext *venue;
    const PharaohBuildingImageProperty *properties;
    const int32_t *highest_id;
    const int32_t *pyramid_style,*pyramid_part_counts,*mastaba_part_counts;
    const PharaohRotatedBuildingOperations *operations;
} PharaohRotatedBuildingContext;

void pharaoh_pyramids_update_all_images(PharaohRotatedBuildingContext *c); /* 004EE560 */
void pharaoh_map_refresh_rotated_building_images(PharaohRotatedBuildingContext *c); /* 0046D840 */
void pharaoh_saved_game_bind_rotated_buildings_step(
    PharaohSavedGameInitializeOperations *ops,PharaohRotatedBuildingContext *c);

#endif

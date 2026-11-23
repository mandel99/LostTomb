#ifndef PHARAOH_BUILDING_TEMPLE_IMAGES_H
#define PHARAOH_BUILDING_TEMPLE_IMAGES_H

#include "pharaoh/map/rotated_buildings.h"

typedef struct PharaohGatehouseImagePart {
    int32_t on_road, size, dy, dx;
} PharaohGatehouseImagePart;

typedef struct PharaohTempleDecorationImage {
    uint32_t group, image_offsets[4];
} PharaohTempleDecorationImage;

typedef struct PharaohTempleImageTables {
    uint32_t gate_style[4], gate_gods[5], gate_order[8][4];
    PharaohGatehouseImagePart gate_parts[4];
    int32_t temple_groups[48];
    PharaohTempleDecorationImage decoration[91];
    int32_t origins[4][2], views[4][4];
} PharaohTempleImageTables;

extern const PharaohTempleImageTables pharaoh_temple_image_tables;

typedef struct PharaohTempleImageContext {
    PharaohMapSurfaceContext *surface;
    PharaohScenarioPlayerRuntime *players;
    const int32_t *current_player;
    const int16_t *god_status;
    const PharaohTempleImageTables *tables;
    void *wall_user_data;
    void (*refresh_walls)(void *user_data, int32_t x, int32_t y, int32_t size);
} PharaohTempleImageContext;

void pharaoh_gatehouse_update_map(PharaohTempleImageContext *context,
    int32_t building); /* 004BFB30 */
void pharaoh_temple_complex_set_tile(PharaohTempleImageContext *context,
    int32_t x, int32_t y, uint32_t group, uint32_t image_offset); /* 004C0860 */
void pharaoh_temple_complex_update_decoration(PharaohTempleImageContext *context,
    int32_t building); /* 004C0BE0 */
void pharaoh_temple_complex_update_map(PharaohTempleImageContext *context,
    int32_t building, uint32_t image_offset); /* 004C0D50 */
void pharaoh_rotated_bind_temple_images(PharaohRotatedBuildingOperations *operations,
    PharaohTempleImageContext *context);
void pharaoh_temple_bind_wall_images(PharaohTempleImageContext *context);

#endif

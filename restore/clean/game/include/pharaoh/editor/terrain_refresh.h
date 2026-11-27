#ifndef PHARAOH_EDITOR_TERRAIN_REFRESH_H
#define PHARAOH_EDITOR_TERRAIN_REFRESH_H
#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/map/surface_image_tables.h"
#include "pharaoh/runtime/city_view.h"

typedef struct PharaohEditorTerrainContext {
    PharaohMapSurfaceContext *surface;
    const PharaohCityViewContext *view;
    uint8_t *elevation; /* Native BYTE AB623C; distinct from AC2D4C height. */
    const uint32_t *selected_elevation; /* BC96CC, shared with construction begin. */
    PharaohMapImageContextRecord *water_auxiliary; /* 5CA958,48 records. */
    PharaohMapImageContextRecord *elevation_auxiliary; /* 5CBDC8,14 records. */
} PharaohEditorTerrainContext;

void pharaoh_editor_terrain_bind_image_tables(PharaohEditorTerrainContext *c,
    PharaohSurfaceImageTables *tables);
void pharaoh_map_refresh_channels_all(PharaohMapSurfaceContext *c);
void pharaoh_editor_elevation_raise_tile(PharaohEditorTerrainContext *c,int32_t x,int32_t y);
void pharaoh_editor_elevation_lower_tile(PharaohEditorTerrainContext *c,int32_t x,int32_t y);
void pharaoh_editor_elevation_smooth_pass(PharaohEditorTerrainContext *c);
void pharaoh_editor_elevation_smooth_all(PharaohEditorTerrainContext *c);
void pharaoh_editor_refresh_clear(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_editor_refresh_trees(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_editor_refresh_water(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_editor_refresh_shrub(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_editor_refresh_rock_kind(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_editor_refresh_dune(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_editor_refresh_meadow(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_editor_refresh_marsh(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_editor_refresh_floodplain(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_editor_refresh_road(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_editor_refresh_canal(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);
void pharaoh_editor_refresh_elevation(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1);

#endif

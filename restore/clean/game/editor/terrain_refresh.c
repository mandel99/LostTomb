#include "pharaoh/editor/terrain_refresh.h"
#include "pharaoh/runtime/platform_measure_speed.h"
#include "pharaoh/map/moisture.h"

void pharaoh_editor_terrain_bind_image_tables(PharaohEditorTerrainContext *c,
    PharaohSurfaceImageTables *tables)
{
    pharaoh_map_surface_bind_image_tables(c->surface,tables);
    c->water_auxiliary=tables->water_auxiliary;
    c->elevation_auxiliary=tables->elevation_auxiliary;
}

/* Native0047BD30. Reload the live dimensions before the real region update. */
void pharaoh_map_refresh_channels_all(PharaohMapSurfaceContext *c)
{
    uint32_t height=(uint32_t)c->map->height,width=(uint32_t)c->map->width;
    pharaoh_map_update_channel_images(c,0,0,(int32_t)(width-1u),(int32_t)(height-1u));
}

/* Complete typed reuse, native0x00478D40. */
void pharaoh_editor_refresh_clear(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
  pharaoh_map_image_context_reset(c->surface->water_context,0x30);
  pharaoh_map_image_context_reset(c->water_auxiliary,0x30);
  pharaoh_map_image_context_reset(c->surface->bank_context,0x18);
  pharaoh_map_image_context_reset(c->surface->fault_context,0x11);
  pharaoh_map_refresh_water(c->surface,x0,y0,x1,y1,1);
  pharaoh_map_refresh_marshland(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_faults(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_rocks(c->surface);
  pharaoh_map_refresh_dune_region(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_empty_region(c->surface,x0,y0,x1,y1,0,0);
  pharaoh_map_refresh_floodplain(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_road_region(c->surface,(int32_t)((uint32_t)x0-1u),(int32_t)((uint32_t)y0-1u),(int32_t)((uint32_t)x1+1u),(int32_t)((uint32_t)y1+1u));
  pharaoh_map_update_channel_images(c->surface,(int32_t)((uint32_t)x0-1u),(int32_t)((uint32_t)y0-1u),(int32_t)((uint32_t)x1+1u),(int32_t)((uint32_t)y1+1u));
  pharaoh_map_refresh_meadow(c->surface,x0,y0,x1,y1);
  return;
}

/* Complete typed reuse, native0x00478E10. */
void pharaoh_editor_refresh_trees(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
  pharaoh_map_image_context_reset(c->surface->water_context,0x30);
  pharaoh_map_image_context_reset(c->water_auxiliary,0x30);
  pharaoh_map_image_context_reset(c->surface->bank_context,0x18);
  pharaoh_map_image_context_reset(c->surface->fault_context,0x11);
  pharaoh_map_refresh_water(c->surface,x0,y0,x1,y1,1);
  pharaoh_map_refresh_marshland(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_floodplain(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_faults(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_dune_region(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_rocks(c->surface);
  pharaoh_map_refresh_trees(c->surface,x0,y0,x1,y1);
  return;
}

/* Complete typed reuse, native0x00478EA0. */
void pharaoh_editor_refresh_water(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
  pharaoh_map_image_context_reset(c->surface->water_context,0x30);
  pharaoh_map_image_context_reset(c->water_auxiliary,0x30);
  pharaoh_map_image_context_reset(c->surface->bank_context,0x18);
  pharaoh_map_image_context_reset(c->surface->fault_context,0x11);
  pharaoh_map_refresh_water(c->surface,x0,y0,x1,y1,1);
  pharaoh_map_refresh_marshland(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_floodplain(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_faults(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_dune_region(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_rocks(c->surface);
  pharaoh_map_refresh_road_region(c->surface,(int32_t)((uint32_t)x0-1u),(int32_t)((uint32_t)y0-1u),(int32_t)((uint32_t)x1+1u),(int32_t)((uint32_t)y1+1u));
  pharaoh_map_update_channel_images(c->surface,(int32_t)((uint32_t)x0-1u),(int32_t)((uint32_t)y0-1u),(int32_t)((uint32_t)x1+1u),(int32_t)((uint32_t)y1+1u));
  return;
}

/* Complete typed reuse, native0x00478F40. */
void pharaoh_editor_refresh_shrub(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
  pharaoh_map_image_context_reset(c->surface->water_context,0x30);
  pharaoh_map_image_context_reset(c->water_auxiliary,0x30);
  pharaoh_map_image_context_reset(c->surface->bank_context,0x18);
  pharaoh_map_image_context_reset(c->surface->fault_context,0x11);
  pharaoh_map_refresh_water(c->surface,x0,y0,x1,y1,1);
  pharaoh_map_refresh_marshland(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_floodplain(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_faults(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_rocks(c->surface);
  pharaoh_map_refresh_dune_region(c->surface,x0,y0,x1,y1);
  pharaoh_actor_walking_no_operation();
  return;
}

/* Complete typed reuse, native0x00478FD0. */
void pharaoh_editor_refresh_rock_kind(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
  pharaoh_map_image_context_reset(c->surface->water_context,0x30);
  pharaoh_map_image_context_reset(c->water_auxiliary,0x30);
  pharaoh_map_image_context_reset(c->surface->bank_context,0x18);
  pharaoh_map_image_context_reset(c->surface->fault_context,0x11);
  pharaoh_map_refresh_water(c->surface,x0,y0,x1,y1,1);
  pharaoh_map_refresh_marshland(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_floodplain(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_faults(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_rocks(c->surface);
  pharaoh_map_refresh_dune_region(c->surface,x0,y0,x1,y1);
  return;
}

/* Complete typed reuse, native0x00479060. */
void pharaoh_editor_refresh_dune(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
  pharaoh_map_image_context_reset(c->surface->water_context,0x30);
  pharaoh_map_image_context_reset(c->water_auxiliary,0x30);
  pharaoh_map_image_context_reset(c->surface->bank_context,0x18);
  pharaoh_map_image_context_reset(c->surface->fault_context,0x11);
  pharaoh_map_refresh_water(c->surface,x0,y0,x1,y1,1);
  pharaoh_map_refresh_marshland(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_floodplain(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_faults(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_rocks(c->surface);
  pharaoh_map_refresh_dunes(c->surface);
  return;
}

/* Complete typed reuse, native0x004790E0. */
void pharaoh_editor_refresh_meadow(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
  pharaoh_map_image_context_reset(c->surface->water_context,0x30);
  pharaoh_map_image_context_reset(c->water_auxiliary,0x30);
  pharaoh_map_image_context_reset(c->surface->bank_context,0x18);
  pharaoh_map_image_context_reset(c->surface->fault_context,0x11);
  pharaoh_map_refresh_water(c->surface,x0,y0,x1,y1,1);
  pharaoh_map_refresh_marshland(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_floodplain(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_faults(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_rocks(c->surface);
  pharaoh_map_refresh_dune_region(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_meadow(c->surface,x0,y0,x1,y1);
  return;
}

/* Complete typed reuse, native0x00479170. */
void pharaoh_editor_refresh_marsh(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
  pharaoh_map_image_context_reset(c->surface->water_context,0x30);
  pharaoh_map_image_context_reset(c->water_auxiliary,0x30);
  pharaoh_map_image_context_reset(c->surface->bank_context,0x18);
  pharaoh_map_image_context_reset(c->surface->fault_context,0x11);
  pharaoh_map_refresh_water(c->surface,x0,y0,x1,y1,1);
  pharaoh_map_refresh_floodplain(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_faults(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_rocks(c->surface);
  pharaoh_map_refresh_dune_region(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_marshland(c->surface,x0,y0,x1,y1);
  return;
}

/* Complete typed reuse, native0x00479200. */
void pharaoh_editor_refresh_floodplain(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
  pharaoh_map_image_context_reset(c->surface->water_context,0x30);
  pharaoh_map_image_context_reset(c->water_auxiliary,0x30);
  pharaoh_map_image_context_reset(c->surface->bank_context,0x18);
  pharaoh_map_image_context_reset(c->surface->fault_context,0x11);
  pharaoh_map_refresh_empty_region(c->surface,x0,y0,x1,y1,0,0);
  pharaoh_map_refresh_water(c->surface,x0,y0,x1,y1,1);
  pharaoh_map_refresh_marshland(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_floodplain(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_faults(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_rocks(c->surface);
  pharaoh_map_refresh_dune_region(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_road_region(c->surface,(int32_t)((uint32_t)x0-1u),(int32_t)((uint32_t)y0-1u),(int32_t)((uint32_t)x1+1u),(int32_t)((uint32_t)y1+1u));
  pharaoh_map_update_channel_images(c->surface,(int32_t)((uint32_t)x0-1u),(int32_t)((uint32_t)y0-1u),(int32_t)((uint32_t)x1+1u),(int32_t)((uint32_t)y1+1u));
  return;
}

/* Complete typed reuse, native0x004792B0. */
void pharaoh_editor_refresh_road(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
  pharaoh_map_image_context_reset(c->surface->water_context,0x30);
  pharaoh_map_image_context_reset(c->water_auxiliary,0x30);
  pharaoh_map_image_context_reset(c->surface->bank_context,0x18);
  pharaoh_map_image_context_reset(c->surface->fault_context,0x11);
  pharaoh_map_refresh_water(c->surface,x0,y0,x1,y1,1);
  pharaoh_map_refresh_marshland(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_floodplain(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_faults(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_rocks(c->surface);
  pharaoh_map_refresh_dune_region(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_roads(c->surface);
  return;
}

/* Complete typed reuse, native0x00479340. */
void pharaoh_editor_refresh_canal(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
  pharaoh_map_image_context_reset(c->surface->water_context,0x30);
  pharaoh_map_image_context_reset(c->water_auxiliary,0x30);
  pharaoh_map_image_context_reset(c->surface->bank_context,0x18);
  pharaoh_map_image_context_reset(c->surface->fault_context,0x11);
  pharaoh_map_refresh_floodplain(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_water(c->surface,(int32_t)((uint32_t)x0-1u),(int32_t)((uint32_t)y0-1u),(int32_t)((uint32_t)x1+1u),(int32_t)((uint32_t)y1+1u),1);
  pharaoh_map_refresh_marshland(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_faults(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_rocks(c->surface);
  pharaoh_map_refresh_dune_region(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_channels_all(c->surface);
  return;
}

/* Complete typed reuse, native0x00479510. */
void pharaoh_editor_refresh_elevation(PharaohEditorTerrainContext *c,
    int32_t x0,int32_t y0,int32_t x1,int32_t y1)
{
  pharaoh_map_image_context_reset(c->surface->water_context,0x30);
  pharaoh_map_image_context_reset(c->water_auxiliary,0x30);
  pharaoh_map_image_context_reset(c->surface->bank_context,0x18);
  pharaoh_map_image_context_reset(c->elevation_auxiliary,0xe);
  pharaoh_map_image_context_reset(c->surface->fault_context,0x11);
  (void)*(volatile const int32_t *)&c->surface->map->height;(void)*(volatile const int32_t *)&c->surface->map->width;pharaoh_actor_walking_no_operation();
  pharaoh_map_refresh_water(c->surface,x0,y0,x1,y1,1);
  pharaoh_map_refresh_marshland(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_floodplain(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_trees(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_meadow(c->surface,x0,y0,x1,y1);
  pharaoh_actor_walking_no_operation();
  pharaoh_map_refresh_faults(c->surface,x0,y0,x1,y1);
  pharaoh_map_refresh_rocks(c->surface);
  pharaoh_map_refresh_empty_region(c->surface,x0,y0,x1,y1,0,0);
  return;
}

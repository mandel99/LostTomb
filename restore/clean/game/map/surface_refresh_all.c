#include "surface_refresh_internal.h"

/* Native004DEA50. The two extra argument bundles before faults/trees reach
 * a genuine RET-only child; all meaningful rectangle endpoints are reloaded.
 * Floodplain is intentionally rebuilt twice in its original position. */
void pharaoh_map_refresh_all_tiles(PharaohMapSurfaceContext *c)
{
    pharaoh_actor_walking_no_operation();
    pharaoh_map_refresh_faults(c,0,0,c->map->width-1,c->map->height-1);
    pharaoh_map_refresh_rocks(c);
    pharaoh_actor_walking_no_operation();
    pharaoh_map_refresh_trees(c,0,0,c->map->width-1,c->map->height-1);
    pharaoh_map_refresh_gardens(c);
    if(!*c->editor_active)pharaoh_map_update_entry_exit_flags(c,0);
    pharaoh_map_refresh_empty_region(c,0,0,c->map->width-1,c->map->height-1,0,0);
    pharaoh_map_refresh_floodplain(c,0,0,c->map->width-1,c->map->height-1);
    pharaoh_map_refresh_meadow(c,0,0,c->map->width-1,c->map->height-1);
    pharaoh_map_refresh_marshland(c,0,0,c->map->width-1,c->map->height-1);
    pharaoh_map_refresh_dunes(c);
    pharaoh_map_refresh_floodplain(c,0,0,c->map->width-1,c->map->height-1);
    pharaoh_map_refresh_water(c,0,0,c->map->width-1,c->map->height-1,0);
    pharaoh_map_refresh_rubble(c,0,0,c->map->width-1,c->map->height-1);
    pharaoh_map_refresh_roads(c);
    pharaoh_map_refresh_paved_region(c,0,0,c->map->width-1,c->map->height-1,1);
    pharaoh_map_refresh_canals(c);
    pharaoh_map_refresh_channel_region(c,0,0,c->map->width-1,c->map->height-1,0);
}

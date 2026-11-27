#include "pharaoh/monuments/pyramid_demolition.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohBuildingTypeRecord)==264 &&
    offsetof(PharaohBuildingTypeRecord,building_type)==16 &&
    offsetof(PharaohBuildingTypeRecord,subtype)==18 &&
    offsetof(PharaohBuildingTypeRecord,pyramid_size_variant)==0x54 &&
    offsetof(PharaohBuildingTypeRecord,grid_offset)==12,
    "native pyramid root and size fields");

/* Read from the pinned program through the open Ghidra session. */
const int32_t pharaoh_pyramid_auxiliary_offsets_regular[5]={
    1368,1822,2276,2730,3184};
const int32_t pharaoh_pyramid_auxiliary_offsets_mastaba[3]={1366,1822,2276};

/* Complete native004F4930, whole ConstructionBrush algorithm reused. */
void pharaoh_pyramid_clear_complex_temple(
    const PharaohPyramidDemolitionContext *c,int32_t building)
{
    PharaohMapSurfaceContext *s=c->surface;
    if (s->buildings[building].building_type!=183) return;
    const uint32_t root=(uint32_t)building-
        (uint32_t)(int32_t)s->buildings[building].subtype;
    const uint32_t size=s->buildings[root].pyramid_size_variant;
    if (!pharaoh_pyramid_has_complex_temple((int32_t)*s->pyramid_style,
            s->pyramid_complex_offsets,(int32_t)size)) return;
    uint32_t cell=(uint32_t)s->buildings[root].grid_offset+
        (uint32_t)s->pyramid_complex_offsets[size];
    int32_t relative=(int32_t)(cell-(uint32_t)s->map->origin);
    const int32_t x=relative%228,y=relative/228;
    pharaoh_pyramid_map_segment_update(s,x,y,5,0);
    cell=s->pyramid_placement->terminal_tile;
    relative=(int32_t)(cell-(uint32_t)s->map->origin);
    const int32_t end_x=relative%228,end_y=relative/228;
    const int32_t width=(int32_t)((uint32_t)end_x-(uint32_t)x+2u);
    const int32_t right=(int32_t)((uint32_t)end_x+2u);
    const int32_t bottom=(int32_t)((uint32_t)end_y+2u);
    pharaoh_map_refresh_empty_region(s,x,y,right,bottom,0,0);
    pharaoh_map_refresh_surrounding_empty(s,x,y,width);
    pharaoh_map_refresh_meadow(s,x,y,right,bottom);
    pharaoh_map_refresh_floodplain(s,x,y,right,bottom);
    pharaoh_map_refresh_water(s,x,y,right,bottom,0);
    pharaoh_map_refresh_marshland(s,x,y,right,bottom);
    pharaoh_map_refresh_rubble(s,x,y,right,bottom);
}

/* Complete native004F5E70, whole ConstructionLines algorithm reused. */
void pharaoh_pyramid_update_auxiliary_reservation(
    const PharaohPyramidDemolitionContext *c,int32_t building,uint8_t reserve)
{
    PharaohMapSurfaceContext *s=c->surface;
    const uint32_t root=(uint32_t)building-
        (uint32_t)(int32_t)s->buildings[building].subtype;
    const uint32_t size=s->buildings[root].pyramid_size_variant;
    const int32_t *offsets=*s->pyramid_style==5u?
        pharaoh_pyramid_auxiliary_offsets_mastaba:
        pharaoh_pyramid_auxiliary_offsets_regular;
    const uint32_t cell=(uint32_t)s->buildings[root].grid_offset+
        (uint32_t)offsets[size];
    const int32_t child=s->building_ids[cell];
    if (child==0 || s->buildings[child].building_type!=215) return;
    if (reserve!=0) {
        (void)pharaoh_map_building_undo_capture(s->undo,child);
        const uint32_t next=*c->placed_count+1u;
        s->buildings[child].state=6;
        *c->placed_count=next;
    } else s->buildings[child].state=2;
}

#include "pharaoh/building/allocation.h"

static void draw_new(PharaohBuildingAllocationContext *c,uint32_t id,uint32_t image)
{
    PharaohBuildingTypeRecord *const b=&c->surface->buildings[id];
    b->house_population=0;b->distance_from_entry=0;
    pharaoh_map_place_surface_square(c->surface,(uint16_t)id,b->x,b->y,1,1,image,8);
}

/* Completed prior BuildingSupport C, native0046CF40. */
void pharaoh_house_change_to_vacant_lots(PharaohBuildingAllocationContext *c,
    PharaohMapBuildingTilesContext *tiles,int32_t building)
{
    PharaohMapSurfaceContext *const s=c->surface;
    PharaohBuildingTypeRecord *const b=&s->buildings[building];
    const uint32_t image=s->groups[305].first_image;
    b->building_type=10;b->subtype=0;
    if (!b->house_is_merged) {s->image_ids[b->grid_offset]=image;return;}
    pharaoh_map_clear_building_tiles(tiles,building,b->x,b->y);
    b->house_is_merged=0;b->size=1;b->house_size=1;
    pharaoh_map_place_surface_square(s,(uint16_t)building,b->x,b->y,1,1,image,8);
    const int32_t x=b->x,y=b->y,x2=(int32_t)((uint32_t)x+1u),y2=(int32_t)((uint32_t)y+1u);
    uint32_t id=(uint32_t)pharaoh_building_allocate(c,*c->current_player,
        (uint32_t)(int32_t)b->building_type,x2,y,0,0);
    /* Possible original bug: failed allocation still writes/draws record0. */
    draw_new(c,id,image);
    id=(uint32_t)pharaoh_building_allocate(c,*c->current_player,
        (uint32_t)(int32_t)b->building_type,x,y2,0,0);draw_new(c,id,image);
    id=(uint32_t)pharaoh_building_allocate(c,*c->current_player,
        (uint32_t)(int32_t)b->building_type,x2,y2,0,0);draw_new(c,id,image);
}

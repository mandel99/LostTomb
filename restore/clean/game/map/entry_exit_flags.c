#include "surface_refresh_internal.h"

static int32_t marker_facing(const PharaohMapSurfaceContext *c,int32_t x,int32_t y)
{
    if(!x)return 2;
    if(x==(int32_t)((uint32_t)c->map->width-1u))return 6;
    if(!y)return 0;
    return y==(int32_t)((uint32_t)c->map->height-1u)?4:-1;
}
static void place_marker(PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t facing,
    uint32_t image_offset,int32_t *flag_x,int32_t *flag_y,int32_t *flag_tile)
{
    if(facing<0)return;
    const int32_t excluded=(int32_t)((uint32_t)c->map->origin+(uint32_t)y*228u+(uint32_t)x);
    PharaohMapAreaCursor cursor={*c->area_x,*c->area_y,0};
    for(int32_t radius=1;radius<10;++radius){
        const int32_t found=pharaoh_map_find_free_terrain(c->map,x,y,1,radius,excluded,&cursor);
        *c->area_x=cursor.x;*c->area_y=cursor.y;
        if(found)break;
    }
    *flag_x=*c->area_x;*flag_y=*c->area_y;
    const uint32_t tile=(uint32_t)c->map->origin+(uint32_t)*flag_y*228u+(uint32_t)*flag_x;
    *flag_tile=(int32_t)tile;c->map->terrain[tile]|=2u;
    int32_t direction=(int32_t)(*c->building_orientation+(uint32_t)facing);
    if(direction>7)direction-=8;
    c->image_ids[tile]=c->groups[0x209].first_image+image_offset+(uint32_t)(direction/2);
}

/* Native0047D870: removal clears the full native mask, not just rock bit2.
 * A failed radius1..9 search still publishes its final coordinates in EXE. */
void pharaoh_map_update_entry_exit_flags(PharaohMapSurfaceContext *c,int32_t remove)
{
    if(remove){
        const uint32_t origin=(uint32_t)c->map->origin;
        const uint32_t entry=origin+(uint32_t)*c->entry_y*228u+(uint32_t)*c->entry_x;
        c->map->terrain[entry]&=0xdfcffffdu;
        const uint32_t exit=origin+(uint32_t)*c->exit_y*228u+(uint32_t)*c->exit_x;
        c->map->terrain[exit]&=0xdfcffffdu;return;
    }
    const int32_t entry_facing=marker_facing(c,*c->scenario_entry_x,*c->scenario_entry_y);
    const int32_t exit_facing=marker_facing(c,*c->scenario_exit_x,*c->scenario_exit_y);
    place_marker(c,*c->scenario_entry_x,*c->scenario_entry_y,entry_facing,0,
        c->entry_x,c->entry_y,c->entry_tile);
    place_marker(c,*c->scenario_exit_x,*c->scenario_exit_y,exit_facing,4,
        c->exit_x,c->exit_y,c->exit_tile);
}

/* Native0047E5C0. Only the four cardinal DWORD slots can be added. */
void pharaoh_map_adjacent_add_gatehouse_flags(PharaohMapSurfaceContext *c)
{
    static const int32_t offsets[4]={-228,1,228,-1};
    const uint32_t tile=*c->current_tile;
    for(uint32_t side=0;side<4;++side){
        const uint32_t neighbor=tile+(uint32_t)offsets[side];
        if(!(c->map->terrain[neighbor]&0x8000u))continue;
        const PharaohBuildingTypeRecord *b=&c->buildings[c->building_ids[neighbor]];
        const uint16_t type=b->building_type;
        if((type==0x3a || type==0xaa || type==0xab || type==0xc8 || type==0xc9 || type==0xca) &&
            (uint16_t)b->subtype==(side&1u?2u:1u))c->adjacent[side*2u]=1;
    }
}

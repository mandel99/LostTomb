#include "pharaoh/building/temple_images.h"
#include "pharaoh/map/construction_region_images.h"
#include "pharaoh/religion/building_types.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohGatehouseImagePart)==16 &&
    sizeof(PharaohTempleDecorationImage)==20 &&
    offsetof(PharaohBuildingTypeRecord,runtime_value_038)==0x38 &&
    offsetof(PharaohBuildingTypeRecord,next_part_building_id)==0x3a &&
    offsetof(PharaohScenarioPlayerRuntime,temple_complex_building_id)==0x472c,
    "native gatehouse and temple image inputs");

static int32_t add(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static uint32_t image(PharaohTempleImageContext *c,uint32_t group)
{return c->surface->groups[group].first_image;}
static PharaohScenarioPlayerRuntime *player(PharaohTempleImageContext *c)
{return &c->players[*c->current_player];}

void pharaoh_gatehouse_update_map(PharaohTempleImageContext *c,int32_t id)
{
    PharaohMapSurfaceContext *const s=c->surface;
    const PharaohBuildingTypeRecord *const b=&s->buildings[id];
    const int32_t x=b->x,y=b->y;
    int32_t god=pharaoh_city_god_with_status(c->god_status,2);
    if (god==-1) {god=pharaoh_city_god_with_status(c->god_status,1);if(god==-1)god=0;}
    const uint32_t god_group=c->tables->gate_gods[god];
    int32_t view=add(b->reserved_aa[0],*s->orientation);
    if (view>=8) view=add(view,-8);
    const uint32_t bias=view==0 || view==3 || view==4 || view==7;
    for (int32_t part=0;part<4;++part) {
        const PharaohGatehouseImagePart *const layout=
            &c->tables->gate_parts[c->tables->gate_order[view][part]];
        int32_t px=x,py=y;
        const uint8_t direction=b->reserved_aa[0];
        if (direction==0) {px=add(px,layout->dx);py=add(py,layout->dy);}
        else if(direction==1) {px=add(px,layout->dy);py=add(py,layout->dx);}
        uint32_t group=c->tables->gate_style[part];
        if (group==430) group=god_group;
        const uint32_t sprite=image(c,group)+bias;
        const int32_t size=layout->size;
        pharaoh_map_place_surface_square(s,(uint16_t)id,px,py,size,size,
            sprite,layout->on_road?0x8048u:0x8008u);
    }
    c->refresh_walls(c->wall_user_data,x,y,5);
}

void pharaoh_temple_complex_set_tile(PharaohTempleImageContext *c,
    int32_t x,int32_t y,uint32_t group,uint32_t image_offset)
{
    PharaohMapSurfaceContext *const s=c->surface;
    if (!pharaoh_map_grid_is_inside(s->geometry,
            add(s->geometry->diamond_x_min,x),add(s->geometry->diamond_y_min,y)))return;
    const uint32_t tile=(uint32_t)s->map->origin+(uint32_t)x+(uint32_t)y*228u;
    const uint32_t sprite=image(c,group)+image_offset;
    *s->current_tile=tile;
    const uint8_t edge=s->edge_flags[tile],properties=s->property_flags[tile];
    s->image_ids[tile]=sprite;s->edge_flags[tile]=(uint8_t)(edge|0x40u);
    const int32_t building=player(c)->temple_complex_building_id;
    s->canal_water[tile]=0;s->property_flags[tile]=(uint8_t)(properties&0xf0u);
    s->building_ids[tile]=(int16_t)building;s->map->terrain[tile]|=8u;
}

void pharaoh_temple_complex_update_decoration(PharaohTempleImageContext *c,int32_t id)
{
    const PharaohBuildingTypeRecord *const b=&c->surface->buildings[id];
    const int32_t direction=b->reserved_aa[0],turn=direction/2;
    const int32_t x=add(b->x,c->tables->origins[turn][0]);
    const int32_t y=add(b->y,c->tables->origins[turn][1]);
    const int32_t view=c->tables->views[*c->surface->orientation/2][turn];
    for (int32_t row=0;row<7;++row) for (int32_t column=0;column<13;++column) {
        const PharaohTempleDecorationImage *const entry=&c->tables->decoration[row*13+column];
        int32_t px,py;
        if(direction==0){px=add(x,column);py=add(y,row);}
        else if(direction==2){px=add(x,-row);py=add(y,-column);}
        else if(direction==4){px=add(x,-column);py=add(y,-row);}
        else{px=add(x,row);py=add(y,column);}
        const uint32_t group=entry->group;
        if (group!=0) {
            const uint32_t offset=entry->image_offsets[view==0?0:view==2?1:view==4?2:3];
            pharaoh_temple_complex_set_tile(c,px,py,group,offset);
            pharaoh_map_refresh_surrounding_empty(c->surface,px,py,1);
        }
    }
}

void pharaoh_temple_complex_update_map(PharaohTempleImageContext *c,
    int32_t id,uint32_t image_offset)
{
    PharaohMapSurfaceContext *const s=c->surface;
    PharaohBuildingTypeRecord *b=&s->buildings[id];
    if (b->runtime_value_038!=0)return; /* previous part */
    const int32_t direction=b->reserved_aa[0];
    int32_t current=id,index=0;
    while(current!=0) {
        const int32_t view=*s->orientation;
        const int32_t lateral=direction==2 || direction==6;
        const uint32_t bias=((view==0 || view==4)?lateral:!lateral)?3u:0u;
        const uint32_t group=(uint32_t)c->tables->temple_groups[(direction/2*4+view/2)*3+index];
        uint32_t sprite;
        if (group==571 && pharaoh_temple_upgrade_present(s->buildings,
                player(c)->temple_complex_building_id,1))sprite=image(c,575)+(bias?3u:2u);
        else if(group==570 && pharaoh_temple_upgrade_present(s->buildings,
                player(c)->temple_complex_building_id,2))sprite=image(c,575)+(bias?1u:0u);
        else sprite=image(c,group)+bias+image_offset;
        b=&s->buildings[current];
        pharaoh_map_place_surface_square(s,(uint16_t)current,b->x,b->y,3,3,sprite,8);
        current=b->next_part_building_id;++index;
    }
    pharaoh_temple_complex_update_decoration(c,id);
}

static void gatehouse(void *data,int32_t id)
{pharaoh_gatehouse_update_map(data,id);}
static void temple(void *data,int32_t id,int32_t offset)
{pharaoh_temple_complex_update_map(data,id,(uint32_t)offset);}
void pharaoh_rotated_bind_temple_images(PharaohRotatedBuildingOperations *ops,
    PharaohTempleImageContext *c)
{
    ops->gatehouse=gatehouse;ops->gatehouse_user_data=c;
    ops->temple_complex=temple;ops->temple_complex_user_data=c;
}

static void walls(void *data,int32_t x,int32_t y,int32_t size)
{pharaoh_map_refresh_wall_region(data,x,y,size);}
void pharaoh_temple_bind_wall_images(PharaohTempleImageContext *c)
{
    c->wall_user_data=c->surface;
    c->refresh_walls=walls;
}

#include "pharaoh/map/house_water.h"

#include <stddef.h>

static int32_t add_i32(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub_i32(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a-(uint32_t)b);}

int32_t pharaoh_map_houses_water_in_radius(
    const PharaohMapHouseWaterContext *c,int32_t building_id,int32_t radius)
{
    int32_t right=add_i32(c->buildings[building_id].x,radius);
    int32_t left=sub_i32(c->buildings[building_id].x,radius);
    int32_t bottom=add_i32(c->buildings[building_id].y,radius);
    int32_t y=sub_i32(c->buildings[building_id].y,radius);
    int32_t occupied=0;
    if(left<0)left=0;if(y<0)y=0;
    if(right>=c->map_width)right=c->map_width-1;
    if(bottom>=c->map_height)bottom=c->map_height-1;
    *c->result_y=y;
    if(y<=bottom) {
        int32_t tile=add_i32(c->map_origin,add_i32(y*228,left));
        do {
            *c->result_x=left;
            if(left<=right) {
                do {
                    const int32_t id=c->building_ids[tile];
                    if(id>0 && c->buildings[id].house_size!=0) {
                        ++occupied;
                        if(((const uint8_t *)&c->buildings[id])[0x77]==0)
                            return 0;
                    }
                    ++*c->result_x;++tile;
                } while(*c->result_x<=right);
            }
            tile=add_i32(tile,add_i32(left-right,227));
            ++*c->result_y;
        } while(*c->result_y<=bottom);
        if(occupied!=0)return 1;
    }
    return 2;
}

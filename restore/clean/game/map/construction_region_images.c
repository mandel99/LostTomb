#include "pharaoh/map/construction_region_images.h"
#include "surface_refresh_internal.h"

typedef struct Region { int32_t left,top,columns,rows; } Region;
static int32_t add(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static Region clip(const PharaohMapSurfaceContext *c,int32_t x,int32_t y,int32_t size)
{
    Region r={add(x,-1),add(y,-1),size,size};
    if(r.left<0){r.columns=add(r.left,size);r.left=0;}
    else if(add(r.left,size)>c->map->width)r.columns=add(c->map->width,-r.left);
    if(r.top<0){r.rows=add(r.top,size);r.top=0;}
    else if(add(r.top,size)>c->map->height)r.rows=add(c->map->height,-r.top);
    return r;
}

static uint32_t wall_cell(PharaohMapSurfaceContext *c)
{
    pharaoh_map_adjacent_terrain_any(c->adjacent,c->map->terrain,
        (int32_t)*c->current_tile,0x4000u);
    pharaoh_map_adjacent_flags_invert(c->adjacent);
    (void)surface_match(c,c->wall_context,48,-1,0);
    uint32_t sprite=c->selection->orientation_offset+c->groups[0x197].first_image+
        c->selection->frame;
    uint32_t tile=*c->current_tile;
    const uint8_t edge=c->edge_flags[tile];
    c->image_ids[tile]=sprite;
    const uint8_t properties=c->property_flags[tile];
    c->edge_flags[tile]=(uint8_t)(edge|0x40u);
    c->property_flags[tile]=(uint8_t)(properties&0xf0u);
    /* The existing native0047E000/0047E750 owners test gatehouse0x8000
     * neighbors and add wall0x4000 neighbors to their raw DWORD flags. */
    if(pharaoh_map_adjacent_floodplain_cardinals(c->adjacent,c->map->terrain,
            (int32_t)*c->current_tile)){
        pharaoh_map_adjacent_add_canal_flags(c->adjacent,c->map->terrain,
            (int32_t)*c->current_tile);
        if(surface_match(c,c->wall_gatehouse_context,15,-1,0)){
            sprite=c->selection->orientation_offset+c->groups[0x197].first_image+
                c->selection->frame;
            tile=*c->current_tile;c->image_ids[tile]=sprite;
        }else pharaoh_actor_walking_no_operation();
    }else pharaoh_actor_walking_no_operation();
    const int32_t y=*c->scan_y,x=*c->scan_x;
    pharaoh_map_refresh_surrounding_empty(c,x,y,1);
    return *c->current_tile;
}

void pharaoh_map_refresh_wall_region(PharaohMapSurfaceContext *c,
    int32_t x,int32_t y,int32_t size)
{
    const Region r=clip(c,x,y,size);
    const int32_t right=add(r.left,r.columns),bottom=add(r.top,r.rows);
    uint32_t tile=(uint32_t)c->map->origin+(uint32_t)r.top*228u+(uint32_t)r.left;
    *c->scan_y=r.top;*c->current_tile=tile;
    if(r.top>=bottom)return;
    int32_t row,column;
    do{
        *c->scan_x=r.left;
        if(r.left<right)do{
            const uint32_t flags=c->map->terrain[tile];
            if((flags&0x4000u) && !(flags&8u))tile=wall_cell(c);
            column=add(*c->scan_x,1);++tile;
            *c->scan_x=column;*c->current_tile=tile;
        }while(column<right);
        row=add(*c->scan_y,1);*c->scan_y=row;row=*c->scan_y;
        tile+=228u-(uint32_t)r.columns;*c->current_tile=tile;
    }while(row<bottom);
}

void pharaoh_map_refresh_road_square(PharaohMapSurfaceContext *c,
    int32_t x,int32_t y,int32_t size)
{
    const Region r=clip(c,x,y,size);
    const int32_t right=add(r.left,r.columns),bottom=add(r.top,r.rows);
    uint32_t tile=(uint32_t)c->map->origin+(uint32_t)r.top*228u+(uint32_t)r.left;
    *c->scan_y=r.top;*c->current_tile=tile;
    if(r.top>=bottom)return;
    int32_t row,column;
    do{
        *c->scan_x=r.left;
        if(r.left<right)do{
            const uint32_t flags=c->map->terrain[tile];
            if((flags&0x40u) && !(flags&12u)){
                pharaoh_map_adjacent_terrain_any(c->adjacent,c->map->terrain,
                    (int32_t)*c->current_tile,0x8000040u);
                pharaoh_map_adjacent_add_gatehouse_flags(c);
                pharaoh_map_set_road_image(c);tile=*c->current_tile;
            }
            column=add(*c->scan_x,1);++tile;
            *c->scan_x=column;*c->current_tile=tile;
        }while(column<right);
        row=add(*c->scan_y,1);tile+=228u-(uint32_t)r.columns;
        *c->scan_y=row;*c->current_tile=tile;
    }while(row<bottom);
}

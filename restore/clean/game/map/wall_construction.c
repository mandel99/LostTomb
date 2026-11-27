#include "pharaoh/map/wall_construction.h"
#include "pharaoh/building/construction_commit_support.h"
#include "pharaoh/core/memory_strings.h"
#include "surface_refresh_internal.h"

static int32_t wall_add(int32_t a,int32_t b)
{ return (int32_t)((uint32_t)a+(uint32_t)b); }

static int32_t wall_clamp(int32_t value,int32_t last)
{
    const int32_t v=value<last?value:last;
    return v<0?0:v;
}

static int32_t wall_magnitude(uint32_t value)
{
    const uint32_t sign=0u-(value>>31);
    return (int32_t)((value^sign)-sign);
}

void pharaoh_map_set_construction_wall_tile(
    const PharaohWallTilePlacementContext *c,int32_t x,int32_t y,
    uint32_t place,uint32_t mask)
{
    PharaohMapSurfaceContext *s=c->surface;
    const int32_t wx=wall_add(s->geometry->diamond_x_min,x);
    const int32_t wy=wall_add(s->geometry->diamond_y_min,y);
    if ((uint8_t)pharaoh_map_grid_is_inside(s->geometry,wx,wy)==0) return;
    const uint32_t origin=(uint32_t)s->map->origin;
    uint32_t cell=origin+(uint32_t)y*228u+(uint32_t)x;
    if (place==1) {
        const uint32_t flags=s->map->terrain[cell],old=flags&0xc04000u;
        if (old!=mask) {
            uint32_t credit=0;
            if (old==0x4000u) credit=(uint32_t)(*c->base_price/2);
            else if (old==0x404000u && mask==0x804000u)
                credit=(uint32_t)(*c->first_upgrade_price/2);
            ++*c->placed_count;
            *c->upgrade_credit+=credit;
        }
        s->map->terrain[cell]=(flags&0xff3fbfffu)|mask;
    } else s->map->terrain[cell]&=0xff3fbfffu;
    s->property_flags[cell]&=0xefu;
    int32_t left=wall_add(x,-1),top=wall_add(y,-1),columns=3,rows=3;
    if (left<0) {columns=wall_add(left,3);left=0;}
    else if (wall_add(left,3)>s->map->width)
        columns=(int32_t)((uint32_t)s->map->width-(uint32_t)left);
    if (top<0) {rows=wall_add(top,3);top=0;}
    else if (wall_add(top,3)>s->map->height)
        rows=(int32_t)((uint32_t)s->map->height-(uint32_t)top);
    *s->scan_y=top;
    cell=origin+(uint32_t)top*228u+(uint32_t)left;
    *s->current_tile=cell;
    const int32_t bottom=wall_add(top,rows),right=wall_add(left,columns);
    if (top>=bottom) return;
    int32_t row=top;
    do {
        *s->scan_x=left;
        int32_t column=left;
        if (left<right) do {
            const uint32_t flags=s->map->terrain[cell];
            if ((flags&0x4000u)!=0 && (flags&8u)==0) {
                pharaoh_map_adjacent_terrain_any(s->adjacent,s->map->terrain,
                    (int32_t)*s->current_tile,0x4000);
                pharaoh_map_adjacent_flags_invert(s->adjacent);
                (void)surface_match(s,s->wall_context,48,-1,0);
                uint32_t image=s->selection->frame+s->groups[0x197].first_image+
                    s->selection->orientation_offset;
                cell=*s->current_tile;
                const uint8_t edge=s->edge_flags[cell];
                s->image_ids[cell]=image;
                const uint8_t properties=s->property_flags[cell];
                s->edge_flags[cell]=(uint8_t)(edge|0x40u);
                s->property_flags[cell]=(uint8_t)(properties&0xf0u);
                if (pharaoh_map_adjacent_floodplain_cardinals(s->adjacent,
                    s->map->terrain,(int32_t)*s->current_tile)!=0) {
                    pharaoh_map_adjacent_add_canal_flags(s->adjacent,
                        s->map->terrain,(int32_t)*s->current_tile);
                    if (surface_match(s,s->wall_gatehouse_context,15,-1,0)!=0) {
                        image=s->selection->frame+s->groups[0x197].first_image+
                            s->selection->orientation_offset;
                        cell=*s->current_tile;s->image_ids[cell]=image;
                    } else pharaoh_actor_walking_no_operation();
                } else pharaoh_actor_walking_no_operation();
                row=*s->scan_y;column=*s->scan_x;
                pharaoh_map_refresh_surrounding_empty(s,column,row,1);
                cell=*s->current_tile;
            }
            column=wall_add(*s->scan_x,1);
            ++cell;*s->scan_x=column;*s->current_tile=cell;
        } while (column<right);
        row=wall_add(*s->scan_y,1);*s->scan_y=row;
        cell+=228u-(uint32_t)columns;*s->current_tile=cell;
    } while (row<bottom);
}

void pharaoh_construction_place_wall_strip(
    const PharaohWallConstructionContext *c,int32_t mode,
    int32_t x1,int32_t y1,int32_t x2,int32_t y2,uint32_t mask)
{
    PharaohMapSurfaceContext *s=c->tiles->surface;
    pharaoh_memory_copy_forward_words64(c->backup->saved_terrain,
        s->map->terrain,0x32c40);
    pharaoh_memory_copy_forward_words64(c->backup->saved_bytes[0],
        c->cleared,0xcb10);
    pharaoh_map_restore_free_tile_images(s,c->backup->saved_images);
    const int32_t last_x=wall_add(s->map->width,-1);
    const int32_t last_y=wall_add(s->map->height,-1);
    x1=wall_clamp(x1,last_x);y1=wall_clamp(y1,last_y);
    x2=wall_clamp(x2,last_x);y2=wall_clamp(y2,last_y);
    int32_t swap;
    if (wall_magnitude((uint32_t)y2-(uint32_t)y1)<
        wall_magnitude((uint32_t)x2-(uint32_t)x1)) {
        if (y1<y2) {const int32_t next=wall_add(y1,1);if(next<=y2)y2=next;}
        else if(y2<y1){const int32_t next=wall_add(y1,-1);if(y2<=next)y2=next;}
    } else if(x1<x2){const int32_t next=wall_add(x1,1);if(next<=x2)x2=next;}
    else if(x2<x1){const int32_t next=wall_add(x1,-1);if(x2<=next)x2=next;}
    if(x2<x1){swap=x1;x1=x2;x2=swap;}
    if(y2<y1){swap=y1;y1=y2;y2=swap;}
    *c->tiles->placed_count=0;
    const uint32_t tail=(uint32_t)s->map->width-(uint32_t)x2+(uint32_t)x1-1u;
    uint32_t cell=(uint32_t)s->map->origin+(uint32_t)y1*228u+(uint32_t)x1;
    for(int32_t y=y1;y<=y2;y=wall_add(y,1)) {
        for(int32_t x=x1;x<=x2;x=wall_add(x,1)) {
            if((s->map->terrain[cell]&0x2635177fu)==0 &&
                (s->flood_tiles[cell]==NULL ||
                    s->flood_tiles[cell]->is_floodplain!=0)) {
                const int32_t wx=wall_add(s->geometry->diamond_x_min,x);
                const int32_t wy=wall_add(s->geometry->diamond_y_min,y);
                if((uint8_t)pharaoh_map_grid_is_inside(s->geometry,wx,wy)!=0 &&
                    (uint8_t)pharaoh_construction_area_has_entry_point(
                        c->entry_points,x,y,x,y)==0 &&
                    s->blocked_grid[cell]==0) {
                    *s->current_tile=cell;
                    pharaoh_map_set_construction_wall_tile(c->tiles,x,y,1,mask);
                }
            }
            ++cell;
        }
        cell+=(uint32_t)*s->border_size+tail;
    }
    if(mode==0) {
        pharaoh_map_routing_update_citizen_rect(c->routing,x1,y1,x2,y2,1);
        pharaoh_map_routing_update_land_noncitizen_rect(c->routing->terrain,
            c->routing->building_ids,c->routing->buildings,c->routing->noncitizen,
            c->routing->geometry->grid_start,c->routing->geometry->width,
            c->routing->geometry->height,x1,y1,x2,y2);
        pharaoh_map_routing_initialize_walls(c->routing);
        *c->map_changed=1;
    }
}

#include "pharaoh/editor/terrain_refresh.h"
#include "pharaoh/runtime/platform_measure_speed.h"

/* Native004793E0/00479460; preserve the shared current-tile cursor on all exits. */
static uint32_t select_tile(PharaohEditorTerrainContext *c,int32_t x,int32_t y)
{
    uint32_t tile=(uint32_t)c->surface->map->origin+(uint32_t)y*228u+(uint32_t)x;
    *c->surface->current_tile=tile;
    return tile;
}

void pharaoh_editor_elevation_raise_tile(PharaohEditorTerrainContext *c,int32_t x,int32_t y)
{
    uint32_t tile=select_tile(c,x,y),value=c->elevation[tile],flags,v;
    if(value>=5u)return;
    if(value!=*c->selected_elevation)return;
    flags=c->surface->map->terrain[tile];if(flags&0x600u)return;
    v=c->surface->property_flags[tile];
    c->surface->property_flags[tile]=(uint8_t)(v&0xf0u);
    c->surface->map->terrain[tile]=flags&0xfbffff93u;
    c->elevation[tile]=(uint8_t)(value+1u);
}

void pharaoh_editor_elevation_lower_tile(PharaohEditorTerrainContext *c,int32_t x,int32_t y)
{
    uint32_t tile=select_tile(c,x,y),flags=c->surface->map->terrain[tile],v,w,value;
    if(flags&0x400u){
        v=c->surface->property_flags[tile];
        c->surface->map->terrain[tile]=(flags&0xfffffbffu)|0x200u;
        w=c->surface->edge_flags[tile];
        c->surface->property_flags[tile]=(uint8_t)(v&0xf0u);
        c->surface->edge_flags[tile]=(uint8_t)(w|0x40u);
    }
    value=c->elevation[tile];
    if(!value){
        flags=c->surface->map->terrain[tile];
        c->surface->map->terrain[tile]=flags&0xfffffdffu;
        return;
    }
    if(value==*c->selected_elevation){
        flags=c->surface->map->terrain[tile];
        c->surface->map->terrain[tile]=flags&0xfffff9ffu;
        c->elevation[tile]=(uint8_t)(value-1u);
    }
}

/* Native00479670. AB623C is unsigned BYTE; four real outside-map queries
 * may reload the live cursor. End-x is exclusive, matching SUB/count native. */
void pharaoh_editor_elevation_smooth_pass(PharaohEditorTerrainContext *c)
{
    uint32_t row=0,tile,remaining,limit;
    int32_t height=c->surface->map->height,first,last,result;
    uint8_t level;
    if(height<=0)return;
    do{
        first=c->surface->row_x_start[row];last=c->surface->row_x_end[row];
        tile=(uint32_t)c->surface->row_start_offset[row];
        *c->surface->current_tile=tile;
        if(first<last){
            remaining=(uint32_t)last-(uint32_t)first;
            do{
                level=c->elevation[tile];
                if(level){
                    limit=(uint32_t)level-1u;
                    if(c->elevation[tile-228u]<limit){
                        result=pharaoh_city_view_grid_offset_is_outside_map(c->view,(int32_t)(tile-228u));
                        tile=*c->surface->current_tile;if(!result)goto decrease;
                    }
                    if(c->elevation[tile+1u]<limit){
                        result=pharaoh_city_view_grid_offset_is_outside_map(c->view,(int32_t)(tile+1u));
                        tile=*c->surface->current_tile;if(!result)goto decrease;
                    }
                    if(c->elevation[tile+228u]<limit){
                        result=pharaoh_city_view_grid_offset_is_outside_map(c->view,(int32_t)(tile+228u));
                        tile=*c->surface->current_tile;if(!result)goto decrease;
                    }
                    if(c->elevation[tile-1u]<limit){
                        result=pharaoh_city_view_grid_offset_is_outside_map(c->view,(int32_t)(tile-1u));
                        tile=*c->surface->current_tile;if(!result)goto decrease;
                    }
                }
                goto advance;
            decrease:
                c->elevation[tile]=(uint8_t)(c->elevation[tile]-1u);
            advance:
                ++tile;--remaining;*c->surface->current_tile=tile;
            }while(remaining);
        }
        height=c->surface->map->height;++row;
    }while((int32_t)row<height);
}

/* Native004795E0. Four ordered passes precede the actual surface refreshes. */
void pharaoh_editor_elevation_smooth_all(PharaohEditorTerrainContext *c)
{
    int32_t last_x,last_y;
    pharaoh_editor_elevation_smooth_pass(c);
    pharaoh_editor_elevation_smooth_pass(c);
    pharaoh_editor_elevation_smooth_pass(c);
    pharaoh_editor_elevation_smooth_pass(c);
    last_y=(int32_t)((uint32_t)c->surface->map->height-1u);
    last_x=(int32_t)((uint32_t)c->surface->map->width-1u);
    /* The real0051F780 is RET; these outgoing arguments are unused. */
    (void)last_y;(void)last_x;pharaoh_actor_walking_no_operation();
    pharaoh_map_refresh_rocks(c->surface);
    last_y=(int32_t)((uint32_t)c->surface->map->height-1u);
    last_x=(int32_t)((uint32_t)c->surface->map->width-1u);
    pharaoh_map_refresh_empty_region(c->surface,0,0,last_x,last_y,0,0);
    last_y=(int32_t)((uint32_t)c->surface->map->height-1u);
    last_x=(int32_t)((uint32_t)c->surface->map->width-1u);
    pharaoh_map_refresh_meadow(c->surface,0,0,last_x,last_y);
    last_y=(int32_t)((uint32_t)c->surface->map->height-1u);
    last_x=(int32_t)((uint32_t)c->surface->map->width-1u);
    pharaoh_map_refresh_floodplain(c->surface,0,0,last_x,last_y);
}

#include "pharaoh/ui/construction_input.h"

void pharaoh_construction_begin(const PharaohConstructionInputContext *c)
{
    if(*c->anchor_tile==0 || *c->input_locked!=0) return;
    const int32_t y=*c->anchor_y,x=*c->anchor_x;
    *c->end_y=y;*c->start_y=y;*c->end_x=x;*c->start_x=x;
    *c->byte_605798=0;
    const uint32_t tile=(uint32_t)*c->map_origin+(uint32_t)y*228u+(uint32_t)x;
    *c->first_tile=tile;
    const uint32_t value=c->bytes_ab623c[tile];
    const int32_t ferry=*c->ferry_state;
    *c->initial_cell_byte=value;
    if((ferry!=0 || *c->tool!=0x88) && pharaoh_construction_backup_map(c->undo)==0)
        return;
    const int32_t mode=*c->tool;
    *c->drag_started=1;
    if(mode==5) (void)pharaoh_map_construction_distances(c->routes,*c->end_x,*c->end_y,0);
    if(*c->tool==6) (void)pharaoh_map_construction_wall_distances(c->routes,*c->end_x,*c->end_y);
    if(*c->tool==0xa8) (void)pharaoh_map_construction_wall_distances(c->routes,*c->end_x,*c->end_y);
    if(*c->tool==0xa9) (void)pharaoh_map_construction_wall_distances(c->routes,*c->end_x,*c->end_y);
    if(*c->tool==8) (void)pharaoh_map_construction_distances(c->routes,*c->end_x,*c->end_y,1);
    if(*c->tool==7) (void)pharaoh_map_construction_distances(c->routes,*c->end_x,*c->end_y,1);
}

uint32_t pharaoh_construction_cancel_live(const PharaohConstructionInputContext *c)
{
    uint32_t changed=*c->current_window!=1;
    if(*c->tool!=0) changed=1;
    const int32_t ferry=*c->ferry_state;
    *c->current_window=1;*c->tool=0;*c->drag_started=0;
    *c->start_x=0;*c->end_x=0;*c->start_y=0;*c->end_y=0;
    if(ferry==1)
        pharaoh_construction_ferry_cancel(c->ferry_state,c->buildings,*c->ferry_first_building);
    if(*c->ferry_state==2) *c->ferry_state=0;
    *c->redraw=1;
    return changed;
}

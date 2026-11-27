#include "pharaoh/ui/city_input_actions.h"
#include "pharaoh/building/kingdom_access.h"
#include "pharaoh/military/transport_boats.h"
#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohFormationCountRecord)==144,"formation stride");
_Static_assert(offsetof(PharaohFormationCountRecord,fields.native_82)==0x82,
    "legion selection signed field");

static int32_t signed_bits(uint32_t bits)
{
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}
static int32_t add(int32_t a,int32_t b)
{ return signed_bits((uint32_t)a+(uint32_t)b); }

/* Reuse of old mixed Unit00582, retaining signed SETGE at0043F057. */
uint32_t pharaoh_minimap_mouse_grid(PharaohMinimap *c,
    const int32_t *mouse_x,const int32_t *mouse_y,
    int32_t x,int32_t y,int32_t width,int32_t height)
{
    pharaoh_minimap_calculate_projection_origin(c->view,c->projection,
        width,height,&c->origin_x,&c->origin_y);
    c->top=y;c->left=x;
    c->right=signed_bits((uint32_t)x+(uint32_t)width*2u);
    c->bottom=add(y,height);
    c->scan_y=add(c->origin_y,-4);
    const int32_t end=add(height,4);
    int32_t row=-4,pixel_y=add(y,-4);
    uint32_t phase=0;
    if(end<=-4) return 0;
    do {
        int32_t pixel_x=add(x,phase?-9:-8),col=-4;
        phase^=1u;c->scan_x=add(c->origin_x,-4);
        if(width>-4) do {
            if(c->scan_x>=0 && c->scan_x<231 &&
                c->scan_y>=0 && c->scan_y<457 && *mouse_y==pixel_y &&
                (*mouse_x==pixel_x || *mouse_x==add(pixel_x,1))){
                c->tile=(uint32_t)c->projection->cells[c->scan_y*231+c->scan_x];
                return signed_bits(c->tile)<INT32_C(0x0fff0000)?c->tile:0;
            }
            pixel_x=add(pixel_x,2);c->scan_x=add(c->scan_x,1);col=add(col,1);
        } while(col<width);
        pixel_y=add(pixel_y,1);row=add(row,1);c->scan_y=add(c->scan_y,1);
    } while(row<end);
    return 0;
}

uint32_t pharaoh_minimap_sidebar_click(const PharaohMinimapClickContext *c)
{
    if(*c->hovered!=1 || (*c->left_down==0 && *c->right_went_down==0)) return 0;
    const int32_t y=*c->editor_active==0?59:30;
    const int32_t tile=signed_bits(pharaoh_minimap_mouse_grid(c->minimap,
        c->mouse_x,c->mouse_y,add(*c->sidebar_x,8),y,73,111));
    if(tile<=0) return 0;
    pharaoh_city_view_focus_tile(c->view,tile,c->focus_x,c->focus_y);
    return 1;
}

uint8_t pharaoh_city_rotate_button_input(const PharaohCityRotateInputContext *c)
{
    const uint32_t saved=*c->tooltip;
    *c->selected_mode=0;
    PharaohImageButtonRecord *const button=c->chrome->menu_button;
    if(pharaoh_image_buttons_handle_mouse(0,0,button,1,*c->trailing_trigger,c->input)==0){
        *c->tooltip=saved;
        return 0;
    }
    const int32_t width=button->width,left=button->x,cursor=c->input->cursor_x;
    *c->tooltip=20;
    const uint32_t mode=cursor<left+width/3?1u:
        cursor>=left+(width*2)/3?2u:0u;
    c->input->primary_parameter=mode;
    memcpy(button->reserved_1c,&mode,sizeof(mode));
    *c->redraw=1;
    pharaoh_city_menu_button_draw(c->chrome);
    *c->selected_mode=c->input->primary_parameter+1u;
    return 1;
}

void pharaoh_city_top_menu_help(const int32_t *hovered,
    const PharaohCityMessageQueueContext *messages,uint8_t *redraw)
{
    const int32_t selected=*hovered;
    if(selected==1) pharaoh_city_message_dialog_show(messages,15,0);
    else if(selected==2) pharaoh_city_message_dialog_show(messages,16,0);
    else if(selected==3) pharaoh_city_message_dialog_show(messages,17,0);
    *redraw=1;
}

static int selection_blocked(const PharaohCitySelectionContext *c)
{
    return (*c->current_window==1 && *c->construction_tool!=0) ||
        *c->selected_grid==0;
}
static int32_t selected_tile(const PharaohCitySelectionContext *c)
{
    return signed_bits((uint32_t)*c->cursor_x+(uint32_t)*c->cursor_y*228u+
        (uint32_t)*c->grid_start);
}

uint32_t pharaoh_city_select_legion(const PharaohCitySelectionContext *c)
{
    if(selection_blocked(c)) return 0;
    const int32_t id=pharaoh_formation_legion_at_grid_offset(c->legion_grid,
        (uint32_t)selected_tile(c));
    if(id<=0 || c->formations[id].fields.native_82>=0) return 0;
    if(pharaoh_formation_legion_is_on_ship(c->formations[id].fields.members,
        c->walkers)!=0) return 0;
    *c->selected_legion=id;*c->current_window=0x1e;*c->redraw=1;
    return 1;
}

static uint32_t select_ship(const PharaohCitySelectionContext *c,
    int32_t id,int warship)
{
    if(warship){*c->selected_warship=id;*c->warship_active=1;}
    else {*c->selected_transport=id;*c->transport_active=1;}
    *c->redraw=1;*c->current_window=(uint8_t)(warship?0x20:0x1f);
    return 1;
}

uint32_t pharaoh_city_select_warship_live(const PharaohCitySelectionContext *c)
{
    if(selection_blocked(c)) return 0;
    const int32_t tile=selected_tile(c);
    int32_t id=pharaoh_walker_find_linked_type_78(c->walkers,c->walker_heads,tile);
    if(id>0 && c->walkers[id].request_event_id>=0) return select_ship(c,id,1);
    for(int32_t i=0;i<8;++i){
        id=pharaoh_walker_find_linked_type_78(c->walkers,c->walker_heads,
            add(tile,c->direction_deltas[i]));
        if(id>0 && c->walkers[id].request_event_id<0) return select_ship(c,id,1);
    }
    return 0;
}

uint32_t pharaoh_city_select_transport_live(const PharaohCitySelectionContext *c)
{
    if(selection_blocked(c)) return 0;
    const int32_t tile=selected_tile(c);
    int32_t id=pharaoh_walker_find_linked_type_77(c->walkers,c->walker_heads,tile);
    if(id>0) return select_ship(c,id,0);
    for(int32_t i=0;i<8;++i){
        id=pharaoh_walker_find_linked_type_77(c->walkers,c->walker_heads,
            add(tile,c->direction_deltas[i]));
        if(id>0) return select_ship(c,id,0);
    }
    return 0;
}

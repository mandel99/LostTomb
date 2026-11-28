#include "pharaoh/ui/editor_city_tools.h"
#include <limits.h>

static uint32_t submenu_for(uint32_t action)
{
    switch(action){
    case 8:return 14;case 2:return 27;case 14:return 15;
    case 13:return 17;case 32:return 37;case 7:return 16;
    case 17:return 18;case 20:return 19;case 24:return 26;
    case 5:return 0;default:return UINT32_MAX;
    }
}
static int32_t tool_for(uint32_t action,int32_t current)
{
    switch(action){
    case 0:return 0x74;case 1:return 0x75;case 3:return 0x77;
    case 4:return 0x78;case 6:return 0x7a;case 10:return 5;
    case 13:return 0x7e;case 26:return 0x86;default:return current;
    }
}

void pharaoh_editor_city_preview_select(const PharaohEditorCityToolsContext *c)
{
    *c->construction->tool=0;
    *c->preview_mode=*c->button_parameter;
    *c->construction->redraw=1;
}

void pharaoh_editor_city_tool_select(const PharaohEditorCityToolsContext *c)
{
    uint32_t action;
    if(*c->button_parameter!=8){
        (void)pharaoh_construction_cancel_live(c->construction);
        action=*c->button_parameter;
    }else action=8;
    const uint32_t submenu=submenu_for(action);
    if(submenu!=UINT32_MAX){
        *c->construction->current_window=6;
        *c->menu->selected_category=(int32_t)submenu;
        const int32_t count=pharaoh_build_menu_count_items(c->menu);
        *c->popup_count=count;
        *c->popup_y_offset=c->popup_sizes[count].y_offset;
        *c->popup_width=c->popup_sizes[count].width;
        *c->construction->redraw=1;
        return;
    }
    *c->editor_category=(int32_t)action;
    *c->construction->tool=tool_for(action,*c->construction->tool);
    *c->construction->drag_started=0;
    *c->construction->start_x=0;*c->construction->end_x=0;
    *c->construction->start_y=0;*c->construction->end_y=0;
    if(*c->construction->tool==5)pharaoh_map_routing_initialize_citizen(c->routing);
    *c->construction->redraw=1;
}

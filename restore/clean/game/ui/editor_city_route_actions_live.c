#include "pharaoh/ui/editor_city_route_actions_live.h"

#include <stddef.h>

/* The callback is the real442800 child of40A090. Both it and409FA0 use the
 * same actual scroll DWORDs through the shared complete C algorithm. */
static void refresh_map_point(void *user_data)
{
    pharaoh_empire_check_scroll_boundaries_borrowed(
        (const PharaohEmpireScrollBorrowedBounds *)user_data);
}

static void commit_map_view(const PharaohEditorCityRouteLiveContext *c)
{
    const PharaohSmallButtonMapViewBorrowedState state={
        c->editor_active,c->selected_map_point,
        c->scroll==NULL?NULL:c->scroll->x,
        c->scroll==NULL?NULL:c->scroll->y,
        c->committed_map_point,c->map_points,c->current_window,c->redraw,
        refresh_map_point,c->scroll
    };
    pharaoh_small_button_commit_map_view_borrowed(&state);
}

/* Whole Native00409FA0,57 instructions. Reused EditorCityRouteActions C;
 * DWORD gates, signed mission BYTE, and post-child stores remain in order. */
void pharaoh_editor_city_enter(const PharaohEditorCityRouteLiveContext *c)
{
    const uint32_t editor=*c->editor_active;
    *c->native_5d2e88=-1;
    *c->native_f53fdc=0;
    if(editor==1u) {
        if(*c->display_mode==1) {
            pharaoh_text_select(c->primary_text,5,123);
            pharaoh_text_select_secondary(c->secondary_text,5,124);
            (void)pharaoh_message_box_show(c->message_box,
                c->message_box_operations,c->primary_text->current,
                c->secondary_text->current,0);
            *c->redraw=1;
            return;
        }
        *c->current_window=21;
        *c->city_map_kind=4;
        *c->city_map_active=1;
        pharaoh_empire_check_scroll_boundaries_borrowed(c->scroll);
        *c->redraw=1;
        return;
    }
    const int32_t custom=*c->custom_scenario;
    if(custom==0 && *c->native_78d828!=0u) {
        pharaoh_city_warning_post(c->warnings,13);
        return;
    }
    *c->current_window=20;
    if(custom==0 && c->tutorial_flags[14]==0 &&
        (int32_t)*c->selected_mission==c->tutorial_rules[14].mission_threshold) {
        *c->current_window=1;
        pharaoh_tutorial_set_flag(c->tutorial,14);
        *c->next_window=20;
    }
    *c->redraw=1;
}

/* Whole Native0040B590,20 instructions including both tail calls. */
void pharaoh_editor_city_return(const PharaohEditorCityRouteLiveContext *c)
{
    if(*c->editor_active!=1u && *c->custom_scenario==0 &&
        *c->native_78d828!=0u) {
        pharaoh_city_warning_post(c->warnings,13);
        return;
    }
    if(*c->current_window==1u)pharaoh_editor_city_enter(c);
    else commit_map_view(c);
}

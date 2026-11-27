#include "pharaoh/scenario/tutorial_complete.h"
#include <stddef.h>

_Static_assert(sizeof(void *)!=4 || (sizeof(PharaohTutorialRule)==28 &&
    offsetof(PharaohTutorialRule,message)==20),"native tutorial message");

void pharaoh_tutorial_set_flag(const PharaohTutorialCompletionContext *c,
    int32_t tutorial)
{
    c->flags[tutorial]=1;
    pharaoh_tutorial_menu_counts_store(c->counts);
    pharaoh_build_menu_update(c->counts->build_menu);
    pharaoh_sidebar_update_build_buttons(c->counts->build_menu);
    pharaoh_tutorial_menu_counts_compare(c->counts);
    pharaoh_tutorial_mission_menu_flags(*c->mission_id,*c->advisor_override,
        c->rules,c->flags,c->menu_flags);
    if(*c->messages->window_id==1)
        pharaoh_sidebar_draw_background(c->sidebar,c->sidebar_operations);
    *c->messages->ui->popup=1;
    pharaoh_city_message_post(c->messages,c->rules[tutorial].message,
        0,0,NULL,0);
}

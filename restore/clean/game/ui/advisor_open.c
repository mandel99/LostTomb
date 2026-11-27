#include "pharaoh/ui/advisor_open.h"
#include "pharaoh/scenario/tutorial_advisors.h"

PharaohPercentageScaleStatus pharaoh_advisors_show_checked(
    const PharaohAdvisorOpenContext *c)
{
    const uint32_t gate=*c->tutorial_override;
    const uint32_t requested=*c->requested_advisor;
    if(gate==0 && c->tutorial_advisors[requested]==0){
        if(*c->current_window==5){
            pharaoh_message_dialog_close_live(c->close_dialog);
            *c->current_window=1;
        }
        pharaoh_city_warning_post(c->warnings,13);
        return PHARAOH_PERCENTAGE_SCALE_VALUE;
    }
    const uint32_t player=*c->initialize->current_player;
    *c->current_window=7;
    *c->last_advisor=requested;
    *c->shown_advisor=requested;
    c->gift_notification_banks[player*UINT32_C(0x49D8)]=0;
    const PharaohPercentageScaleStatus status=
        pharaoh_advisors_initialize(c->initialize);
    if(status!=PHARAOH_PERCENTAGE_SCALE_VALUE) return status;
    *c->redraw=1;
    *c->animation_step=6;
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}

PharaohPercentageScaleStatus pharaoh_advisors_show_last(
    const PharaohAdvisorOpenContext *c)
{
    if(*c->tutorial_override==0 &&
        pharaoh_tutorial_find_available_advisor(c->tutorial_advisors,
            c->saved_advisor)==0){
        pharaoh_city_warning_post(c->warnings,13);
        return PHARAOH_PERCENTAGE_SCALE_VALUE;
    }
    const uint32_t player=*c->initialize->current_player;
    const uint32_t selected=*c->saved_advisor;
    *c->current_window=7;
    *c->active_advisor=selected;
    c->gift_notification_banks[player*UINT32_C(0x49D8)]=0;
    const PharaohPercentageScaleStatus status=
        pharaoh_advisors_initialize(c->initialize);
    if(status!=PHARAOH_PERCENTAGE_SCALE_VALUE) return status;
    *c->redraw=1;*c->animation_step=6;
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}

#include "pharaoh/scenario/tutorial_messages.h"

static int16_t legion_message_body(int infantry,int archers,int chariots)
{
    if(infantry){
        if(archers) return (int16_t)(chariots?592:589);
        return (int16_t)(chariots?590:586);
    }
    if(archers) return (int16_t)(chariots?591:587);
    return 588;
}

/* Complete MissionProgress reuse; preserve the observed retained packet
   instead of the reference candidate's unqualified zero initialization. */
void pharaoh_tutorial_update_mission_messages(
    const PharaohTutorialMissionMessagesContext *c)
{
    const PharaohTutorialCompletionContext *const t=c->completion;
    if(*t->advisor_override!=0) return;
    for(int32_t i=15;i<=25;++i){
        if(t->flags[i]==0 && t->rules[i].mission_threshold==*t->mission_id)
            pharaoh_tutorial_set_flag(t,i);
    }
    if(t->menu_flags[44]!=0) return;
    const int infantry=t->menu_flags[60]!=0 && c->allowed_legions[0]!=0;
    const int archers=t->menu_flags[72]!=0 && c->allowed_legions[1]!=0;
    const int chariots=t->menu_flags[84]!=0 && c->allowed_legions[2]!=0;
    if(infantry || archers || chariots){
        c->retained_parameters[1]=584;
        c->retained_parameters[0]=585;
        c->retained_parameters[8]=legion_message_body(infantry,archers,chariots);
        *t->messages->ui->popup=1;
        pharaoh_city_message_post(t->messages,131,0,0,c->retained_parameters,0);
    }
    t->menu_flags[44]=1;
}

#include "pharaoh/ui/briefing_hint.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohScenarioPlayerRuntime)==0x49d8 &&
    offsetof(PharaohScenarioPlayerRuntime,monument_rating)==0x42d4,
    "native tutorial monument rating");

/* Complete typed reuse of TutorialManager/HintText.c. Native priorities,
 * signed building counts, and the unchanged text slot on failure are retained. */
uint8_t pharaoh_briefing_select_tutorial_hint(const PharaohBriefingHintContext *c)
{
    const uint8_t *const flags=c->tutorial_flags;
    int32_t hint;
    if(*c->scenario_mode!=0) return 0;
    switch(*c->mission_id) {
    case 0:
        if(flags[0] && c->active_buildings[167]<1) hint=17;
        else if(flags[3] && c->active_buildings[81]<1) hint=18;
        else if(flags[2]) hint=20;
        else if(flags[1]) hint=19;
        else hint=21;
        break;
    case 1:
        if(flags[5]) hint=22;
        else if(flags[4]) hint=23;
        else hint=24;
        break;
    case 2:
        if(flags[6] && c->active_buildings[206]<1 &&
            c->active_buildings[46]<1) hint=25;
        else if(flags[9]) hint=26;
        else if(flags[7]) hint=27;
        else hint=28;
        break;
    case 3:
        if(flags[10]) return 0;
        hint=33;
        break;
    case 4:
        if(c->players[*c->current_player].monument_rating==*c->monument_goal)
            return 0;
        if(flags[13]) hint=32;
        else if(flags[12]) hint=29;
        else if(flags[11]) hint=30;
        else hint=31;
        break;
    default:return 0;
    }
    pharaoh_text_select(c->selection,62,hint);
    return 1;
}

#ifndef PHARAOH_UI_BRIEFING_HINT_H
#define PHARAOH_UI_BRIEFING_HINT_H
#include "pharaoh/runtime/text_loading.h"
#include "pharaoh/scenario/scenario_init_custom.h"

typedef struct PharaohBriefingHintContext {
    const int32_t *scenario_mode, *current_player;
    const int8_t *mission_id;
    const uint8_t *tutorial_flags; /*native7027B8, at least14 bytes*/
    const int32_t *active_buildings; /*native70573C,237 signed DWORD counts*/
    const PharaohScenarioPlayerRuntime *players;
    const int32_t *monument_goal;
    PharaohTextSelection *selection;
} PharaohBriefingHintContext;

/* Native004E2530: only AL is defined. A false result preserves selection. */
uint8_t pharaoh_briefing_select_tutorial_hint(const PharaohBriefingHintContext *c);
#endif

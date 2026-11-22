#ifndef PHARAOH_UI_BUILD_MENU_H
#define PHARAOH_UI_BUILD_MENU_H

#include "pharaoh/commerce/resource_supply.h"
#include "pharaoh/monuments/build_requirements.h"
#include "pharaoh/scenario/campaign_start.h"
#include "pharaoh/scenario/tutorial_rules.h"

enum { PHARAOH_BUILD_MENU_GROUPS=38, PHARAOH_BUILD_MENU_ITEMS=30 };

typedef struct PharaohSidebarBuildButton {
    uint8_t enabled;
    uint8_t reserved_01[3];
    int32_t category;
    uint8_t reserved_08[28];
} PharaohSidebarBuildButton;
_Static_assert(sizeof(PharaohSidebarBuildButton)==36, "native sidebar stride");

typedef struct PharaohBuildMenuContext {
    const PharaohEmpireSupplyContext *supply;
    PharaohMonumentBuildRequirements *monuments;
    const PharaohSettingsFile *settings;
    const int16_t *allowed_buildings, *complexes_allowed, *god_status;
    const int16_t *goal_text_ids;
    const PharaohTutorialRule *tutorial_rules; /* 26 native entries */
    const uint8_t *tutorial_flags;
    const int32_t *campaign_mode, *cheat_allow_all, *editor_active;
    int32_t final_mission_id;
    const int32_t *types; /* 38 groups of30 rows, native table starts5CE3BC */
    int32_t *enabled;
    int32_t *selected_category;
    PharaohSidebarBuildButton *sidebar_buttons[2]; /* 12 native records of36 bytes */
} PharaohBuildMenuContext;

/* Native00404350,00404120,004051A0; selected_category retains the last
 * category observed by the two native sidebar tables. */
void pharaoh_build_menu_update(PharaohBuildMenuContext *c);
int32_t pharaoh_build_menu_count_items(PharaohBuildMenuContext *c);
void pharaoh_sidebar_update_build_buttons(PharaohBuildMenuContext *c);

#endif

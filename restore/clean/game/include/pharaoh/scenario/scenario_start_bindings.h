#ifndef PHARAOH_SCENARIO_START_BINDINGS_H
#define PHARAOH_SCENARIO_START_BINDINGS_H
#include "pharaoh/scenario/scenario_start_mission.h"
#include "pharaoh/scenario/saved_scene_storage.h"
#include "pharaoh/scenario/campaign_records.h"
#include "pharaoh/ui/build_menu.h"
#include "pharaoh/ui/city_message_text.h"
#include "pharaoh/runtime/walker_action_profile.h"

typedef struct PharaohScenarioStartBindings {
    PharaohSavedSceneStorage *scene;
    PharaohBuildMenuContext *build_menu;
    PharaohCityMessageScenarioReset *messages;
    PharaohMissionRecordApplyState *records;
    const PharaohMissionRecordApplyOperations *record_operations;
    PharaohWalkerActionProfileState *profile;
    PharaohProfileQueryCounter query_frequency;
} PharaohScenarioStartBindings;

/* Bind the real reset/four final/profile children of005405B0. Child data and
 * operation providers must already be live. Map IO and campaign IO retain
 * their own user_data; the binding must outlive the start context. */
void pharaoh_scenario_bind_start_steps(PharaohScenarioStartMissionContext *start,
    PharaohScenarioStartBindings *bindings);
#endif

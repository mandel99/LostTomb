#include "pharaoh/scenario/scenario_start_bindings.h"
#include "pharaoh/core/static_objects.h"

static void reset_pairs(void *data)
{
    const PharaohScenarioStartBindings *const b = data;
    pharaoh_native_pairs_set_minus_one_004de140(b->scene->destinations[130]);
}
static void build_menu(void *data)
{
    pharaoh_build_menu_update(((PharaohScenarioStartBindings *)data)->build_menu);
}
static void sidebar(void *data)
{
    pharaoh_sidebar_update_build_buttons(
        ((PharaohScenarioStartBindings *)data)->build_menu);
}
static void messages(void *data)
{
    pharaoh_city_message_initialize_scenario(
        ((PharaohScenarioStartBindings *)data)->messages);
}
static void records(void *data)
{
    const PharaohScenarioStartBindings *const b = data;
    pharaoh_mission_records_apply(b->records,b->record_operations);
}
static void profile(void *data)
{
    const PharaohScenarioStartBindings *const b = data;
    pharaoh_walker_action_profile_initialize(b->profile,b->query_frequency);
}
void pharaoh_scenario_bind_start_steps(PharaohScenarioStartMissionContext *start,
    PharaohScenarioStartBindings *b)
{
    start->steps_user_data = b;
    start->game_is_replay = b->scene->view.game_is_replay;
    start->players = b->scene->view.players;
    start->current_player = b->scene->view.current_player;
    start->reset_native_pairs = reset_pairs;
    start->final_steps[0] = build_menu;
    start->final_steps[1] = sidebar;
    start->final_steps[2] = messages;
    start->final_steps[3] = records;
    start->initialize_walker_actions = profile;
    b->records->settings = start->settings;
    b->records->finished = &b->scene->mission_records.finished;
    b->records->started = &b->scene->mission_records.started;
    b->records->tutorial_legions_message_posted =
        start->tutorial_legions_message_posted;
    b->messages->messages = b->scene->destinations[77];
    b->messages->queue_delay = b->scene->destinations[78];
    b->messages->next_sequence = b->scene->destinations[79];
    b->messages->current_id = b->scene->destinations[80];
    b->messages->population_shown = b->scene->destinations[81];
    b->messages->counts = b->scene->destinations[82];
    b->messages->delays = b->scene->destinations[83];
}

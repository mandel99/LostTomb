#include "pharaoh/runtime/simulation_tick.h"

enum {
    SIMULATION_FLOOD_PEAK_STATE = 3,
    SIMULATION_OSIRIS_FARM_DESTRUCTION_PENDING = 2
};

void pharaoh_city_gods_decrease_effect_timers(
    PharaohScenarioPlayerRuntime *players, int32_t current_player)
{
    PharaohScenarioPlayerRuntime *const player = &players[current_player];

    if (player->ra_trade_bonus_months >= 0)
        --player->ra_trade_bonus_months;
    if (player->ra_trade_penalty_months >= 0)
        --player->ra_trade_penalty_months;
    if (player->ra_trade_minor_penalty_months >= 0)
        --player->ra_trade_minor_penalty_months;
    if (player->effect_timer_45cc >= 0)
        --player->effect_timer_45cc;
    if (player->effect_timer_47b4 >= 0)
        --player->effect_timer_47b4;
}

void pharaoh_simulation_tick_run(const PharaohSimulationTickContext *context)
{
    const PharaohSimulationTickOperations *const operations =
        context->operations;
    void *const user_data = operations->user_data;

    pharaoh_random_generator_advance(context->random_generator);
    pharaoh_animation_tick_counters_advance(context->animation_counters);
    operations->reduce_undo_time(user_data);

    if (*context->suppress_flood_event == 0) {
        operations->dispatch_city_simulation(user_data);
        pharaoh_city_player_one_countdown_advance(
            context->city_player_one_countdown);
        operations->update_floods(user_data);
        if (*context->flood_state == SIMULATION_FLOOD_PEAK_STATE &&
            *context->osiris_flood_destroys_farms ==
                SIMULATION_OSIRIS_FARM_DESTRUCTION_PENDING) {
            *context->osiris_flood_destroys_farms = 0;
        }
    }

    operations->update_walkers(user_data);
    operations->animate_monuments(user_data);
    operations->process_earthquakes(user_data);
    operations->check_city_victory(user_data);
}

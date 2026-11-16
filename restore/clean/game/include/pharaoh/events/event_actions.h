#ifndef PHARAOH_EVENTS_EVENT_ACTIONS_H
#define PHARAOH_EVENTS_EVENT_ACTIONS_H

#include "pharaoh/events/event_queries.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum PharaohRequestState {
    PHARAOH_REQUEST_NORMAL = 0,
    PHARAOH_REQUEST_OVERDUE = 1,
    PHARAOH_REQUEST_DISPATCHED = 2,
    PHARAOH_REQUEST_DISPATCHED_LATE = 3,
    PHARAOH_REQUEST_IGNORED = 4,
    PHARAOH_REQUEST_RECEIVED = 5,
    PHARAOH_REQUEST_TROOPS_RETURN = 6,
    PHARAOH_REQUEST_TROOPS_RETURN_TOO_LATE = 7,
    PHARAOH_REQUEST_FINISHED = 8
};

enum PharaohRequestSubtype {
    PHARAOH_REQUEST_GENERIC = 0,
    PHARAOH_REQUEST_TROOPS = 1,
    PHARAOH_REQUEST_FESTIVAL = 3,
    PHARAOH_REQUEST_CONSTRUCTION = 4,
    PHARAOH_REQUEST_FAMINE = 5,
    PHARAOH_REQUEST_CITY_IN_TROUBLE = 6
};

enum PharaohEventEffect {
    PHARAOH_EFFECT_LEGIONS_RETURN,
    PHARAOH_EFFECT_LOCUSTS,
    PHARAOH_EFFECT_FROGS,
    PHARAOH_EFFECT_HAILSTORM,
    PHARAOH_EFFECT_BLOOD_RIVER,
    PHARAOH_EFFECT_CRIME_WAVE,
    PHARAOH_EFFECT_MUMMIES
};

typedef struct PharaohEventPlayerState {
    int32_t treasury;
    int32_t *storage_stored;
    int32_t kingdom_ignored_request_penalty;
    int32_t num_sea_routes;
    int32_t num_land_routes;
    int32_t sea_trade_problem_duration;
    int32_t land_trade_problem_duration;
    int32_t wages_kingdom;
    int32_t health;
    int32_t exit_point_grid_offset;
} PharaohEventPlayerState;

typedef struct PharaohEmpireCityState {
    int32_t map_object;
    uint8_t type;
    uint8_t is_open;
    uint8_t *sells;
    uint8_t *buys;
} PharaohEmpireCityState;

typedef struct PharaohEmpireObjectState {
    uint8_t city_type;
    uint16_t image_id;
} PharaohEmpireObjectState;

typedef bool (*PharaohEventDueFunction)(
    void *user_data, const PharaohScenarioEvent *event, bool any);
typedef PharaohScenarioEvent *(*PharaohEventByIdFunction)(
    void *user_data, int32_t event_id);
typedef void (*PharaohEventPhrasesFunction)(
    void *user_data, const PharaohScenarioEvent *event, int16_t phrases[16]);
typedef int32_t (*PharaohEventMessageTextFunction)(
    void *user_data, const PharaohScenarioEvent *event);
typedef void (*PharaohPostMessageFunction)(void *user_data, int32_t type,
    int32_t parameter1, int32_t parameter2, const int16_t *phrases,
    int32_t extra);
typedef bool (*PharaohChainTargetFunction)(void *user_data,
    const PharaohScenarioEvent *event, int32_t target_id);
typedef int32_t (*PharaohReasonPhraseFunction)(void *user_data,
    const PharaohScenarioEvent *event, int32_t action);
typedef void (*PharaohChainEventFunction)(void *user_data,
    PharaohScenarioEvent *event, int32_t target_id, int32_t phrase);
typedef int32_t (*PharaohEventIntegerFunction)(
    void *user_data, const PharaohScenarioEvent *event);
typedef bool (*PharaohTroopsDispatchedFunction)(void *user_data, int32_t event_id);
typedef void (*PharaohEventEffectFunction)(
    void *user_data, enum PharaohEventEffect effect, int32_t value);
typedef void (*PharaohIntegerEffectFunction)(void *user_data, int32_t value);
typedef int32_t (*PharaohDestroyBuildingFunction)(
    void *user_data, int32_t building_type);
typedef int32_t (*PharaohRouteCityFunction)(void *user_data, int32_t route);
typedef bool (*PharaohRouteOpenFunction)(void *user_data, int32_t route);
typedef uint16_t (*PharaohObjectImageFunction)(void *user_data, int32_t object);
typedef bool (*PharaohObjectResourceFunction)(
    void *user_data, int32_t object, int32_t resource);
typedef void (*PharaohResourceUpdateFunction)(void *user_data, int32_t resource);
typedef void (*PharaohExternalEventFunction)(
    void *user_data, PharaohScenarioEvent *event, int32_t value);

typedef struct PharaohEventActionContext {
    PharaohEventPlayerState *player;
    int32_t current_year;
    int32_t current_month;
    int32_t scenario_start_year;
    int32_t current_request;
    int32_t *trade_prices;
    int32_t *trade_route_limits;
    size_t resource_count;
    PharaohEmpireCityState *cities;
    size_t city_count;
    PharaohEmpireObjectState *objects;
    size_t object_count;
    bool message_use_popup;
    bool pottery_tutorial_set;
    int32_t mission_id;
    int32_t pottery_tutorial_mission;
    bool custom_scenario;
    PharaohEventDueFunction is_due;
    PharaohEventByIdFunction event_by_id;
    PharaohEventPhrasesFunction init_phrases;
    PharaohEventPhrasesFunction fill_phrases;
    PharaohEventMessageTextFunction message_text;
    PharaohPostMessageFunction post_message;
    PharaohChainTargetFunction is_chain_target;
    PharaohReasonPhraseFunction reason_phrase;
    PharaohChainEventFunction chain_event;
    PharaohEventIntegerFunction months_left;
    PharaohEventIntegerFunction months_margin;
    PharaohEventIntegerFunction travel_months;
    PharaohTroopsDispatchedFunction troops_dispatched;
    PharaohEventEffectFunction effect;
    PharaohIntegerEffectFunction change_kingdom;
    PharaohIntegerEffectFunction change_health;
    PharaohIntegerEffectFunction set_tutorial_flag;
    PharaohDestroyBuildingFunction destroy_first_building;
    PharaohRouteCityFunction city_for_trade_route;
    PharaohRouteOpenFunction trade_route_open;
    PharaohObjectImageFunction object_city_image;
    PharaohObjectResourceFunction object_sells_resource;
    PharaohObjectResourceFunction object_buys_resource;
    PharaohResourceUpdateFunction trade_status_update;
    PharaohExternalEventFunction process_flood;
    PharaohExternalEventFunction process_gift;
    void *user_data;
    int32_t temple_wage_bonus;
} PharaohEventActionContext;

void pharaoh_event_process(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_reputation_increase(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_reputation_decrease(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_troops_request(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_request(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_demand_increase(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_demand_decrease(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_price_increase(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_price_decrease(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_sea_trade_problem(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_land_trade_problem(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_wage_increase(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_wage_decrease(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_contaminated_water(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_gold_mine_collapse(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_clay_pit_flood(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_locusts(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_frogs(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_hailstorm(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_blood_river(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_crime_wave(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_city_status_change(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_process_message(PharaohEventActionContext *context,
    PharaohScenarioEvent *event);
void pharaoh_event_invasion_called_off(PharaohScenarioEvent *event);

#endif

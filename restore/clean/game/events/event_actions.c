#include "pharaoh/events/event_actions.h"

enum {
    FLAG_CHAIN_ONLY = 1, FLAG_FIRED = 4, FLAG_BY_FAVOUR = 0x10,
    RESOURCE_POTTERY = 13, RESOURCE_DEBEN = 36, RESOURCE_TROOPS = 37,
    BUILDING_CLAY_PIT = 109, BUILDING_GOLD_MINE = 161,
    TRADE_PROBLEM_MONTHS = 48, TRADE_LOW = 15, TRADE_MEDIUM = 25,
    TRADE_HIGH = 40, WAGES_MAX = 45, WAGES_MIN = 5,
    PRICE_FLOOR_BUY = 2, REMINDER_MONTHS = 6, OVERDUE_MONTHS = 24,
    IGNORED_PENALTY = 3, REQUEST_BLOCK = 18, RETURN_EARLY_MONTHS = 2,
    DEFAULT_TRAVEL_MONTHS = 12, FLOOD_BETTER = 100,
    TUTORIAL_POTTERY_REQUEST = 8,
    P_NONE = 0, P_FIRST = 1, P_SECOND = 2, P_MONTHS = 6, P_TEXT = 8,
    MSG_REQUEST = 130, MSG_GENERAL = 131, MSG_CITY_SAVED = 262,
    MSG_BATTLE_WON = 263, MSG_LAND_PROBLEM = 264, MSG_SEA_PROBLEM = 265,
    MSG_CITY_UNDER_SIEGE = 266, MSG_WAGES_UP = 267, MSG_WAGES_DOWN = 268,
    MSG_BAD_WATER = 269, MSG_GOLD_COLLAPSE = 270, MSG_CLAY_FLOOD = 271,
    MSG_BATTLE_REQUEST = 272, MSG_TRADE_UP = 273, MSG_TRADE_DOWN = 274,
    MSG_FESTIVAL_REQUEST = 275, MSG_CONSTRUCTION_REQUEST = 276,
    MSG_PRICE_UP = 277, MSG_PRICE_DOWN = 278, MSG_TROOPS_LATE = 287,
    MSG_TROOPS_RETURN = 288, MSG_COMPLIANCE = 309, MSG_STORAGE_READY = 314,
    MSG_FAMINE_REQUEST = 330, MSG_CITY_FELL = 338, MSG_CITY_CONQUERED = 339,
    MSG_LOCUSTS = 483, MSG_FROGS = 484, MSG_HAILSTORM = 485,
    MSG_BLOOD_RIVER = 486, MSG_CRIME_WAVE = 491, MSG_MUMMY = 496,
    STATUS_FELL = 0, STATUS_CONQUERED = 1, STATUS_NEW_ROUTE = 2,
    STATUS_LOST_ROUTE = 3, STATUS_UNDER_SIEGE = 4,
    MESSAGE_CITY_SAVED = 0, MESSAGE_BATTLE_WON = 1, MESSAGE_SUBTYPES = 4,
    CITY_OURS = 0, CITY_PHARAOH_TRADING = 1, CITY_PHARAOH = 2,
    CITY_EGYPTIAN_TRADING = 3, CITY_EGYPTIAN = 4,
    CITY_FOREIGN_TRADING = 5, CITY_FOREIGN = 6
};

static bool due(PharaohEventActionContext *c, const PharaohScenarioEvent *e)
{
    return c->is_due(c->user_data, e, false);
}

static void popup(PharaohEventActionContext *c, int32_t type, int32_t p1,
    int32_t p2, const int16_t *phrases)
{
    c->message_use_popup = true;
    c->post_message(c->user_data, type, p1, p2, phrases, 0);
}

static void chain(PharaohEventActionContext *c, PharaohScenarioEvent *e,
    int32_t target, int32_t action)
{
    if (c->is_chain_target(c->user_data, e, target)) {
        c->chain_event(c->user_data, e, target,
            c->reason_phrase(c->user_data, e, action));
    }
}

static void chain_done(PharaohEventActionContext *c, PharaohScenarioEvent *e)
{
    chain(c, e, e->on_completed, 0);
}

static int32_t request_message(const PharaohScenarioEvent *e)
{
    if (e->subtype == PHARAOH_REQUEST_FESTIVAL) return MSG_FESTIVAL_REQUEST;
    if (e->subtype == PHARAOH_REQUEST_CONSTRUCTION) return MSG_CONSTRUCTION_REQUEST;
    if (e->subtype == PHARAOH_REQUEST_FAMINE) return MSG_FAMINE_REQUEST;
    return MSG_REQUEST;
}

static int32_t request_phrase(const PharaohScenarioEvent *e, int32_t base,
    int16_t *phrase)
{
    static const int8_t blocks[PHARAOH_REQUEST_SUBTYPE_COUNT] =
        { 0, -1, -1, 1, 2, 3, 4 };
    if (e->subtype >= PHARAOH_REQUEST_SUBTYPE_COUNT || blocks[e->subtype] < 0)
        return 0;
    *phrase = (int16_t)(base - (e->sender_faction != 0u) +
        REQUEST_BLOCK * blocks[e->subtype]);
    return 1;
}

static bool starts_now(const PharaohEventActionContext *c,
    const PharaohScenarioEvent *e)
{
    return (e->flags & (FLAG_CHAIN_ONLY | FLAG_BY_FAVOUR)) == 0u &&
        c->current_year == e->year_value + c->scenario_start_year &&
        c->current_month == e->month;
}

static void check_compliance(PharaohEventActionContext *c,
    PharaohScenarioEvent *e, int16_t p[16])
{
    bool trouble;
    if (e->can_comply_shown != 0u) return;
    trouble = e->subtype == PHARAOH_REQUEST_CITY_IN_TROUBLE;
    if (e->item_value == RESOURCE_DEBEN) {
        if (c->player->treasury < e->amount_value) return;
        e->can_comply_shown = 1;
        c->message_use_popup = true;
        p[P_MONTHS] = e->months_left;
        p[P_NONE] = p[P_SECOND] = p[P_FIRST] = -1;
        c->post_message(c->user_data, MSG_COMPLIANCE, e->id, 0, p, trouble);
    } else {
        int32_t needed = e->amount_value * 100;
        if (c->player->storage_stored[e->item_value] < needed) return;
        e->can_comply_shown = 1;
        c->message_use_popup = true;
        p[P_MONTHS] = e->months_left;
        c->current_request = e->id;
        p[P_NONE] = p[P_SECOND] = p[P_FIRST] = -1;
        c->post_message(c->user_data, MSG_STORAGE_READY, e->id, 0, p, trouble);
    }
}

static void request_active(PharaohEventActionContext *c,
    PharaohScenarioEvent *e, int16_t p[16], int32_t type)
{
    int16_t months = --e->months_left;
    if (e->state == PHARAOH_REQUEST_NORMAL) {
        if (months == REMINDER_MONTHS) {
            c->message_use_popup = true;
            p[P_MONTHS] = months;
            (void)request_phrase(e, 41, &p[P_FIRST]);
            c->post_message(c->user_data, type, e->id, 0, p, 0);
        } else if (months <= 0) {
            c->message_use_popup = true;
            (void)request_phrase(e, 43, &p[P_FIRST]);
            e->months_left = OVERDUE_MONTHS;
            p[P_MONTHS] = OVERDUE_MONTHS;
            e->state = PHARAOH_REQUEST_OVERDUE;
            c->post_message(c->user_data, MSG_REQUEST, e->id, 0, p, 0);
            c->change_kingdom(c->user_data, -IGNORED_PENALTY);
            c->player->kingdom_ignored_request_penalty = IGNORED_PENALTY;
        }
    } else if (e->state == PHARAOH_REQUEST_OVERDUE && months <= 0) {
        chain(c, e, e->on_refusal, 1);
        e->state = PHARAOH_REQUEST_IGNORED;
        e->is_active = 0;
    }
    check_compliance(c, e, p);
}

static void request_dispatched(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    if (--e->months_left > 0) return;
    c->message_use_popup = true;
    if (e->state == PHARAOH_REQUEST_DISPATCHED) chain(c, e, e->on_completed, 0);
    else chain(c, e, e->on_too_late, 2);
    e->state = PHARAOH_REQUEST_FINISHED;
    e->is_active = 0;
    e->flags |= FLAG_FIRED;
}

static void request_start(PharaohEventActionContext *c, PharaohScenarioEvent *e,
    int16_t p[16], int32_t type)
{
    e->is_active = 1;
    if ((e->item_value == RESOURCE_DEBEN && c->player->treasury >= e->amount_value) ||
        (e->item_value != RESOURCE_DEBEN &&
         c->player->storage_stored[e->item_value] >= e->amount_value * 100))
        e->can_comply_shown = 1;
    e->months_left = e->months_initial;
    c->message_use_popup = true;
    (void)request_phrase(e, 39, &p[P_FIRST]);
    c->post_message(c->user_data, type, e->id, 0, p, 0);
    if (!c->pottery_tutorial_set && c->mission_id == c->pottery_tutorial_mission &&
        e->type == PHARAOH_EVENT_REQUEST && e->item_value == RESOURCE_POTTERY &&
        !c->custom_scenario) {
        c->pottery_tutorial_set = true;
        c->set_tutorial_flag(c->user_data, TUTORIAL_POTTERY_REQUEST);
    }
}

static void troops_returning(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    int32_t type;
    if (--e->months_left >= 1) return;
    type = e->state == PHARAOH_REQUEST_TROOPS_RETURN ?
        MSG_TROOPS_RETURN : MSG_TROOPS_LATE;
    popup(c, type, 0, c->player->exit_point_grid_offset, NULL);
    c->effect(c->user_data, PHARAOH_EFFECT_LEGIONS_RETURN, e->id);
    e->state = PHARAOH_REQUEST_FINISHED;
    e->is_active = 0;
}

static void troops_months_up(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    int16_t left;
    c->message_use_popup = true;
    if (e->state == PHARAOH_REQUEST_NORMAL) {
        chain(c, e, e->on_refusal, 1);
        e->state = PHARAOH_REQUEST_FINISHED; e->is_active = 0; return;
    }
    if (e->state != PHARAOH_REQUEST_RECEIVED) {
        e->state = PHARAOH_REQUEST_FINISHED; e->is_active = 0; return;
    }
    if (e->months_left > RETURN_EARLY_MONTHS) {
        chain(c, e, e->on_too_late, 2);
        e->state = PHARAOH_REQUEST_TROOPS_RETURN_TOO_LATE;
        left = (int16_t)(c->travel_months(c->user_data, e) - e->months_left);
    } else if (c->troops_dispatched(c->user_data, e->id)) {
        chain(c, e, e->on_completed, 0);
        e->state = PHARAOH_REQUEST_TROOPS_RETURN;
        left = (int16_t)c->travel_months(c->user_data, e);
    } else {
        chain(c, e, e->on_defeat, 3);
        e->state = PHARAOH_REQUEST_FINISHED; e->is_active = 0; return;
    }
    e->months_left = left < 1 ? 1 : left;
}

static void troops_waiting(PharaohEventActionContext *c,
    PharaohScenarioEvent *e, int16_t p[16], int32_t type, int32_t months)
{
    int16_t block = e->subtype != PHARAOH_REQUEST_TROOPS ? REQUEST_BLOCK : 0;
    int32_t pharaoh = e->sender_faction != 0u;
    int32_t travel, ahead;
    p[P_MONTHS] = (int16_t)c->months_margin(c->user_data, e);
    if (e->is_active == 0u) {
        if (!starts_now(c, e)) return;
        e->is_active = 1; e->months_left = e->months_initial;
        c->message_use_popup = true;
        p[P_FIRST] = (int16_t)(block + (pharaoh ? 2 : 3));
        c->post_message(c->user_data, type, e->id, 0, p, 0);
        return;
    }
    travel = c->travel_months(c->user_data, e);
    if (travel <= 0) travel = DEFAULT_TRAVEL_MONTHS;
    ahead = months - travel;
    if (ahead == e->months_margin) p[P_FIRST] = (int16_t)(block + (pharaoh ? 4 : 5));
    else if (ahead == e->coin) p[P_FIRST] = (int16_t)(block + (pharaoh ? 6 : 7));
    else return;
    c->post_message(c->user_data, MSG_REQUEST, e->id, 0, p, 0);
}

static bool plague_fires(PharaohEventActionContext *c, PharaohScenarioEvent *e)
{
    if (!due(c, e)) return false;
    e->flags |= FLAG_FIRED;
    return true;
}

static void plague_effect(PharaohEventActionContext *c, PharaohScenarioEvent *e,
    enum PharaohEventEffect effect, int32_t message, int32_t value)
{
    if (plague_fires(c, e)) {
        c->effect(c->user_data, effect, value);
        popup(c, message, 0, 0, NULL);
        chain_done(c, e);
    }
}

void pharaoh_event_process_reputation_increase(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    PharaohScenarioEvent *parent;
    int16_t p[16];
    int32_t parameter;
    if (!due(c, e)) return;
    parent = c->event_by_id(c->user_data, e->parent_id);
    parameter = parent != NULL ? parent->id : e->id;
    c->init_phrases(c->user_data, e, p);
    popup(c, MSG_GENERAL, parameter, 0, p);
    c->change_kingdom(c->user_data, e->amount_value);
    e->flags |= FLAG_FIRED; e->is_active = 0;
    chain_done(c, e);
}

void pharaoh_event_process_reputation_decrease(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    int16_t p[16];
    if (!due(c, e)) return;
    c->init_phrases(c->user_data, e, p);
    popup(c, MSG_GENERAL, e->id, 0, p);
    c->change_kingdom(c->user_data, -e->amount_value);
    c->player->kingdom_ignored_request_penalty = e->amount_value;
    e->flags |= FLAG_FIRED; e->is_active = 0;
    chain_done(c, e);
}

void pharaoh_event_process_troops_request(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    int16_t p[16];
    int32_t city_asks, type, months;
    if (e->state == PHARAOH_REQUEST_FINISHED) return;
    city_asks = e->subtype == PHARAOH_REQUEST_TROOPS;
    p[P_SECOND] = (int16_t)(e->sender_faction != 0u ?
        (city_asks ? 0 : REQUEST_BLOCK) :
        (city_asks ? 1 : REQUEST_BLOCK + 1));
    p[P_TEXT] = (int16_t)c->message_text(c->user_data, e);
    type = city_asks ? MSG_CITY_UNDER_SIEGE : MSG_BATTLE_REQUEST;
    c->fill_phrases(c->user_data, e, p);
    months = c->months_left(c->user_data, e);
    if (months == 0) { troops_months_up(c, e); return; }
    if (e->state == PHARAOH_REQUEST_NORMAL) troops_waiting(c, e, p, type, months);
    else if (e->state == PHARAOH_REQUEST_RECEIVED) {
        if (--e->months_left < 1) e->months_left = 1;
    } else if (e->state == PHARAOH_REQUEST_TROOPS_RETURN ||
        e->state == PHARAOH_REQUEST_TROOPS_RETURN_TOO_LATE) troops_returning(c, e);
}

void pharaoh_event_process_request(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    int16_t p[16];
    int32_t type;
    if (e->item_value == RESOURCE_TROOPS) {
        pharaoh_event_process_troops_request(c, e); return;
    }
    if (e->state == PHARAOH_REQUEST_FINISHED || e->state == PHARAOH_REQUEST_IGNORED)
        return;
    type = request_message(e);
    (void)request_phrase(e, 37, &p[P_SECOND]);
    p[P_TEXT] = (int16_t)c->message_text(c->user_data, e);
    c->fill_phrases(c->user_data, e, p);
    if (e->state == PHARAOH_REQUEST_DISPATCHED ||
        e->state == PHARAOH_REQUEST_DISPATCHED_LATE) request_dispatched(c, e);
    else if (e->is_active != 0u) request_active(c, e, p, type);
    else if (starts_now(c, e)) request_start(c, e, p, type);
}

static int32_t raised_limit(int32_t limit)
{
    return limit >= TRADE_MEDIUM ? TRADE_HIGH :
        (limit < TRADE_LOW ? TRADE_LOW : TRADE_MEDIUM);
}

static int32_t lowered_limit(int32_t limit)
{
    return limit > TRADE_MEDIUM ? TRADE_MEDIUM :
        (limit <= TRADE_LOW ? 0 : TRADE_LOW);
}

void pharaoh_event_process_demand_increase(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    int32_t *limit;
    int16_t p[16];
    if (!due(c, e)) return;
    e->flags |= FLAG_FIRED;
    limit = &c->trade_route_limits[(size_t)e->route_value * c->resource_count +
        (size_t)e->item_value];
    if (*limit < TRADE_HIGH) {
        *limit = raised_limit(*limit);
        if (c->trade_route_open(c->user_data, e->route_value)) {
            c->init_phrases(c->user_data, e, p);
            popup(c, MSG_TRADE_UP,
                c->city_for_trade_route(c->user_data, e->route_value),
                e->item_value, p);
        }
    }
    chain_done(c, e);
}

void pharaoh_event_process_demand_decrease(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    int32_t *limit;
    int16_t p[16];
    if (!due(c, e)) return;
    e->flags |= FLAG_FIRED;
    limit = &c->trade_route_limits[(size_t)e->route_value * c->resource_count +
        (size_t)e->item_value];
    if (*limit > 0) {
        *limit = lowered_limit(*limit);
        c->trade_status_update(c->user_data, e->item_value);
        if (c->trade_route_open(c->user_data, e->route_value)) {
            c->init_phrases(c->user_data, e, p);
            popup(c, MSG_TRADE_DOWN,
                c->city_for_trade_route(c->user_data, e->route_value),
                e->item_value, p);
        }
    }
    chain_done(c, e);
}

void pharaoh_event_process_price_increase(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    size_t i;
    int16_t p[16];
    if (!due(c, e)) return;
    e->flags |= FLAG_FIRED;
    i = (size_t)e->item_value * 2u;
    c->trade_prices[i] += e->amount_value;
    c->trade_prices[i + 1u] += e->amount_value;
    c->init_phrases(c->user_data, e, p);
    popup(c, MSG_PRICE_UP, e->amount_value, e->item_value, p);
    chain_done(c, e);
}

void pharaoh_event_process_price_decrease(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    size_t i;
    int32_t *buy, *sell;
    int16_t p[16];
    if (!due(c, e)) return;
    e->flags |= FLAG_FIRED;
    i = (size_t)e->item_value * 2u;
    buy = &c->trade_prices[i]; sell = &c->trade_prices[i + 1u];
    if (*sell <= 0) return;
    if (*sell > e->amount_value) { *buy -= e->amount_value; *sell -= e->amount_value; }
    else { *buy = PRICE_FLOOR_BUY; *sell = 0; }
    c->init_phrases(c->user_data, e, p);
    popup(c, MSG_PRICE_DOWN, e->amount_value, e->item_value, p);
    chain_done(c, e);
}

static void trade_problem(PharaohEventActionContext *c, PharaohScenarioEvent *e,
    bool sea)
{
    int16_t p[16];
    int32_t routes;
    if (!due(c, e)) return;
    e->flags |= FLAG_FIRED;
    routes = sea ? c->player->num_sea_routes : c->player->num_land_routes;
    if (routes > 0) {
        if (sea) c->player->sea_trade_problem_duration = TRADE_PROBLEM_MONTHS;
        else c->player->land_trade_problem_duration = TRADE_PROBLEM_MONTHS;
        c->message_use_popup = true;
        c->init_phrases(c->user_data, e, p);
        c->post_message(c->user_data, sea ? MSG_SEA_PROBLEM : MSG_LAND_PROBLEM,
            0, 0, p, 0);
    }
    chain_done(c, e);
}

void pharaoh_event_process_sea_trade_problem(PharaohEventActionContext *c,
    PharaohScenarioEvent *e) { trade_problem(c, e, true); }
void pharaoh_event_process_land_trade_problem(PharaohEventActionContext *c,
    PharaohScenarioEvent *e) { trade_problem(c, e, false); }

static void wage_change(PharaohEventActionContext *c, PharaohScenarioEvent *e,
    bool increase)
{
    int32_t wages;
    int16_t p[16];
    if (!due(c, e)) return;
    e->flags |= FLAG_FIRED;
    wages = c->player->wages_kingdom +
        (increase ? e->amount_value : -e->amount_value);
    if ((increase && c->temple_wage_bonus + wages <= WAGES_MAX) ||
        (!increase && c->temple_wage_bonus + wages >= WAGES_MIN)) {
        c->player->wages_kingdom = wages;
        c->init_phrases(c->user_data, e, p);
        popup(c, increase ? MSG_WAGES_UP : MSG_WAGES_DOWN, 0, 0, p);
    }
    chain_done(c, e);
}

void pharaoh_event_process_wage_increase(PharaohEventActionContext *c,
    PharaohScenarioEvent *e) { wage_change(c, e, true); }
void pharaoh_event_process_wage_decrease(PharaohEventActionContext *c,
    PharaohScenarioEvent *e) { wage_change(c, e, false); }

void pharaoh_event_process_contaminated_water(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    int32_t loss;
    int16_t p[16];
    if (!due(c, e)) return;
    e->flags |= FLAG_FIRED;
    loss = c->player->health > 80 ? -50 : (c->player->health > 60 ? -40 : -25);
    c->change_health(c->user_data, loss);
    c->init_phrases(c->user_data, e, p);
    popup(c, MSG_BAD_WATER, 0, 0, p);
    chain_done(c, e);
}

static void building_disaster(PharaohEventActionContext *c,
    PharaohScenarioEvent *e, int32_t building, int32_t message, bool phrases)
{
    int32_t grid;
    int16_t p[16];
    if (!due(c, e)) return;
    e->flags |= FLAG_FIRED;
    grid = c->destroy_first_building(c->user_data, building);
    if (grid != 0) {
        if (phrases) { c->init_phrases(c->user_data, e, p); popup(c, message, 0, grid, p); }
        else popup(c, message, 0, grid, NULL);
    }
    chain_done(c, e);
}

void pharaoh_event_process_gold_mine_collapse(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{ building_disaster(c, e, BUILDING_GOLD_MINE, MSG_GOLD_COLLAPSE, true); }
void pharaoh_event_process_clay_pit_flood(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{ building_disaster(c, e, BUILDING_CLAY_PIT, MSG_CLAY_FLOOD, false); }

void pharaoh_event_process_locusts(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    if (!due(c, e)) return;
    e->flags |= FLAG_FIRED;
    c->effect(c->user_data, PHARAOH_EFFECT_LOCUSTS, 0);
    popup(c, MSG_LOCUSTS, 0, 0, NULL);
    chain_done(c, e);
}

void pharaoh_event_process_frogs(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{ plague_effect(c, e, PHARAOH_EFFECT_FROGS, MSG_FROGS, e->amount_value); }
void pharaoh_event_process_hailstorm(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{ plague_effect(c, e, PHARAOH_EFFECT_HAILSTORM, MSG_HAILSTORM, 0); }
void pharaoh_event_process_blood_river(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{ plague_effect(c, e, PHARAOH_EFFECT_BLOOD_RIVER, MSG_BLOOD_RIVER, e->amount_value); }
void pharaoh_event_process_crime_wave(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{ plague_effect(c, e, PHARAOH_EFFECT_CRIME_WAVE, MSG_CRIME_WAVE, e->amount_value); }

static int32_t new_city_type(int32_t subtype, int32_t type)
{
    switch (subtype) {
        case STATUS_FELL:
            if (type == CITY_EGYPTIAN_TRADING) return CITY_FOREIGN_TRADING;
            return type == CITY_EGYPTIAN ? CITY_FOREIGN : -1;
        case STATUS_CONQUERED:
            if (type == CITY_FOREIGN_TRADING) return CITY_EGYPTIAN_TRADING;
            return type == CITY_FOREIGN ? CITY_EGYPTIAN : -1;
        case STATUS_NEW_ROUTE:
            if (type == CITY_FOREIGN) return CITY_FOREIGN_TRADING;
            if (type == CITY_EGYPTIAN) return CITY_EGYPTIAN_TRADING;
            return type == CITY_PHARAOH ? CITY_PHARAOH_TRADING : -1;
        case STATUS_LOST_ROUTE:
        case STATUS_UNDER_SIEGE:
            if (type == CITY_FOREIGN_TRADING) return CITY_FOREIGN;
            if (type == CITY_EGYPTIAN_TRADING) return CITY_EGYPTIAN;
            return type == CITY_PHARAOH_TRADING ? CITY_PHARAOH : -1;
        default: return -1;
    }
}

static bool city_trades(int32_t type)
{
    return type == CITY_OURS || type == CITY_PHARAOH_TRADING ||
        type == CITY_EGYPTIAN_TRADING || type == CITY_FOREIGN_TRADING;
}

static void set_city_type(PharaohEventActionContext *c, int32_t city_id,
    int32_t object_id, int32_t type, bool closes_route)
{
    PharaohEmpireCityState *city = &c->cities[city_id];
    PharaohEmpireObjectState *object = &c->objects[object_id];
    size_t resource;
    object->city_type = (uint8_t)type;
    object->image_id = c->object_city_image(c->user_data, object_id);
    city->type = object->city_type;
    if (closes_route) city->is_open = 0;
    for (resource = 0; resource < c->resource_count; ++resource) {
        city->sells[resource] = 0; city->buys[resource] = 0;
        if (city_trades(city->type)) {
            city->sells[resource] = (uint8_t)c->object_sells_resource(
                c->user_data, object_id, (int32_t)resource);
            city->buys[resource] = (uint8_t)c->object_buys_resource(
                c->user_data, object_id, (int32_t)resource);
        }
        c->trade_status_update(c->user_data, (int32_t)resource);
    }
}

static int32_t city_status_message(int32_t subtype)
{
    if (subtype == STATUS_FELL) return MSG_CITY_FELL;
    if (subtype == STATUS_CONQUERED) return MSG_CITY_CONQUERED;
    return MSG_GENERAL;
}

void pharaoh_event_process_city_status_change(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    int32_t city_id, object_id, type;
    int16_t p[16];
    if (!due(c, e)) return;
    e->flags |= FLAG_FIRED;
    city_id = c->city_for_trade_route(c->user_data, e->route_value);
    if (city_id != 0) {
        object_id = c->cities[city_id].map_object;
        type = new_city_type(e->subtype, c->objects[object_id].city_type);
        if (type >= 0) {
            p[P_SECOND] = (int16_t)(126 + 4 * e->subtype);
            p[P_FIRST] = (int16_t)(127 + 4 * e->subtype);
            p[P_TEXT] = (int16_t)c->message_text(c->user_data, e);
            c->fill_phrases(c->user_data, e, p);
            set_city_type(c, city_id, object_id, type,
                e->subtype == STATUS_LOST_ROUTE || e->subtype == STATUS_UNDER_SIEGE);
            popup(c, city_status_message(e->subtype), 0, 0, p);
        }
    }
    chain_done(c, e);
}

static int32_t general_message_type(const PharaohScenarioEvent *e)
{
    if (e->subtype == MESSAGE_CITY_SAVED) return MSG_CITY_SAVED;
    if (e->subtype == MESSAGE_BATTLE_WON) return MSG_BATTLE_WON;
    return MSG_GENERAL;
}

void pharaoh_event_process_message(PharaohEventActionContext *c,
    PharaohScenarioEvent *e)
{
    int16_t p[16];
    if (!due(c, e)) return;
    e->flags |= FLAG_FIRED;
    c->message_use_popup = true;
    if (e->subtype < MESSAGE_SUBTYPES) {
        p[P_SECOND] = (int16_t)(146 + 4 * e->subtype);
        p[P_FIRST] = (int16_t)(147 + 4 * e->subtype);
        p[P_TEXT] = (int16_t)c->message_text(c->user_data, e);
    } else p[P_FIRST] = p[P_SECOND] = p[P_TEXT] = -1;
    c->fill_phrases(c->user_data, e, p);
    c->post_message(c->user_data, general_message_type(e), 0, 0, p, 0);
    chain_done(c, e);
}

void pharaoh_event_invasion_called_off(PharaohScenarioEvent *e)
{
    e->is_active = 0;
    e->flags |= FLAG_FIRED;
}

void pharaoh_event_process(PharaohEventActionContext *c, PharaohScenarioEvent *e)
{
    switch (e->type) {
        case PHARAOH_EVENT_REQUEST: pharaoh_event_process_request(c, e); break;
        case PHARAOH_EVENT_SEA_TRADE_PROBLEM: pharaoh_event_process_sea_trade_problem(c, e); break;
        case PHARAOH_EVENT_LAND_TRADE_PROBLEM: pharaoh_event_process_land_trade_problem(c, e); break;
        case PHARAOH_EVENT_WAGE_INCREASE: pharaoh_event_process_wage_increase(c, e); break;
        case PHARAOH_EVENT_WAGE_DECREASE: pharaoh_event_process_wage_decrease(c, e); break;
        case PHARAOH_EVENT_CONTAMINATED_WATER: pharaoh_event_process_contaminated_water(c, e); break;
        case PHARAOH_EVENT_GOLD_MINE_COLLAPSE: pharaoh_event_process_gold_mine_collapse(c, e); break;
        case PHARAOH_EVENT_CLAY_PIT_FLOOD: pharaoh_event_process_clay_pit_flood(c, e); break;
        case PHARAOH_EVENT_DEMAND_INCREASE: pharaoh_event_process_demand_increase(c, e); break;
        case PHARAOH_EVENT_DEMAND_DECREASE: pharaoh_event_process_demand_decrease(c, e); break;
        case PHARAOH_EVENT_PRICE_INCREASE: pharaoh_event_process_price_increase(c, e); break;
        case PHARAOH_EVENT_PRICE_DECREASE: pharaoh_event_process_price_decrease(c, e); break;
        case PHARAOH_EVENT_REPUTATION_INCREASE: pharaoh_event_process_reputation_increase(c, e); break;
        case PHARAOH_EVENT_REPUTATION_DECREASE: pharaoh_event_process_reputation_decrease(c, e); break;
        case PHARAOH_EVENT_CITY_STATUS_CHANGE: pharaoh_event_process_city_status_change(c, e); break;
        case PHARAOH_EVENT_MESSAGE: pharaoh_event_process_message(c, e); break;
        case PHARAOH_EVENT_FLOOD_WORSE: c->process_flood(c->user_data, e, 0); break;
        case PHARAOH_EVENT_FLOOD_BETTER: c->process_flood(c->user_data, e, FLOOD_BETTER); break;
        case PHARAOH_EVENT_GIFT_FROM_PHARAOH: c->process_gift(c->user_data, e, 0); break;
        case PHARAOH_EVENT_LOCUSTS: pharaoh_event_process_locusts(c, e); break;
        case PHARAOH_EVENT_FROGS: pharaoh_event_process_frogs(c, e); break;
        case PHARAOH_EVENT_HAILSTORM: pharaoh_event_process_hailstorm(c, e); break;
        case PHARAOH_EVENT_BLOOD_RIVER: pharaoh_event_process_blood_river(c, e); break;
        case PHARAOH_EVENT_CRIME_WAVE: pharaoh_event_process_crime_wave(c, e); break;
        case PHARAOH_EVENT_MUMMY:
            plague_effect(c, e, PHARAOH_EFFECT_MUMMIES, MSG_MUMMY, e->amount_value);
            break;
        default: break;
    }
}

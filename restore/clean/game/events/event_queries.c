#include "pharaoh/events/event_queries.h"

enum {
    EVENT_FLAG_BY_FAVOUR = 0x10,
    EVENT_INVADER_ENEMY = 1,
    EVENT_INVADER_EGYPT = 2,
    EVENT_INVADER_PHARAOH = 3,
    EVENT_INVADER_BEDOUINS = 4,
    EVENT_CITY_STATUS_SUBTYPE_COUNT = 5,
    EVENT_MESSAGE_SUBTYPE_COUNT = 4,
    EVENT_REQUEST_FESTIVAL_SUBTYPE = 3,
    EVENT_INVASION_PHRASE_FIRST_ROUTE = 9,
    LANG_GROUP_CITY_STATUS_EVENTS = 35,
    LANG_GROUP_REQUEST_KINDS = 149,
    LANG_GROUP_MESSAGE_EVENTS = 150
};

static int32_t subtype_text(const PharaohScenarioEvent *event,
    uint8_t subtype_count, int32_t first, int32_t step)
{
    if (event->subtype >= subtype_count) {
        return -1;
    }
    return first + step * event->subtype;
}

static size_t sender_index(const PharaohScenarioEvent *event)
{
    return event->sender_faction != 0u ? 1u : 0u;
}

int32_t pharaoh_event_announcement_text(const PharaohScenarioEvent *event)
{
    static const int32_t request_text[PHARAOH_REQUEST_SUBTYPE_COUNT] = {
        0x35, 0x11, 0x23, 0x47, 0x59, 0x6b, 0x7d
    };
    static const int32_t invasion_text[4] = { 0xbe, 0xb4, 0xaa, 0x107 };

    switch (event->type) {
    case PHARAOH_EVENT_REQUEST:
        if (event->subtype < PHARAOH_REQUEST_SUBTYPE_COUNT)
            return request_text[event->subtype] -
                (event->sender_faction != 0u ? 1 : 0);
        break;
    case PHARAOH_EVENT_INVASION:
        if (event->item.value >= 1 && event->item.value <= 4)
            return invasion_text[event->item.value - 1];
        break;
    case PHARAOH_EVENT_EARTHQUAKE: return 0xda;
    case PHARAOH_EVENT_SEA_TRADE_PROBLEM: return 0xde;
    case PHARAOH_EVENT_LAND_TRADE_PROBLEM: return 0xe2;
    case PHARAOH_EVENT_WAGE_INCREASE: return 0xe6;
    case PHARAOH_EVENT_WAGE_DECREASE: return 0xea;
    case PHARAOH_EVENT_CONTAMINATED_WATER: return 0xee;
    case PHARAOH_EVENT_GOLD_MINE_COLLAPSE: return 0xf2;
    case PHARAOH_EVENT_DEMAND_INCREASE: return 0xd2;
    case PHARAOH_EVENT_DEMAND_DECREASE: return 0xd6;
    case PHARAOH_EVENT_PRICE_INCREASE: return 0xca;
    case PHARAOH_EVENT_PRICE_DECREASE: return 0xce;
    case PHARAOH_EVENT_REPUTATION_INCREASE: return 0xc2;
    case PHARAOH_EVENT_REPUTATION_DECREASE: return 0xc6;
    case PHARAOH_EVENT_CITY_STATUS_CHANGE:
        if (event->subtype < EVENT_CITY_STATUS_SUBTYPE_COUNT)
            return 0x81 + 4 * event->subtype;
        break;
    case PHARAOH_EVENT_MESSAGE:
        if (event->subtype < EVENT_MESSAGE_SUBTYPE_COUNT)
            return 0x95 + 4 * event->subtype;
        break;
    case PHARAOH_EVENT_FLOOD_WORSE: return 0xfa;
    case PHARAOH_EVENT_FLOOD_BETTER: return 0xfe;
    case PHARAOH_EVENT_GIFT_FROM_PHARAOH:
        return 0x125 - (event->sender_faction != 0u ? 1 : 0);
    default:
        break;
    }
    return -1;
}

static const PharaohRequestMessageRule *request_rule(
    const PharaohEventQueryContext *context, const PharaohScenarioEvent *event)
{
    if (context->request_rules == NULL ||
        event->subtype >= PHARAOH_REQUEST_SUBTYPE_COUNT) {
        return NULL;
    }
    return &context->request_rules[event->subtype];
}

static const PharaohRequestMessageRule *followup_request_rule(
    const PharaohEventQueryContext *context, const PharaohScenarioEvent *event)
{
    if (event->type != PHARAOH_EVENT_REQUEST) {
        return NULL;
    }
    return request_rule(context, event);
}

static const PharaohEventTypeText *type_text(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event)
{
    if (context->type_texts == NULL || event->type >= PHARAOH_EVENT_TYPE_COUNT) {
        return NULL;
    }
    return &context->type_texts[event->type];
}

static int32_t invader_text(const PharaohScenarioEvent *event, int32_t enemy,
    int32_t egypt, int32_t pharaoh, int32_t bedouins)
{
    switch ((uint16_t)event->item_value) {
        case EVENT_INVADER_ENEMY:
            return enemy;
        case EVENT_INVADER_EGYPT:
            return egypt;
        case EVENT_INVADER_PHARAOH:
            return pharaoh;
        case EVENT_INVADER_BEDOUINS:
            return bedouins;
        default:
            return -1;
    }
}

static const PharaohScenarioEvent *event_by_id(
    const PharaohEventQueryContext *context, int32_t id)
{
    if (id < 0 || (size_t)id >= context->event_count || context->events == NULL) {
        return NULL;
    }
    return &context->events[id];
}

bool pharaoh_event_parent_by_favour(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event)
{
    const PharaohScenarioEvent *parent = event_by_id(context, event->parent_id);
    return parent != NULL && (parent->flags & EVENT_FLAG_BY_FAVOUR) != 0u;
}

int32_t pharaoh_event_reason_group(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event, int32_t action)
{
    switch (action) {
        case 0:
            return pharaoh_event_reason_group_completed(context, event);
        case 1:
            return pharaoh_event_reason_group_refused(context, event);
        case 2:
            return pharaoh_event_reason_group_too_late(context, event);
        case 3:
            return pharaoh_event_reason_group_defeat(context, event);
        default:
            return -1;
    }
}

int32_t pharaoh_event_reason_group_completed(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event)
{
    const PharaohRequestMessageRule *rule = request_rule(context, event);
    const PharaohEventTypeText *text = type_text(context, event);
    switch (event->type) {
        case PHARAOH_EVENT_REQUEST:
            return rule != NULL ? rule->completed_group : -1;
        case PHARAOH_EVENT_INVASION:
            return invader_text(event, 32, 33, 34, 50);
        case PHARAOH_EVENT_CITY_STATUS_CHANGE:
            return subtype_text(event, EVENT_CITY_STATUS_SUBTYPE_COUNT, 23, 1);
        case PHARAOH_EVENT_MESSAGE:
            return subtype_text(event, EVENT_MESSAGE_SUBTYPE_COUNT, 28, 1);
        default:
            return text != NULL ? text->reason_group : -1;
    }
}

int32_t pharaoh_event_reason_group_refused(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event)
{
    const PharaohRequestMessageRule *rule = followup_request_rule(context, event);
    if (rule != NULL) {
        return rule->refused_group;
    }
    return event->type == PHARAOH_EVENT_GIFT_FROM_PHARAOH ? 51 : -1;
}

int32_t pharaoh_event_reason_group_too_late(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event)
{
    const PharaohRequestMessageRule *rule = followup_request_rule(context, event);
    if (rule != NULL) {
        return rule->too_late_group;
    }
    return event->type == PHARAOH_EVENT_GIFT_FROM_PHARAOH ? 52 : -1;
}

int32_t pharaoh_event_reason_group_defeat(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event)
{
    const PharaohRequestMessageRule *rule = followup_request_rule(context, event);
    return rule != NULL ? rule->defeat_group : -1;
}

int32_t pharaoh_event_reason_column(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event)
{
    const PharaohRequestMessageRule *rule = request_rule(context, event);
    const PharaohEventTypeText *text = type_text(context, event);
    switch (event->type) {
        case PHARAOH_EVENT_REQUEST:
            return rule != NULL ? rule->reason_column : -1;
        case PHARAOH_EVENT_INVASION:
            return invader_text(event, 16, 17, 18, 34);
        case PHARAOH_EVENT_CITY_STATUS_CHANGE:
            return subtype_text(event, EVENT_CITY_STATUS_SUBTYPE_COUNT, 7, 1);
        case PHARAOH_EVENT_MESSAGE:
            return subtype_text(event, EVENT_MESSAGE_SUBTYPE_COUNT, 12, 1);
        default:
            return text != NULL ? text->reason_column : -1;
    }
}

int32_t pharaoh_event_message_text_completed(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event)
{
    const PharaohRequestMessageRule *rule = request_rule(context, event);
    const PharaohEventTypeText *text = type_text(context, event);
    switch (event->type) {
        case PHARAOH_EVENT_REQUEST:
            return rule != NULL ? rule->completed[sender_index(event)] : -1;
        case PHARAOH_EVENT_INVASION:
            return invader_text(event, 189, 179, 169, 262);
        case PHARAOH_EVENT_CITY_STATUS_CHANGE:
            return subtype_text(event, EVENT_CITY_STATUS_SUBTYPE_COUNT, 128, 4);
        case PHARAOH_EVENT_MESSAGE:
            return subtype_text(event, EVENT_MESSAGE_SUBTYPE_COUNT, 148, 4);
        case PHARAOH_EVENT_GIFT_FROM_PHARAOH:
            return 287 - (int32_t)sender_index(event);
        default:
            return text != NULL ? text->completed : -1;
    }
}

int32_t pharaoh_event_message_text_refused(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event)
{
    const PharaohRequestMessageRule *rule = followup_request_rule(context, event);
    if (rule != NULL) {
        return rule->refused[sender_index(event)];
    }
    return event->type == PHARAOH_EVENT_GIFT_FROM_PHARAOH
        ? 291 - (int32_t)sender_index(event) : -1;
}

int32_t pharaoh_event_message_text_too_late(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event)
{
    const PharaohRequestMessageRule *rule = followup_request_rule(context, event);
    if (rule != NULL) {
        return rule->too_late[sender_index(event)];
    }
    return event->type == PHARAOH_EVENT_GIFT_FROM_PHARAOH
        ? 289 - (int32_t)sender_index(event) : -1;
}

int32_t pharaoh_event_message_text_defeat(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event)
{
    const PharaohRequestMessageRule *rule = followup_request_rule(context, event);
    return rule != NULL ? rule->defeat[sender_index(event)] : -1;
}

int32_t pharaoh_event_reason_phrase(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event, int32_t action)
{
    int32_t id;
    int32_t variant;
    bool use_action_text = false;
    const PharaohScenarioEvent *next;

    switch (action) {
        case 0: id = event->on_completed; variant = event->reason[0]; break;
        case 1: id = event->on_refusal; variant = event->reason[1]; break;
        case 2: id = event->on_too_late; variant = event->reason[2]; break;
        case 3: id = event->on_defeat; variant = event->reason[3]; break;
        default: return -1;
    }
    next = event_by_id(context, id);
    if (variant >= 6) {
        int32_t group;
        int32_t column;
        if (next == NULL) {
            return -1;
        }
        group = pharaoh_event_reason_group(context, event, action);
        column = pharaoh_event_reason_column(context, next);
        if (group < 0 || group >= PHARAOH_EVENT_REASON_GROUP_COUNT ||
            column < 0 || column >= PHARAOH_EVENT_REASON_COLUMN_COUNT ||
            context->reason_phrases == NULL) {
            variant = 0;
            use_action_text = true;
        } else {
            variant = context->reason_phrases[
                group * PHARAOH_EVENT_REASON_COLUMN_COUNT + column];
        }
    }
    if (!use_action_text) {
        if (variant >= 3 && variant <= 5 && next != NULL &&
            context->event_message_text != NULL && context->phrase_variant != NULL) {
            return context->phrase_variant(context->user_data,
                context->event_message_text(context->user_data, next), variant);
        }
        if (variant >= 6) {
            return -1;
        }
    }
    if (context->phrase_variant == NULL) {
        return -1;
    }
    switch (action) {
        case 0:
            return context->phrase_variant(context->user_data,
                pharaoh_event_message_text_completed(context, event), variant);
        case 1:
            return context->phrase_variant(context->user_data,
                pharaoh_event_message_text_refused(context, event), variant);
        case 2:
            return context->phrase_variant(context->user_data,
                pharaoh_event_message_text_too_late(context, event), variant);
        default:
            return context->phrase_variant(context->user_data,
                pharaoh_event_message_text_defeat(context, event), variant);
    }
}

static bool is_festival_supply(int32_t resource)
{
    if (resource >= PHARAOH_RESOURCE_MIN_FOOD &&
        resource < PHARAOH_RESOURCE_MAX_FOOD) {
        return true;
    }
    return resource == PHARAOH_RESOURCE_BEER || resource == PHARAOH_RESOURCE_LINEN ||
        resource == PHARAOH_RESOURCE_LUXURY_GOODS ||
        resource == PHARAOH_RESOURCE_DEBEN;
}

static bool is_building_material(int32_t resource)
{
    return resource == PHARAOH_RESOURCE_BRICKS ||
        resource == PHARAOH_RESOURCE_TIMBER || resource == PHARAOH_RESOURCE_STONE ||
        resource == PHARAOH_RESOURCE_LIMESTONE ||
        resource == PHARAOH_RESOURCE_GRANITE ||
        resource == PHARAOH_RESOURCE_SANDSTONE || resource == PHARAOH_RESOURCE_DEBEN;
}

bool pharaoh_event_request_resource_allowed(
    const PharaohScenarioEvent *event, int32_t resource)
{
    switch (event->subtype) {
        case 0:
        case 6:
            return resource != PHARAOH_RESOURCE_TROOPS &&
                resource != PHARAOH_RESOURCE_GOLD && resource != PHARAOH_RESOURCE_UNUSED;
        case 1:
        case 2:
            return resource == PHARAOH_RESOURCE_TROOPS;
        case 3:
            return is_festival_supply(resource);
        case 4:
            return is_building_material(resource);
        case 5:
            return resource >= PHARAOH_RESOURCE_MIN_FOOD &&
                resource < PHARAOH_RESOURCE_MAX_FOOD;
        default:
            return false;
    }
}

bool pharaoh_event_resource_in_range(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event, int32_t resource)
{
    if (context->resource_min == NULL || context->resource_max == NULL ||
        resource < context->resource_min(context->user_data, event) ||
        resource >= context->resource_max(context->user_data, event)) {
        return false;
    }
    switch (event->type) {
        case PHARAOH_EVENT_REQUEST:
            return pharaoh_event_request_resource_allowed(event, resource);
        case PHARAOH_EVENT_GIFT_FROM_PHARAOH:
            return context->gift_resource_allowed != NULL &&
                context->gift_resource_allowed(context->user_data, resource);
        case PHARAOH_EVENT_DEMAND_INCREASE:
        case PHARAOH_EVENT_DEMAND_DECREASE:
        case PHARAOH_EVENT_PRICE_INCREASE:
        case PHARAOH_EVENT_PRICE_DECREASE:
            return resource != PHARAOH_RESOURCE_UNUSED &&
                resource != PHARAOH_RESOURCE_GOLD;
        default:
            return true;
    }
}

int32_t pharaoh_event_resource_group(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event)
{
    if (context->type_properties == NULL || event->type >= PHARAOH_EVENT_TYPE_COUNT) {
        return -1;
    }
    return context->type_properties[event->type].resource_text_group;
}

bool pharaoh_event_has_months(const PharaohScenarioEvent *event)
{
    return event->type == PHARAOH_EVENT_REQUEST || event->type == PHARAOH_EVENT_INVASION;
}

bool pharaoh_event_has_followup(const PharaohScenarioEvent *event)
{
    return event->type == PHARAOH_EVENT_REQUEST && event->subtype != 0u &&
        event->subtype <= 2u;
}

bool pharaoh_event_is_invasion_kind(const PharaohScenarioEvent *event)
{
    return event->type == PHARAOH_EVENT_INVASION;
}

bool pharaoh_editor_event_has_subtype(const PharaohScenarioEvent *event)
{
    return event->type == PHARAOH_EVENT_REQUEST ||
        event->type == PHARAOH_EVENT_CITY_STATUS_CHANGE ||
        event->type == PHARAOH_EVENT_MESSAGE;
}

bool pharaoh_editor_event_has_phrase_number(const PharaohScenarioEvent *event)
{
    int16_t route;
    if (event->type != PHARAOH_EVENT_INVASION) {
        return false;
    }
    if (event->route.fixed < 0) {
        route = event->route.max;
    } else if (event->route.min < 0) {
        route = event->route.fixed;
    } else {
        return false;
    }
    return route >= EVENT_INVASION_PHRASE_FIRST_ROUTE;
}

bool pharaoh_editor_event_has_phrase_list(const PharaohScenarioEvent *event)
{
    return event->type == PHARAOH_EVENT_REQUEST &&
        event->subtype == EVENT_REQUEST_FESTIVAL_SUBTYPE;
}

int32_t pharaoh_editor_event_subtype_count(const PharaohScenarioEvent *event)
{
    switch (event->type) {
        case PHARAOH_EVENT_REQUEST:
            return PHARAOH_REQUEST_SUBTYPE_COUNT;
        case PHARAOH_EVENT_CITY_STATUS_CHANGE:
            return EVENT_CITY_STATUS_SUBTYPE_COUNT;
        case PHARAOH_EVENT_MESSAGE:
            return EVENT_MESSAGE_SUBTYPE_COUNT;
        default:
            return 0;
    }
}

void pharaoh_editor_event_set_type(PharaohScenarioEvent *event, int32_t type)
{
    const uint8_t next = (uint8_t)type;
    if (event->type == next) {
        return;
    }
    event->type = next;
    event->item.fixed = 1;
    event->item.min = -1;
    event->item.max = -1;
    if ((event->flags & EVENT_FLAG_BY_FAVOUR) != 0u &&
        next != PHARAOH_EVENT_INVASION) {
        event->flags = 0;
    }
}

bool pharaoh_event_type_chains(const PharaohScenarioEvent *event)
{
    return event->type == PHARAOH_EVENT_REQUEST ||
        event->type == PHARAOH_EVENT_GIFT_FROM_PHARAOH;
}

int32_t pharaoh_event_subtype_group(const PharaohScenarioEvent *event)
{
    switch (event->type) {
        case PHARAOH_EVENT_REQUEST:
            return LANG_GROUP_REQUEST_KINDS;
        case PHARAOH_EVENT_CITY_STATUS_CHANGE:
            return LANG_GROUP_CITY_STATUS_EVENTS;
        case PHARAOH_EVENT_MESSAGE:
            return LANG_GROUP_MESSAGE_EVENTS;
        default:
            return -1;
    }
}

static const uint8_t event_type_uses_resource[PHARAOH_EVENT_TYPE_COUNT] = {
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0
};
bool pharaoh_event_type_uses_resource(const PharaohScenarioEvent *event)
{
    return event_type_uses_resource[event->type] != 0u;
}

static const uint8_t event_type_uses_city[PHARAOH_EVENT_TYPE_COUNT] = {
    0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1,
    0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0
};

static const uint8_t event_type_is_invasion[PHARAOH_EVENT_TYPE_COUNT] = {
    0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

bool pharaoh_event_type_uses_city(const PharaohScenarioEvent *event)
{
    return event_type_uses_city[event->type] != 0u;
}

bool pharaoh_event_type_is_invasion(const PharaohScenarioEvent *event)
{
    return event_type_is_invasion[event->type] != 0u;
}

bool pharaoh_event_uses_trade_city(const PharaohScenarioEvent *event)
{
    return pharaoh_event_type_uses_city(event) &&
        !pharaoh_event_type_is_invasion(event);
}

PharaohScenarioEvent *pharaoh_scenario_invasion_next(
    PharaohScenarioEvent *events, int32_t count,
    const PharaohScenarioEvent *previous, bool create,
    uint8_t *invasion_search_flag, int32_t *current_event_index)
{
    const int32_t first = previous != NULL ? previous->id + 1 : 0;
    int32_t index = first;

    *invasion_search_flag = 1;
    while (index < count) {
        PharaohScenarioEvent *event = &events[index];
        if (event->type == PHARAOH_EVENT_INVASION) {
            event->id = (int16_t)index;
            *current_event_index = index;
            return event;
        }
        ++index;
    }
    if (create && first < 140) {
        events[index].id = (int16_t)index;
        *current_event_index = index;
        return &events[140];
    }
    --*current_event_index;
    return NULL;
}

uint32_t pharaoh_event_is_future_result(
    const PharaohScenarioEvent *event,
    const PharaohEventCalendarView *calendar)
{
    const uint32_t event_year =
        (uint32_t)(int32_t)event->year.value +
        (uint32_t)(int32_t)*calendar->scenario_start_year;
    const int32_t current_year = *calendar->current_year;

    if (current_year < (int32_t)event_year) {
        return (event_year & UINT32_C(0xFFFFFF00)) | UINT32_C(1);
    }
    if (current_year > (int32_t)event_year) {
        return event_year & UINT32_C(0xFFFFFF00);
    }
    return (uint32_t)(*calendar->current_month < (int32_t)event->month);
}

uint8_t pharaoh_event_is_running(const PharaohScenarioEvent *event,
    const PharaohEventCalendarView *calendar)
{
    uint32_t elapsed;
    if ((event->flags & UINT32_C(0x11)) != 0u) return 0;
    if ((uint8_t)pharaoh_event_is_future_result(event, calendar) != 0u)
        return 0;
    elapsed = (uint32_t)*calendar->current_year -
        (uint32_t)(int32_t)event->year.value -
        (uint32_t)(int32_t)*calendar->scenario_start_year;
    elapsed = elapsed * 12u - (uint32_t)event->month +
        (uint32_t)*calendar->current_month;
    return (uint8_t)((int32_t)elapsed <= (int32_t)event->months_initial);
}

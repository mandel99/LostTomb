#ifndef PHARAOH_EVENTS_EVENT_QUERIES_H
#define PHARAOH_EVENTS_EVENT_QUERIES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum PharaohEventType {
    PHARAOH_EVENT_NONE = 0,
    PHARAOH_EVENT_REQUEST = 1,
    PHARAOH_EVENT_INVASION = 2,
    PHARAOH_EVENT_EARTHQUAKE = 3,
    PHARAOH_EVENT_REVOLT = 4,
    PHARAOH_EVENT_CHANGE_PHARAOH = 5,
    PHARAOH_EVENT_SEA_TRADE_PROBLEM = 6,
    PHARAOH_EVENT_LAND_TRADE_PROBLEM = 7,
    PHARAOH_EVENT_WAGE_INCREASE = 8,
    PHARAOH_EVENT_WAGE_DECREASE = 9,
    PHARAOH_EVENT_CONTAMINATED_WATER = 10,
    PHARAOH_EVENT_GOLD_MINE_COLLAPSE = 11,
    PHARAOH_EVENT_CLAY_PIT_FLOOD = 12,
    PHARAOH_EVENT_DEMAND_INCREASE = 13,
    PHARAOH_EVENT_DEMAND_DECREASE = 14,
    PHARAOH_EVENT_PRICE_INCREASE = 15,
    PHARAOH_EVENT_PRICE_DECREASE = 16,
    PHARAOH_EVENT_REPUTATION_INCREASE = 17,
    PHARAOH_EVENT_REPUTATION_DECREASE = 18,
    PHARAOH_EVENT_CITY_STATUS_CHANGE = 19,
    PHARAOH_EVENT_MESSAGE = 20,
    PHARAOH_EVENT_FLOOD_WORSE = 21,
    PHARAOH_EVENT_FLOOD_BETTER = 22,
    PHARAOH_EVENT_GIFT_FROM_PHARAOH = 23,
    PHARAOH_EVENT_LOCUSTS = 24,
    PHARAOH_EVENT_FROGS = 25,
    PHARAOH_EVENT_HAILSTORM = 26,
    PHARAOH_EVENT_BLOOD_RIVER = 27,
    PHARAOH_EVENT_CRIME_WAVE = 28,
    PHARAOH_EVENT_MUMMY = 29,
    PHARAOH_EVENT_TYPE_COUNT = 30
};

enum PharaohResource {
    PHARAOH_RESOURCE_MIN_FOOD = 1,
    PHARAOH_RESOURCE_MAX_FOOD = 9,
    PHARAOH_RESOURCE_BRICKS = 12,
    PHARAOH_RESOURCE_POTTERY = 13,
    PHARAOH_RESOURCE_BEER = 15,
    PHARAOH_RESOURCE_LINEN = 17,
    PHARAOH_RESOURCE_LUXURY_GOODS = 19,
    PHARAOH_RESOURCE_TIMBER = 20,
    PHARAOH_RESOURCE_GOLD = 21,
    PHARAOH_RESOURCE_STONE = 24,
    PHARAOH_RESOURCE_LIMESTONE = 25,
    PHARAOH_RESOURCE_GRANITE = 26,
    PHARAOH_RESOURCE_UNUSED = 27,
    PHARAOH_RESOURCE_SANDSTONE = 30,
    PHARAOH_RESOURCE_DEBEN = 36,
    PHARAOH_RESOURCE_TROOPS = 37
};

enum PharaohEventQueryLimits {
    PHARAOH_REQUEST_SUBTYPE_COUNT = 7,
    PHARAOH_EVENT_REASON_GROUP_COUNT = 54,
    PHARAOH_EVENT_REASON_COLUMN_COUNT = 36
};

typedef struct PharaohScenarioValue {
    int16_t value;
    int16_t fixed;
    int16_t min;
    int16_t max;
} PharaohScenarioValue;

typedef struct PharaohScenarioEarthquakeData {
    int16_t duration;
    int16_t delay;
    int16_t ticks;
    int16_t locations[8];
} PharaohScenarioEarthquakeData;

typedef struct PharaohScenarioInvasionData {
    int16_t first_formation_id;
    int16_t home_x;
    int16_t home_y;
    int16_t layout;
    int16_t destination_x;
    int16_t destination_y;
    int16_t destination_building_id;
    int16_t formation_count;
    int16_t ignore_soldiers;
    uint8_t reserved[4];
} PharaohScenarioInvasionData;

typedef struct PharaohScenarioEvent {
    int16_t id;
    uint8_t type;
    uint8_t month;
    union {
        PharaohScenarioValue item;
        struct {
            int16_t item_runtime_value;
            int16_t item_value;
            int16_t item_min;
            int16_t item_max;
        };
    };
    union {
        PharaohScenarioValue amount;
        struct {
            int16_t amount_runtime_value;
            int16_t amount_value;
            int16_t amount_min;
            int16_t amount_max;
        };
    };
    union {
        PharaohScenarioValue year;
        struct {
            int16_t year_runtime_value;
            int16_t year_value;
            int16_t year_min;
            int16_t year_max;
        };
    };
    union {
        PharaohScenarioValue route;
        struct {
            int16_t route_runtime_value;
            int16_t route_value;
            int16_t route_min;
            int16_t route_max;
        };
    };
    int16_t on_completed;
    int16_t on_refusal;
    uint32_t flags;
    int16_t months_initial;
    int16_t months_left;
    int16_t state;
    uint8_t is_active;
    uint8_t can_comply_shown;
    int16_t parent_id;
    uint8_t phrase_variant;
    uint8_t parent_phrase_variant;
    int16_t attack_target;
    union {
        PharaohScenarioEarthquakeData earthquake;
        PharaohScenarioInvasionData invasion_data;
    } data;
    int16_t earthquake_step;
    int16_t on_too_late;
    int16_t on_defeat;
    uint8_t sender_faction;
    uint8_t invasion_warnings_shown;
    PharaohScenarioValue invasion;
    uint8_t subtype;
    uint8_t months_margin;
    uint8_t coin;
    uint8_t parent_city;
    int16_t parent_phrase;
    int16_t parent_item;
    int16_t parent_amount;
    uint8_t reason[4];
    uint8_t chain_marked;
    uint8_t reserved[13];
} PharaohScenarioEvent;

_Static_assert(sizeof(PharaohScenarioEvent) == 0x7c,
    "scenario event must keep the native 124-byte record layout");
_Static_assert(offsetof(PharaohScenarioEvent, flags) == 0x28 &&
    offsetof(PharaohScenarioEvent, reason) == 0x6a,
    "scenario event fields must keep their native offsets");

typedef struct PharaohRequestMessageRule {
    int32_t completed[2];
    int32_t refused[2];
    int32_t too_late[2];
    int32_t defeat[2];
    int32_t completed_group;
    int32_t refused_group;
    int32_t too_late_group;
    int32_t defeat_group;
    int32_t reason_column;
} PharaohRequestMessageRule;

typedef struct PharaohEventTypeText {
    int32_t completed;
    int32_t reason_group;
    int32_t reason_column;
} PharaohEventTypeText;

typedef struct PharaohEventTypeProperty {
    int32_t resource_text_group;
} PharaohEventTypeProperty;

typedef int32_t (*PharaohEventTextFunction)(
    void *user_data, const PharaohScenarioEvent *event);
typedef int32_t (*PharaohPhraseVariantFunction)(
    void *user_data, int32_t text, int32_t variant);
typedef int32_t (*PharaohResourceBoundFunction)(
    void *user_data, const PharaohScenarioEvent *event);
typedef bool (*PharaohResourceAllowedFunction)(void *user_data, int32_t resource);

typedef struct PharaohEventQueryContext {
    const PharaohScenarioEvent *events;
    size_t event_count;
    const PharaohRequestMessageRule *request_rules;
    const PharaohEventTypeText *type_texts;
    const PharaohEventTypeProperty *type_properties;
    const int32_t *reason_phrases;
    PharaohEventTextFunction event_message_text;
    PharaohPhraseVariantFunction phrase_variant;
    PharaohResourceBoundFunction resource_min;
    PharaohResourceBoundFunction resource_max;
    PharaohResourceAllowedFunction gift_resource_allowed;
    void *user_data;
} PharaohEventQueryContext;

/* Original addresses 0x0044EF60 through 0x0044FD30. */
bool pharaoh_event_parent_by_favour(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event);
int32_t pharaoh_event_reason_group(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event, int32_t action);
int32_t pharaoh_event_reason_group_completed(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event);
int32_t pharaoh_event_reason_group_refused(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event);
int32_t pharaoh_event_reason_group_too_late(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event);
int32_t pharaoh_event_reason_group_defeat(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event);
int32_t pharaoh_event_reason_column(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event);
int32_t pharaoh_event_message_text_completed(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event);
int32_t pharaoh_event_message_text_refused(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event);
int32_t pharaoh_event_message_text_too_late(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event);
int32_t pharaoh_event_message_text_defeat(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event);
int32_t pharaoh_event_reason_phrase(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event, int32_t action);

/* Original addresses 0x00449440 through 0x00449B30. */
bool pharaoh_event_request_resource_allowed(
    const PharaohScenarioEvent *event, int32_t resource);
bool pharaoh_event_resource_in_range(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event, int32_t resource);
int32_t pharaoh_event_resource_group(const PharaohEventQueryContext *context,
    const PharaohScenarioEvent *event);
bool pharaoh_event_has_months(const PharaohScenarioEvent *event);
bool pharaoh_event_has_followup(const PharaohScenarioEvent *event);
bool pharaoh_event_is_invasion_kind(const PharaohScenarioEvent *event);
bool pharaoh_event_type_chains(const PharaohScenarioEvent *event);
bool pharaoh_editor_event_has_subtype(const PharaohScenarioEvent *event);
bool pharaoh_editor_event_has_phrase_number(const PharaohScenarioEvent *event);
bool pharaoh_editor_event_has_phrase_list(const PharaohScenarioEvent *event);
int32_t pharaoh_editor_event_subtype_count(const PharaohScenarioEvent *event);
void pharaoh_editor_event_set_type(PharaohScenarioEvent *event, int32_t type);
int32_t pharaoh_event_subtype_group(const PharaohScenarioEvent *event);

/* Complete C translation of Pharaoh.exe 0x0044F660. */
int32_t pharaoh_event_announcement_text(const PharaohScenarioEvent *event);

PharaohScenarioEvent *pharaoh_scenario_request_next_active(
    PharaohScenarioEvent *events, int32_t count,
    const PharaohScenarioEvent *previous);
uint8_t pharaoh_event_record_is_enabled_0044d4c0(
    const uint8_t *record);

#endif

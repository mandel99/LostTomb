#ifndef PHARAOH_EDITOR_EVENT_BUTTONS_H
#define PHARAOH_EDITOR_EVENT_BUTTONS_H

#include "pharaoh/events/event_queries.h"

#include <stdbool.h>
#include <stdint.h>

enum PharaohEditorChoiceHandler {
    PHARAOH_EDITOR_CHOOSE_TYPE,
    PHARAOH_EDITOR_CHOOSE_SUBTYPE,
    PHARAOH_EDITOR_CHOOSE_MONTH,
    PHARAOH_EDITOR_CHOOSE_REASON_COMPLETED,
    PHARAOH_EDITOR_CHOOSE_REASON_REFUSED,
    PHARAOH_EDITOR_CHOOSE_REASON_TOO_LATE,
    PHARAOH_EDITOR_CHOOSE_REASON_DEFEAT,
    PHARAOH_EDITOR_CHOOSE_ITEM
};

typedef void (*PharaohEditorOpenListFunction)(void *user_data, int32_t x,
    int32_t y, int32_t count, int32_t language_group, bool keep_items,
    enum PharaohEditorChoiceHandler handler, int32_t rows, int16_t *target);
typedef void (*PharaohEditorSetTypeFunction)(
    void *user_data, PharaohScenarioEvent *event, int32_t type);
typedef int32_t (*PharaohEditorEventQueryFunction)(
    void *user_data, const PharaohScenarioEvent *event);
typedef bool (*PharaohEditorResourceQueryFunction)(void *user_data,
    const PharaohScenarioEvent *event, int32_t resource);
typedef int32_t (*PharaohEditorResourceListFunction)(void *user_data,
    const PharaohScenarioEvent *event, int32_t *items);

typedef struct PharaohEventEditorContext {
    PharaohScenarioEvent *selected_event;
    int16_t editor_value;
    int16_t editor_field;
    uint8_t editor_phrase;
    int32_t display_mode;
    int32_t button_parameter;
    uint8_t mummy_allowed;
    bool redraw_requested;
    int32_t *select_items;
    PharaohEditorOpenListFunction open_list;
    PharaohEditorSetTypeFunction set_type;
    PharaohEditorEventQueryFunction subtype_count;
    PharaohEditorEventQueryFunction subtype_group;
    PharaohEditorEventQueryFunction has_subtype;
    PharaohEditorEventQueryFunction shows_resource;
    PharaohEditorEventQueryFunction resource_min;
    PharaohEditorEventQueryFunction resource_max;
    PharaohEditorEventQueryFunction resource_group;
    PharaohEditorEventQueryFunction has_chain;
    PharaohEditorEventQueryFunction has_followup;
    PharaohEditorEventQueryFunction type_chains;
    PharaohEditorResourceQueryFunction resource_in_range;
    PharaohEditorResourceListFunction resource_list;
    void *user_data;
} PharaohEventEditorContext;

void pharaoh_editor_event_type_chosen(PharaohEventEditorContext *context);
void pharaoh_editor_event_phrase_chosen(PharaohEventEditorContext *context);
void pharaoh_editor_event_type_button(PharaohEventEditorContext *context);
void pharaoh_editor_event_subtype_chosen(PharaohEventEditorContext *context);
void pharaoh_editor_event_subtype_button(PharaohEventEditorContext *context);
void pharaoh_editor_event_month_chosen(PharaohEventEditorContext *context);
void pharaoh_editor_event_month_button(PharaohEventEditorContext *context);
void pharaoh_editor_event_trigger_button(PharaohEventEditorContext *context);
void pharaoh_editor_event_sender_button(PharaohEventEditorContext *context);
void pharaoh_editor_event_attack_target_button(PharaohEventEditorContext *context);
void pharaoh_editor_event_reason_completed_chosen(PharaohEventEditorContext *context);
void pharaoh_editor_event_reason_completed_button(PharaohEventEditorContext *context);
void pharaoh_editor_event_reason_refused_chosen(PharaohEventEditorContext *context);
void pharaoh_editor_event_reason_refused_button(PharaohEventEditorContext *context);
void pharaoh_editor_event_reason_too_late_chosen(PharaohEventEditorContext *context);
void pharaoh_editor_event_reason_too_late_button(PharaohEventEditorContext *context);
void pharaoh_editor_event_reason_defeat_chosen(PharaohEventEditorContext *context);
void pharaoh_editor_event_reason_defeat_button(PharaohEventEditorContext *context);
void pharaoh_editor_event_item_chosen(PharaohEventEditorContext *context);
void pharaoh_editor_event_item_button(PharaohEventEditorContext *context);

#endif

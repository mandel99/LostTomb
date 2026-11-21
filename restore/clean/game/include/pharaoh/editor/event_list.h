#ifndef PHARAOH_EDITOR_EVENT_LIST_H
#define PHARAOH_EDITOR_EVENT_LIST_H

#include "pharaoh/events/event_queries.h"

#include <stdbool.h>
#include <stdint.h>

enum { PHARAOH_EDITOR_EVENT_CAPACITY = 140 };

typedef struct PharaohEditorEventList {
    int32_t count;
    PharaohScenarioEvent events[PHARAOH_EDITOR_EVENT_CAPACITY];
    int32_t selected;
    int32_t first_visible;
    int32_t current_year;
    int16_t scenario_start_year;
    uint8_t current_month;
    uint8_t current_window;
    uint8_t previous_window;
    uint8_t redraw_requested;
    int32_t button_parameter;
    bool (*event_referenced)(void *user_data, int32_t event_index);
    void (*capacity_warning)(void *user_data);
    void *user_data;
} PharaohEditorEventList;

void pharaoh_editor_event_reset(PharaohScenarioEvent *event);
bool pharaoh_editor_event_due(const PharaohEditorEventList *list,
    const PharaohScenarioEvent *event, bool ignore_flags);
PharaohScenarioEvent *pharaoh_editor_event_append(
    PharaohEditorEventList *list, bool limited);
bool pharaoh_editor_event_erase(PharaohEditorEventList *list, int32_t index);
void pharaoh_editor_event_open_detail(PharaohEditorEventList *list);
void pharaoh_editor_event_select_visible(PharaohEditorEventList *list);
void pharaoh_editor_event_create(PharaohEditorEventList *list);
void pharaoh_editor_event_delete_selected(PharaohEditorEventList *list);

#endif

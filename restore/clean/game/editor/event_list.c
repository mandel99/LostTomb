#include "pharaoh/editor/event_list.h"

#include <stddef.h>
#include <string.h>

enum {
    EVENT_FLAG_CHAIN_ONLY = 1,
    EVENT_FLAG_FIRED = 4,
    EVENT_FLAG_REUSABLE = 8,
    EVENT_DETAIL_WINDOW = 0x18,
    EVENT_LIMITED_CAPACITY = 120
};

_Static_assert(offsetof(PharaohScenarioEvent, year) == 0x14 &&
    offsetof(PharaohScenarioEvent, is_active) == 0x32 &&
    offsetof(PharaohScenarioEvent, on_too_late) == 0x52 &&
    offsetof(PharaohScenarioEvent, on_defeat) == 0x54,
    "event-list native field offsets");

static void write_i16(PharaohScenarioEvent *event, size_t offset, int16_t value)
{
    memcpy((uint8_t *)event + offset, &value, sizeof(value));
}

static void write_u32(PharaohScenarioEvent *event, size_t offset, uint32_t value)
{
    memcpy((uint8_t *)event + offset, &value, sizeof(value));
}

void pharaoh_editor_event_reset(PharaohScenarioEvent *event)
{
    uint8_t *const raw = (uint8_t *)event;
    write_i16(event, 0x00, -1);
    raw[0x02] = 0;
    raw[0x03] = 0;
    write_i16(event, 0x24, -1);
    write_i16(event, 0x26, -1);
    write_i16(event, 0x52, -1);
    write_i16(event, 0x54, -1);
    write_u32(event, 0x28, 2);
    write_i16(event, 0x18, 1);
    write_i16(event, 0x1a, 2);
    write_i16(event, 0x16, -1);
    write_i16(event, 0x10, 5);
    write_i16(event, 0x12, 10);
    write_i16(event, 0x0e, -1);
    write_i16(event, 0x5c, 0);
    write_i16(event, 0x5e, 49);
    write_i16(event, 0x5a, -1);
    write_i16(event, 0x06, 1);
    write_i16(event, 0x08, -1);
    write_i16(event, 0x0a, -1);
    write_i16(event, 0x20, 1);
    write_i16(event, 0x22, 8);
    write_i16(event, 0x1e, -1);
    write_i16(event, 0x2c, 2);
    write_i16(event, 0x2e, 0);
    write_i16(event, 0x30, 0);
    raw[0x32] = 0;
    raw[0x33] = 0;
    write_i16(event, 0x34, -1);
    raw[0x36] = 0;
    write_i16(event, 0x38, 0);
    write_i16(event, 0x3a, 0);
    write_i16(event, 0x3c, 0);
    write_i16(event, 0x3e, 0);
    write_i16(event, 0x40, 0);
    write_i16(event, 0x42, 0);
    write_i16(event, 0x44, 0);
    write_i16(event, 0x46, 0);
    write_i16(event, 0x4a, 0);
}

bool pharaoh_editor_event_due(const PharaohEditorEventList *list,
    const PharaohScenarioEvent *event, bool ignore_flags)
{
    if (!ignore_flags &&
        (event->flags & (EVENT_FLAG_CHAIN_ONLY | EVENT_FLAG_FIRED)) != 0u) {
        return false;
    }
    const int32_t event_year = (int32_t)event->year.value +
        (int32_t)list->scenario_start_year;
    return list->current_year == event_year &&
        list->current_month == event->month;
}

PharaohScenarioEvent *pharaoh_editor_event_append(
    PharaohEditorEventList *list, bool limited)
{
    if (limited && list->count > EVENT_LIMITED_CAPACITY - 1) return NULL;
    for (int32_t index = 0; index < list->count; ++index) {
        PharaohScenarioEvent *event = &list->events[index];
        if ((event->flags & EVENT_FLAG_REUSABLE) != 0u &&
            (event->flags & EVENT_FLAG_FIRED) != 0u &&
            event->is_active == 0u &&
            !pharaoh_editor_event_due(list, event, true) &&
            (event->type != PHARAOH_EVENT_INVASION ||
             !list->event_referenced(list->user_data, index))) {
            return event;
        }
    }
    if (list->count >= PHARAOH_EDITOR_EVENT_CAPACITY) {
        if (!limited) list->capacity_warning(list->user_data);
        return NULL;
    }
    PharaohScenarioEvent *event = &list->events[list->count++];
    pharaoh_editor_event_reset(event);
    event->id = (int16_t)(list->count - 1);
    return event;
}

static void decrement_reference(int16_t *reference, int32_t removed)
{
    if (*reference > removed) --*reference;
}

bool pharaoh_editor_event_erase(PharaohEditorEventList *list, int32_t index)
{
    if (index < 0 || index >= list->count) return false;
    if (index < list->count - 1) {
        memmove(&list->events[index], &list->events[index + 1],
            (size_t)(list->count - index - 1) * sizeof(list->events[0]));
        for (int32_t current = index; current < list->count - 1; ++current) {
            list->events[current].id = (int16_t)current;
        }
    }
    --list->count;
    for (int32_t current = 0; current < list->count; ++current) {
        PharaohScenarioEvent *event = &list->events[current];
        decrement_reference(&event->on_completed, index);
        decrement_reference(&event->on_refusal, index);
        decrement_reference(&event->on_too_late, index);
        decrement_reference(&event->on_defeat, index);
    }
    if (list->first_visible >= list->count) {
        if (--list->first_visible < 0) list->first_visible = 0;
    }
    if (list->selected >= list->count) list->selected = -1;
    return true;
}

void pharaoh_editor_event_open_detail(PharaohEditorEventList *list)
{
    if (list->selected < 0 || list->selected >= list->count ||
        list->current_window == EVENT_DETAIL_WINDOW) return;
    const uint8_t previous = list->current_window;
    list->current_window = EVENT_DETAIL_WINDOW;
    list->redraw_requested = 1;
    list->previous_window = previous;
}

void pharaoh_editor_event_select_visible(PharaohEditorEventList *list)
{
    const int32_t selected = list->button_parameter + list->first_visible;
    if (selected < 0 || selected >= list->count) {
        list->selected = -1;
        return;
    }
    list->selected = selected;
    pharaoh_editor_event_open_detail(list);
}

void pharaoh_editor_event_create(PharaohEditorEventList *list)
{
    PharaohScenarioEvent *event = pharaoh_editor_event_append(list, true);
    if (event == NULL) return;
    const int16_t id = event->id;
    pharaoh_editor_event_reset(event);
    event->id = id;
    ((uint8_t *)event)[0x02] = 1;
    ((uint8_t *)event)[0x60] = 0;
    write_u32(event, 0x28, 0);
    write_i16(event, 0x06, 1);
    write_i16(event, 0x08, -1);
    write_i16(event, 0x0a, -1);
    write_i16(event, 0x16, 1);
    write_i16(event, 0x18, -1);
    write_i16(event, 0x1a, -1);
    write_i16(event, 0x1e, 1);
    write_i16(event, 0x20, -1);
    write_i16(event, 0x22, -1);
    write_i16(event, 0x0e, 8);
    write_i16(event, 0x10, -1);
    write_i16(event, 0x12, -1);
    write_i16(event, 0x2c, 12);
    write_i16(event, 0x24, -1);
    write_i16(event, 0x26, -1);
    write_i16(event, 0x52, -1);
    write_i16(event, 0x54, -1);
}

void pharaoh_editor_event_delete_selected(PharaohEditorEventList *list)
{
    (void)pharaoh_editor_event_erase(list, list->selected);
}

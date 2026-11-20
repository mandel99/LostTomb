#include "pharaoh/ui/map_selection_invasions.h"

#include <string.h>

enum {
    EVENT_TYPE = 0x02,
    EVENT_LINK_COMPLETED = 0x24,
    EVENT_LINK_REFUSED = 0x26,
    EVENT_FLAGS = 0x28,
    EVENT_LINK_TOO_LATE = 0x52,
    EVENT_LINK_DEFEAT = 0x54,
    EVENT_SUBTYPE = 0x60,
    EVENT_VISITED = 0x6e,
    EVENT_REQUEST = 1,
    EVENT_INVASION = 2,
    EVENT_GIFT_FROM_PHARAOH = 23
};

static int16_t event_i16(const PharaohMapSelectionEventRecord *event,
    size_t offset)
{
    int16_t value;
    memcpy(&value, event->bytes + offset, sizeof(value));
    return value;
}

static uint32_t event_u32(const PharaohMapSelectionEventRecord *event,
    size_t offset)
{
    uint32_t value;
    memcpy(&value, event->bytes + offset, sizeof(value));
    return value;
}

static PharaohMapSelectionEventRecord *event_by_id(
    PharaohMapSelectionEventList *events, int32_t id)
{
    if (id < 0 || id >= events->count ||
        id >= PHARAOH_MAP_SELECTION_EVENT_CAPACITY) return NULL;
    return &events->records[id];
}

static PharaohMapSelectionEventRecord *linked_event(
    PharaohMapSelectionEventList *events,
    const PharaohMapSelectionEventRecord *source, size_t link_offset)
{
    const int32_t id = event_i16(source, link_offset);
    PharaohMapSelectionEventRecord *const target = event_by_id(events, id);
    if (target == NULL || (event_u32(target, EVENT_FLAGS) & 1u) == 0 ||
        event_i16(target, 0) == event_i16(source, 0)) return NULL;
    return target;
}

void pharaoh_map_selection_mark_linked_events(
    PharaohMapSelectionEventList *events,
    PharaohMapSelectionEventRecord *event, int32_t depth)
{
    PharaohMapSelectionEventRecord *target;
    uint8_t type;
    if (event == NULL || depth >= 7) return;
    ++depth;
    do {
        event->bytes[EVENT_VISITED] = 1;
        target = linked_event(events, event, EVENT_LINK_COMPLETED);
        if (target != NULL) {
            pharaoh_map_selection_mark_linked_events(events, target, depth);
        }
        type = event->bytes[EVENT_TYPE];
        if (type == EVENT_REQUEST || type == EVENT_GIFT_FROM_PHARAOH) {
            target = linked_event(events, event, EVENT_LINK_REFUSED);
            if (target != NULL) {
                pharaoh_map_selection_mark_linked_events(
                    events, target, depth);
            }
            target = linked_event(events, event, EVENT_LINK_TOO_LATE);
            if (target != NULL) {
                pharaoh_map_selection_mark_linked_events(
                    events, target, depth);
            }
        }
        if (type != EVENT_REQUEST || event->bytes[EVENT_SUBTYPE] == 0 ||
            event->bytes[EVENT_SUBTYPE] > 2) return;
        target = linked_event(events, event, EVENT_LINK_DEFEAT);
        if (target == NULL) return;
        event = target;
        ++depth;
    } while (depth < 8);
}

static PharaohMapSelectionEventRecord *next_invasion(
    PharaohMapSelectionEventList *events,
    const PharaohMapSelectionEventRecord *previous,
    int32_t *current_event_index, uint8_t *invasion_search_flag)
{
    int32_t index = previous != NULL ? event_i16(previous, 0) + 1 : 0;
    *invasion_search_flag = 1;
    while (index < events->count &&
           index < PHARAOH_MAP_SELECTION_EVENT_CAPACITY) {
        PharaohMapSelectionEventRecord *const event = &events->records[index];
        if (event->bytes[EVENT_TYPE] == EVENT_INVASION) {
            const int16_t id = (int16_t)index;
            memcpy(event->bytes, &id, sizeof(id));
            *current_event_index = index;
            return event;
        }
        ++index;
    }
    --*current_event_index;
    return NULL;
}

int32_t pharaoh_map_selection_count_invasions(
    PharaohMapSelectionEventList *events, int32_t *current_event_index,
    uint8_t *invasion_search_flag)
{
    PharaohMapSelectionEventRecord *event;
    int32_t result = 0;
    int32_t count = events->count;
    if (count < 0) count = 0;
    if (count > PHARAOH_MAP_SELECTION_EVENT_CAPACITY) {
        count = PHARAOH_MAP_SELECTION_EVENT_CAPACITY;
    }
    for (int32_t index = 0; index < count; ++index) {
        events->records[index].bytes[EVENT_VISITED] = 0;
    }
    for (int32_t index = 0; index < count; ++index) {
        if ((event_u32(&events->records[index], EVENT_FLAGS) & 0x10u) != 0) {
            pharaoh_map_selection_mark_linked_events(
                events, &events->records[index], 1);
        }
    }
    event = next_invasion(events, NULL, current_event_index,
        invasion_search_flag);
    while (event != NULL) {
        if (event->bytes[EVENT_VISITED] == 0) ++result;
        event = next_invasion(events, event, current_event_index,
            invasion_search_flag);
    }
    return result;
}

#ifndef PHARAOH_UI_MAP_SELECTION_INVASIONS_H
#define PHARAOH_UI_MAP_SELECTION_INVASIONS_H

#include <stdint.h>

enum { PHARAOH_MAP_SELECTION_EVENT_CAPACITY = 150 };

typedef struct PharaohMapSelectionEventRecord {
    uint8_t bytes[0x7c];
} PharaohMapSelectionEventRecord;

typedef struct PharaohMapSelectionEventList {
    int32_t count;
    PharaohMapSelectionEventRecord records[PHARAOH_MAP_SELECTION_EVENT_CAPACITY];
    uint8_t metadata[24];
} PharaohMapSelectionEventList;

_Static_assert(sizeof(PharaohMapSelectionEventList) == 18628,
    "native scenario-event list size");

/* Complete typed C translations of native 0x0041E270 and 0x0041E3B0.
   The two former globals modified by the invasion iterator are explicit. */
void pharaoh_map_selection_mark_linked_events(
    PharaohMapSelectionEventList *events,
    PharaohMapSelectionEventRecord *event, int32_t depth);
int32_t pharaoh_map_selection_count_invasions(
    PharaohMapSelectionEventList *events, int32_t *current_event_index,
    uint8_t *invasion_search_flag);

#endif

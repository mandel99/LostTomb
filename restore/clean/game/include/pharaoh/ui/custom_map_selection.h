#ifndef PHARAOH_UI_CUSTOM_MAP_SELECTION_H
#define PHARAOH_UI_CUSTOM_MAP_SELECTION_H

#include "pharaoh/runtime/file_list.h"
#include "pharaoh/scenario/campaign_file.h"

enum { PHARAOH_CUSTOM_MAP_VISIBLE_ROWS = 15 };

typedef struct PharaohCustomMapSelectionState {
    PharaohFileList *files;
    int32_t selected_file;
    int32_t first_visible_file;
} PharaohCustomMapSelectionState;

/* Native 0x0041F810 in custom-map mode: load maps\\*.map and sort names. */
int32_t pharaoh_custom_map_selection_load(
    PharaohCustomMapSelectionState *state);

/* Native 0x0041F810 / 0x0041DF70 path stores campaign names in source order;
   each row's selected index is its mission ID. */
int32_t pharaoh_campaign_mission_selection_load(
    PharaohCustomMapSelectionState *state, const PharaohCampaign *campaign);

/* Selected-list-index calculation used by the 0x0041F670 row callback. */
int32_t pharaoh_custom_map_selection_select_visible(
    PharaohCustomMapSelectionState *state, int32_t visible_row);

/* Native arrow callback 0x0041F460; direction 0 is up, 1 is down. */
int32_t pharaoh_custom_map_selection_scroll(
    PharaohCustomMapSelectionState *state, int32_t direction);

/* Native scrollbar callback 0x0041F4D0, for custom-map mode (0). */
int32_t pharaoh_custom_map_selection_drag_scrollbar(
    PharaohCustomMapSelectionState *state, int32_t mouse_x, int32_t mouse_y,
    int32_t origin_x, int32_t origin_y);

#endif

#ifndef PHARAOH_UI_MISSION_INTERMEZZO_H
#define PHARAOH_UI_MISSION_INTERMEZZO_H

#include "pharaoh/runtime/settings_ini.h"
#include "pharaoh/scenario/campaign_start.h"

#include <stdint.h>

typedef struct PharaohMissionChoiceInputState {
    const PharaohCampaignIndex *index;
    const PharaohSettingsFile *settings;
    uint8_t input_enabled;
    int32_t origin_x;
    int32_t origin_y;
    uint8_t *selected_choice;
    uint8_t *redraw_requested;
} PharaohMissionChoiceInputState;

typedef struct PharaohMissionChoiceInputOperations {
    void *user_data;
    int32_t (*mouse_inside)(void *user_data, int32_t x, int32_t y,
        int32_t width, int32_t height);
    void (*play_fanfare)(void *user_data, const char *path,
        uint32_t one, uint32_t zero);
    void (*selection_failure)(void *user_data, uint32_t origin);
} PharaohMissionChoiceInputOperations;

/* Complete typed translation of native 0x00419090. selection_failure must not
   return, matching the old verified C owner's campaign invariant guard. */
void pharaoh_mission_choice_handle_input(
    PharaohMissionChoiceInputState *state,
    const PharaohMissionChoiceInputOperations *operations);

typedef struct PharaohIntermezzoState {
    int32_t screen_width;
    int32_t screen_height;
    uint32_t mode;
    uint32_t background_image;
    int32_t scenario_mode;
    int8_t mission_id;
    uint8_t classical_names;
    int32_t *view_x;
    int32_t *view_y;
    int32_t *view_width;
    int32_t *view_height;
    uint32_t *music_flag_a;
    uint32_t *music_flag_b;
    uint8_t *redraw_requested;
} PharaohIntermezzoState;

typedef struct PharaohIntermezzoOperations {
    void *user_data;
    void (*draw_sprite)(void *user_data, uint32_t image,
        int32_t x, int32_t y);
    void (*play_mission_track)(void *user_data, uint32_t victory,
        uint8_t classical_names, int8_t mission_id);
} PharaohIntermezzoOperations;

/* Complete typed translation of native 0x0041A850. */
void pharaoh_intermezzo_prepare(PharaohIntermezzoState *state,
    const PharaohIntermezzoOperations *operations);

/* Complete typed translation of native 0x004181A0. */
int32_t pharaoh_mission_transition_text_id(
    const PharaohCampaignIndex *index, const PharaohSettingsFile *settings,
    int32_t scenario_mode);

typedef struct PharaohMissionChoiceRenderState {
    const PharaohCampaignIndex *index;
    const PharaohSettingsFile *settings;
    uint8_t selected_choice;
    int32_t origin_x;
    int32_t origin_y;
    int32_t screen_width;
    int32_t screen_height;
    int32_t choice_background_image;
    int32_t choice_link_image_base;
    int32_t marker_image_base;
    int32_t large_font;
    int32_t black_font;
    int32_t *panel_x;
    int32_t *panel_y;
    int32_t *view_x;
    int32_t *view_y;
    int32_t *view_width;
    int32_t *view_height;
} PharaohMissionChoiceRenderState;

typedef struct PharaohMissionChoiceRenderOperations {
    void *user_data;
    int32_t (*mouse_inside)(void *user_data, int32_t x, int32_t y,
        int32_t width, int32_t height);
    void (*draw_sprite)(void *user_data, int32_t image,
        int32_t x, int32_t y);
    void (*draw_language)(void *user_data, int32_t group, int32_t entry,
        int32_t x, int32_t y, int32_t font, int32_t color);
    void (*draw_language_multiline)(void *user_data, int32_t group,
        int32_t entry, int32_t x, int32_t y, int32_t width,
        int32_t font, int32_t color);
} PharaohMissionChoiceRenderOperations;

/* Complete typed translations of native 0x00419140 and 0x0041AAA0. */
void pharaoh_mission_choice_draw_markers(
    const PharaohMissionChoiceRenderState *state,
    const PharaohMissionChoiceRenderOperations *operations);
void pharaoh_mission_choice_draw(
    PharaohMissionChoiceRenderState *state,
    const PharaohMissionChoiceRenderOperations *operations);

#endif

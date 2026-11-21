#ifndef PHARAOH_UI_VICTORY_DIALOG_H
#define PHARAOH_UI_VICTORY_DIALOG_H

#include <stdint.h>

enum PharaohVictoryState {
    PHARAOH_VICTORY_STATE_LOST = 0,
    PHARAOH_VICTORY_STATE_WON = 1
};

enum PharaohDefeatOptions {
    PHARAOH_DEFEAT_NEW_GAME = 1,
    PHARAOH_DEFEAT_REPLAY = 2,
    PHARAOH_DEFEAT_LOWER_DIFFICULTY = 3
};

typedef struct PharaohVictoryDialogState {
    int32_t origin_x;
    int32_t origin_y;
    int32_t screen_width;
    int32_t screen_height;
    uint8_t victory_state;
    int32_t focused_button;
    int8_t mission_id;
    int32_t scenario_mode;
    uint8_t campaign_mode;
    int32_t large_font;
    int32_t black_font;
    int32_t *panel_x;
    int32_t *panel_y;
    int32_t *view_x;
    int32_t *view_y;
    int32_t *view_width;
    int32_t *view_height;
} PharaohVictoryDialogState;

typedef struct PharaohVictoryDialogOperations {
    void *user_data;
    void (*draw_panel)(void *user_data, int32_t x, int32_t y,
        int32_t width_blocks, int32_t height_blocks);
    void (*draw_large_label)(void *user_data, int32_t label,
        int32_t x, int32_t y, int32_t width_blocks, int32_t focused);
    void (*draw_language_centered)(void *user_data, int32_t group,
        int32_t entry, int32_t x, int32_t y, int32_t width,
        int32_t font, int32_t color);
    int32_t (*defeat_options)(void *user_data);
} PharaohVictoryDialogOperations;

/* Complete typed translations of native 0x004191F0 and 0x00419DB0. */
void pharaoh_victory_dialog_draw_background(
    PharaohVictoryDialogState *state,
    const PharaohVictoryDialogOperations *operations);
void pharaoh_victory_dialog_draw_foreground(
    PharaohVictoryDialogState *state,
    const PharaohVictoryDialogOperations *operations);

typedef struct PharaohVictoryMonumentState {
    int32_t panel_x;
    int32_t panel_y;
    int32_t black_font;
    int32_t monument_building_ids[3];
    int32_t monument_text_ids[3];
    int16_t city_progress_sixteenths;
} PharaohVictoryMonumentState;

typedef struct PharaohVictoryMonumentOperations {
    void *user_data;
    int32_t (*building_type)(void *user_data, int32_t building_id);
    uint32_t (*pyramid_progress)(void *user_data, int32_t building_id);
    uint32_t (*stored_progress)(void *user_data, int32_t building_id);
    uint32_t (*monument_progress)(void *user_data, int32_t building_type,
        int32_t building_id);
    void (*draw_language)(void *user_data, int32_t group, int32_t entry,
        int32_t x, int32_t y, int32_t font, int32_t color);
} PharaohVictoryMonumentOperations;

/* Complete typed translation of native 0x004192B0. */
int32_t pharaoh_victory_count_finished_monuments(
    const PharaohVictoryMonumentState *state,
    const PharaohVictoryMonumentOperations *operations);

#endif

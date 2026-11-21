#ifndef PHARAOH_EDITOR_NAVIGATION_BUTTONS_H
#define PHARAOH_EDITOR_NAVIGATION_BUTTONS_H

#include <stdint.h>

typedef struct PharaohEditorNavigationState {
    int32_t button_parameter;
    int32_t *milestones;
    uint8_t *current_screen;
    uint8_t *redraw_requested;
} PharaohEditorNavigationState;

typedef struct PharaohEditorNavigationOperations {
    void *user_data;
    void (*open_numeric_input)(void *user_data, int32_t x, int32_t y,
        int32_t digits, int32_t *target);
} PharaohEditorNavigationOperations;

/* Complete typed translations of native 0x00408C20, 0x00408CD0,
   0x00408F00, 0x00409390, 0x004093A0 and 0x004093D0. */
void pharaoh_editor_open_flood(PharaohEditorNavigationState *state);
void pharaoh_editor_open_allowed_buildings(
    PharaohEditorNavigationState *state);
void pharaoh_editor_open_win_criteria(PharaohEditorNavigationState *state);
void pharaoh_editor_open_settings(PharaohEditorNavigationState *state);
void pharaoh_editor_settings_milestone(PharaohEditorNavigationState *state,
    const PharaohEditorNavigationOperations *operations);
void pharaoh_editor_settings_start_year(
    PharaohEditorNavigationState *state);

#endif

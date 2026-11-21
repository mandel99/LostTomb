#ifndef PHARAOH_EDITOR_MONUMENT_BUTTONS_H
#define PHARAOH_EDITOR_MONUMENT_BUTTONS_H

#include <stdint.h>

enum PharaohEditorListTargetWidth {
    PHARAOH_EDITOR_TARGET_INT8 = 1,
    PHARAOH_EDITOR_TARGET_INT16 = 2
};

typedef struct PharaohEditorMonumentDefinition {
    int32_t kind;
    int32_t subkind;
} PharaohEditorMonumentDefinition;

typedef struct PharaohEditorMonumentState {
    int32_t display_mode;
    int32_t button_parameter;
    int16_t monument_definition_ids[3];
    uint8_t animation_set;
    int32_t definition_count;
    const PharaohEditorMonumentDefinition *definitions;
    int32_t *provisions_needed;
    int32_t *select_items;
    uint8_t *current_screen;
    uint8_t *redraw_requested;
} PharaohEditorMonumentState;

typedef struct PharaohEditorMonumentOperations {
    void *user_data;
    int32_t (*goods_available)(void *user_data, int32_t definition_id);
    int32_t (*definition_allowed)(void *user_data, int32_t era,
        const PharaohEditorMonumentDefinition *definition);
    void (*open_list)(void *user_data, int32_t x, int32_t y,
        int32_t count, int32_t language_group, void *target,
        enum PharaohEditorListTargetWidth target_width,
        int32_t items_prepared, int32_t include_none, int32_t rows);
    int32_t (*resource_available)(void *user_data, int32_t resource);
    int32_t (*provision_kind_count)(void *user_data, int32_t monument);
    void (*show_popup)(void *user_data, int32_t message);
    void (*open_numeric_input)(void *user_data, int32_t x, int32_t y,
        int32_t digits, int32_t *target);
} PharaohEditorMonumentOperations;

/* Complete typed translations of native callback entries 0x0040C8A0,
   0x0040C920 and 0x0040CBE0. Their old typed LostTomb owner was recovered
   from the retired instruction-level callback cohort. */
void pharaoh_editor_open_monuments(PharaohEditorMonumentState *state);
void pharaoh_editor_select_monument(PharaohEditorMonumentState *state,
    const PharaohEditorMonumentOperations *operations);
void pharaoh_editor_set_monument_era(PharaohEditorMonumentState *state,
    const PharaohEditorMonumentOperations *operations);

/* Complete typed translation of native callback entry 0x0040CC80. */
void pharaoh_editor_edit_burial_provision(
    PharaohEditorMonumentState *state,
    const PharaohEditorMonumentOperations *operations);

#endif

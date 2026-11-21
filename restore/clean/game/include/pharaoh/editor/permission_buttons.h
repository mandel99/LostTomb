#ifndef PHARAOH_EDITOR_PERMISSION_BUTTONS_H
#define PHARAOH_EDITOR_PERMISSION_BUTTONS_H

#include <stdint.h>

typedef struct PharaohEditorPermissionRecord {
    uint8_t type;
    uint8_t enabled;
    uint8_t reserved_02[22];
    uint8_t permission_state_enabled;
    uint8_t reserved_19[5];
    uint8_t primary[14];
    uint8_t reserved_2c[7];
    uint8_t selection_marker;
    uint8_t secondary[8];
    uint8_t reserved_3c[2];
    uint8_t permission_state[36];
} PharaohEditorPermissionRecord;

enum PharaohEditorPermissionCompletion {
    PHARAOH_PERMISSION_COMPLETION_NONE,
    PHARAOH_PERMISSION_COMPLETION_PRIMARY,
    PHARAOH_PERMISSION_COMPLETION_SECONDARY
};

typedef struct PharaohEditorPermissionState {
    PharaohEditorPermissionRecord *records;
    int32_t current_record;
    int32_t button_parameter;
    const int32_t *building_kind_by_button;
    uint16_t *allowed_buildings;
    int32_t *selected_slot;
    uint32_t *previous_choice;
    int32_t *selection_mode;
    int32_t *select_items;
    uint8_t *redraw_requested;
} PharaohEditorPermissionState;

typedef struct PharaohEditorPermissionOperations {
    void *user_data;
    void (*open_list)(void *user_data, int32_t x, int32_t y,
        int32_t count, int32_t language_group, uint8_t *target,
        enum PharaohEditorPermissionCompletion completion, int32_t rows);
    int32_t (*has_special_assignment)(void *user_data,
        int32_t record, int32_t kind);
    int32_t (*find_primary_assignment)(void *user_data,
        int32_t record, int32_t kind);
    int32_t (*find_secondary_assignment)(void *user_data,
        int32_t record, int32_t kind);
    int32_t (*requirement_available)(void *user_data,
        int32_t requirement);
    void (*show_popup)(void *user_data, int32_t message);
} PharaohEditorPermissionOperations;

/* Complete typed translations of native 0x00408610, 0x00408840 and
   0x00408920. */
void pharaoh_editor_open_restricted_primary_permission(
    PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations);
void pharaoh_editor_open_primary_permission(
    PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations);
void pharaoh_editor_open_secondary_permission(
    PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations);

/* Complete typed translations of native 0x00444A40, 0x00408710,
   0x00408820 and 0x00408830. */
void pharaoh_editor_permission_set(PharaohEditorPermissionState *state,
    int32_t record, int32_t kind, int32_t value);
void pharaoh_editor_permission_update(PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations,
    int32_t secondary);
void pharaoh_editor_primary_permission_selected(
    PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations);
void pharaoh_editor_secondary_permission_selected(
    PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations);

/* Complete typed translation of native 0x00408CE0. */
void pharaoh_editor_toggle_building_permission(
    PharaohEditorPermissionState *state,
    const PharaohEditorPermissionOperations *operations);

#endif

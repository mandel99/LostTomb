#ifndef PHARAOH_EDITOR_VALIDATION_H
#define PHARAOH_EDITOR_VALIDATION_H

#include <stdbool.h>
#include <stdint.h>

typedef struct PharaohEditorPoint16 {
    int16_t x;
    int16_t y;
} PharaohEditorPoint16;

typedef struct PharaohEditorPoint32 {
    int32_t x;
    int32_t y;
} PharaohEditorPoint32;

typedef struct PharaohEditorValidationState {
    const uint32_t *terrain;
    uint32_t grid_base;
    PharaohEditorPoint16 terrain_kind_points16[4];
    PharaohEditorPoint32 terrain_kind_points32[4];
    PharaohEditorPoint16 road_points[8];
    PharaohEditorPoint16 allowed_pair[2];
    PharaohEditorPoint16 road_pair[2];
    PharaohEditorPoint16 allowed_points[8];
    PharaohEditorPoint16 road_points_second[8];
    uint32_t input_pending;
    uint8_t current_window;
    uint8_t return_window;
    uint32_t final_state;
    uint8_t redraw_requested;
    bool (*terrain_kind_valid)(void *user_data, uint32_t terrain,
        int32_t alternate);
    bool (*extra_check)(void *user_data);
    void (*show_message)(void *user_data, int32_t first, int32_t second);
    void (*commit_valid_map)(void *user_data, int32_t mode);
    void *user_data;
} PharaohEditorValidationState;

bool pharaoh_editor_terrain_allowed(uint32_t terrain);
bool pharaoh_editor_validate_terrain_kinds(
    const PharaohEditorValidationState *state);
bool pharaoh_editor_validate_road_points(
    const PharaohEditorValidationState *state);
bool pharaoh_editor_validate_allowed_pair(
    const PharaohEditorValidationState *state);
bool pharaoh_editor_validate_road_pair(
    const PharaohEditorValidationState *state);
bool pharaoh_editor_validate_allowed_points(
    const PharaohEditorValidationState *state);
bool pharaoh_editor_validate_second_road_points(
    const PharaohEditorValidationState *state);
bool pharaoh_editor_validate_and_commit(PharaohEditorValidationState *state);

typedef struct PharaohEditorTerrainKindContext {
    const uint32_t *mode_allowed;
    const uint32_t *default_allowed;
    uint32_t scene;
    int16_t alternate_index;
    uint32_t rule_mode;
} PharaohEditorTerrainKindContext;
uint32_t pharaoh_editor_terrain_kind_result(
    const PharaohEditorTerrainKindContext *context,
    uint32_t terrain, int8_t mode);
#endif

#ifndef PHARAOH_EDITOR_TERRAIN_KINDS_H
#define PHARAOH_EDITOR_TERRAIN_KINDS_H

#include <stdint.h>

typedef struct PharaohEditorTerrainKindState {
    uint32_t *terrain;
    uint32_t grid_origin;
    uint32_t current_grid_offset;
    uint32_t same_kind;
    uint32_t changed_kind;
} PharaohEditorTerrainKindState;

void pharaoh_editor_terrain_set_rock_kind(
    PharaohEditorTerrainKindState *state,
    int32_t x, int32_t y, uint32_t kind);
void pharaoh_editor_terrain_set_004779f0(
    PharaohEditorTerrainKindState *state, int32_t x, int32_t y);
void pharaoh_editor_terrain_set_00477ab0(
    PharaohEditorTerrainKindState *state, int32_t x, int32_t y);
void pharaoh_editor_terrain_set_00477b10(
    PharaohEditorTerrainKindState *state, int32_t x, int32_t y);
void pharaoh_editor_terrain_set_00477b70(
    PharaohEditorTerrainKindState *state, int32_t x, int32_t y);
void pharaoh_editor_terrain_set_00477c20(
    PharaohEditorTerrainKindState *state, const uint8_t *blocked,
    int32_t x, int32_t y);
void pharaoh_editor_terrain_set_00477c80(
    PharaohEditorTerrainKindState *state, int32_t x, int32_t y);
void pharaoh_editor_terrain_set_00477ce0(
    PharaohEditorTerrainKindState *state, int32_t x, int32_t y);

#endif

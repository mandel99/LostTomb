#include "pharaoh/editor/terrain_kinds.h"

enum { PHARAOH_EDITOR_GRID_WIDTH = 228 };

/* Complete C translation of Pharaoh.exe 0x00477A40. */
void pharaoh_editor_terrain_set_rock_kind(
    PharaohEditorTerrainKindState *state,
    int32_t x, int32_t y, uint32_t kind)
{
    const uint32_t offset = state->grid_origin +
        (uint32_t)y * PHARAOH_EDITOR_GRID_WIDTH + (uint32_t)x;
    const uint32_t terrain = state->terrain[offset];
    state->current_grid_offset = offset;
    if ((terrain & UINT32_C(0x20300002)) == kind) {
        state->same_kind = 1;
        return;
    }
    state->changed_kind = 1;
    state->terrain[offset] =
        ((terrain & UINT32_C(0xdfcffffd)) | kind) & UINT32_C(0xb9faf682);
}

#define DEFINE_TERRAIN_SETTER(name, bit, clear_mask) \
void name(PharaohEditorTerrainKindState *state, int32_t x, int32_t y) \
{ \
    const uint32_t offset = state->grid_origin + \
        (uint32_t)y * PHARAOH_EDITOR_GRID_WIDTH + (uint32_t)x; \
    state->current_grid_offset = offset; \
    if ((state->terrain[offset] & (bit)) != 0) { state->same_kind = 1; return; } \
    state->changed_kind = 1; \
    state->terrain[offset] = (state->terrain[offset] & (clear_mask)) | (bit); \
}

/* Complete C translations of Pharaoh.exe 0x004779F0, 0x00477AB0,
 * 0x00477B10, 0x00477B70, 0x00477C80 and 0x00477CE0. */
DEFINE_TERRAIN_SETTER(pharaoh_editor_terrain_set_004779f0,
    UINT32_C(1), UINT32_C(0x99caf681))
DEFINE_TERRAIN_SETTER(pharaoh_editor_terrain_set_00477ab0,
    UINT32_C(0x2000000), UINT32_C(0x9bcbf680))
DEFINE_TERRAIN_SETTER(pharaoh_editor_terrain_set_00477b10,
    UINT32_C(0x800), UINT32_C(0x99cafe80))
DEFINE_TERRAIN_SETTER(pharaoh_editor_terrain_set_00477b70,
    UINT32_C(0x40000), UINT32_C(0x99cef680))
DEFINE_TERRAIN_SETTER(pharaoh_editor_terrain_set_00477c80,
    UINT32_C(0x40), UINT32_C(0xd9cbf7c0))
DEFINE_TERRAIN_SETTER(pharaoh_editor_terrain_set_00477ce0,
    UINT32_C(0x100), UINT32_C(0xd9cbf7c0))

/* Complete C translation of Pharaoh.exe 0x00477C20. */
void pharaoh_editor_terrain_set_00477c20(
    PharaohEditorTerrainKindState *state, const uint8_t *blocked,
    int32_t x, int32_t y)
{
    const uint32_t offset = state->grid_origin +
        (uint32_t)y * PHARAOH_EDITOR_GRID_WIDTH + (uint32_t)x;
    state->current_grid_offset = offset;
    if (blocked[offset] != 0) return;
    if ((state->terrain[offset] & 4u) != 0) {
        state->same_kind = 1;
        return;
    }
    state->changed_kind = 1;
    state->terrain[offset] = (state->terrain[offset] & UINT32_C(0x9dcaf784)) | 4u;
}

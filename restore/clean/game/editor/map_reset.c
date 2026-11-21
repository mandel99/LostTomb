#include "pharaoh/editor/map_reset.h"

#include <string.h>

enum {
    EDITOR_RECORD_BYTES = 0x2260,
    DEFAULT_MAP_IMAGE = 6,
    DEFAULT_CAMERA_X = 109,
    DEFAULT_CAMERA_Y = 218
};

void pharaoh_editor_record_pool_clear(void *records, int32_t *count)
{
    memset(records, 0, EDITOR_RECORD_BYTES);
    *count = 0;
}

void pharaoh_editor_map_fill_default(PharaohEditorMapGrid *grid)
{
    if (grid->height <= 0) return;
    for (int32_t row = 0; row < grid->height; ++row) {
        const int32_t left = grid->row_left[row];
        const int32_t right = grid->row_right[row];
        if (left < right) {
            int32_t *image = grid->images + grid->row_offsets[row];
            uint32_t count = (uint32_t)right - (uint32_t)left;
            do {
                *image++ = DEFAULT_MAP_IMAGE;
            } while (--count != 0u);
        }
    }
}

void pharaoh_editor_map_reset(PharaohEditorMapResetState *state,
    int32_t size_index)
{
    const PharaohEditorMapSize size = state->sizes[(uint32_t)size_index];
    state->dimensions[0] = size.width;
    state->dimensions[1] = size.height;
    state->dimensions[2] = size.grid_width;
    state->dimensions[3] = size.grid_height;

    for (int32_t i = 0; i < PHARAOH_EDITOR_RESET_WORD_SLOTS; ++i) {
        state->word_slots[i] = -1;
    }
    for (int32_t i = 0; i < PHARAOH_EDITOR_RESET_WORD_PAIRS; ++i) {
        state->paired_words_low[i] = -1;
        state->paired_words_high[i] = -1;
    }
    for (int32_t i = 0; i < PHARAOH_EDITOR_RESET_DWORD_SLOTS; ++i) {
        state->dword_slots[i] = -1;
    }

    state->grid.height = size.height;
    state->prepare_rows(state->user_data);
    state->prepare_offsets(state->user_data);
    pharaoh_editor_map_fill_default(&state->grid);
    state->setup_map(state->user_data, state->orientation);
    state->connect_map(state->user_data, 0, 0,
        (int32_t)((uint32_t)size.width - 1u),
        (int32_t)((uint32_t)size.height - 1u), 0, 0);
    state->refresh_terrain(state->user_data);
    state->refresh_regions(state->user_data);
    state->refresh_editor(state->user_data);
    state->refresh_images(state->user_data);
    state->refresh_edges(state->user_data);
    state->refresh_final(state->user_data);
    state->camera_x = DEFAULT_CAMERA_X;
    state->camera_y = DEFAULT_CAMERA_Y;
    state->map_view_orientation = 0;
}

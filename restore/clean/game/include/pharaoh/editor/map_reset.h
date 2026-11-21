#ifndef PHARAOH_EDITOR_MAP_RESET_H
#define PHARAOH_EDITOR_MAP_RESET_H

#include <stdint.h>

enum {
    PHARAOH_EDITOR_RESET_WORD_SLOTS = 10,
    PHARAOH_EDITOR_RESET_WORD_PAIRS = 16,
    PHARAOH_EDITOR_RESET_DWORD_SLOTS = 26
};

typedef struct PharaohEditorMapSize {
    int32_t width;
    int32_t height;
    int32_t grid_width;
    int32_t grid_height;
} PharaohEditorMapSize;

typedef struct PharaohEditorMapGrid {
    int32_t height;
    const int32_t *row_left;
    const int32_t *row_right;
    const int32_t *row_offsets;
    int32_t *images;
} PharaohEditorMapGrid;

typedef struct PharaohEditorMapResetState {
    const PharaohEditorMapSize *sizes;
    int32_t dimensions[4];
    int16_t word_slots[PHARAOH_EDITOR_RESET_WORD_SLOTS];
    int16_t paired_words_low[PHARAOH_EDITOR_RESET_WORD_PAIRS];
    int16_t paired_words_high[PHARAOH_EDITOR_RESET_WORD_PAIRS];
    int32_t dword_slots[PHARAOH_EDITOR_RESET_DWORD_SLOTS];
    PharaohEditorMapGrid grid;
    int32_t orientation;
    int32_t camera_x;
    int32_t camera_y;
    int32_t map_view_orientation;
    void (*prepare_rows)(void *user_data);
    void (*prepare_offsets)(void *user_data);
    void (*setup_map)(void *user_data, int32_t orientation);
    void (*connect_map)(void *user_data, int32_t left, int32_t top,
        int32_t right, int32_t bottom, int32_t first, int32_t second);
    void (*refresh_terrain)(void *user_data);
    void (*refresh_regions)(void *user_data);
    void (*refresh_editor)(void *user_data);
    void (*refresh_images)(void *user_data);
    void (*refresh_edges)(void *user_data);
    void (*refresh_final)(void *user_data);
    void *user_data;
} PharaohEditorMapResetState;

void pharaoh_editor_record_pool_clear(void *records, int32_t *count);
void pharaoh_editor_map_fill_default(PharaohEditorMapGrid *grid);
void pharaoh_editor_map_reset(PharaohEditorMapResetState *state,
    int32_t size_index);

#endif

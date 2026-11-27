#ifndef PHARAOH_UI_STORAGE_ORDER_FOREGROUND_H
#define PHARAOH_UI_STORAGE_ORDER_FOREGROUND_H

#include <stdint.h>

typedef struct PharaohStorageOrderForegroundState {
    int32_t building_info_x;
    int32_t building_info_y;
    int32_t building_info_height;
    int32_t info_width_blocks;
    int32_t focus_button_id;
    int32_t normal_font;
} PharaohStorageOrderForegroundState;

typedef struct PharaohStorageOrderForegroundOperations {
    void *user_data;
    void (*draw_button_border)(void *user_data, int32_t style,
        int32_t x, int32_t y, int32_t width, int32_t height,
        int32_t focused, int32_t origin_x, int32_t origin_y);
    void (*draw_centered_language)(void *user_data, int32_t group,
        int32_t entry, int32_t x, int32_t y, int32_t width,
        int32_t font, int32_t color, int32_t origin_x, int32_t origin_y);
} PharaohStorageOrderForegroundOperations;

void pharaoh_building_info_draw_bazaar_foreground(
    const PharaohStorageOrderForegroundState *state,
    const PharaohStorageOrderForegroundOperations *operations);
void pharaoh_building_info_draw_granary_foreground(
    const PharaohStorageOrderForegroundState *state,
    const PharaohStorageOrderForegroundOperations *operations);
void pharaoh_building_info_draw_storage_yard_foreground(
    const PharaohStorageOrderForegroundState *state,
    const PharaohStorageOrderForegroundOperations *operations);

#endif

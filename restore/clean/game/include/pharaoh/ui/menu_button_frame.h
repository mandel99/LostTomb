#ifndef PHARAOH_UI_MENU_BUTTON_FRAME_H
#define PHARAOH_UI_MENU_BUTTON_FRAME_H

#include <stdint.h>

typedef struct PharaohMenuButtonFrameOperations {
    void *user_data;
    void (*draw_sprite)(void *user_data, int32_t image,
        int32_t x, int32_t y);
} PharaohMenuButtonFrameOperations;

/* Native 0x00536C30: tiled 16px button frame, selected art at image + 8. */
void pharaoh_menu_button_frame_draw(
    const PharaohMenuButtonFrameOperations *operations, int32_t base_image,
    int32_t x, int32_t y, int32_t width, int32_t height, int32_t selected);

#endif

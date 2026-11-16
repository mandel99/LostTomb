#ifndef PHARAOH_UI_SIDEBAR_LAYOUT_H
#define PHARAOH_UI_SIDEBAR_LAYOUT_H

#include <stdint.h>

typedef struct PharaohSidebarClipPositions {
    uint32_t selector_other_right_edge;
    uint32_t selector_one_right_edge;
} PharaohSidebarClipPositions;

void pharaoh_sidebar_init_positions(
    uint32_t logical_screen_width,
    PharaohSidebarClipPositions *positions);

#endif

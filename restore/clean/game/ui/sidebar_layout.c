#include "pharaoh/ui/sidebar_layout.h"

enum {
    PHARAOH_LOGICAL_WIDTH_640 = 640,
    PHARAOH_LOGICAL_WIDTH_800 = 800,
    PHARAOH_LOGICAL_WIDTH_1024 = 1024,
    PHARAOH_SELECTOR_OTHER_RIGHT_EDGE_640 = 478,
    PHARAOH_SELECTOR_ONE_RIGHT_EDGE_640 = 598,
    PHARAOH_SELECTOR_OTHER_RIGHT_EDGE_800 = 598,
    PHARAOH_SELECTOR_ONE_RIGHT_EDGE_800 = 718,
    PHARAOH_SELECTOR_OTHER_RIGHT_EDGE_1024 = 838,
    PHARAOH_SELECTOR_ONE_RIGHT_EDGE_1024 = 958
};

void pharaoh_sidebar_init_positions(
    uint32_t logical_screen_width,
    PharaohSidebarClipPositions *positions)
{
    if (logical_screen_width == PHARAOH_LOGICAL_WIDTH_640) {
        positions->selector_other_right_edge =
            PHARAOH_SELECTOR_OTHER_RIGHT_EDGE_640;
        positions->selector_one_right_edge =
            PHARAOH_SELECTOR_ONE_RIGHT_EDGE_640;
        return;
    }

    if (logical_screen_width == PHARAOH_LOGICAL_WIDTH_800) {
        positions->selector_other_right_edge =
            PHARAOH_SELECTOR_OTHER_RIGHT_EDGE_800;
        positions->selector_one_right_edge =
            PHARAOH_SELECTOR_ONE_RIGHT_EDGE_800;
        return;
    }

    if (logical_screen_width == PHARAOH_LOGICAL_WIDTH_1024) {
        positions->selector_other_right_edge =
            PHARAOH_SELECTOR_OTHER_RIGHT_EDGE_1024;
        positions->selector_one_right_edge =
            PHARAOH_SELECTOR_ONE_RIGHT_EDGE_1024;
    }
}

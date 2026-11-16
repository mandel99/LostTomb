#include "pharaoh/runtime/window_geometry.h"
#include <stdio.h>
#include <stdlib.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d: %s\n", __LINE__, #x); exit(1); } } while (0)

int main(void)
{
    static const int32_t modes[] = {1, 2, 3, 99};
    static const int32_t widths[] = {640, 800, 1024, 640};
    static const int32_t heights[] = {480, 600, 768, 480};
    for (unsigned i = 0; i < sizeof(modes) / sizeof(modes[0]); ++i) {
        PharaohInitialWindowGeometry g;
        pharaoh_initial_window_geometry(modes[i], &g);
        CHECK(g.display_mode == modes[i]);
        CHECK(g.window_width == widths[i] && g.window_height == heights[i]);
        CHECK(g.viewport_width == widths[i] && g.viewport_height == heights[i]);
        CHECK(g.ui_origin_x == (widths[i] - 640) / 2);
        CHECK(g.ui_origin_y == (heights[i] - 480) / 2);
        CHECK(g.rgb565_pitch_bytes == widths[i] * 2);
        CHECK(g.framebuffer_bytes == (uint32_t)(widths[i] * heights[i] * 2));
        CHECK(g.hit_width == widths[i] && g.hit_height == heights[i]);
        CHECK(g.display_mode_status == 1);
    }
    puts("native startup geometry: 4 display-mode cases passed");
    return 0;
}

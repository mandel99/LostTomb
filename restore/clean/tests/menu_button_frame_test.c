#include "pharaoh/ui/menu_button_frame.h"

typedef struct DrawLog {
    int32_t count;
    int32_t images[16];
    int32_t x[16];
    int32_t y[16];
} DrawLog;

#define CHECK(condition) do { if (!(condition)) return __LINE__; } while (0)

static void record_sprite(void *user_data, int32_t image, int32_t x, int32_t y)
{
    DrawLog *const log = user_data;
    if (log->count >= 16) return;
    log->images[log->count] = image;
    log->x[log->count] = x;
    log->y[log->count] = y;
    ++log->count;
}

int main(void)
{
    DrawLog log = { 0 };
    const PharaohMenuButtonFrameOperations operations = {
        &log, record_sprite
    };

    pharaoh_menu_button_frame_draw(&operations, 100, 10, 20, 25, 25, 0);
    CHECK(log.count == 4);
    CHECK(log.images[0] == 100 && log.x[0] == 10 && log.y[0] == 20);
    CHECK(log.images[1] == 102 && log.x[1] == 19 && log.y[1] == 20);
    CHECK(log.images[2] == 106 && log.x[2] == 10 && log.y[2] == 29);
    CHECK(log.images[3] == 104 && log.x[3] == 19 && log.y[3] == 29);

    log.count = 0;
    pharaoh_menu_button_frame_draw(&operations, 100, 0, 0, 40, 40, 1);
    CHECK(log.count == 8);
    CHECK(log.images[0] == 108 && log.images[1] == 109 &&
        log.images[2] == 110);
    CHECK(log.images[3] == 115 && log.images[4] == 111);
    CHECK(log.images[5] == 114 && log.images[6] == 113 &&
        log.images[7] == 112);
    return 0;
}

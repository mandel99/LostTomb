#include "pharaoh/ui/message_box.h"

#include <stdio.h>
#include <string.h>

#define CHECK(x) do { if (!(x)) { fprintf(stderr, "line %d: %s\n", \
    __LINE__, #x); return 1; } } while (0)

typedef struct TestDraw {
    int32_t panel_x, panel_y, panel_w, panel_h;
    int32_t inner_calls, inner_w, centered_calls, label_calls, button_calls;
    int32_t labels[9];
    uint32_t colors[9];
} TestDraw;

typedef struct TestModal {
    uint8_t *window;
    int32_t frame_calls, close_calls;
} TestModal;

static int32_t measure(void *data, const char *text, int32_t font)
{
    (void)data;
    return (int32_t)strlen(text) * font;
}

static void panel(void *data, int32_t x, int32_t y, int32_t w, int32_t h)
{
    TestDraw *const draw = data;
    draw->panel_x = x; draw->panel_y = y; draw->panel_w = w; draw->panel_h = h;
}

static void inner(void *data, int32_t x, int32_t y, int32_t w, int32_t h)
{
    TestDraw *const draw = data;
    ++draw->inner_calls; draw->inner_w = w; (void)x; (void)y; (void)h;
}

static void centered(void *data, const char *text, int32_t x, int32_t y,
    int32_t width, int32_t font)
{
    TestDraw *const draw = data;
    ++draw->centered_calls;
    (void)text; (void)x; (void)y; (void)width; (void)font;
}

static void label(void *data, int32_t group, int32_t entry, int32_t x,
    int32_t y, int32_t width, int32_t font)
{
    TestDraw *const draw = data;
    draw->labels[draw->label_calls++] = entry;
    (void)group; (void)x; (void)y; (void)width; (void)font;
}

static void button(void *data, int32_t x, int32_t y, int32_t width,
    int32_t height, uint32_t color)
{
    TestDraw *const draw = data;
    draw->colors[draw->button_calls++] = color;
    (void)x; (void)y; (void)width; (void)height;
}

static int32_t no_message(void *data) { (void)data; return 0; }
static int32_t dispatch_message(void *data) { (void)data; return 1; }
static void wait_message(void *data) { (void)data; }
static int32_t modal_frame(void *data, const char *title, const char *body,
    uint32_t type)
{
    TestModal *const modal = data;
    CHECK(*modal->window == UINT8_C(0x54));
    (void)title; (void)body; (void)type;
    return ++modal->frame_calls == 2 ? 7 : -1;
}
static void post_close(void *data) { ++((TestModal *)data)->close_calls; }

int main(void)
{
    TestDraw draw = {0};
    PharaohMessageBoxState state = {0};
    const PharaohMessageBoxOperations ops = {
        &draw, measure, panel, inner, centered, label, button
    };

    state.screen_width = 640; state.screen_height = 480;
    state.title_font = 2; state.body_font = 1; state.title_draw_font = 3;
    state.focused_button = 2;
    CHECK(pharaoh_message_box_draw(&state, &ops, "Title", "Body", 4) == -1);
    CHECK(state.button_count == 2);
    CHECK(state.buttons[0].result == 6 && state.buttons[1].result == 7);
    CHECK(draw.panel_x == 305 && draw.panel_y == 190);
    CHECK(draw.panel_w == 3 && draw.panel_h == 7);
    CHECK(draw.inner_calls == 1 && draw.inner_w == 2 &&
        draw.centered_calls == 2);
    CHECK(draw.label_calls == 2 && draw.labels[0] == 1 && draw.labels[1] == 0);
    CHECK(draw.button_calls == 2 && draw.colors[0] == 0 &&
        draw.colors[1] == UINT32_C(0xf800));
    CHECK(state.buttons[0].left == 310 && state.buttons[0].top == 260);
    CHECK(state.buttons[1].right == 330 && state.buttons[1].bottom == 285);
    pharaoh_message_box_select(&state, 0);
    CHECK(pharaoh_message_box_draw(&state, &ops, "Title", "Body", 4) == 6);
    pharaoh_message_box_select(&state, 1);
    CHECK(pharaoh_message_box_draw(&state, &ops, "Title", "Body", 4) == 7);

    {
        uint8_t window = 0, redraw = 0, close_pending = 1;
        uint32_t unavailable = 0, ready = 1;
        TestModal modal = {&window, 0, 0};
        PharaohMessageBoxModalState modal_state = {
            &state, &window, &redraw, &close_pending, &unavailable, &ready
        };
        const PharaohMessageBoxModalOperations modal_ops = {
            &modal, no_message, dispatch_message, wait_message,
            modal_frame, post_close
        };
        CHECK(pharaoh_message_box_show(&modal_state, &modal_ops,
            "Title", "Body", 4) == 7);
        CHECK(window == 0 && redraw == 1 && close_pending == 0);
        CHECK(modal.frame_calls == 2 && modal.close_calls == 1);
    }
    return 0;
}

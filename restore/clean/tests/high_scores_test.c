#include "pharaoh/ui/high_scores.h"

#include <stdio.h>
#include <string.h>

#define CHECK(condition) do { if (!(condition)) { \
    fprintf(stderr, "check failed at line %d: %s\n", __LINE__, #condition); \
    return 1; \
} } while (0)

typedef struct TestContext {
    PharaohHighScoreRecord source[PHARAOH_HIGH_SCORE_CAPACITY];
    int32_t read_result;
    size_t read_size;
    int32_t panel_calls;
    int32_t inner_panel_calls;
    int32_t centered_calls;
    int32_t language_calls;
    int32_t text_calls;
    int32_t number_calls;
    int32_t sprite_calls;
    int32_t last_group;
    int32_t last_entry;
    int32_t last_image;
    int32_t last_x;
    int32_t last_y;
} TestContext;

static int32_t read_records(void *user_data, const char *path,
    void *destination, size_t byte_count)
{
    TestContext *const test = user_data;
    CHECK(strcmp(path, "save\\highscore.jas") == 0);
    test->read_size = byte_count;
    memcpy(destination, test->source, byte_count);
    return test->read_result;
}

static void draw_panel(void *user_data, int32_t x, int32_t y,
    int32_t width, int32_t height)
{
    TestContext *const test = user_data;
    ++test->panel_calls;
    test->last_x = x;
    test->last_y = y;
    (void)width;
    (void)height;
}

static void draw_inner_panel(void *user_data, int32_t x, int32_t y,
    int32_t width, int32_t height)
{
    TestContext *const test = user_data;
    ++test->inner_panel_calls;
    test->last_x = x;
    test->last_y = y;
    (void)width;
    (void)height;
}

static void draw_centered_language(void *user_data, int32_t group,
    int32_t entry, int32_t x, int32_t y, int32_t width,
    int32_t font, int32_t color)
{
    TestContext *const test = user_data;
    ++test->centered_calls;
    test->last_group = group;
    test->last_entry = entry;
    test->last_x = x;
    test->last_y = y;
    (void)width;
    (void)font;
    (void)color;
}

static int32_t draw_language(void *user_data, int32_t group, int32_t entry,
    int32_t x, int32_t y, int32_t font, int32_t color)
{
    TestContext *const test = user_data;
    ++test->language_calls;
    test->last_group = group;
    test->last_entry = entry;
    test->last_x = x;
    test->last_y = y;
    (void)font;
    (void)color;
    return x + 17;
}

static void draw_text(void *user_data, const char *text, int32_t x,
    int32_t y, int32_t font, int32_t color)
{
    TestContext *const test = user_data;
    ++test->text_calls;
    test->last_x = x;
    test->last_y = y;
    (void)text;
    (void)font;
    (void)color;
}

static void draw_number(void *user_data, int32_t value, int32_t width,
    int32_t x, int32_t y, int32_t font, int32_t color)
{
    TestContext *const test = user_data;
    ++test->number_calls;
    test->last_x = x;
    test->last_y = y;
    (void)value;
    (void)width;
    (void)font;
    (void)color;
}

static void draw_sprite(void *user_data, int32_t image, int32_t x, int32_t y)
{
    TestContext *const test = user_data;
    ++test->sprite_calls;
    test->last_image = image;
    test->last_x = x;
    test->last_y = y;
}

static PharaohHighScoresOperations operations(TestContext *test)
{
    PharaohHighScoresOperations result = {0};
    result.user_data = test;
    result.read_records = read_records;
    result.draw_panel = draw_panel;
    result.draw_inner_panel = draw_inner_panel;
    result.draw_centered_language = draw_centered_language;
    result.draw_language = draw_language;
    result.draw_text = draw_text;
    result.draw_number = draw_number;
    result.draw_sprite = draw_sprite;
    return result;
}

static int test_load(void)
{
    TestContext test = {0};
    PharaohHighScoresState state = {0};
    const PharaohHighScoresOperations ops = operations(&test);

    state.record_count = 4;
    test.read_result = 1;
    test.source[0].valid = 1;
    test.source[2].valid = 3;
    CHECK(pharaoh_high_scores_load(&state, &ops,
        "save\\highscore.jas") == 1);
    CHECK(test.read_size == 0x1db0u);
    CHECK(state.valid_count == 2);
    test.read_result = 0;
    CHECK(pharaoh_high_scores_load(&state, &ops,
        "save\\highscore.jas") == 0);
    CHECK(state.valid_count == 0);
    return 0;
}

static int test_input(void)
{
    PharaohHighScoresState state = {0};
    PharaohHighScoresInput input = {0};
    uint8_t redraw = 0;
    uint8_t window = 0x53;

    state.valid_count = 25;
    state.redraw_request = &redraw;
    state.current_window_id = &window;
    pharaoh_high_scores_scroll_button(&state, 0);
    CHECK(state.first_visible == 0 && redraw == 0);
    pharaoh_high_scores_scroll_button(&state, 1);
    CHECK(state.first_visible == 1 && redraw == 2);
    state.first_visible = 5;
    redraw = 0;
    pharaoh_high_scores_scroll_button(&state, 1);
    CHECK(state.first_visible == 5 && redraw == 0);
    pharaoh_high_scores_scroll_button(&state, 2);
    CHECK(state.first_visible == 5 && redraw == 0);

    input.cursor_x = 600;
    input.cursor_y = 401;
    input.left_down = 1;
    CHECK(pharaoh_high_scores_drag_scrollbar(&state, &input) == 1);
    CHECK(state.first_visible == 5 && redraw == 2);
    input.cursor_x = 601;
    CHECK(pharaoh_high_scores_drag_scrollbar(&state, &input) == 0);

    input.left_down = 0;
    input.right_released = 1;
    CHECK(pharaoh_high_scores_handle_input(&state, &input) == 1);
    CHECK(window == 0 && redraw == 1);
    return 0;
}

static int test_draw(void)
{
    TestContext test = {0};
    PharaohHighScoresState state = {0};
    const PharaohHighScoresOperations ops = operations(&test);
    uint8_t redraw = 0;
    char missions[PHARAOH_HIGH_SCORE_CAPACITY]
        [PHARAOH_HIGH_SCORE_MISSION_CAPACITY] = {{0}};

    state.record_count = 1;
    state.valid_count = 1;
    state.records[0].valid = 1;
    state.records[0].difficulty = 2;
    state.records[0].score = 1234;
    memcpy(state.records[0].family, "Family", 7);
    memcpy(missions[0], "Mission", 8);
    state.mission_names = missions;
    state.panel_image_base = 1000;
    state.redraw_request = &redraw;
    pharaoh_high_scores_draw_background(&state, &ops);
    CHECK(test.panel_calls == 1);
    CHECK(test.inner_panel_calls == 2);
    CHECK(test.centered_calls == 2);
    CHECK(test.language_calls == 6);
    CHECK(test.text_calls == 2);
    CHECK(test.number_calls == 1);
    CHECK(test.sprite_calls == 0);
    CHECK(redraw == 1);

    memset(&test, 0, sizeof(test));
    state.valid_count = 25;
    state.first_visible = 5;
    pharaoh_high_scores_draw_scroll_thumb(&state, &ops);
    CHECK(test.sprite_calls == 1);
    CHECK(test.last_image == 1039);
    CHECK(test.last_x == 584);
    CHECK(test.last_y == 387);
    return 0;
}

int main(void)
{
    CHECK(test_load() == 0);
    CHECK(test_input() == 0);
    CHECK(test_draw() == 0);
    return 0;
}

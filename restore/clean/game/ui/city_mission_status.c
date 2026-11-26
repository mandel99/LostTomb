#include "pharaoh/ui/city_mission_status.h"
#include "pharaoh/runtime/platform_measure_speed.h"

#include <string.h>

_Static_assert(offsetof(PharaohScenarioPlayerRuntime, difficulty) == 0x47a8,
    "native mission time-limit difficulty field");

static int32_t add(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a + (uint32_t)b); }
static int32_t sub(int32_t a, int32_t b)
{ return (int32_t)((uint32_t)a - (uint32_t)b); }

static int32_t player_word(const PharaohCityMissionStatusContext *context,
    uint32_t offset)
{
    int32_t value;
    const PharaohScenarioPlayerRuntime *const player =
        &context->players[*context->current_player];
    memcpy(&value, (const uint8_t *)player + offset, sizeof(value));
    return value;
}

static void label(const PharaohCityMissionStatusContext *c, int32_t font,
    int32_t group, int32_t entry, int32_t x, int32_t y, uint16_t color)
{
    pharaoh_ui_text_draw_language(c->text, c->small_font, group, entry,
        x, y, font, color, *c->origin_x, *c->origin_y);
}

static void number(const PharaohCityMissionStatusContext *c, int32_t font,
    int32_t value, const char *suffix, int32_t x, int32_t y, uint16_t color)
{
    pharaoh_ui_text_draw_number(c->text, c->small_font, value, '@', suffix,
        x, y, font, color, *c->origin_x, *c->origin_y);
}

static void text(const PharaohCityMissionStatusContext *c,
    const char *value, int32_t x, int32_t y, uint16_t color)
{
    c->text->drawing->font_id = (uint32_t)*c->small_font_id;
    pharaoh_bitmap_text_draw_native(c->small_font, c->text->drawing,
        (const uint8_t *)value, x, y, color);
}

static int32_t months_left(const PharaohCityMissionStatusContext *c,
    int32_t final_year)
{
    const int32_t year = *c->year_0078d7bc;
    const int32_t years = add(year, 1) < final_year ?
        sub(sub(final_year, year), 1) : 0;
    return (int32_t)(((uint32_t)years * 3u + 3u) * 4u -
        (uint32_t)*c->month_0078d7c0);
}

void pharaoh_city_draw_mission_status(const PharaohCityMissionStatusContext *c)
{
    PharaohBitmapTextDrawState *const drawing = c->text->drawing;
    int32_t font = *c->small_font_id;
    if (*c->tomb_placement_006de514 != 0 || player_word(c, 0x44d0) != 0) {
        const int32_t entry = *c->tomb_placement_006de514 != 0 ? 6 : 5;
        const uint16_t color = *c->pixel_mode_00e92dfc == 2 ?
            UINT16_C(0xff3a) : UINT16_C(0x7f9a);
        drawing->cursor_x = 0;
        label(c, *c->small_font_id, 6, entry, 8, 47, 0);
        label(c, font, 6, entry, 7, 46, color);
    } else if (*c->campaign_gate_00784b60 == 0) {
        if (*c->time_limit_00784c48 != 0) {
            int32_t year = *c->base_year_0078d7b8;
            const int32_t difficulty = c->players[*c->current_player].difficulty;
            if (difficulty == 0) year = add(year, 7);
            else if (difficulty == 1) year = add(year, 2);
            const int32_t months = months_left(c, year);
            pharaoh_graphics_and_rect(c->graphics, 1, 40, 230, 20, 0x1fe3);
            font = *c->small_font_id;
            for (int32_t x = 7; x < 9; ++x) {
                for (int32_t y = 45; y < 49; y += 2) {
                    drawing->cursor_x = 0;
                    label(c, font, 6, 2, x, y, 0);
                    number(c, font, months, " ", add(drawing->cursor_x, x), y, 0);
                }
            }
            drawing->cursor_x = 0;
            label(c, font, 6, 2, 7, 46, 0xf800);
            number(c, font, months, " ", add(drawing->cursor_x, 7), 46, 0xf800);
        } else if (*c->survival_limit_00784c50 != 0) {
            const int32_t months = months_left(c, *c->base_year_0078d7b8);
            pharaoh_graphics_and_rect(c->graphics, 1, 42, 200, 18, 0x1fe3);
            drawing->cursor_x = 0;
            label(c, *c->small_font_id, 6, 3, 2, 46, 0xf800);
            number(c, *c->small_font_id, months, " ",
                add(drawing->cursor_x, 2), 46, 0xf800);
        }
    }
    drawing->cursor_x = 0;
    pharaoh_actor_walking_no_operation();
    pharaoh_actor_walking_no_operation();
    if (*c->allow_all_00605a0c == 1) {
        text(c, "Allow All ", add(drawing->cursor_x, 10), 61, 0xffff);
    }
    if (*c->debug_stats_00f53f70 != 0) {
        number(c, *c->small_font_id, player_word(c, 0x10),
            " Happiness", 2, 80, 0);
        number(c, *c->small_font_id, player_word(c, 0x10),
            " Happiness", 1, 81, 0xffff);
        number(c, *c->small_font_id, player_word(c, 0x46a8),
            " Max prosperity", 2, 100, 0);
        number(c, *c->small_font_id, player_word(c, 0x46a8),
            " Max prosperity", 1, 101, 0xffff);
    }
    const int32_t saved_cursor = drawing->cursor_x;
    if (*c->event_kind_00605a00 != 0) {
        drawing->cursor_x = 0;
        label(c, *c->small_font_id, 157, *c->event_id_00605a08, 0, 121, 0xffff);
        const char *message = NULL;
        switch (*c->event_kind_00605a00) {
        case 1: message = "Major Good"; break;
        case 2: message = "Minor Good"; break;
        case 3: message = "Major Bad"; break;
        case 4: message = "Minor Bad"; break;
        default: break;
        }
        if (message != NULL) text(c, message, add(drawing->cursor_x, 1), 121, 0xffff);
        text(c, "Event", drawing->cursor_x, 101, 0xffff);
        number(c, *c->small_font_id, *c->event_level_00605a04,
            "Level", add(drawing->cursor_x, 2), 121, 0xffff);
    }
    drawing->cursor_x = saved_cursor;
    if (*c->paused_00e38e60 != 0) {
        const int32_t left = sub(pharaoh_city_view_get_viewport_width_pixels(
            c->view), 448) / 2;
        pharaoh_panel_frame_draw_native(c->panel, left, 40, 28, 3);
        pharaoh_ui_text_draw_centered_language(c->text, c->normal_font,
            13, 2, left, 58, 448, *c->normal_font_id, 0,
            *c->origin_x, *c->origin_y);
    }
}

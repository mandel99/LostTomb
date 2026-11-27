#include "pharaoh/ui/city_pending_actions.h"

#include "pharaoh/building/kingdom_access.h"
#include "pharaoh/ui/window_layout_key.h"

#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohCityProblemFormation) == 0x90 &&
    offsetof(PharaohCityProblemFormation, x) == 0x2e &&
    offsetof(PharaohCityProblemFormation, flag_42) == 0x42 &&
    offsetof(PharaohCityProblemFormation, flag_69) == 0x69 &&
    offsetof(PharaohCityProblemFormation, problem_key) == 0x88,
    "Native problem-group formation overlay");
_Static_assert(sizeof(PharaohMenuMessageRecord) == 48 &&
    offsetof(PharaohMenuMessageRecord, kind) == 8 &&
    offsetof(PharaohMenuMessageRecord, message) == 10 &&
    offsetof(PharaohMenuMessageRecord, sequence) == 12,
    "Native sorted message fields");

static int32_t signed_bits(uint32_t bits)
{ return bits <= INT32_MAX ? (int32_t)bits :
    (int32_t)((int64_t)bits - INT64_C(0x100000000)); }
static int32_t add(int32_t a, int32_t b)
{ return signed_bits((uint32_t)a + (uint32_t)b); }
static int32_t sub(int32_t a, int32_t b)
{ return signed_bits((uint32_t)a - (uint32_t)b); }

uint32_t pharaoh_city_problem_group_tile(const PharaohCityPendingMessageContext *c,
    int32_t key)
{
    for (int32_t id = 1; id < 50; ++id) {
        const PharaohCityProblemFormation *f = &c->formations[id];
        if (f->state == 1 && f->flag_42 == 0 && f->flag_69 == 0 &&
            f->problem_key == key) {
            const uint32_t tile = (uint32_t)*c->grid_origin +
                (uint32_t)(int32_t)f->x + (uint32_t)(int32_t)f->y * 228u;
            return f->x > 0 || f->y > 0 ? tile : 0;
        }
    }
    return 0;
}

void pharaoh_city_messages_sort(const PharaohCityPendingMessageContext *c)
{
    int32_t key = -1;
    if (*c->selected_message >= 0)
        key = c->messages[*c->selected_message].sequence;
    for (int32_t pass = 1000; pass != 0; --pass) {
        for (int32_t index = 0; index < 999; ++index) {
            PharaohMenuMessageRecord *a = &c->messages[index];
            PharaohMenuMessageRecord *b = &c->messages[index + 1];
            if (a->message == 0) {
                if (b->message == 0) continue;
            } else {
                if (a->sequence >= b->sequence || b->message == 0) continue;
            }
            memcpy(c->swap, a, sizeof(*a));
            memcpy(a, b, sizeof(*a));
            memcpy(b, c->swap, sizeof(*a));
        }
    }
    *c->message_count = 0;
    *c->scroll_limit = 0;
    int32_t count = 0;
    for (int32_t index = 0; index < 1000; ++index)
        if (c->messages[index].message != 0) ++count;
    *c->message_count = count;
    if (count > 10) *c->scroll_limit = count - 10;
    else *c->scroll_index = 0;
    if (key < 0) return;
    for (int32_t index = 0; index < 1000; ++index) {
        if (c->messages[index].sequence == key) {
            *c->selected_message = index;
            return;
        }
    }
}

void pharaoh_city_message_list_open(const PharaohCityPendingMessageContext *c)
{
    pharaoh_city_messages_sort(c);
    *c->current_window = 17;
    *c->redraw = 1;
}

void pharaoh_city_sidebar_messages(const PharaohCityPendingMessageContext *c)
{ pharaoh_city_message_list_open(c); }

static uint8_t problem_eligible(const PharaohCityPendingMessageContext *c,
    const PharaohMenuMessageRecord *record, int32_t *category)
{
    if (record->message == 0 || record->kind <
        signed_bits((uint32_t)*c->current_year - 1u)) return 0;
    *category = c->layouts[record->message].subtype;
    if (*category == 1) return 1;
    if (*category != 7) return 0;
    if (signed_bits(pharaoh_city_problem_group_tile(c,
        signed_bits(record->first))) > 0) return 1;
    return signed_bits(record->second) > 0;
}

void pharaoh_city_problem_locate(const PharaohCityPendingMessageContext *c)
{
    int32_t category;
    pharaoh_city_messages_sort(c);
    *c->problem_count = 0;
    for (int32_t index = 0; index < 999; ++index) {
        if (problem_eligible(c, &c->messages[index], &category))
            *c->problem_count += 1u;
    }
    if (signed_bits(*c->problem_count) <= 0) {
        *c->problem_index = 0;
        return;
    }
    if (signed_bits(*c->problem_index) >= signed_bits(*c->problem_count))
        *c->problem_index = 0;
    uint32_t seen = 0;
    for (int32_t index = 0; index < 999; ++index) {
        const PharaohMenuMessageRecord *record = &c->messages[index];
        if (!problem_eligible(c, record, &category)) continue;
        const uint32_t problem = *c->problem_index;
        ++seen;
        if (signed_bits(problem) >= signed_bits(seen)) continue;
        uint32_t tile = record->second;
        *c->problem_index = problem + 1u;
        if (category == 7) {
            tile = pharaoh_city_problem_group_tile(c, signed_bits(record->first));
            if (signed_bits(tile) <= 0) {
                tile = record->second;
                if (signed_bits(tile) <= 0) continue;
            }
        }
        pharaoh_city_view_focus_tile(c->view, signed_bits(tile),
            c->focus_x, c->focus_y);
        *c->current_window = 1;
        *c->redraw = 1;
        return;
    }
}

void pharaoh_city_sidebar_problem(const PharaohCityPendingMessageContext *c)
{
    if (*c->button_parameter == 0) return;
    const uint32_t now = c->clock->time_get_time(c->clock->user_data);
    if (signed_bits(now - *c->last_problem_clock) > 3000)
        *c->problem_index = 0;
    *c->last_problem_clock = now;
    pharaoh_city_problem_locate(c);
    *c->redraw = 1;
}

void pharaoh_city_label_draw_animated_ui(const PharaohCityBuildPopupDrawContext *c,
    int32_t id, int32_t x, int32_t y, int32_t width_blocks,
    int32_t force, int32_t type, int32_t origin_x, int32_t origin_y)
{
    const PharaohUiLayoutContext *layout = c->text->layout;
    if (layout->state->initialized == 0) pharaoh_ui_layout_initialize(layout);
    const int32_t key_x = sub(x, origin_x), key_y = sub(y, origin_y);
    const int32_t key = pharaoh_ui_layout_window_key(*c->window_id,
        *c->window_seven_layout_base);
    PharaohUiLayoutNode *node = pharaoh_ui_layout_find(layout->state->entries,
        3, key, key_x, key_y);
    int32_t base_x = key_x, base_y = key_y;
    if (node != NULL) {
        width_blocks = node->values_08[2];
        base_x = node->values_08[0];
        base_y = node->values_08[1];
    }
    const int32_t screen_x = add(origin_x, base_x);
    const int32_t screen_y = add(origin_y, base_y);
    const int32_t width = signed_bits((uint32_t)width_blocks * 16u);
    PharaohLayoutHighlightState *highlight = c->text->highlight;
    if (highlight->edit_mode == 3 && screen_x < highlight->mouse_x &&
        highlight->mouse_x < add(screen_x, width) &&
        screen_y < highlight->mouse_y && highlight->mouse_y < add(screen_y, 24)) {
        highlight->clear_highlight(highlight->user_data);
        highlight->marked_width = width_blocks;
        highlight->marked_height = 24;
        highlight->marked_key_x = key_x;
        highlight->marked_key_y = key_y;
        highlight->marked_x = base_x;
        highlight->marked_y = base_y;
        highlight->draw_border(highlight->user_data, 1, screen_x, screen_y,
            width, 24, UINT32_C(0xFFFF));
        highlight->draw_border(highlight->user_data, 0, screen_x, screen_y,
            width, 24, UINT32_C(0xFFFF));
    }
    pharaoh_label_draw_animated(c->labels, id, screen_x, screen_y,
        width_blocks, force, type);
}

static int32_t brush_offset(int32_t category)
{
    switch (category) {
        case 14: return -1;
        case 15: return 4;
        case 16: return 6;
        case 18: return 17;
        case 19: return 19;
        case 26: return 22;
        case 27: return 34;
        default: return INT32_MAX;
    }
}

static void popup_label(const PharaohCityBuildPopupDrawContext *c,
    int32_t x, int32_t y, int32_t width, int32_t type)
{
    pharaoh_city_label_draw_animated_ui(c, 6, x, y, width, 1, type,
        *c->origin_x, *c->origin_y);
}

static void popup_text(const PharaohCityBuildPopupDrawContext *c,
    const PharaohBitmapFont *font, int32_t font_id, uint16_t color,
    int32_t group, int32_t entry, int32_t x, int32_t y, int32_t width)
{
    pharaoh_ui_text_draw_centered_language(c->text, font, group, entry, x, y,
        width, font_id, color, *c->origin_x, *c->origin_y);
}

void pharaoh_city_build_popup_draw_foreground(
    const PharaohCityBuildPopupDrawContext *c)
{
    int32_t width = *c->collapsed == 1 ? *c->collapsed_x : *c->expanded_x;
    if (*c->menu->editor_active == 1) width = *c->expanded_x;
    int32_t item = -1;
    for (int32_t row = 0; row < *c->count; ++row) {
        item = pharaoh_city_build_menu_next_item(c->menu,
            *c->menu->selected_category, item);
        const uint8_t focused = (uint8_t)(*c->focused_button == row + 1);
        const int32_t type = focused ? 1 : 2;
        const uint16_t color = (uint16_t)(focused ?
            UINT32_C(0xFAC1) : UINT32_C(0x3EFA));
        const int32_t font_id = focused ? *c->focused_font_id : *c->normal_font_id;
        const PharaohBitmapFont *font = focused ? c->focused_font : c->normal_font;
        const int32_t group = *c->menu->selected_category;
        const int32_t y = add(*c->y_offset, row * 24);
        int32_t building = c->menu->types[group * 30 + item];
        const int32_t brush = brush_offset(group);
        if (brush != INT32_MAX || group == 17 || group == 37) {
            int32_t text, text_group = 48;
            if (brush != INT32_MAX) text = add(building, brush);
            else if (group == 17) text = add(building, building <= 8 ? 9 : 39);
            else {
                text_group = 64;
                if (row != 0) text = add(*c->climate, 102);
                else {
                    const uint8_t climate = (uint8_t)*c->climate;
                    text = climate == 2 ? 83 : climate == 1 ? 82 : 84;
                }
            }
            popup_label(c, add(width, -170), add(y, 110), 10, type);
            popup_text(c, font, font_id, color, text_group, text,
                add(width, -170), add(y, 113), 160);
        } else if (group == 28) {
            const int32_t monument = c->goal_definition_ids[building - 1];
            const PharaohScenarioPlayerRuntime *player =
                &c->menu->supply->players[*c->menu->supply->current_player];
            if (((item == 1 || item == 2) &&
                monument == c->goal_definition_ids[0] &&
                player->monument_definition_ids[0] == 0) ||
                (item == 2 && monument == c->goal_definition_ids[1] &&
                player->monument_definition_ids[1] == 0) || monument == 0)
                continue;
            popup_label(c, add(width, -298), add(y, 110), 18, type);
            popup_text(c, font, font_id, color, 198, monument,
                add(width, -266), add(y, 113), 176);
        } else {
            const PharaohUiLayoutState *layout = c->text->layout->state;
            int32_t label_width = pharaoh_ui_layout_label_width(layout->label_width);
            popup_label(c, add(sub(width, label_width), -97), add(y, 110),
                add(layout->label_columns, 7), type);
            building = c->menu->types[*c->menu->selected_category * 30 + item];
            label_width = pharaoh_ui_layout_label_width(layout->label_width);
            if (*c->menu->selected_category == 36) {
                const PharaohScenarioPlayerRuntime *player =
                    &c->menu->supply->players[*c->menu->supply->current_player];
                const int32_t complex = c->menu->monuments->demand->complex->
                    buildings[player->temple_complex_building_id].building_type;
                popup_text(c, font, font_id, color, 189,
                    add(item, (complex - 65) * 2),
                    add(sub(width, label_width), -97), add(y, 113), 176);
            } else popup_text(c, font, font_id, color, 28, building,
                add(sub(width, label_width), -89), add(y, 113), 176);
            if (building == 7) building = 90;
            if (building != 57 && c->models[building].cost != 0)
                pharaoh_ui_text_draw_number(c->text, font, c->models[building].cost,
                    '@', c->money_postfix, add(width, -82), add(y, 114), font_id,
                    0, *c->origin_x, *c->origin_y);
        }
    }
}

void pharaoh_city_build_popup_draw_background(
    const PharaohCityPendingBackgroundContext *c)
{
    pharaoh_sidebar_draw_background(c->sidebar, c->sidebar_operations);
    pharaoh_city_render_current_view(c->city);
    if (*c->redraw >= 2) {
        *c->popup->menu_animation = 0;
        pharaoh_city_build_popup_draw_foreground(c->popup);
    }
}

uint8_t pharaoh_city_pending_draw_background(
    const PharaohCityPendingBackgroundContext *c, int32_t force)
{
    if (force != 0) *c->redraw = 1;
    else if (*c->redraw == 0) return 1;
    const uint32_t window = *c->modal_active != 0 ?
        *c->modal_return_window : *c->current_window;
    if (window == 1)
        pharaoh_sidebar_draw_background(c->sidebar, c->sidebar_operations);
    else if (window == 6) pharaoh_city_build_popup_draw_background(c);
    else if (window != 2 && window != 18) return 0;
    *c->redraw = 0;
    return 1;
}

static void slide_clip(const PharaohCitySidebarSlideContext *c, int32_t right)
{
    /* These are the image/primitive views of the same native D3B558 owner. */
    c->images->clip->clip_x_max = right;
    c->graphics->clip_x1 = right;
}

static void slide_buttons(const PharaohCitySidebarSlideContext *c,
    const PharaohCityInputImageTable *table, int32_t x, int32_t count)
{
    pharaoh_image_buttons_draw(x, 30, table->records, count, c->button_draw);
}

uint8_t pharaoh_city_sidebar_slide(const PharaohCitySidebarSlideContext *c)
{
    const int32_t duration = *c->frame_duration;
    const uint32_t increment = duration <= 24 ? 1u : duration <= 60 ? 2u :
        duration <= 96 ? 3u : 4u;
    const uint32_t step = *c->step + increment;
    *c->step = step;
    if (signed_bits(step) >= 47) {
        if (*c->collapsed == 0) {
            pharaoh_city_view_set_viewport_without_sidebar(c->view);
            *c->collapsed = 1;
        } else {
            pharaoh_city_view_set_viewport_with_sidebar(c->view);
            *c->collapsed = 0;
        }
        *c->current_window = 1;
        pharaoh_city_view_check_camera_boundaries(c->view);
        return pharaoh_city_pending_draw_background(c->background, 1);
    }
    const int32_t offset = c->offsets[*c->collapsed == 1 ? 47u - step : step];
    pharaoh_city_render_current_view(c->city);
    if (*c->screen_width == 1024) slide_clip(c, 1000);
    else if (*c->screen_width == 800) slide_clip(c, 760);
    *c->ui_redraw = 1;
    (void)pharaoh_image_draw(c->images, add(*c->game_image_base, 1),
        *c->collapsed_x, 30);
    slide_buttons(c, &c->buttons->collapsed_build, *c->collapsed_x, 12);
    slide_buttons(c, &c->buttons->collapsed_expand, *c->collapsed_x, 1);
    const int32_t x = add(*c->expanded_x, offset);
    (void)pharaoh_image_draw(c->images, *c->game_image_base, x, 30);
    slide_buttons(c, &c->buttons->expanded_build, x, 16);
    slide_buttons(c, &c->buttons->expanded_extra, x, 2);
    slide_buttons(c, &c->buttons->expanded_top, x, 2);
    pharaoh_sidebar_draw_caption(c->content, 1, add(x, 11));
    pharaoh_sidebar_draw_category(c->content, 1, add(x, 11));
    const int32_t amount = add(sub(offset > 8 ? 8 : offset, offset), 145);
    pharaoh_graphics_fill_rect(c->graphics, add(x, 8), 59, amount, 111, 0);
    int32_t image;
    if (*c->screen_width == 1024) {
        (void)pharaoh_image_draw(c->images, add(*c->game_image_base, 5),
            *c->collapsed_x, 480);
        image = add(*c->game_image_base, 4);
    } else if (*c->screen_width == 800) {
        (void)pharaoh_image_draw(c->images, add(*c->game_image_base, 7),
            *c->collapsed_x, 480);
        image = add(*c->game_image_base, 6);
    } else {
        slide_clip(c, *c->screen_width);
        return 1;
    }
    (void)pharaoh_image_draw(c->images, image, x, 480);
    slide_clip(c, *c->screen_width);
    return 1;
}

void pharaoh_city_sidebar_slide_input(const PharaohCityIngameDispatchContext *c)
{
    if (*c->current_window != 2) return;
    *c->keyboard_field = 0;
    *c->image_focus = 0;
    *c->arrow_focus = 0;
    *c->menu_focus = 0;
    *c->ui_mouse_x = *c->frame->mouse_x;
    *c->ui_mouse_y = *c->frame->mouse_y;
    if (*c->mouse_drag_state == 2) {
        *c->mouse_drag_state = 0;
        *c->redraw = 1;
    }
    pharaoh_camera_scroll_apply(c->scroll,
        pharaoh_camera_scroll_direction(c->scroll_input));
}

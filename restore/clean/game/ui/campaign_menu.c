#include "pharaoh/ui/campaign_menu.h"

enum { CAMPAIGN_MENU_WINDOW = 0x17 };

int32_t pharaoh_campaign_menu_period_valid(int32_t period)
{
    return period >= 0 && period < 9;
}

uint32_t pharaoh_campaign_menu_period_lock_mask(uint32_t period)
{
    return period & UINT32_C(0xffffff00);
}

int32_t pharaoh_campaign_menu_image_button_count(
    uint32_t campaign_mode, uint32_t selected_period,
    uint32_t latest_period)
{
    if (campaign_mode == 0) return selected_period == latest_period;
    return ((int8_t)pharaoh_campaign_menu_period_lock_mask(selected_period)
        != 0) ? 0 : 2;
}

/* Typed logical callback 0x0041BB10; native entry is stored in the button table. */
void pharaoh_campaign_menu_select_period(
    PharaohCampaignMenuState *state, uint32_t period)
{
    *state->redraw_requested = 1;
    *state->selected_period = period;
}

/* Typed logical callback 0x0041C310; native entry is stored in the button table. */
int32_t pharaoh_campaign_menu_open_individual_missions(
    PharaohCampaignMenuState *state,
    const PharaohCampaignMenuOperations *operations)
{
    if (*state->current_window_id != CAMPAIGN_MENU_WINDOW ||
        *state->campaign_mode != 1) return 0;
    *state->map_changed = 1;
    *state->returns_to_front_menu = 0;
    *state->selection_flag = 0;
    *state->map_mode = 2;
    operations->show_individual_missions(operations->user_data);
    *state->redraw_requested = 1;
    return 1;
}

/* Typed logical callback 0x0041C350; native entry is stored in the button table. */
int32_t pharaoh_campaign_menu_open_campaigns(
    PharaohCampaignMenuState *state)
{
    if (*state->current_window_id == CAMPAIGN_MENU_WINDOW) return 0;
    *state->current_window_id = CAMPAIGN_MENU_WINDOW;
    *state->campaign_mode = 1;
    *state->redraw_requested = 1;
    return 1;
}

int32_t pharaoh_campaign_menu_period_video(uint32_t period, int32_t cpu,
    int32_t screen_width, int32_t screen_height, PharaohCampaignPeriodVideo *video)
{
    static const char *const movies[5][2] = {
        {"Binks\\low\\pre_dynastic_small.bik",
         "Binks\\high\\pre_dynastic_big.bik"},
        {"Binks\\low\\archaic_small.bik",
         "Binks\\high\\archaic_big.bik"},
        {"Binks\\low\\old_kingdom_small.bik",
         "Binks\\high\\old_kingdom_big.bik"},
        {"Binks\\low\\middle_kingdom_small.bik",
         "Binks\\high\\middle_kingdom_big.bik"},
        {"Binks\\low\\new_king_small.bik",
         "Binks\\high\\new_king_big.bik"}
    };
    if (period >= 5) return 0;
    video->path = movies[period][cpu >= 233];
    video->x = (screen_width - 480) / 2;
    video->y = (screen_height - 286) / 2;
    return 1;
}

/* Typed logical callback 0x0041BB70; native entry is stored in the button table. */
int32_t pharaoh_campaign_menu_start_selected(
    PharaohCampaignMenuState *state,
    const PharaohCampaignMenuOperations *operations)
{
    const uint32_t period = *state->selected_period;
    const uint32_t mode = *state->campaign_mode;

    if (mode == 0 && period != *state->latest_period) return 0;

    *state->mission_id = 0;
    *state->returns_to_front_menu = 0;
    *state->selection_flag = 0;
    *state->scenario_mode = 0;
    *state->campaign_started = 1;
    *state->redraw_requested = 1;
    *state->selection_kind = (mode != 0) + 1;

    if (period < 5) {
        operations->play_period_intro(operations->user_data, period);
    }
    operations->start_campaign(operations->user_data, (int8_t)period);
    if ((uint8_t)*state->mission_id == UINT8_C(0x7f)) {
        *state->current_window_id = UINT8_C(0x34);
    } else {
        operations->begin_briefing(operations->user_data);
    }
    return 1;
}

/* Complete C translation of Pharaoh.exe 0x00418140. */
int32_t pharaoh_campaign_briefing_message_id(
    const PharaohCampaignIndex *index, const PharaohSettingsFile *settings,
    uint32_t campaign_mode)
{
    const int32_t link = settings->campaign_link;
    int32_t message;

    if (campaign_mode == 0 && link >= 0 && link < index->count) {
        message = index->records[link].link.intro_id;
        if (settings->classical_names == 0) return message;
        if (message > 438) return message + 15;
    } else {
        if (settings->mission_id == 0x7f) return 200;
        message = settings->mission_id;
    }
    return message + 200;
}

/* Complete C translation of Pharaoh.exe 0x004181E0. */
void pharaoh_campaign_begin_briefing(
    const PharaohCampaignBriefingContext *context)
{
    pharaoh_timed_window_transition_start(&context->transition,
        0, UINT8_C(0x35), 1000);
    context->briefing->started = 0;
    context->briefing->scroll_position = 0;
    context->briefing->scroll_drag = 0;
    context->briefing->message_id = pharaoh_campaign_briefing_message_id(
        context->index, context->settings, context->campaign_mode);
}

void pharaoh_campaign_menu_draw_background(PharaohImageDrawContext *drawing,
    int32_t image, int32_t origin_x, int32_t origin_y,
    int32_t *loading_progress, uint8_t *menu_redraw)
{
    (void)pharaoh_image_draw(drawing, image, origin_x - 0xc0, origin_y - 0x90);
    *loading_progress = 100;
    *menu_redraw = 1;
}

void pharaoh_campaign_menu_draw(const PharaohCampaignMenuDrawContext *context)
{
    const PharaohCampaignMenuState *const menu = context->menu_state;
    const uint32_t mode = *menu->campaign_mode;
    const uint32_t selected_period = *menu->selected_period;
    const uint32_t latest_period = *menu->latest_period;
    const int32_t origin_x = *context->screen_origin_x_00e38da8;
    const int32_t origin_y = *context->screen_origin_y_00e38da4;
    const PharaohBitmapTextCenteredNativeContext title_draw = {
        context->font_00f56fb4, context->text_draw_state,
        context->glyph_map, context->center_offset_00e20770
    };
    const PharaohBitmapTextCenteredNativeContext period_title_draw = {
        context->font_00f56fbc, context->text_draw_state,
        context->glyph_map, context->center_offset_00e20770
    };
    const PharaohBitmapTextCatalogCenteredNativeContext catalog_centered = {
        context->text_selection, &period_title_draw,
        context->current_text_00e20768, context->reset_flag_00d3b504
    };
    const PharaohBitmapWrappedNativeContext wrapped_period_draw = {
        context->font_00f56f94, context->text_draw_state,
        context->wrapped_word_metrics, context->line_buffer_00e3439c,
        context->maximum_width_00e2076c, (uint32_t)*context->font_id_00f56f94
    };
    const PharaohBitmapWrappedNativeContext wrapped_footer_draw = {
        context->font_00f56f8c, context->text_draw_state,
        context->wrapped_word_metrics, context->line_buffer_00e3439c,
        context->maximum_width_00e2076c, (uint32_t)*context->font_id_00f56f8c
    };
    const PharaohBitmapTextCatalogWrappedNativeContext wrapped_period = {
        context->text_selection, &wrapped_period_draw,
        context->current_text_00e20768
    };
    const PharaohBitmapTextCatalogWrappedNativeContext wrapped_footer = {
        context->text_selection, &wrapped_footer_draw,
        context->current_text_00e20768
    };
    int32_t title_image;
    int32_t title_y;
    int32_t selected_unavailable = 0;
    char expanded_title[212];

    *context->layout_x_00d3b56c =
        (0x400 - *context->screen_width_00e38db0) / 2;
    *context->layout_width_00d3b574 = *context->screen_width_00e38db0;
    *context->layout_y_00d3b568 =
        (0x300 - *context->screen_height_00e38dac) / 2;
    *context->layout_height_00d3b570 = *context->screen_height_00e38dac;
    *context->menu_redraw_flag_00e38dc9 = 1;

    title_y = origin_y + (mode == 0 ? 0x3c : 0x38);
    if (selected_period < 5) {
        title_image = *context->title_image_base_00f55c94 +
            (int32_t)selected_period;
    } else if (selected_period == 5) {
        title_image = *context->period_image_base_00f57314 + 4;
    } else if (selected_period == 6) {
        title_image = *context->period_image_base_00f57314 + 2;
    } else if (selected_period == 7) {
        title_image = *context->period_image_base_00f57314 + 5;
    } else {
        title_image = *context->period_image_base_00f57314 + 7;
    }
    (void)pharaoh_image_draw(context->image_draw, title_image,
        origin_x + 0x4e, title_y);

    pharaoh_text_select(context->text_selection, 0x125,
        mode == 0 ? 5 : 6);
    *context->current_text_00e20768 = context->text_selection->current;
    const uint32_t saved_text_format = *context->text_format_mode_00784f0c;
    *context->text_format_mode_00784f0c = 0;
    pharaoh_event_text_expand(context->token_entries,
        context->token_operations, *context->current_text_00e20768,
        expanded_title, 200);
    *context->text_format_mode_00784f0c = saved_text_format;
    pharaoh_bitmap_text_draw_centered_native(&title_draw,
        (const uint8_t *)expanded_title, origin_x + 0x14,
        origin_y + 0x11, 600, *context->font_id_00f56fb4, 0);

    if (pharaoh_campaign_menu_period_valid((int32_t)selected_period)) {
        pharaoh_bitmap_text_draw_catalog_centered_native(&catalog_centered,
            0x126, (int32_t)(selected_period * 4u), origin_x + 0x153,
            origin_y + 0x3c, 0x11b, *context->font_id_00f56fbc, 0);

        int32_t description_entry;
        if (mode == 0) {
            description_entry = selected_period < latest_period ? 1 :
                (selected_period != latest_period) + 2;
        } else {
            description_entry =
                ((uint8_t)pharaoh_campaign_menu_period_lock_mask(
                    selected_period) != 0) + 2;
        }
        pharaoh_bitmap_text_draw_catalog_wrapped_native(&wrapped_period,
            0x126, description_entry + (int32_t)(selected_period * 4u),
            origin_x + 0x15b, origin_y + 0x74, 0x10d, 0);
    }

    for (int32_t row = 0; row < 9; ++row) {
        const PharaohCampaignMenuRenderRecord *const record =
            &context->geometry_0057be78[row];
        const int32_t row_width =
            (int32_t)record->rect.right - record->rect.left;
        const int32_t row_x = record->rect.left + origin_x;
        const int32_t row_y = record->rect.top + origin_y;
        const int32_t focused = *context->selected_row_007056c8 == row + 1;
        int32_t available;

        if (mode == 0) {
            available = (uint32_t)row != latest_period;
        } else {
            const int32_t row_period = record->period_id;
            available = pharaoh_campaign_menu_period_valid(row_period) &&
                (uint8_t)pharaoh_campaign_menu_period_lock_mask(
                    (uint32_t)row_period) != 0;
        }
        if ((uint32_t)row == selected_period) {
            selected_unavailable = !available;
        }

        const int32_t font_id = available ?
            *context->font_id_00f56fa4 : *context->font_id_00f56f8c;
        const int32_t width_blocks =
            (row_width + ((row_width >> 31) & 0x0f)) >> 4;
        pharaoh_large_label_draw_animated(context->label_renderer, 6,
            row_x, row_y, width_blocks, !available && focused);
        pharaoh_menu_draw_centered_rect(context->menu_renderer, 0x1b,
            row, &record->rect, font_id, 0, 0, 6);
    }

    if (mode == 0 || mode == 1) {
        pharaoh_bitmap_text_draw_catalog_wrapped_native(&wrapped_footer,
            0x126, 0x29, origin_x + 0x1e, origin_y + 0x10a,
            0x90, 0);
        pharaoh_bitmap_text_draw_catalog_wrapped_native(&wrapped_footer,
            0x126, 0x2a, origin_x + 0x1e, origin_y + 0x16e,
            0x90, 0);
    }

    if (mode == 1) {
        for (int32_t button = 0; button < 2; ++button) {
            const PharaohMenuRect *const rect =
                &context->geometry_0057be78[button + 9].rect;
            const int32_t selected = button == 1 ||
                *context->selected_row_007056c8 == button + 10;
            pharaoh_menu_draw_border_rect(context->menu_renderer, 0,
                rect, selected, 0, 0);
            pharaoh_menu_draw_centered_rect(context->menu_renderer, 0x126,
                button == 0 ? 0x26 : 0x27, rect,
                *context->font_id_00f56f8c, 0, 4, 7);
        }
    }

    if (selected_unavailable) {
        if (mode == 0) {
            pharaoh_bitmap_text_draw_selection_native(
                context->font_00f56f8c, context->text_draw_state,
                context->text_selection, 0x126, 0x24,
                origin_x + 0x1a4, origin_y + 0x1bd,
                *context->font_id_00f56f8c, 0);
            *context->window_redraw_request = 1;
            return;
        }
        pharaoh_bitmap_text_draw_selection_native(
            context->font_00f56f8c, context->text_draw_state,
            context->text_selection, 0x126, 0x25,
            origin_x + 0x226, origin_y + 0x1bd,
            *context->font_id_00f56f8c, 0);
        pharaoh_bitmap_text_draw_selection_native(
            context->font_00f56f8c, context->text_draw_state,
            context->text_selection, 0x2c, 0xd9,
            origin_x + 0x17c, origin_y + 0x1bd,
            *context->font_id_00f56f8c, 0);
    }

    *context->window_redraw_request = 1;
}

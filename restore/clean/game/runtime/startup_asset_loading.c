#include "pharaoh/runtime/startup_asset_loading.h"

#include "pharaoh/runtime/text_loading.h"

#include <stdbool.h>

enum {
    PHARAOH_STARTUP_CAMPAIGN_SELECTION = 16,
    PHARAOH_STARTUP_PIXEL_BYTES_PER_PIXEL = 2
};

static const char *const startup_sprite_library_names[
    PHARAOH_STARTUP_SPRITE_LIBRARY_COUNT] = {
    "Pharaoh_Unloaded", "SprMain", "Hyksos", "Pharaoh_General",
    "Pharaoh_Terrain", "SPHINX1A", "SPHINX1B", "SPHINX1C", "SPHINX1D",
    "SPHINX_EXTRA", "TEMPLE_NILE", "empire", "SprAmbient", "Pharaoh_Fonts",
    "Egyptian", "SprMain2", "expansion", "PyramidDummy", "CaesareumDummy",
    "LtHouseDummy", "LibMainDummy", "LibMainDummy", "LibMainDummy",
    "LibExtra", "MausCarryoverDummy", "TombThutDummy", "AbuSimbelDummy",
    "TombTutDummy", "TombSetiDummy", "TombRamDummy"
};

static uint32_t startup_pixel_buffer_bytes(uint32_t width, uint32_t height)
{
    const uint32_t byte_count = width * height *
        PHARAOH_STARTUP_PIXEL_BYTES_PER_PIXEL;
    const bool signed_result_is_negative = (byte_count & 0x80000000u) != 0u;
    if (signed_result_is_negative || byte_count <
            PHARAOH_STARTUP_PIXEL_BUFFER_MINIMUM_TEST) {
        return PHARAOH_STARTUP_PIXEL_BUFFER_MINIMUM_BYTES;
    }
    return byte_count;
}

void pharaoh_loading_screen_draw_stage(
    const PharaohStartupAssetLoadContext *context, int32_t stage_index)
{
    if (stage_index == 0) {
        context->operations->clear_loading_surface(
            context->operations->user_data);
    }
    context->operations->draw_sprite(context->operations->user_data,
        context->loading_stage_image_base + (uint32_t)stage_index,
        context->loading_image_x, context->loading_image_y);
    context->state->loading_progress_percent = 100;
}

void pharaoh_loading_screen_draw_background(
    const PharaohStartupAssetLoadContext *context)
{
    context->operations->draw_sprite(context->operations->user_data,
        context->loading_background_image, context->loading_image_x,
        context->loading_image_y);
    context->state->loading_progress_percent = 100;
}

static void draw_startup_text(const PharaohStartupAssetLoadContext *context,
                              int32_t entry)
{
    pharaoh_text_select(context->text_selection,
        PHARAOH_STARTUP_TEXT_GROUP, entry);
    context->operations->draw_status_text(context->operations->user_data,
        context->text_selection->current);
}

static void draw_startup_stage(const PharaohStartupAssetLoadContext *context,
                               int32_t stage_index, int32_t text_entry)
{
    pharaoh_loading_screen_draw_stage(context, stage_index);
    draw_startup_text(context, text_entry);
    context->operations->process_loading_updates(
        context->operations->user_data, 1);
    draw_startup_text(context, text_entry);
}

int32_t pharaoh_startup_asset_setup(
    const PharaohStartupAssetLoadContext *context)
{
    (void)pharaoh_load_game_data(context->game_data, "Pharaohmap.inf",
        context->state->map_info_file_buffer, 0);
    context->state->selected_campaign_id =
        PHARAOH_STARTUP_CAMPAIGN_SELECTION;
    context->state->startup_asset_setup_flag = 1;
    context->operations->create_directory(context->operations->user_data,
        "Save");
    context->operations->create_directory(context->operations->user_data,
        "Maps");

    const uint32_t pixel_buffer_bytes = startup_pixel_buffer_bytes(
        context->screen_width, context->screen_height);
    context->state->screen_pixel_buffer = (uint8_t *)
        context->operations->allocate(context->operations->user_data,
            pixel_buffer_bytes);

    (void)pharaoh_sprite_libraries_load_one(context->sprite_libraries,
        startup_sprite_library_names);
    draw_startup_stage(context, 0,
        PHARAOH_STARTUP_TEXT_FIRST_ENTRY);
    if (context->state->screen_pixel_buffer == NULL) {
        return 1;
    }

    (void)pharaoh_sprite_libraries_load_one(context->sprite_libraries,
        startup_sprite_library_names);
    draw_startup_stage(context, 1,
        PHARAOH_STARTUP_TEXT_FIRST_ENTRY + 1);

    (void)pharaoh_sprite_libraries_load_one(context->sprite_libraries,
        startup_sprite_library_names);
    draw_startup_stage(context, 2,
        PHARAOH_STARTUP_TEXT_FIRST_ENTRY + 2);

    for (uint32_t library = 0; library < 5; ++library) {
        (void)pharaoh_sprite_libraries_load_one(context->sprite_libraries,
            startup_sprite_library_names);
    }
    draw_startup_stage(context, 3,
        PHARAOH_STARTUP_TEXT_FIRST_ENTRY + 3);

    pharaoh_sprite_libraries_load_all(context->sprite_libraries,
        startup_sprite_library_names);
    pharaoh_loading_screen_draw_stage(context, 3);
    draw_startup_text(context, PHARAOH_STARTUP_TEXT_FIRST_ENTRY + 4);
    context->operations->process_loading_updates(
        context->operations->user_data, 1);
    draw_startup_text(context, PHARAOH_STARTUP_TEXT_FIRST_ENTRY + 4);
    pharaoh_loading_screen_draw_background(context);
    context->operations->process_loading_updates(
        context->operations->user_data, 1);
    context->operations->write_startup_status(context->operations->user_data,
        "OK: Setup loaded DAT files");
    return 0;
}

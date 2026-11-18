#include "pharaoh/runtime/application_window_startup.h"

#include "pharaoh/application_identity.h"
#include "pharaoh/runtime/text_loading.h"

static const char startup_debug_status[] = "Debug and status report on ";

enum {
    APPLICATION_RANDOM_SEED_1 = 0x54657687,
    APPLICATION_RANDOM_SEED_2 = 0x72641663,
    APPLICATION_RANDOM_SEED_3 = 0x34518632
};

static void reset_pre_window_state(
    PharaohApplicationWindowStartupState *state)
{
    uint32_t index;
    for (index = 0;
         index < PHARAOH_APPLICATION_WINDOW_RESET_FLAG_COUNT;
         ++index) {
        state->reset_state.pre_window_flags[index] = 0;
    }
    state->reset_state.pre_window_status = 0;
    for (index = 0;
         index < PHARAOH_APPLICATION_WINDOW_RESET_WORD_COUNT;
         ++index) {
        state->reset_state.pre_window_words[index] = 0;
    }
}

int32_t pharaoh_application_window_initialize(
    PharaohApplicationWindowStartupState *state,
    PharaohGraphicsStartupState *graphics,
    const PharaohApplicationWindowStartupRequest *request,
    const PharaohApplicationWindowStartupOperations *operations)
{
    PharaohApplicationDisplayStartup display_startup;
    PharaohApplicationWindowRequest window_request;
    PharaohVideoPlayer *video_player;

    pharaoh_application_capture_current_directory(&state->directory,
        operations->directory_operations);
    state->random_generator.seed_1 = APPLICATION_RANDOM_SEED_1;
    state->random_generator.seed_2 = APPLICATION_RANDOM_SEED_2;
    state->random_generator.seed_3 = APPLICATION_RANDOM_SEED_3;
    reset_pre_window_state(state);
    graphics->mode_status_byte = UINT8_C(1);

    display_startup.graphics = graphics;
    display_startup.graphics_operations = operations->graphics_operations;
    pharaoh_application_display_prepare(&display_startup,
        request->settings_display_mode);
    state->geometry = display_startup.geometry;

    operations->graphics_operations->report_status(
        operations->graphics_operations->user_data, startup_debug_status,
        0, 0);
    (void)operations->co_initialize(operations->user_data, NULL);
    pharaoh_text_select(request->window_title_selection, 9, 0);

    window_request.instance = request->instance;
    window_request.window_title = request->window_title_selection->current;
    window_request.window_procedure = request->window_procedure;
    if (pharaoh_application_window_create(&state->window, &window_request,
            operations->window_operations) == 0) {
        return 0;
    }

    graphics->main_window = state->window.window;
    graphics->primary_surface = NULL;
    graphics->back_buffer_surface = NULL;
    graphics->direct_draw_device = NULL;
    graphics->graphics_clipper = NULL;

    video_player = operations->allocate_video_player(operations->user_data,
        PHARAOH_APPLICATION_VIDEO_PLAYER_NATIVE_BYTES);
    if (video_player == NULL) {
        state->video_player = NULL;
    } else {
        pharaoh_video_player_initialize(video_player, state->window.window);
        state->video_player = video_player;
    }

    return pharaoh_application_display_begin_session(&display_startup);
}

#include "pharaoh/runtime/smacker_window_video_start.h"
#include "pharaoh/runtime/platform_measure_speed.h"

#include <stdint.h>
#include <string.h>

enum {
    SMACKER_WINDOW_PLAYING = 0x13,
    SMACKER_VIDEO_SYSTEM_MODE_FULLSCREEN = 2,
    SMACKER_SOUND_FLAG = 0x2000,
    SMACKER_SPECIAL_SOUND_RETURN_WINDOW_A = 0x41,
    SMACKER_SPECIAL_SOUND_RETURN_WINDOW_B = 0x3c,
    SMACKER_SURFACE_FORMAT_565 = 0x565,
    SMACKER_SURFACE_MODE_565 = 0xc0000000u,
    SMACKER_SURFACE_MODE_OTHER = 0x80000000u
};

static const uint32_t smacker_open_frame_limit = UINT32_MAX;
static const uint32_t smacker_surface_lost_hresult = UINT32_C(0x887601c2);

_Static_assert(sizeof(PharaohSmackerSurfaceDescription) == 0x6c,
    "Smacker frame lock scratch buffer is 108 bytes");

static int32_t smacker_native_integer_increment(int32_t value)
{
    const uint32_t incremented_bits = (uint32_t)value + UINT32_C(1);
    int32_t native_value;
    memcpy(&native_value, &incremented_bits, sizeof(native_value));
    return native_value;
}

int32_t pharaoh_smacker_video_draw(
    PharaohSmackerWindowVideoState *state,
    const PharaohSmackerVideoDrawOperations *operations)
{
    PharaohSmackerSurfaceDescription description = { 0 };
    PharaohSmackerLockedSurface locked_surface = { 0 };
    const int32_t uses_front_surface =
        state->graphics_refresh_state->smacker_uses_front_surface != 0;
    const int32_t surface = uses_front_surface != 0 ?
        PHARAOH_SMACKER_FRONT_SURFACE : PHARAOH_SMACKER_MAIN_SURFACE;
    int32_t lock_result;

    if (state->playback_active == 0) {
        return 0;
    }
    if (operations->wait_for_movie(operations->user_data,
            state->movie_handle) != 0) {
        return 0;
    }

    description.size_bytes = sizeof(description);
    lock_result = operations->lock_surface(operations->user_data, surface,
        &description, &locked_surface);
    if (uses_front_surface != 0 &&
        (uint32_t)lock_result == smacker_surface_lost_hresult) {
        while (operations->wait_for_surface_recovery(operations->user_data,
                surface) == 0) {
        }
        goto advance_movie;
    }
    if (lock_result != 0) {
        if (uses_front_surface != 0) {
            const int32_t converted_error =
                operations->convert_surface_error(operations->user_data,
                    lock_result);
            operations->report_surface_lock_failure(operations->user_data,
                smacker_native_integer_increment(converted_error));
            state->graphics_refresh_state->smacker_uses_front_surface = 0;
        }
        goto advance_movie;
    }

    operations->smack_to_buffer(operations->user_data, state->movie_handle,
        state->movie_x, state->movie_y, locked_surface.destination_pitch,
        locked_surface.movie_height, locked_surface.surface_pixels,
        state->surface_buffer_mode);
    operations->smack_do_frame(operations->user_data, state->movie_handle);
    operations->unlock_surface(operations->user_data, surface,
        locked_surface.surface_pixels);

advance_movie:
    {
        const PharaohSmackerFramePosition position =
            operations->get_frame_position(operations->user_data,
                state->movie_handle);
        const uint32_t last_frame = position.frame_count - UINT32_C(1);
        if (position.frame_index < last_frame) {
            operations->smack_next_frame(operations->user_data,
                state->movie_handle);
            return 1;
        }
    }

    if (state->movie_handle != NULL) {
        operations->close_movie(operations->user_data, state->movie_handle);
        state->movie_handle = NULL;
    }
    operations->close_media_resources(operations->user_data);
    operations->report_natural_end(operations->user_data, state->movie_path);
    operations->report_memory_after_natural_end(operations->user_data,
        smacker_native_integer_increment(
            operations->available_memory(operations->user_data)));
    pharaoh_smacker_window_video_end(state);
    return 1;
}

int32_t pharaoh_smacker_movie_open(
    PharaohSmackerWindowVideoState *state, const char *filename,
    int32_t x, int32_t y, int32_t flags,
    const PharaohSmackerMovieOpenOperations *operations)
{
    uint32_t sound_flags = 0;

    state->playback_active = 0;
    state->graphics_refresh_state->smacker_video_playing = 0;
    if (state->sound_binding_initialized == 0) {
        operations->bind_sound_to_miles(operations->user_data,
            state->audio_driver);
        state->sound_binding_initialized = 1;
    }

    operations->report_movie_start(operations->user_data, filename);
    state->movie_x = x;
    state->movie_y = y;
    state->surface_buffer_mode = state->display_surface_format ==
        SMACKER_SURFACE_FORMAT_565 ? SMACKER_SURFACE_MODE_565 :
        SMACKER_SURFACE_MODE_OTHER;
    state->movie_handle = NULL;
    state->movie_flags = flags;

    operations->make_game_media_path(operations->user_data, filename,
        state->movie_path, PHARAOH_SMACKER_PATH_CAPACITY);
    state->resource_path_prepared = operations->probe_media_path(
        operations->user_data, state->movie_path);
    if (state->resource_path_prepared == 0) {
        operations->resolve_media_path(operations->user_data,
            state->movie_path, PHARAOH_SMACKER_PATH_CAPACITY);
    }

    if (state->audio_driver != NULL &&
        ((state->return_window_id == SMACKER_SPECIAL_SOUND_RETURN_WINDOW_A ||
            state->return_window_id == SMACKER_SPECIAL_SOUND_RETURN_WINDOW_B) ||
            state->movie_audio_setting != 0)) {
        sound_flags = SMACKER_SOUND_FLAG;
    }

    state->movie_handle = operations->open_movie(operations->user_data,
        state->movie_path, sound_flags, smacker_open_frame_limit);
    if (state->movie_handle == NULL) {
        operations->close_media_resources(operations->user_data);
        operations->report_movie_open_failure(operations->user_data,
            state->movie_path);
        operations->report_memory_after_open_failure(operations->user_data,
            smacker_native_integer_increment(
                operations->available_memory(operations->user_data)));
        return 0;
    }

    pharaoh_actor_walking_no_operation();
    pharaoh_actor_walking_no_operation();
    pharaoh_graphics_refresh_screen(state->graphics_refresh_state,
        state->graphics_refresh_operations, 1);
    state->playback_active = 1;
    state->graphics_refresh_state->smacker_video_playing = 1;
    (void)pharaoh_smacker_video_draw(state,
        operations->video_draw_operations);
    operations->close_media_resources(operations->user_data);
    return 1;
}

int32_t pharaoh_smacker_window_video_start(
    PharaohSmackerWindowVideoState *state, const char *filename,
    int32_t x, int32_t y, int32_t flags, int32_t return_window_id,
    const PharaohSmackerMovieOpenOperations *operations)
{
    if (state->video_system_state == 0) {
        return 0;
    }

    state->return_window_id = return_window_id;
    if (pharaoh_smacker_movie_open(state, filename, x, y, flags,
            operations) == 0) {
        state->graphics_refresh_state->current_window_id =
            (uint8_t)return_window_id;
        if (state->video_system_state ==
                SMACKER_VIDEO_SYSTEM_MODE_FULLSCREEN) {
            *state->redraw_requested = 1;
        }
        return 0;
    }

    state->graphics_refresh_state->current_window_id =
        SMACKER_WINDOW_PLAYING;
    return 1;
}

void pharaoh_smacker_window_video_end(PharaohSmackerWindowVideoState *state)
{
    state->graphics_refresh_state->current_window_id =
        (uint8_t)state->return_window_id;
    state->playback_active = 0;
    state->graphics_refresh_state->smacker_video_playing = 0;
    if (state->video_system_state ==
            SMACKER_VIDEO_SYSTEM_MODE_FULLSCREEN) {
        *state->redraw_requested = 1;
    }
    state->video_system_state = 0;
}

void pharaoh_smacker_window_video_stop(
    PharaohSmackerWindowVideoState *state,
    const PharaohSmackerWindowVideoStopOperations *operations)
{
    if (state->playback_active != 0) {
        if (state->resource_path_prepared == 0) {
            operations->resolve_media_path(operations->user_data,
                state->movie_path, PHARAOH_SMACKER_PATH_CAPACITY);
        }

        if (state->movie_handle != NULL) {
            operations->close_movie(operations->user_data,
                state->movie_handle);
            state->movie_handle = NULL;
        }

        operations->close_media_resources(operations->user_data);
        operations->report_user_stop(operations->user_data,
            state->movie_path);
        operations->report_memory_after_stop(operations->user_data,
            smacker_native_integer_increment(
                operations->available_memory(operations->user_data)));
        pharaoh_smacker_window_video_end(state);
    }
}

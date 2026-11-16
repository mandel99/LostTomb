#include "pharaoh/runtime/video_player.h"

#include <stddef.h>
#include <stdio.h>

_Static_assert(offsetof(PharaohVideoPlayer, stopped) == 4,
    "native video player stopped flag offset");
_Static_assert(offsetof(PharaohVideoPlayer, window) == 8,
    "native video player window offset");
_Static_assert(sizeof(void *) != 4 ||
    (offsetof(PharaohVideoPlayer, buffer) == 12 &&
     offsetof(PharaohVideoPlayer, movie) == 16 &&
     sizeof(PharaohVideoPlayer) == 28),
    "video player retains the native 32-bit object layout");
_Static_assert(offsetof(PharaohBinkMovieHeader, frame_count) == 0x10 &&
    offsetof(PharaohBinkMovieHeader, frame_number) == 0x14,
    "Bink frame counters match the offsets read by the native loop");
_Static_assert(sizeof(void *) != 4 || sizeof(PharaohVideoMessage) == 28,
    "video message retains the 32-bit Win32 MSG field layout");

static const char video_bink_error_prefix[] = "ERR: Bink error: ";
static const char video_bink_open_error[] = "Unable to load BINK!\n";
static const char video_movie_probe_mode[] = "rb";
static const char video_movie_filename_format[] = "%s";
static const char video_movie_drive_path_format[] = "%c:\\%s";

enum PharaohVideoSoundStreamId {
    PHARAOH_VIDEO_STREAM_MUSIC = 0,
    PHARAOH_VIDEO_STREAM_CITY = 2,
    PHARAOH_VIDEO_STREAM_SPEECH = 1
};

static void change_video_display_mode(
    PharaohVideoRuntimeState *state,
    const PharaohVideoLoopOperations *operations,
    uint32_t mode)
{
    *state->display_transition_active = UINT32_C(1);
    operations->release_display_device(operations->user_data, 1);
    operations->set_display_mode(operations->user_data, mode);
    operations->reset_video_buffers(operations->user_data);
    operations->redraw_background(operations->user_data);
    (void)operations->initialize_display_device(operations->user_data);
    *state->display_transition_active = UINT32_C(0);
    operations->update_display_confirmation(operations->user_data);
    *state->redraw_requested = UINT8_C(1);
}

void pharaoh_video_player_initialize(PharaohVideoPlayer *player,
                                     void *window)
{
    player->window = window;
    player->stopped = UINT8_C(1);
    player->movie = NULL;
    player->buffer = NULL;
}

void pharaoh_video_player_close(PharaohVideoPlayer *player,
                                const PharaohVideoPlayerBackend *backend)
{
    if (player->movie != NULL) {
        backend->close_movie(backend->user_data, player->movie);
        player->movie = NULL;
    }

    if (player->buffer != NULL) {
        backend->close_buffer(backend->user_data, player->buffer);
        player->buffer = NULL;
    }
}

uint8_t pharaoh_video_player_end(PharaohVideoPlayer *player,
                                 PharaohVideoRuntimeState *state,
                                 const PharaohVideoPlayerBackend *backend)
{
    *state->display_mode = (uint8_t)player->saved_mode;
    player->stopped = UINT8_C(1);
    *state->redraw_requested = UINT8_C(1);

    if (player->movie != NULL) {
        backend->close_movie(backend->user_data, player->movie);
        player->movie = NULL;
    }

    if (player->buffer != NULL) {
        backend->close_buffer(backend->user_data, player->buffer);
        player->buffer = NULL;
    }

    return UINT8_C(1);
}

void pharaoh_video_player_play_loop(PharaohVideoPlayer *player,
                                    PharaohVideoRuntimeState *state,
                                    const PharaohVideoLoopOperations *operations)
{
    if (state->video_system_active == 0) {
        return;
    }

    operations->clear_surfaces(operations->user_data);
    const uint32_t previous_mode = state->previous_display_mode;
    const int32_t switch_mode = state->switch_display_mode != 0;
    if (switch_mode != 0) {
        change_video_display_mode(state, operations, 1);
    }

    const PharaohBinkMovieHeader *movie =
        (const PharaohBinkMovieHeader *)player->movie;
    player->x = (int32_t)((state->screen_width - movie->width) >> 1);
    player->y = (int32_t)((state->screen_height - movie->height) >> 1);
    operations->show_cursor(operations->user_data, 0);

    int32_t stopped_by_input = 0;
    while (movie->frame_number < movie->frame_count &&
           stopped_by_input == 0) {
        (void)operations->tick(operations->user_data);
        operations->bink_do_frame(operations->user_data, player->movie);
        operations->bink_next_frame(operations->user_data, player->movie);
        operations->blit_movie(operations->user_data, player);

        int32_t wait_for_frame;
        do {
            const char *error =
                operations->bink_last_error(operations->user_data);
            if (error[0] != '\0') {
                operations->report_bink_error(operations->user_data,
                    video_bink_error_prefix, error, 0);
            }

            PharaohVideoMessage message;
            while (operations->peek_remove_message(operations->user_data,
                       &message) != 0) {
                if (message.message_id == PHARAOH_VIDEO_MESSAGE_KEY_DOWN ||
                    message.message_id == PHARAOH_VIDEO_MESSAGE_SYSTEM_KEY_DOWN ||
                    message.message_id == PHARAOH_VIDEO_MESSAGE_LEFT_BUTTON_DOWN) {
                    (void)pharaoh_video_player_end(player, state,
                        operations->player_backend);
                    if (switch_mode != 0) {
                        change_video_display_mode(state, operations,
                            previous_mode);
                    }
                    stopped_by_input = 1;
                    break;
                }

                operations->translate_message(operations->user_data,
                    &message);
                operations->dispatch_message(operations->user_data,
                    &message);
                operations->bink_service(operations->user_data,
                    player->movie);
            }

            if (stopped_by_input != 0) {
                break;
            }

            operations->bink_service(operations->user_data, player->movie);
            wait_for_frame = operations->bink_wait(operations->user_data,
                player->movie);
        } while (wait_for_frame != 0);
    }

    if (stopped_by_input == 0) {
        (void)pharaoh_video_player_end(player, state,
            operations->player_backend);
        if (switch_mode != 0) {
            change_video_display_mode(state, operations, previous_mode);
        }
    }

    operations->show_cursor(operations->user_data, 1);
}

uint8_t pharaoh_video_player_play(PharaohVideoPlayer *player,
                                 PharaohVideoRuntimeState *state,
                                 const char *filename, int32_t x, int32_t y,
                                 const PharaohVideoOpenOperations *operations)
{
    char movie_path[100];

    if (player->movie != NULL) {
        operations->loop_operations->player_backend->close_movie(
            operations->loop_operations->player_backend->user_data,
            player->movie);
    }

    if (operations->sound_manager_active != 0) {
        operations->close_sound_stream(operations->user_data,
            operations->sound_manager, PHARAOH_VIDEO_STREAM_MUSIC);
        operations->close_sound_stream(operations->user_data,
            operations->sound_manager, PHARAOH_VIDEO_STREAM_CITY);
        operations->close_sound_stream(operations->user_data,
            operations->sound_manager, PHARAOH_VIDEO_STREAM_SPEECH);
        operations->set_movie_sound_device(operations->user_data,
            operations->movie_sound_device);
    }

    (void)snprintf(movie_path, sizeof(movie_path),
        video_movie_filename_format, filename);
    PharaohMovieProbeFile *probe = operations->open_probe_file(
        operations->user_data, movie_path, video_movie_probe_mode);
    if (probe == NULL) {
        (void)snprintf(movie_path, sizeof(movie_path),
            video_movie_drive_path_format, (int)state->movie_drive_letter,
            filename);
    } else {
        operations->close_probe_file(operations->user_data, probe);
    }

    player->movie = operations->bink_open(operations->user_data, movie_path, 0);
    if (player->movie == NULL) {
        operations->output_debug_string(operations->user_data,
            video_bink_open_error);
        return UINT8_C(0);
    }

    player->saved_mode = *state->display_mode;
    *state->display_mode = UINT8_C(0x4a);
    player->stopped = UINT8_C(0);
    *state->redraw_requested = UINT8_C(0);
    player->x = x;
    player->y = y;
    pharaoh_video_player_play_loop(player, state, operations->loop_operations);
    return UINT8_C(1);
}


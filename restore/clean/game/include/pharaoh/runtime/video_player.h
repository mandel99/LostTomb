#ifndef PHARAOH_RUNTIME_VIDEO_PLAYER_H
#define PHARAOH_RUNTIME_VIDEO_PLAYER_H

#include <stdint.h>
#include "pharaoh/runtime/digital_sound_stream.h"

enum PharaohVideoSoundStreamId {
    PHARAOH_VIDEO_STREAM_MUSIC = 0,
    PHARAOH_VIDEO_STREAM_SPEECH = 1,
    PHARAOH_VIDEO_STREAM_CITY = 2
};

typedef struct PharaohVideoPlayer {
    uint32_t saved_mode;
    uint8_t stopped;
    uint8_t reserved[3];
    void *window;
    void *buffer;
    void *movie;
    int32_t x;
    int32_t y;
} PharaohVideoPlayer;

typedef struct PharaohVideoPlayerBackend {
    void *user_data;
    void (*close_movie)(void *user_data, void *movie);
    void (*close_buffer)(void *user_data, void *buffer);
} PharaohVideoPlayerBackend;

typedef struct PharaohBinkMovieHeader {
    uint32_t width;
    uint32_t height;
    uint32_t reserved_08;
    uint32_t reserved_0c;
    uint32_t frame_count;
    uint32_t frame_number;
} PharaohBinkMovieHeader;

typedef struct PharaohVideoMessage {
    void *window;
    uint32_t message_id;
    uintptr_t wparam;
    intptr_t lparam;
    uint32_t time;
    int32_t point_x;
    int32_t point_y;
} PharaohVideoMessage;

typedef struct PharaohVideoRuntimeState {
    uint8_t *display_mode;
    uint8_t *redraw_requested;
    uint32_t *display_transition_active;
    int32_t video_system_active;
    int32_t switch_display_mode;
    uint32_t previous_display_mode;
    uint32_t screen_width;
    uint32_t screen_height;
    uint8_t movie_drive_letter;
} PharaohVideoRuntimeState;

typedef struct PharaohVideoLoopOperations {
    void *user_data;
    void *bink_user_data;
    void *blit_user_data;
    const PharaohVideoPlayerBackend *player_backend;
    void (*clear_surfaces)(void *user_data);
    void (*release_display_device)(void *user_data, uint32_t release_device);
    void (*set_display_mode)(void *user_data, uint32_t mode);
    void (*reset_video_buffers)(void *user_data);
    void (*redraw_background)(void *user_data);
    int32_t (*initialize_display_device)(void *user_data);
    void (*update_display_confirmation)(void *user_data);
    uint32_t (*tick)(void *user_data);
    void (*bink_do_frame)(void *user_data, void *movie);
    void (*bink_next_frame)(void *user_data, void *movie);
    void (*blit_movie)(void *user_data, PharaohVideoPlayer *player);
    const char *(*bink_last_error)(void *user_data);
    void (*report_bink_error)(void *user_data, const char *prefix,
                              const char *detail, int32_t code);
    int32_t (*peek_remove_message)(void *user_data,
                                   PharaohVideoMessage *message);
    void (*translate_message)(void *user_data,
                              const PharaohVideoMessage *message);
    void (*dispatch_message)(void *user_data,
                             const PharaohVideoMessage *message);
    void (*bink_service)(void *user_data, void *movie);
    int32_t (*bink_wait)(void *user_data, void *movie);
    void (*show_cursor)(void *user_data, int32_t visible);
} PharaohVideoLoopOperations;

typedef struct PharaohMovieProbeFile PharaohMovieProbeFile;

typedef struct PharaohVideoOpenOperations {
    void *user_data;
    void *bink_user_data;
    const PharaohVideoLoopOperations *loop_operations;
    int32_t sound_manager_active;
    PharaohDigitalSoundDeviceState *sound_manager;
    const PharaohDigitalSoundStreamCloseOperations *sound_operations;
    void (*set_movie_sound_device)(void *user_data, uint32_t sound_device);
    PharaohMovieProbeFile *(*open_probe_file)(void *user_data,
                                               const char *filename,
                                               const char *mode);
    void (*close_probe_file)(void *user_data, PharaohMovieProbeFile *file);
    void *(*bink_open)(void *user_data, const char *filename,
                       uint32_t flags);
    void (*output_debug_string)(void *user_data, const char *message);
} PharaohVideoOpenOperations;

enum PharaohVideoMessageId {
    PHARAOH_VIDEO_MESSAGE_CLOSE = 0x0010,
    PHARAOH_VIDEO_MESSAGE_KEY_DOWN = 0x0100,
    PHARAOH_VIDEO_MESSAGE_SYSTEM_KEY_DOWN = 0x0104,
    PHARAOH_VIDEO_MESSAGE_LEFT_BUTTON_DOWN = 0x0201
};

/* Original entry 0x004133D0. Initializes only the fields written by the
   native member function; saved_mode, coordinates and padding are preserved. */
void pharaoh_video_player_initialize(PharaohVideoPlayer *player,
                                     void *window);

/* Original entry 0x004133F0. Releases the movie before its buffer. */
void pharaoh_video_player_close(PharaohVideoPlayer *player,
                                const PharaohVideoPlayerBackend *backend);

/* Original entry 0x00413420. Returns the native low-byte success value. */
uint8_t pharaoh_video_player_end(PharaohVideoPlayer *player,
                                 PharaohVideoRuntimeState *state,
                                 const PharaohVideoPlayerBackend *backend);

/* Original entry 0x00413690. Runs the Bink frame and Windows message loop. */
void pharaoh_video_player_play_loop(PharaohVideoPlayer *player,
                                    PharaohVideoRuntimeState *state,
                                    const PharaohVideoLoopOperations *operations);

/* Original entry 0x00413580. Returns the native low-byte open result. */
uint8_t pharaoh_video_player_play(PharaohVideoPlayer *player,
                                  PharaohVideoRuntimeState *state,
                                  const char *filename, int32_t x, int32_t y,
                                  const PharaohVideoOpenOperations *operations);

#endif

#ifndef PHARAOH_RUNTIME_VIDEO_PLAYER_H
#define PHARAOH_RUNTIME_VIDEO_PLAYER_H

#include <stdint.h>

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

typedef struct PharaohVideoRuntimeState {
    uint8_t *display_mode;
    uint8_t *redraw_requested;
} PharaohVideoRuntimeState;

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

#endif

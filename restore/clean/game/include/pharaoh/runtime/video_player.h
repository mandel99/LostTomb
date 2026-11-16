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

/* Original entry 0x004133D0. Initializes only the fields written by the
   native member function; saved_mode, coordinates and padding are preserved. */
void pharaoh_video_player_initialize(PharaohVideoPlayer *player,
                                     void *window);

#endif

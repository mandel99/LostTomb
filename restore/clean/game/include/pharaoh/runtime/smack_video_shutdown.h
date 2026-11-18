#ifndef PHARAOH_RUNTIME_SMACK_VIDEO_SHUTDOWN_H
#define PHARAOH_RUNTIME_SMACK_VIDEO_SHUTDOWN_H

#include <stdint.h>

typedef struct PharaohSmackVideoShutdownOperations {
    void *user_data;
    void (*close_sound_device)(void *user_data, void *sound_state);
    void (*close_movie)(void *user_data, void *movie_handle);
    void (*finish_sound_shutdown)(void *user_data);
} PharaohSmackVideoShutdownOperations;

typedef struct PharaohSmackVideoShutdownContext {
    uint32_t video_system_active;
    uint32_t sound_device_already_closed;
    void *sound_state;
    void **movie_handle;
    const PharaohSmackVideoShutdownOperations *operations;
} PharaohSmackVideoShutdownContext;

/* Original entry 0x005391E0. */
void pharaoh_smack_video_shutdown(
    const PharaohSmackVideoShutdownContext *context);

#endif

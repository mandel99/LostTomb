#ifndef PHARAOH_RUNTIME_APPLICATION_SHUTDOWN_H
#define PHARAOH_RUNTIME_APPLICATION_SHUTDOWN_H

#include "pharaoh/runtime/video_player.h"

typedef struct PharaohApplicationShutdownOperations {
    void *user_data;
    void (*release_video_player_storage)(void *user_data,
        PharaohVideoPlayer *player);
    void (*co_uninitialize)(void *user_data);
    void (*report_normal_exit)(void *user_data);
} PharaohApplicationShutdownOperations;

typedef struct PharaohApplicationShutdownContext {
    PharaohVideoPlayer **video_player;
    const PharaohVideoPlayerBackend *video_backend;
    const PharaohApplicationShutdownOperations *operations;
} PharaohApplicationShutdownContext;

/* Original Pharaoh.exe entry 0x004CFF50. Closes Bink, frees its containing
   player allocation, uninitializes COM, then emits the normal-exit report. */
void pharaoh_application_shutdown_finish(
    const PharaohApplicationShutdownContext *context);

#endif

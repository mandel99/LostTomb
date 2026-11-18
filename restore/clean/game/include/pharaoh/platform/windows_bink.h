#ifndef PHARAOH_PLATFORM_WINDOWS_BINK_H
#define PHARAOH_PLATFORM_WINDOWS_BINK_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "pharaoh/runtime/video_player.h"
#include "pharaoh/runtime/video_blit.h"

typedef struct PharaohWindowsBink {
    HMODULE module;
    void *(WINAPI *open)(const char *filename, uint32_t flags);
    void (WINAPI *close)(void *movie);
    void (WINAPI *buffer_close)(void *buffer);
    void (WINAPI *do_frame)(void *movie);
    void (WINAPI *next_frame)(void *movie);
    void (WINAPI *service)(void *movie);
    int32_t (WINAPI *wait)(void *movie);
    const char *(WINAPI *get_error)(void);
    int32_t (WINAPI *set_sound_system)(FARPROC open_miles, uint32_t device);
    uint32_t (WINAPI *surface_type)(void *surface);
    int32_t (WINAPI *copy_to_buffer)(void *movie, void *destination,
                                     int32_t pitch, uint32_t height,
                                     int32_t x, int32_t y, uint32_t surface_type);
    FARPROC open_miles;
    PharaohVideoBlitOperations blit_operations;
} PharaohWindowsBink;

int32_t pharaoh_windows_bink_open_library(PharaohWindowsBink *backend);
void pharaoh_windows_bink_close_library(PharaohWindowsBink *backend);
void pharaoh_windows_bink_bind_video_operations(
    PharaohWindowsBink *backend, PharaohVideoPlayerBackend *player,
    PharaohVideoLoopOperations *loop, PharaohVideoOpenOperations *open,
    PharaohVideoBlitContext *blit);

#endif

#ifndef PHARAOH_PLATFORM_WINDOWS_SMACKER_H
#define PHARAOH_PLATFORM_WINDOWS_SMACKER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "pharaoh/runtime/graphics_startup.h"
#include "pharaoh/runtime/smacker_window_video_start.h"
#include "pharaoh/runtime/status_log_format.h"

typedef struct PharaohWindowsSmacker {
    HMODULE module;
    int (WINAPI *sound_use_miles)(void *audio_driver);
    void *(WINAPI *open_movie)(const char *path, uint32_t sound_flags,
        uint32_t first_frame);
    int (WINAPI *wait_for_movie)(void *movie);
    void (WINAPI *to_buffer)(void *movie, int32_t x, int32_t y,
        int32_t destination_pitch, uint32_t movie_height,
        void *surface_pixels, uint32_t surface_buffer_mode);
    void (WINAPI *do_frame)(void *movie);
    void (WINAPI *next_frame)(void *movie);
    void (WINAPI *close_movie)(void *movie);
    void *movie_handle;
    PharaohGraphicsStartupState *graphics;
    PharaohStatusLogFormatState *status_state;
    const PharaohStatusLogFormatOperations *status_operations;
    void *media_user_data;
    void (*resolve_media_path)(void *user_data, char *path,
        uint32_t path_capacity);
    void (*close_media_resources)(void *user_data);
} PharaohWindowsSmacker;

int32_t pharaoh_windows_smacker_open_library(PharaohWindowsSmacker *backend);
void pharaoh_windows_smacker_close_library(PharaohWindowsSmacker *backend);
void pharaoh_windows_smacker_bind_operations(
    PharaohWindowsSmacker *backend,
    PharaohSmackerMovieOpenOperations *movie_open,
    PharaohSmackerVideoDrawOperations *video_draw,
    PharaohSmackerWindowVideoStopOperations *video_stop);

#endif

#ifndef PHARAOH_RUNTIME_SMACKER_WINDOW_VIDEO_START_H
#define PHARAOH_RUNTIME_SMACKER_WINDOW_VIDEO_START_H

#include <stdint.h>

#include "pharaoh/runtime/graphics_refresh.h"

enum { PHARAOH_SMACKER_PATH_CAPACITY = 64 };

typedef struct PharaohSmackerVideoDrawOperations
    PharaohSmackerVideoDrawOperations;

typedef struct PharaohSmackerWindowVideoState {
    int32_t video_system_state;
    int32_t playback_active;
    int32_t return_window_id;
    int32_t sound_binding_initialized;
    void *audio_driver;
    uint32_t display_surface_format;
    uint8_t movie_audio_setting;
    uint32_t surface_buffer_mode;
    int32_t movie_x;
    int32_t movie_y;
    int32_t movie_flags;
    int32_t resource_path_prepared;
    void *movie_handle;
    char movie_path[PHARAOH_SMACKER_PATH_CAPACITY];
    uint8_t *redraw_requested;
    PharaohGraphicsRefreshState *graphics_refresh_state;
    const PharaohGraphicsRefreshOperations *graphics_refresh_operations;
} PharaohSmackerWindowVideoState;

typedef struct PharaohSmackerMovieOpenOperations {
    void *user_data;
    void (*bind_sound_to_miles)(void *user_data, void *audio_driver);
    void (*report_movie_start)(void *user_data, const char *filename);
    void (*make_game_media_path)(void *user_data, const char *filename,
        char *path, uint32_t path_capacity);
    int32_t (*probe_media_path)(void *user_data, const char *path);
    void (*resolve_media_path)(void *user_data, char *path,
        uint32_t path_capacity);
    void *(*open_movie)(void *user_data, const char *path,
        uint32_t sound_flags, uint32_t first_frame);
    void (*close_media_resources)(void *user_data);
    void (*report_movie_open_failure)(void *user_data, const char *path);
    int32_t (*available_memory)(void *user_data);
    void (*report_memory_after_open_failure)(void *user_data, int32_t value);
    const PharaohSmackerVideoDrawOperations *video_draw_operations;
} PharaohSmackerMovieOpenOperations;

typedef struct PharaohSmackerSurfaceDescription {
    uint32_t size_bytes;
    uint32_t native_words[26];
} PharaohSmackerSurfaceDescription;

typedef struct PharaohSmackerLockedSurface {
    int32_t destination_pitch;
    void *surface_pixels;
    uint32_t movie_height;
} PharaohSmackerLockedSurface;

typedef struct PharaohSmackerFramePosition {
    uint32_t frame_count;
    uint32_t frame_index;
} PharaohSmackerFramePosition;

enum PharaohSmackerSurface {
    PHARAOH_SMACKER_MAIN_SURFACE,
    PHARAOH_SMACKER_FRONT_SURFACE
};

struct PharaohSmackerVideoDrawOperations {
    void *user_data;
    int32_t (*wait_for_movie)(void *user_data, void *movie);
    int32_t (*lock_surface)(void *user_data, int32_t surface,
        PharaohSmackerSurfaceDescription *description,
        PharaohSmackerLockedSurface *locked_surface);
    int32_t (*wait_for_surface_recovery)(void *user_data, int32_t surface);
    int32_t (*convert_surface_error)(void *user_data, int32_t result);
    void (*report_surface_lock_failure)(void *user_data, int32_t value);
    void (*smack_to_buffer)(void *user_data, void *movie, int32_t x,
        int32_t y, int32_t destination_pitch, uint32_t movie_height,
        void *surface_pixels, uint32_t surface_buffer_mode);
    void (*smack_do_frame)(void *user_data, void *movie);
    void (*unlock_surface)(void *user_data, int32_t surface,
        void *surface_pixels);
    PharaohSmackerFramePosition (*get_frame_position)(void *user_data,
        void *movie);
    void (*smack_next_frame)(void *user_data, void *movie);
    void (*close_movie)(void *user_data, void *movie);
    void (*close_media_resources)(void *user_data);
    void (*report_natural_end)(void *user_data, const char *path);
    int32_t (*available_memory)(void *user_data);
    void (*report_memory_after_natural_end)(void *user_data, int32_t value);
};

typedef struct PharaohSmackerWindowVideoStopOperations {
    void *user_data;
    void (*resolve_media_path)(void *user_data, char *path,
        uint32_t path_capacity);
    void (*close_movie)(void *user_data, void *movie);
    void (*close_media_resources)(void *user_data);
    void (*report_user_stop)(void *user_data, const char *path);
    int32_t (*available_memory)(void *user_data);
    void (*report_memory_after_stop)(void *user_data, int32_t value);
} PharaohSmackerWindowVideoStopOperations;

/* Typed translation of 0x00538DC0, directly composing the clean 0x00538E30
   owner and retaining the EXE's return-window and status rules. */
int32_t pharaoh_smacker_window_video_start(
    PharaohSmackerWindowVideoState *state, const char *filename,
    int32_t x, int32_t y, int32_t flags, int32_t return_window_id,
    const PharaohSmackerMovieOpenOperations *operations);

/* Typed translation of 0x00538E30. File/media and Smacker APIs are explicit
   runtime operations; sound preparation, refresh, and playback state order
   remain in C. */
int32_t pharaoh_smacker_movie_open(
    PharaohSmackerWindowVideoState *state, const char *filename,
    int32_t x, int32_t y, int32_t flags,
    const PharaohSmackerMovieOpenOperations *operations);

/* Typed translation of 0x00538FA0. DirectDraw and SmackW32 calls are typed
   operations; surface-loss policy, frame progression and natural completion
   remain in C. */
int32_t pharaoh_smacker_video_draw(
    PharaohSmackerWindowVideoState *state,
    const PharaohSmackerVideoDrawOperations *operations);

/* Typed translation of 0x00539220, which completes playback and restores
   the requested return window. */
void pharaoh_smacker_window_video_end(PharaohSmackerWindowVideoState *state);

/* Typed translation of 0x00539170. Performs stop side effects in native
   order and then applies the shared video-end state transition. */
void pharaoh_smacker_window_video_stop(
    PharaohSmackerWindowVideoState *state,
    const PharaohSmackerWindowVideoStopOperations *operations);

#endif

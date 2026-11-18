#ifndef PHARAOH_RUNTIME_VIDEO_BLIT_H
#define PHARAOH_RUNTIME_VIDEO_BLIT_H

#include <stdint.h>
#include "pharaoh/runtime/graphics_startup.h"
#include "pharaoh/runtime/status_log_format.h"
#include "pharaoh/runtime/video_player.h"

typedef struct PharaohVideoBlitOperations {
    void *surface_user_data;
    void *bink_user_data;
    int32_t (*lock_video_surface)(void *user_data, void *surface,
                                    void **pixels, int32_t *pitch);
    int32_t (*restore_surface)(void *user_data, void *surface);
    void (*unlock_video_surface)(void *user_data, void *surface,
                                    void *pixels);
    uint32_t (*bink_surface_type)(void *user_data, void *surface);
    void (*bink_copy_to_buffer)(void *user_data, void *movie,
                                void *destination, int32_t pitch,
                                uint32_t height, uint32_t surface_type);
    int32_t (*present_video_surface)(void *user_data,
        void *primary_surface, void *video_surface,
        const PharaohGraphicsWindowRect *destination,
        int32_t source_x, int32_t source_y,
        uint32_t source_width, uint32_t source_height);
} PharaohVideoBlitOperations;

typedef struct PharaohVideoBlitContext {
    void *primary_surface;
    void *video_surface;
    uint32_t video_surface_width;
    uint32_t video_surface_height;
    PharaohGraphicsWindowRect *screen_bounds;
    void *screen_bounds_user_data;
    int32_t (*refresh_screen_bounds)(void *user_data,
        PharaohGraphicsWindowRect *screen_bounds);
    uint8_t *write_to_primary_active;
    PharaohStatusLogFormatState *status_state;
    const PharaohStatusLogFormatOperations *status_operations;
    const PharaohVideoBlitOperations *operations;
} PharaohVideoBlitContext;

/* Native entry 0x00413470. */
uint8_t pharaoh_video_player_blit(const PharaohVideoBlitContext *context,
                                  const PharaohVideoPlayer *player);

#endif

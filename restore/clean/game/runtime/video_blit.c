#include "pharaoh/runtime/video_blit.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>
#include <stddef.h>

static const char video_primary_lock_error[] =
    "ERR:SMK releasing write_to_primary, surface lock error:";
static const char video_primary_blit_error[] =
    "ERR:SMK video frame blit failed:";

uint8_t pharaoh_video_player_blit(const PharaohVideoBlitContext *context,
                                  const PharaohVideoPlayer *player)
{
    const PharaohVideoBlitOperations *const operations = context->operations;
    void *pixels = NULL;
    int32_t pitch = 0;
    int32_t result;

    if (player->movie == NULL || player->stopped != 0) {
        return 0;
    }

    if (context->refresh_screen_bounds != NULL) {
        (void)context->refresh_screen_bounds(
            context->screen_bounds_user_data, context->screen_bounds);
    }

    result = operations->lock_video_surface(operations->surface_user_data,
        context->video_surface, &pixels, &pitch);
    while (result == (int32_t)DDERR_SURFACELOST) {
        if (operations->restore_surface(operations->surface_user_data,
                context->video_surface) != 0) {
            return 1;
        }
        result = operations->lock_video_surface(operations->surface_user_data,
            context->video_surface, &pixels, &pitch);
    }

    if (result == DD_OK) {
        const PharaohBinkMovieHeader *const movie =
            (const PharaohBinkMovieHeader *)player->movie;
        const int32_t direct_to_primary =
            context->video_surface == context->primary_surface;
        const int32_t source_x = 0;
        const int32_t source_y = 0;
        uint8_t *destination = (uint8_t *)pixels;
        if (direct_to_primary != 0) {
            const ptrdiff_t client_origin =
                (ptrdiff_t)context->screen_bounds->top * pitch +
                (ptrdiff_t)context->screen_bounds->left * 2;
            const ptrdiff_t movie_origin = (ptrdiff_t)player->y * pitch +
                (ptrdiff_t)player->x * 2;
            destination += client_origin + movie_origin;
        }
        const uint32_t surface_type = operations->bink_surface_type(
            operations->bink_user_data, context->video_surface);
        operations->bink_copy_to_buffer(operations->bink_user_data,
            player->movie, destination, pitch, movie->height, surface_type);
        operations->unlock_video_surface(operations->surface_user_data,
            context->video_surface, pixels);
        if (direct_to_primary != 0) {
            return 1;
        }
        result = operations->present_video_surface(
            operations->surface_user_data, context->primary_surface,
            context->video_surface, context->screen_bounds,
            source_x, source_y, movie->width, movie->height);
        if (result == (int32_t)DDERR_SURFACELOST) {
            if (operations->restore_surface(operations->surface_user_data,
                    context->primary_surface) == 0) {
                result = operations->present_video_surface(
                    operations->surface_user_data, context->primary_surface,
                    context->video_surface, context->screen_bounds,
                    source_x, source_y, movie->width, movie->height);
            }
        }
        if (result == DD_OK) {
            return 1;
        }
        pharaoh_status_log_format(video_primary_blit_error, NULL,
            (int32_t)pharaoh_graphics_hresult_error_character(result) + 1,
            context->status_state, context->status_operations);
        *context->write_to_primary_active = 0;
        return 1;
    }

    pharaoh_status_log_format(video_primary_lock_error, NULL,
        (int32_t)pharaoh_graphics_hresult_error_character(result) + 1,
        context->status_state, context->status_operations);
    *context->write_to_primary_active = 0;
    return 1;
}

#include "pharaoh/assets/sprite_pixel_loading.h"
#include "pharaoh/assets/sprite_pixel_conversion.h"

#include <string.h>

enum {
    PHARAOH_SPRITE_PATH_BYTES = 260
};

static char *append_text(char *destination, const char *source)
{
    while ((*destination = *source) != '\0') {
        ++destination;
        ++source;
    }
    return destination;
}

void pharaoh_sprite_release_pixels(const PharaohSpritePixelLoadContext *context,
    uint8_t **pixels, int32_t preserve_shared_buffer)
{
    uint8_t *allocation = *pixels;
    if (allocation == NULL) {
        return;
    }

    if (allocation == context->shared_pixel_buffer) {
        if (preserve_shared_buffer != 0) {
            *pixels = NULL;
        }
        return;
    }

    context->memory_operations->release(context->memory_user_data, allocation);
    *pixels = NULL;
}

uint32_t pharaoh_sprite_load_pixels(const PharaohSpritePixelLoadContext *context,
    const char *name, uint8_t **pixels, uint8_t convert,
    uint32_t byte_count, uint8_t compressed, int32_t destination_offset,
    uint32_t file_offset)
{
    char path[PHARAOH_SPRITE_PATH_BYTES];
    static const char data_directory[] = "data\\";
    static const char sprite_extension[] = ".555";

    char *path_end = append_text(path, data_directory);
    path_end = append_text(path_end, name);
    (void)append_text(path_end, sprite_extension);

    pharaoh_sprite_release_pixels(context, pixels, 0);
    if (byte_count == 0) {
        byte_count = (uint32_t)pharaoh_game_data_file_size_from_offset(
            context->game_data, path, (int32_t)file_offset);
    }

    if ((int32_t)byte_count > 0) {
        /* The native operator new contract does not return for allocation
           failure; the supplied allocator must preserve that contract. */
        *pixels = context->memory_operations->allocate(
            context->memory_user_data, byte_count);
    }

    if ((int32_t)byte_count > 0) {
        uint8_t *destination = *pixels + destination_offset;
        if (pharaoh_load_game_data(context->game_data, path, destination,
                (int32_t)file_offset) == 1 && convert != 0) {
            if (compressed != 0) {
                pharaoh_sprite_convert_compressed_555_to_565(
                    context->pixel_mode, *pixels, (int32_t)byte_count);
                return byte_count;
            }
            pharaoh_sprite_convert_raw_555_to_565(context->pixel_mode,
                *pixels, (int32_t)byte_count);
        }
    }

    return byte_count;
}

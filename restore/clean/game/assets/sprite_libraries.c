#include "pharaoh/assets/sprite_libraries.h"

#include "pharaoh/assets/sprite_pixel_loading.h"
#include "pharaoh/assets/sprite_pixel_conversion.h"

#include <string.h>

enum {
    PHARAOH_SG3_BUFFER_BYTES = 0xD8928,
    PHARAOH_SG3_IMAGE_COUNT_OFFSET = 16,
    PHARAOH_SG3_BITMAP_COUNT_OFFSET = 20,
    PHARAOH_SG3_GROUP_FIRST_IMAGES_OFFSET = 0x50,
    PHARAOH_SG3_BITMAP_RECORDS_OFFSET = 0x2A8,
    PHARAOH_SG3_IMAGE_RECORDS_OFFSET = 0x9F28,
    PHARAOH_SG3_GROUP_COUNT = 300,
    PHARAOH_SG3_SYSTEM_IMAGE_COUNT = 200,
    PHARAOH_SG3_SYSTEM_PIXEL_BYTES = 0x992E,
    PHARAOH_SPRITE_DATA_PATH_BYTES = 260
};

static uint32_t load_u32(const uint8_t *bytes)
{
    uint32_t value;
    memcpy(&value, bytes, sizeof(value));
    return value;
}

static int16_t load_i16(const uint8_t *bytes)
{
    int16_t value;
    memcpy(&value, bytes, sizeof(value));
    return value;
}

static char *append_string(char *destination, const char *source)
{
    while (*destination != '\0') {
        ++destination;
    }
    while ((*destination = *source) != '\0') {
        ++destination;
        ++source;
    }
    return destination;
}

static void copy_string(char *destination, const char *source)
{
    while ((*destination = *source) != '\0') {
        ++destination;
        ++source;
    }
}

uint8_t pharaoh_sprite_library_read(
    const PharaohSpriteLibraryReadContext *context,
    PharaohSpriteLibrary *library, const char *name, int32_t library_index,
    PharaohSpriteImageRecord *images, uint32_t first_image,
    PharaohSpriteTagRecord *tags, uint32_t first_tag)
{
    uint8_t metadata[PHARAOH_SG3_BUFFER_BYTES];
    char path[PHARAOH_SPRITE_DATA_PATH_BYTES] = "data\\";
    uint32_t previous_image_count = library->image_count;
    uint32_t system_image_skip = 0;
    uint32_t pixel_skip = 0;
    uint32_t cursor;
    uint8_t loaded;

    if (library->pixels != NULL) {
        context->pixels->memory_operations->release(
            context->pixels->memory_user_data, library->pixels);
        library->pixels = NULL;
    }
    library->name[0] = '\0';
    library->pixel_bytes = 0;
    library->image_count = 0;
    library->tag_count = 0;
    append_string(library->name, name);
    append_string(path, name);
    append_string(path, ".sg3");
    library->first_image = first_image;
    library->first_tag = first_tag;

    /* The native failure path reads fields from an uninitialized stack buffer.
       This typed reader processes metadata only when the SG3 read succeeds. */
    if (pharaoh_load_game_data(context->game_data, path, metadata, 0) != 0) {
        const uint32_t file_image_count =
            load_u32(metadata + PHARAOH_SG3_IMAGE_COUNT_OFFSET);
        const uint32_t bitmap_count =
            load_u32(metadata + PHARAOH_SG3_BITMAP_COUNT_OFFSET);

        if (library_index != 0 &&
            strcmp((const char *)(metadata + PHARAOH_SG3_BITMAP_RECORDS_OFFSET),
                "system.bmp") == 0) {
            system_image_skip = PHARAOH_SG3_SYSTEM_IMAGE_COUNT;
            pixel_skip = PHARAOH_SG3_SYSTEM_PIXEL_BYTES;
        }

        library->image_count = file_image_count - system_image_skip;
        memcpy(images,
            metadata + PHARAOH_SG3_IMAGE_RECORDS_OFFSET +
                system_image_skip * PHARAOH_SPRITE_IMAGE_RECORD_BYTES,
            (size_t)library->image_count * sizeof(*images));

        for (uint32_t group = 0; group < PHARAOH_SG3_GROUP_COUNT; ++group) {
            const int32_t image_number = (int32_t)load_i16(
                metadata + PHARAOH_SG3_GROUP_FIRST_IMAGES_OFFSET +
                    group * sizeof(int16_t)) - (int32_t)system_image_skip;
            if (image_number <= 0) {
                continue;
            }

            const uint32_t tag_index = library->tag_count++;
            const uint16_t tag_id = (uint16_t)(first_tag + tag_index);
            tags[tag_index].library_index = (uint32_t)library_index;
            tags[tag_index].image_index =
                (uint32_t)image_number + first_image - 1u;
            memcpy(&images[image_number - 1].tag_id, &tag_id,
                sizeof(tag_id));
        }

        for (uint32_t file = 0; file < bitmap_count; ++file) {
            copy_string(library->bitmap_names[file],
                (const char *)(metadata + PHARAOH_SG3_BITMAP_RECORDS_OFFSET +
                    file * PHARAOH_SPRITE_LIBRARY_BITMAP_NAME_BYTES));
        }
    }

    library->pixel_bytes = pharaoh_sprite_load_pixels(context->pixels, name,
        &library->pixels, 0, 0, 0, 0, pixel_skip);
    loaded = (uint8_t)(library->pixel_bytes != 0);
    if (loaded != 0) {
        cursor = (uint32_t)sizeof(uint32_t);
        *context->pixel_cursor = cursor;
        for (uint32_t index = 0; index < library->image_count; ++index) {
            PharaohSpriteImageRecord *image = &images[index];
            image->library_index = (uint32_t)library_index;
            if (image->is_external != 0) {
                if (image->pixel_offset == 0) {
                    image->pixel_offset = 1;
                }
                continue;
            }

            image->pixel_offset = cursor;
            cursor += image->pixel_bytes;
            if ((int32_t)image->pixel_bytes <= 0) {
                continue;
            }

            uint8_t *pixel_data = library->pixels + image->pixel_offset;
            if (image->draw_type == 30) {
                if (image->has_isometric_top == 0) {
                    pharaoh_sprite_convert_raw_555_to_565(
                        context->pixels->pixel_mode, pixel_data,
                        (int32_t)image->pixel_bytes);
                } else {
                    pharaoh_sprite_convert_raw_555_to_565(
                        context->pixels->pixel_mode, pixel_data,
                        (int32_t)image->isometric_top_bytes);
                    pharaoh_sprite_convert_compressed_555_to_565(
                        context->pixels->pixel_mode,
                        pixel_data + image->isometric_top_bytes,
                        (int32_t)(image->pixel_bytes -
                            image->isometric_top_bytes));
                }
            } else {
                image->has_isometric_top = 0;
                if (image->is_compressed != 0) {
                    pharaoh_sprite_convert_compressed_555_to_565(
                        context->pixels->pixel_mode, pixel_data,
                        (int32_t)image->pixel_bytes);
                } else {
                    pharaoh_sprite_convert_raw_555_to_565(
                        context->pixels->pixel_mode, pixel_data,
                        (int32_t)image->pixel_bytes);
                }
            }
        }
        *context->pixel_cursor = cursor;
    }

    if ((int32_t)library->image_count <= (int32_t)previous_image_count) {
        library->image_count = previous_image_count;
    }
    return loaded;
}
static const uint32_t tag_limits[PHARAOH_SPRITE_LIBRARY_COUNT] = {
    0x27, 0xfd, 0x10e, 0x1f2, 0x234, 0x235, 0x236, 0x237, 0x238, 0x239,
    0x240, 0x241, 0x27a, 0x282, 0x293, 0x2c7, 0x2f0, 0x2fc, 0x2fd, 0x2fe,
    0x2ff, 0x300, 0x301, 0x312, 0x317, 0x32c, 0x33c, 0x353, 0x370, 0x393
};

void pharaoh_sprite_libraries_reset(
    PharaohSpriteLibraryResetState *libraries,
    const PharaohSpritePixelLoadContext *pixel_context)
{
    pharaoh_sprite_release_pixels(pixel_context,
        &libraries->alternate_pixel_buffer, 0);
    pharaoh_sprite_release_pixels(pixel_context,
        &libraries->scratch_pixel_buffer, 0);

    for (uint32_t index = 0; index < PHARAOH_SPRITE_LIBRARY_COUNT; ++index) {
        PharaohSpriteLibrary *library = &libraries->items[index];
        if (library->pixels != NULL) {
            pixel_context->memory_operations->release(
                pixel_context->memory_user_data, library->pixels);
            library->pixels = NULL;
        }

        library->name[0] = '\0';
        library->pixel_bytes = 0;
        library->image_count = 0;
        library->tag_count = 0;
        libraries->tag_limits[index] = tag_limits[index];
    }

    libraries->loaded_count = 0;
    libraries->next_tag = 1;
    libraries->next_image = 1;
    libraries->pixel_bytes = 0;
}

static uint8_t names_equal_ignoring_ascii_case(const char *left,
    const char *right)
{
    while (*left != '\0' && *right != '\0') {
        unsigned char a = (unsigned char)*left++;
        unsigned char b = (unsigned char)*right++;
        if (a >= 'A' && a <= 'Z') {
            a = (unsigned char)(a + ('a' - 'A'));
        }
        if (b >= 'A' && b <= 'Z') {
            b = (unsigned char)(b + ('a' - 'A'));
        }
        if (a != b) {
            return 0;
        }
    }
    return (uint8_t)(*left == *right);
}

uint8_t pharaoh_sprite_library_load_file(
    const PharaohSpriteLibraryLoadContext *context,
    const char *name, int32_t library_index)
{
    PharaohSpriteLibraryResetState *libraries = context->libraries;
    PharaohSpriteLibrary *library;
    uint32_t first_image;
    uint32_t first_tag;
    uint8_t is_new;
    uint8_t already_loaded = 0;
    uint8_t result = 1;

    if (name == NULL) {
        return 0;
    }
    if (library_index == -1) {
        library_index = (int32_t)libraries->loaded_count;
    }

    library = &libraries->items[library_index];
    is_new = (uint8_t)(library->image_count == 0);
    if (is_new != 0) {
        first_tag = libraries->next_tag;
        first_image = libraries->next_image;
    } else {
        already_loaded = names_equal_ignoring_ascii_case(name, library->name);
        first_image = library->first_image;
        first_tag = library->first_tag;
        libraries->pixel_bytes -= library->pixel_bytes;
    }

    if (already_loaded == 0) {
        result = pharaoh_sprite_library_read(context->reader, library, name,
            library_index, context->images + first_image, first_image,
            context->tags + first_tag, first_tag);
        if (result != 0 && is_new != 0) {
            libraries->next_tag += library->tag_count;
            libraries->next_image += library->image_count;
            ++libraries->loaded_count;
        }
    }

    libraries->pixel_bytes += library->pixel_bytes;
    {
        const int32_t previous_end = library_index > 0
            ? (int32_t)libraries->tag_limits[library_index - 1] - 1
            : 0;
        const int32_t expected_count =
            ((int32_t)libraries->tag_limits[library_index] - 1) -
            previous_end;
        if ((int32_t)library->tag_count != expected_count) {
            context->show_tag_sync_warning(context->warning_user_data,
                name, library->tag_count, (uint32_t)expected_count);
        }
    }
    return result;
}

uint8_t pharaoh_sprite_library_load_named(
    const PharaohSpriteLibraryLoadContext *context, const char *name)
{
    return pharaoh_sprite_library_load_file(context, name, -1);
}

uint8_t pharaoh_sprite_libraries_load_one(
    const PharaohSpriteLibraryLoadContext *context,
    const char *const names[PHARAOH_SPRITE_LIBRARY_COUNT])
{
    uint32_t index = context->libraries->loaded_count;
    if (index >= PHARAOH_SPRITE_LIBRARY_COUNT) {
        return 0;
    }

    const char *name = names[index];
    if (name == NULL || name[0] == '\0') {
        return 0;
    }
    return pharaoh_sprite_library_load_named(context, name);
}

void pharaoh_sprite_libraries_load_all(
    const PharaohSpriteLibraryLoadContext *context,
    const char *const names[PHARAOH_SPRITE_LIBRARY_COUNT])
{
    uint8_t result = 1;
    do {
        uint32_t index = context->libraries->loaded_count;
        if (index >= PHARAOH_SPRITE_LIBRARY_COUNT) {
            return;
        }

        const char *name = names[index];
        if (name != NULL && name[0] != '\0') {
            result = pharaoh_sprite_library_load_named(context, name);
        }
    } while (result != 0);
}

#include "pharaoh/platform/windows_startup_host.h"
#include "pharaoh/ui/map_selection_rating.h"
#include "pharaoh/platform/windows_application_window.h"
#include "pharaoh/platform/windows_graphics.h"
#include "pharaoh/platform/windows_startup.h"
#include "pharaoh/platform/windows_status_log.h"
#include "pharaoh/platform/windows_video_io.h"
#include "pharaoh/assets/sprite_pixel_conversion.h"
#include "pharaoh/ui/main_menu.h"
#include "pharaoh/ui/bitmap_font.h"
#include "pharaoh/ui/panel_frame.h"
#include "pharaoh/ui/menu_button_frame.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <share.h>

enum {
    PHARAOH_IMAGE_GROUP_PANEL_BUTTON = 284,
    PHARAOH_IMAGE_GROUP_PLAYER_BACKGROUND = 29,
    PHARAOH_IMAGE_GROUP_OUTER_PANEL = 401,
    PHARAOH_IMAGE_GROUP_INNER_PANEL = 402,
    PHARAOH_IMAGE_GROUP_BUTTON_BORDER = 443,
    PHARAOH_GENERAL_LIBRARY_INDEX = 3,
    PHARAOH_GENERAL_SYSTEM_IMAGE_COUNT = 200,
    /* Pharaoh_General.sg3 group 401 / 402 after its 200 system images. */
    PHARAOH_GENERAL_OUTER_PANEL_LOCAL_IMAGE = 302,
    PHARAOH_GENERAL_INNER_PANEL_LOCAL_IMAGE = 446,
    PHARAOH_STARTUP_LOADING_FIRST_RUNTIME_IMAGE = 509,
    PHARAOH_STARTUP_EXPANSION_LOGO_RUNTIME_IMAGE = 900,
    PHARAOH_STARTUP_SYSTEM_IMAGE_COUNT = 200,
    PHARAOH_STARTUP_RESERVED_RUNTIME_IMAGE_COUNT = 1
};

static const int32_t starting_favor_by_difficulty[] = {
    70, 60, 50, 50, 40
};

static const char *const startup_sprite_libraries[
    PHARAOH_SPRITE_LIBRARY_COUNT] = {
    "Pharaoh_Unloaded", "SprMain", "Hyksos", "Pharaoh_General",
    "Pharaoh_Terrain", "SPHINX1A", "SPHINX1B", "SPHINX1C", "SPHINX1D",
    "SPHINX_EXTRA", "TEMPLE_NILE", "empire", "SprAmbient", "Pharaoh_Fonts",
    "Egyptian", "SprMain2", "expansion", "PyramidDummy", "CaesareumDummy",
    "LtHouseDummy", "LibMainDummy", "LibMainDummy", "LibMainDummy",
    "LibExtra", "MausCarryoverDummy", "TombThutDummy", "AbuSimbelDummy",
    "TombTutDummy", "TombSetiDummy", "TombRamDummy"
};

static int32_t message_box_measure_text(void *user_data, const char *text,
    int32_t font);

static void *allocate_sprite_memory(void *user_data, uint32_t byte_count)
{
    (void)user_data;
    return malloc(byte_count);
}

static void release_sprite_memory(void *user_data, void *allocation)
{
    (void)user_data;
    free(allocation);
}

static int32_t read_external_sprite_pixels(PharaohWindowsStartupHost *host,
    const char *path, uint32_t offset, uint8_t *destination,
    uint32_t byte_count)
{
    const PharaohFileReadOperations *const files = &host->file_operations;
    PharaohReadOnlyFile *file;
    size_t file_size;
    size_t read_count;

    if (path == NULL || destination == NULL || byte_count == 0 ||
        offset > INT32_MAX || files->open == NULL || files->size == NULL ||
        files->read == NULL || files->seek == NULL || files->close == NULL) {
        return 0;
    }
    file = files->open(host->files.user_data, path);
    if (file == NULL) return 0;
    file_size = files->size(host->files.user_data, file);
    if ((size_t)offset > file_size ||
        (size_t)byte_count > file_size - (size_t)offset ||
        files->seek(host->files.user_data, file, (int32_t)offset, 0) != 0) {
        files->close(host->files.user_data, file);
        return 0;
    }
    read_count = files->read(host->files.user_data, destination, 1,
        byte_count, file);
    files->close(host->files.user_data, file);
    return read_count == byte_count;
}

static void clear_startup_model_bytes(void *user_data, void *destination,
    uint32_t byte_count, uint8_t value)
{
    (void)user_data;
    memset(destination, value, byte_count);
}

static int32_t load_startup_model_text(void *user_data, const char *filename,
    void *destination, uint32_t capacity, int32_t skip_bytes)
{
    PharaohWindowsStartupHost *const host = user_data;
    (void)capacity;
    return pharaoh_load_game_data(&host->game_data, filename, destination,
        skip_bytes);
}

static uint32_t startup_model_text_length(void *user_data,
    const char *filename)
{
    PharaohWindowsStartupHost *const host = user_data;
    const int32_t file_bytes = pharaoh_file_size(&host->file_size, filename);
    return file_bytes > 0 ? (uint32_t)file_bytes : 0;
}

static int32_t continue_after_model_mismatch(
    PharaohWindowsStartupHost *host, const char *title, const char *format,
    uint32_t actual_plus_one, uint32_t expected)
{
    char prompt[256];
    (void)snprintf(host->startup_model_mismatch_text,
        sizeof(host->startup_model_mismatch_text), format,
        actual_plus_one, expected);
    (void)snprintf(prompt, sizeof(prompt),
        "%s\r\n\r\nWould you still like to continue?",
        host->startup_model_mismatch_text);
    return MessageBoxA((HWND)host->window_startup.window.window,
        prompt, title, MB_YESNO) == IDYES;
}

static int32_t continue_after_building_model_mismatch(void *user_data,
    uint32_t actual_plus_one, uint32_t expected)
{
    return continue_after_model_mismatch(user_data,
        "Building model files out of sync",
        "ERR: Building model has incorrect no of lines (%u), expected %u",
        actual_plus_one, expected);
}

static int32_t continue_after_figure_model_mismatch(void *user_data,
    uint32_t actual_plus_one, uint32_t expected)
{
    return continue_after_model_mismatch(user_data,
        "Figure model files out of sync",
        "ERR: Figure model has incorrect no of lines (%u), expected %u",
        actual_plus_one, expected);
}

static int32_t continue_after_tax_model_mismatch(void *user_data,
    uint32_t actual_plus_one, uint32_t expected)
{
    return continue_after_model_mismatch(user_data,
        "tax sentiment model files out of sync",
        "ERR: Tax sentiment model has incorrect no of lines (%u), expected %u",
        actual_plus_one, expected);
}

static void report_building_model_status(void *user_data,
    PharaohBuildingModelStatus status)
{
    PharaohWindowsStartupHost *const host = user_data;
    const char *message = status == PHARAOH_BUILDING_MODEL_LOADED
        ? " OK: model loaded"
        : status == PHARAOH_BUILDING_MODEL_FILE_MISSING
            ? "ERR:No Pharaoh model txt file"
            : status == PHARAOH_BUILDING_MODEL_NO_SPARE_SCREEN
                ? "ERR:no spare screen for model"
                : host->startup_model_mismatch_text;
    pharaoh_status_log_format(message, NULL, 0, &host->log_format_state,
        &host->log_format_operations);
}

static void report_figure_model_status(void *user_data,
    PharaohFigureModelStatus status)
{
    PharaohWindowsStartupHost *const host = user_data;
    const char *message = status == PHARAOH_FIGURE_MODEL_LOADED
        ? " OK: figure model data loaded"
        : status == PHARAOH_FIGURE_MODEL_FILE_MISSING
            ? "ERR:No Figure model txt file"
            : status == PHARAOH_FIGURE_MODEL_NO_SPARE_SCREEN
                ? "ERR:no spare screen for model"
                : host->startup_model_mismatch_text;
    pharaoh_status_log_format(message, NULL, 0, &host->log_format_state,
        &host->log_format_operations);
}

static void report_tax_model_status(void *user_data,
    PharaohTaxSentimentModelStatus status)
{
    PharaohWindowsStartupHost *const host = user_data;
    const char *message = status == PHARAOH_TAX_MODEL_LOADED
        ? " OK: tax sentiment data loaded"
        : status == PHARAOH_TAX_MODEL_FIGURE_FILE_MISSING
            ? "ERR:No Figure model txt file"
            : status == PHARAOH_TAX_MODEL_NO_SPARE_SCREEN
                ? "ERR:no spare screen for model"
                : host->startup_model_mismatch_text;
    pharaoh_status_log_format(message, NULL, 0, &host->log_format_state,
        &host->log_format_operations);
}

static int32_t continue_after_trade_model_mismatch(void *user_data,
    uint32_t actual_plus_one, uint32_t expected)
{
    return continue_after_model_mismatch(user_data,
        "trade_recommends files out of sync",
        "ERR: trade_recommends has incorrect no of lines (%u), expected %u",
        actual_plus_one, expected);
}

static void report_trade_model_status(void *user_data,
    PharaohTradeModelStatus status)
{
    PharaohWindowsStartupHost *const host = user_data;
    const char *message = status == PHARAOH_TRADE_MODEL_LOADED
        ? " OK: trade_recommends data loaded"
        : status == PHARAOH_TRADE_MODEL_FILE_MISSING
            ? "ERR:No trade_recommends txt file"
            : status == PHARAOH_TRADE_MODEL_NO_SPARE_SCREEN
                ? "ERR:no spare screen for model"
                : host->startup_model_mismatch_text;
    pharaoh_status_log_format(message, NULL, 0, &host->log_format_state,
        &host->log_format_operations);
}

static void *open_campaign_file(void *user_data, const char *filename,
    const char *mode, uint32_t share_flags)
{
    (void)user_data;
    return _fsopen(filename, mode, (int)share_flags);
}

static char *read_campaign_line(void *user_data, char *buffer,
    int32_t buffer_capacity, void *stream)
{
    (void)user_data;
    return fgets(buffer, buffer_capacity, (FILE *)stream);
}

static int32_t close_campaign_file(void *user_data, void *stream)
{
    (void)user_data;
    return fclose((FILE *)stream) == 0;
}

static void report_campaign_message(void *user_data, const char *message,
    int32_t first_argument, int32_t second_argument)
{
    PharaohWindowsStartupHost *const host = user_data;
    (void)first_argument;
    (void)second_argument;
    pharaoh_status_log_format(message, NULL, 0, &host->log_format_state,
        &host->log_format_operations);
}

static int32_t load_startup_reason_text(void *user_data, const char *filename,
    char *destination, uint32_t capacity, int32_t mode)
{
    return load_startup_model_text(user_data, filename, destination,
        capacity, mode);
}

static int32_t startup_reason_text_size(void *user_data, const char *filename)
{
    return (int32_t)startup_model_text_length(user_data, filename);
}

static int32_t show_startup_reason_prompt(void *user_data, void *owner_window,
    const char *message, const char *title)
{
    (void)user_data;
    return MessageBoxA((HWND)owner_window, message, title, MB_YESNO);
}

static void report_startup_reason_status(void *user_data, const char *message)
{
    PharaohWindowsStartupHost *const host = user_data;
    pharaoh_status_log_format(message, NULL, 0, &host->log_format_state,
        &host->log_format_operations);
}

static void report_sprite_tag_mismatch(void *user_data, const char *library,
    uint32_t actual_tags, uint32_t expected_tags)
{
    PharaohWindowsStartupHost *const host = user_data;
    (void)library;
    (void)expected_tags;
    pharaoh_status_log_format("ERR: Sprite tag count mismatch", NULL,
        (int32_t)actual_tags, &host->log_format_state,
        &host->log_format_operations);
}

static void clear_video_surfaces(void *user_data)
{
    PharaohWindowsStartupHost *const host = user_data;
    (void)pharaoh_graphics_surfaces_clear(&host->graphics,
        &host->graphics_operations);
}

static void report_bink_error(void *user_data, const char *prefix,
    const char *detail, int32_t code)
{
    PharaohWindowsStartupHost *const host = user_data;
    pharaoh_status_log_format(prefix, detail, code, &host->log_format_state,
        &host->log_format_operations);
}

static void report_graphics_status(void *user_data, const char *message,
    int32_t unused_argument, int32_t detail)
{
    PharaohWindowsStartupHost *const host = user_data;
    (void)unused_argument;
    pharaoh_status_log_format(message, NULL, detail, &host->log_format_state,
        &host->log_format_operations);
}

static void dispatch_graphics_message(void *user_data, uint32_t message,
    int32_t argument_1, int32_t argument_2)
{
    PharaohWindowsStartupHost *const host = user_data;
    if (host->window_startup.window.window != NULL) {
        (void)PostMessageA((HWND)host->window_startup.window.window, message,
            (WPARAM)argument_1, (LPARAM)argument_2);
    }
}

static int32_t get_current_window_id(void *user_data)
{
    const PharaohWindowsStartupHost *const host = user_data;
    return host->current_window_id;
}

static int32_t refresh_video_screen_bounds(void *user_data,
    PharaohGraphicsWindowRect *bounds)
{
    PharaohWindowsStartupHost *const host = user_data;
    RECT client_rectangle;
    POINT client_origin = { 0, 0 };
    const HWND window = (HWND)host->window_startup.window.window;

    if (GetClientRect(window, &client_rectangle) == 0 ||
        ClientToScreen(window, &client_origin) == 0) {
        return 0;
    }

    bounds->left = client_origin.x;
    bounds->top = client_origin.y;
    bounds->right = client_origin.x +
        client_rectangle.right - client_rectangle.left;
    bounds->bottom = client_origin.y +
        client_rectangle.bottom - client_rectangle.top;
    return 1;
}

int32_t pharaoh_windows_startup_host_refresh_video_bounds(
    PharaohWindowsStartupHost *host)
{
    return refresh_video_screen_bounds(host, &host->video_screen_bounds);
}

static void *allocate_startup_pixels(void *user_data, size_t byte_count)
{
    (void)user_data;
    return malloc(byte_count);
}

static void create_startup_directory(void *user_data, const char *path)
{
    (void)user_data;
    (void)CreateDirectoryA(path, NULL);
}

static void clear_loading_surface(void *user_data)
{
    PharaohWindowsStartupHost *const host = user_data;
    (void)pharaoh_graphics_surfaces_clear(&host->graphics,
        &host->graphics_operations);
}

static void draw_sprite_pixels(void *user_data, uint32_t image_id,
    int32_t x, int32_t y, uint16_t mask_color)
{
    PharaohWindowsStartupHost *const host = user_data;
    if (image_id == PHARAOH_STARTUP_EXPANSION_LOGO_RUNTIME_IMAGE) {
        const uint32_t expansion_first_image =
            host->loaded_libraries->items[16].first_image;
        const uint32_t expansion_image_offset =
            PHARAOH_STARTUP_EXPANSION_LOGO_RUNTIME_IMAGE -
            PHARAOH_STARTUP_SYSTEM_IMAGE_COUNT -
            PHARAOH_STARTUP_RESERVED_RUNTIME_IMAGE_COUNT;
        if (expansion_first_image == 0 ||
            expansion_image_offset >=
                host->loaded_libraries->items[16].image_count ||
            expansion_first_image + expansion_image_offset >=
                PHARAOH_WINDOWS_STARTUP_IMAGE_CAPACITY) {
            return;
        }
        image_id = expansion_first_image + expansion_image_offset;
    }
    PharaohSpriteImageRecord *image;
    PharaohSpriteLibrary *library;
    DDSURFACEDESC description;
    uint8_t *source;
    uint8_t *destination;
    uint8_t *external_pixels = NULL;
    uint8_t *opacity = NULL;
    uint32_t row_bytes;
    int32_t source_x = 0;
    int32_t source_y = 0;
    int32_t destination_x = x;
    int32_t destination_y = y;
    int32_t copy_width;
    int32_t copy_height;
    int32_t row;

    if (image_id >= PHARAOH_WINDOWS_STARTUP_IMAGE_CAPACITY) {
        return;
    }
    image = &host->sprite_images[image_id];
    /* Native type 10 (00410FB0) copies an opaque 16x16 tile. General.sg3
       outer/inner panels use it; the normal raw path below is equivalent. */
    if (image->width <= 0 || image->height <= 0 ||
        (image->draw_type != 0 && image->draw_type != 1 && image->draw_type != 10 && image->draw_type != 20 &&
         (image->draw_type != 30 || image->has_isometric_top != 0)) ||
        image->library_index >= PHARAOH_SPRITE_LIBRARY_COUNT) {
        return;
    }

    row_bytes = (uint32_t)(uint16_t)image->width * 2u;
    if (image->is_compressed == 0 &&
        (uint64_t)row_bytes * (uint16_t)image->height > image->pixel_bytes) {
        return;
    }
    library = &host->loaded_libraries->items[image->library_index];
    if (image->is_external != 0) {
        const uint32_t bitmap_index = image->bitmap_id;
        const char *bitmap_name;
        const char *extension;
        char pixel_path[PHARAOH_GAME_DATA_PATH_BYTES];
        char library_pixel_path[PHARAOH_GAME_DATA_PATH_BYTES];
        int32_t pixel_path_length;
        int32_t library_path_length;
        uint32_t external_offset;

        if ((image->draw_type != 0 && image->draw_type != 1) ||
            image->is_compressed != 0 || image->pixel_offset == 0 ||
            bitmap_index >= PHARAOH_SPRITE_LIBRARY_BITMAP_COUNT ||
            image->pixel_bytes > INT32_MAX) {
            return;
        }
        bitmap_name = library->bitmap_names[bitmap_index];
        extension = strrchr(bitmap_name, '.');
        if (bitmap_name[0] == '\0' || extension == NULL) {
            return;
        }
        pixel_path_length = snprintf(pixel_path, sizeof(pixel_path),
            "data\\%.*s.555", (int)(extension - bitmap_name), bitmap_name);
        if (pixel_path_length <= 0 ||
            (size_t)pixel_path_length >= sizeof(pixel_path)) {
            return;
        }
        library_path_length = snprintf(library_pixel_path,
            sizeof(library_pixel_path), "data\\%s.555", library->name);
        if (library_path_length <= 0 ||
            (size_t)library_path_length >= sizeof(library_pixel_path)) {
            return;
        }
        /* External SG3 offsets are one-based, matching native
           ImageLoadExternalData(header->data - 1). */
        external_offset = image->pixel_offset - 1u;
        external_pixels = host->sprite_memory_operations.allocate(host,
            image->pixel_bytes);
        if (external_pixels == NULL) return;
        if (read_external_sprite_pixels(host, pixel_path, external_offset,
                external_pixels, image->pixel_bytes) == 0 &&
            read_external_sprite_pixels(host, library_pixel_path,
                external_offset, external_pixels, image->pixel_bytes) == 0) {
            host->sprite_memory_operations.release(host, external_pixels);
            return;
        }
        pharaoh_sprite_convert_raw_555_to_565(
            host->sprite_pixels.pixel_mode, external_pixels,
            (int32_t)image->pixel_bytes);
        source = external_pixels;
    } else {
        if (library->pixels == NULL ||
            image->pixel_offset > library->pixel_bytes ||
            image->pixel_bytes > library->pixel_bytes - image->pixel_offset) {
            return;
        }
        source = library->pixels + image->pixel_offset;
    }

    if (image->is_compressed != 0) {
        const uint8_t *cursor = source;
        const uint8_t *const end = source + image->pixel_bytes;
        const size_t decoded_bytes = (size_t)row_bytes * (uint16_t)image->height;
        external_pixels = host->sprite_memory_operations.allocate(host,
            (uint32_t)(decoded_bytes + decoded_bytes / 2u));
        if (external_pixels == NULL) return;
        memset(external_pixels, 0, decoded_bytes + decoded_bytes / 2u);
        opacity = external_pixels + decoded_bytes;
        for (int32_t line = 0; line < image->height; ++line) {
            int32_t column = 0;
            while (column < image->width) {
                if (cursor >= end) goto malformed_sprite;
                uint32_t count = *cursor++;
                const int transparent = count == 255;
                if (transparent) {
                    if (cursor >= end) goto malformed_sprite;
                    count = *cursor++;
                } else if (count < 1 || count > 16) {
                    count = 15; /* Native RLE dispatch default. */
                }
                if (count == 0 || count > (uint32_t)(image->width - column))
                    goto malformed_sprite;
                if (!transparent) {
                    if ((size_t)(end - cursor) < count * 2u) goto malformed_sprite;
                    uint8_t *const output = external_pixels + (size_t)line * row_bytes +
                        (size_t)column * 2u;
                    if (image->draw_type == 20) {
                        for (uint32_t pixel = 0; pixel < count; ++pixel)
                            memcpy(output + pixel * 2u, &mask_color, sizeof(mask_color));
                    } else memcpy(output, cursor, count * 2u);
                    memset(opacity + (size_t)line * (uint16_t)image->width +
                        (size_t)column, 1, count);
                    cursor += count * 2u;
                }
                column += (int32_t)count;
            }
        }
        source = external_pixels;
    }

    if (destination_x < 0) {
        source_x = -destination_x;
        destination_x = 0;
    }
    if (destination_y < 0) {
        source_y = -destination_y;
        destination_y = 0;
    }
    copy_width = image->width - source_x;
    copy_height = image->height - source_y;
    if (copy_width > host->graphics.render_width - destination_x) {
        copy_width = host->graphics.render_width - destination_x;
    }
    if (copy_height > host->graphics.render_height - destination_y) {
        copy_height = host->graphics.render_height - destination_y;
    }
    if (copy_width <= 0 || copy_height <= 0) {
        host->sprite_memory_operations.release(host, external_pixels);
        return;
    }

    memset(&description, 0, sizeof(description));
    description.dwSize = sizeof(description);
    if (((LPDIRECTDRAWSURFACE)host->graphics.back_buffer_surface)->lpVtbl->Lock(
            host->graphics.back_buffer_surface, NULL, &description,
            DDLOCK_WAIT, NULL) != DD_OK) {
        host->sprite_memory_operations.release(host, external_pixels);
        return;
    }
    const uint32_t destination_bits = description.ddpfPixelFormat.dwRGBBitCount;
    const uint32_t destination_pixel_bytes = destination_bits / 8;
    if (destination_bits != 16 &&
        (destination_bits != 32 ||
         description.ddpfPixelFormat.dwRBitMask != 0x00ff0000u ||
         description.ddpfPixelFormat.dwGBitMask != 0x0000ff00u ||
         description.ddpfPixelFormat.dwBBitMask != 0x000000ffu)) {
        (void)((LPDIRECTDRAWSURFACE)host->graphics.back_buffer_surface)->lpVtbl->Unlock(
            host->graphics.back_buffer_surface, description.lpSurface);
        host->sprite_memory_operations.release(host, external_pixels);
        return;
    }
    destination = (uint8_t *)description.lpSurface +
        description.lPitch * destination_y +
        (size_t)destination_x * destination_pixel_bytes;
    for (row = 0; row < copy_height; ++row) {
        const uint8_t *const source_row = source +
            row_bytes * (uint32_t)(source_y + row) + (uint32_t)source_x * 2u;
        uint8_t *const destination_row = destination + description.lPitch * row;
        if (destination_bits == 16 && image->is_compressed == 0 &&
            image->draw_type != 0) {
            memcpy(destination_row, source_row, (size_t)copy_width * 2u);
        } else {
            /* SG3 payloads remain 555 unless the library reader converted
               them to 565. A windowed desktop surface may instead be XRGB. */
            const int32_t source_is_565 = host->sprite_pixels.pixel_mode ==
                PHARAOH_SPRITE_PIXEL_MODE_RGB565;
            for (int32_t column = 0; column < copy_width; ++column) {
                if (opacity != NULL && opacity[
                    (size_t)(source_y + row) * (uint16_t)image->width +
                    (size_t)(source_x + column)] == 0) continue;
                uint16_t pixel;
                memcpy(&pixel, source_row + (size_t)column * 2u, sizeof(pixel));
                /* Native raw keyed blits compare F81F before any conversion.
                   RLE transparency is carried by skip runs, not RGB values. */
                if (opacity == NULL && image->draw_type == 0 && pixel == 0xf81f) continue;
                if (destination_bits == 16) {
                    memcpy(destination_row + (size_t)column * 2u, &pixel, sizeof(pixel));
                    continue;
                }
                const uint32_t red = (pixel >> (source_is_565 ? 11 : 10)) & 31u;
                const uint32_t green = (pixel >> 5) & (source_is_565 ? 63u : 31u);
                const uint32_t blue = pixel & 31u;
                const uint32_t red8 = (red << 3) | (red >> 2);
                const uint32_t green8 = source_is_565 ?
                    (green << 2) | (green >> 4) : (green << 3) | (green >> 2);
                const uint32_t blue8 = (blue << 3) | (blue >> 2);
                const uint32_t output_pixel =
                    (red8 << 16) | (green8 << 8) | blue8;
                memcpy(destination_row + (size_t)column * 4u,
                    &output_pixel, sizeof(output_pixel));
            }
        }
    }
    (void)((LPDIRECTDRAWSURFACE)host->graphics.back_buffer_surface)->lpVtbl->Unlock(
        host->graphics.back_buffer_surface, description.lpSurface);
    host->sprite_memory_operations.release(host, external_pixels);
    return;
malformed_sprite:
    host->sprite_memory_operations.release(host, external_pixels);
}

static void draw_loading_sprite(void *data, uint32_t image, int32_t x, int32_t y)
{ draw_sprite_pixels(data, image, x, y, 0); }

static void draw_menu_image(void *data, int32_t image, int32_t x, int32_t y)
{ draw_loading_sprite(data, (uint32_t)image, x, y); }

static void draw_font_glyph(void *data, uint32_t image, int32_t x,
    int32_t y, uint16_t color)
{ draw_sprite_pixels(data, image, x, y, color); }

static uint32_t menu_group_image(const PharaohWindowsStartupHost *host,
    uint32_t native_group)
{
    if (native_group >= host->loaded_libraries->next_tag) return 0;
    return host->sprite_tags[native_group].image_index;
}

static int32_t general_image(const PharaohWindowsStartupHost *host,
    int32_t image)
{
    return (int32_t)host->loaded_libraries->items[
        PHARAOH_GENERAL_LIBRARY_INDEX].first_image +
        image - PHARAOH_GENERAL_SYSTEM_IMAGE_COUNT;
}

static int32_t general_local_image(const PharaohWindowsStartupHost *host,
    int32_t image)
{
    return (int32_t)host->loaded_libraries->items[
        PHARAOH_GENERAL_LIBRARY_INDEX].first_image + image - 1;
}

static void draw_menu_language(void *data, int32_t group, int32_t entry,
    int32_t x, int32_t y, int32_t width, int32_t font, int32_t color)
{
    PharaohWindowsStartupHost *const host = data;
    PharaohBitmapFont bitmap_font = { 0 };
    pharaoh_text_select(&host->text_selection, group, entry);
    if (host->text_selection.current == NULL) return;
    if (font < 634 || font > 641) return;
    bitmap_font.images = host->sprite_images;
    bitmap_font.image_count = host->loaded_libraries->next_image;
    bitmap_font.first_image = menu_group_image(host, (uint32_t)font);
    const int32_t is_small_font = font == 634;
    const int32_t is_large_font = font == 640 || font == 641;
    /* Native width (004CBF30) and drawing (004CC770) intentionally use
       different space widths for the two large fonts. */
    bitmap_font.measure_space = is_small_font ? 4 : is_large_font ? 10 : 6;
    bitmap_font.draw_space = is_small_font ? 4 : is_large_font ? 8 : 6;
    bitmap_font.measure_gap = is_small_font || is_large_font ? 1 : 0;
    bitmap_font.draw_gap = is_small_font ? 1 : 0;
    bitmap_font.baseline = is_small_font ? 9 : font == 640 ? 23 : font == 641 ? 24 : 11;
    bitmap_font.user_data = host;
    bitmap_font.draw_glyph = draw_font_glyph;
    if (bitmap_font.first_image == 0) return;
    pharaoh_bitmap_text_draw_centered(&bitmap_font,
        (const uint8_t *)host->text_selection.current, x, y, width, (uint16_t)color);
}

static void main_menu_draw_border(void *data, int32_t style, int32_t x,
    int32_t y, int32_t width, int32_t height, int32_t selected)
{
    const PharaohMenuButtonFrameOperations operations = {
        data, draw_menu_image
    };
    (void)style;
    pharaoh_menu_button_frame_draw(&operations,
        (int32_t)menu_group_image(data, PHARAOH_IMAGE_GROUP_BUTTON_BORDER),
        x, y, width, height, selected);
}

static void draw_large_label_static(PharaohWindowsStartupHost *host,
    int32_t x, int32_t y, int32_t width_blocks, int32_t style)
{
    int32_t animations[7] = { 0 };
    PharaohLabelRenderer labels = { 0 };
    animations[6] = 100;
    labels.block_size = 16;
    labels.opening_animation = 5;
    labels.growing_animation = 15;
    labels.blocks_offset = 13;
    labels.pieces_per_style = 3;
    labels.panel_button_images =
        (int32_t)menu_group_image(host, PHARAOH_IMAGE_GROUP_PANEL_BUTTON);
    labels.animations = animations;
    labels.draw_image = draw_menu_image;
    labels.user_data = host;
    pharaoh_large_label_draw_animated(&labels, 6, x, y,
        width_blocks, style);
}

static void main_menu_sound_update(void *data, int32_t mode)
{
    (void)data;
    (void)mode;
}

static int32_t main_menu_sound_is_playing(void *data, int32_t channel)
{
    PharaohWindowsStartupHost *const host = data;
    return pharaoh_digital_sound_stream_is_playing(&host->sound.state,
        (uint32_t)channel, &host->sound.stream_operations);
}

static void main_menu_sound_play(void *data, const char *path,
    int32_t channel, int32_t loops)
{
    PharaohWindowsStartupHost *const host = data;
    (void)pharaoh_digital_sound_stream_play(&host->sound.state, path,
        (uint32_t)channel, (uint32_t)loops, &host->sound.stream_operations);
}

static int32_t high_score_read_records(void *user_data, const char *path,
    void *destination, size_t byte_count)
{
    PharaohWindowsStartupHost *const host = user_data;
    const PharaohFileReadOperations *const files = &host->file_operations;
    PharaohReadOnlyFile *const file = files->open(host->files.user_data, path);
    size_t read_count;
    if (file == NULL) return 0;
    read_count = files->read(host->files.user_data, destination, 1,
        byte_count, file);
    files->close(host->files.user_data, file);
    return read_count == byte_count;
}

static void high_score_draw_panel(void *user_data, int32_t x, int32_t y,
    int32_t width_blocks, int32_t height_blocks)
{
    const PharaohPanelFrameOperations frame = {
        user_data, draw_menu_image,
        (int32_t)menu_group_image(user_data,
            PHARAOH_IMAGE_GROUP_OUTER_PANEL), 0
    };
    pharaoh_panel_frame_draw(&frame, x, y, width_blocks, height_blocks);
}

static void high_score_draw_inner_panel(void *user_data, int32_t x, int32_t y,
    int32_t width_blocks, int32_t height_blocks)
{
    const PharaohPanelFrameOperations frame = {
        user_data, draw_menu_image, 0,
        (int32_t)menu_group_image(user_data,
            PHARAOH_IMAGE_GROUP_INNER_PANEL)
    };
    pharaoh_inner_panel_frame_draw(&frame, x, y,
        width_blocks, height_blocks);
}

static void high_score_draw_centered_language(void *user_data, int32_t group,
    int32_t entry, int32_t x, int32_t y, int32_t width, int32_t font,
    int32_t color)
{
    draw_menu_language(user_data, group, entry, x, y, width, font, color);
}

static int32_t high_score_draw_language(void *user_data, int32_t group,
    int32_t entry, int32_t x, int32_t y, int32_t font, int32_t color)
{
    PharaohWindowsStartupHost *const host = user_data;
    PharaohBitmapFont bitmap_font = { 0 };
    pharaoh_text_select(&host->text_selection, group, entry);
    if (host->text_selection.current == NULL || font < 634 || font > 641) {
        return x;
    }
    bitmap_font.images = host->sprite_images;
    bitmap_font.image_count = host->loaded_libraries->next_image;
    bitmap_font.first_image = menu_group_image(host, (uint32_t)font);
    bitmap_font.measure_space = font == 634 ? 4 :
        (font == 640 || font == 641) ? 10 : 6;
    bitmap_font.draw_space = font == 634 ? 4 :
        (font == 640 || font == 641) ? 8 : 6;
    bitmap_font.measure_gap = font == 634 || font >= 640 ? 1 : 0;
    bitmap_font.draw_gap = font == 634 ? 1 : 0;
    bitmap_font.baseline = font == 634 ? 9 : font == 640 ? 23 :
        font == 641 ? 24 : 11;
    bitmap_font.user_data = host;
    bitmap_font.draw_glyph = draw_font_glyph;
    if (bitmap_font.first_image == 0) return x;
    const int32_t width = pharaoh_bitmap_text_width(&bitmap_font,
        (const uint8_t *)host->text_selection.current);
    pharaoh_bitmap_text_draw_at(&bitmap_font,
        (const uint8_t *)host->text_selection.current, x, y, (uint16_t)color);
    return x + width;
}

static void high_score_draw_text(void *user_data, const char *text,
    int32_t x, int32_t y, int32_t font, int32_t color)
{
    PharaohWindowsStartupHost *const host = user_data;
    PharaohBitmapFont bitmap_font = { 0 };
    if (text == NULL || font < 634 || font > 641) return;
    bitmap_font.images = host->sprite_images;
    bitmap_font.image_count = host->loaded_libraries->next_image;
    bitmap_font.first_image = menu_group_image(host, (uint32_t)font);
    bitmap_font.measure_space = font == 634 ? 4 :
        (font == 640 || font == 641) ? 10 : 6;
    bitmap_font.draw_space = font == 634 ? 4 :
        (font == 640 || font == 641) ? 8 : 6;
    bitmap_font.measure_gap = font == 634 || font >= 640 ? 1 : 0;
    bitmap_font.draw_gap = font == 634 ? 1 : 0;
    bitmap_font.baseline = font == 634 ? 9 : font == 640 ? 23 :
        font == 641 ? 24 : 11;
    bitmap_font.user_data = host;
    bitmap_font.draw_glyph = draw_font_glyph;
    if (bitmap_font.first_image != 0) {
        pharaoh_bitmap_text_draw_at(&bitmap_font, (const uint8_t *)text,
            x, y, (uint16_t)color);
    }
}

static void high_score_draw_centered_text(void *user_data, const char *text,
    int32_t x, int32_t y, int32_t width, int32_t font, int32_t color)
{
    PharaohWindowsStartupHost *const host = user_data;
    PharaohBitmapFont bitmap_font = { 0 };
    if (text == NULL || font < 634 || font > 641) return;
    bitmap_font.images = host->sprite_images;
    bitmap_font.image_count = host->loaded_libraries->next_image;
    bitmap_font.first_image = menu_group_image(host, (uint32_t)font);
    bitmap_font.measure_space = font == 634 ? 4 :
        (font == 640 || font == 641) ? 10 : 6;
    bitmap_font.draw_space = font == 634 ? 4 :
        (font == 640 || font == 641) ? 8 : 6;
    bitmap_font.measure_gap = font == 634 || font >= 640 ? 1 : 0;
    bitmap_font.draw_gap = font == 634 ? 1 : 0;
    bitmap_font.baseline = font == 634 ? 9 : font == 640 ? 23 :
        font == 641 ? 24 : 11;
    bitmap_font.user_data = host;
    bitmap_font.draw_glyph = draw_font_glyph;
    if (bitmap_font.first_image != 0) {
        pharaoh_bitmap_text_draw_centered(&bitmap_font,
            (const uint8_t *)text, x, y, width, (uint16_t)color);
    }
}

static void high_score_draw_wrapped_text(void *user_data, const char *text,
    int32_t x, int32_t y, int32_t width, int32_t font, int32_t color)
{
    PharaohWindowsStartupHost *const host = user_data;
    PharaohBitmapFont bitmap_font = { 0 };
    if (text == NULL || font < 634 || font > 641) return;
    bitmap_font.images = host->sprite_images;
    bitmap_font.image_count = host->loaded_libraries->next_image;
    bitmap_font.first_image = menu_group_image(host, (uint32_t)font);
    bitmap_font.measure_space = font == 634 ? 4 :
        (font == 640 || font == 641) ? 10 : 6;
    bitmap_font.draw_space = font == 634 ? 4 :
        (font == 640 || font == 641) ? 8 : 6;
    bitmap_font.measure_gap = font == 634 || font >= 640 ? 1 : 0;
    bitmap_font.draw_gap = font == 634 ? 1 : 0;
    bitmap_font.baseline = font == 634 ? 9 : font == 640 ? 23 :
        font == 641 ? 24 : 11;
    bitmap_font.user_data = host;
    bitmap_font.draw_glyph = draw_font_glyph;
    if (bitmap_font.first_image != 0) {
        pharaoh_bitmap_text_draw_wrapped(&bitmap_font,
            (const uint8_t *)text, x, y, width, (uint16_t)color);
    }
}

static void draw_language_number_language(PharaohWindowsStartupHost *host,
    int32_t value, int32_t prefix_group, int32_t prefix_entry,
    int32_t suffix_group, int32_t suffix_entry, int32_t x, int32_t y,
    int32_t width, int32_t font, int32_t color)
{
    char prefix[256];
    char suffix[256] = "";
    char text[768];

    pharaoh_text_select(&host->text_selection, prefix_group, prefix_entry);
    if (host->text_selection.current == NULL) return;
    (void)strncpy_s(prefix, sizeof(prefix), host->text_selection.current,
        _TRUNCATE);
    if (suffix_group >= 0) {
        pharaoh_text_select(&host->text_selection, suffix_group, suffix_entry);
        if (host->text_selection.current != NULL) {
            (void)strncpy_s(suffix, sizeof(suffix),
                host->text_selection.current, _TRUNCATE);
        }
    }
    (void)snprintf(text, sizeof(text), "%s %d %s", prefix, value, suffix);
    high_score_draw_centered_text(host, text, x, y, width, font, color);
}

static void draw_two_languages(PharaohWindowsStartupHost *host,
    int32_t first_group, int32_t first_entry, int32_t second_group,
    int32_t second_entry, int32_t x, int32_t y, int32_t width,
    int32_t font, int32_t color)
{
    char first[256];
    char second[256];
    char text[768];

    pharaoh_text_select(&host->text_selection, first_group, first_entry);
    if (host->text_selection.current == NULL) return;
    (void)strncpy_s(first, sizeof(first), host->text_selection.current,
        _TRUNCATE);
    pharaoh_text_select(&host->text_selection, second_group, second_entry);
    if (host->text_selection.current == NULL) return;
    (void)strncpy_s(second, sizeof(second), host->text_selection.current,
        _TRUNCATE);
    (void)snprintf(text, sizeof(text), "%s %s", first, second);
    high_score_draw_centered_text(host, text, x, y, width, font, color);
}

static void draw_name_input_caret(void *user_data, const char *text,
    int32_t x, int32_t y, int32_t font, int32_t color)
{
    PharaohWindowsStartupHost *const host = user_data;
    PharaohBitmapFont bitmap_font = { 0 };
    LPDIRECTDRAWSURFACE surface =
        (LPDIRECTDRAWSURFACE)host->graphics.back_buffer_surface;
    DDSURFACEDESC description;
    DDBLTFX effects;
    RECT rectangle;

    /* 0x00537140 alternates 200 ms hidden and 400 ms visible. */
    if (surface == NULL || GetTickCount() % 600u < 200u ||
        font < 634 || font > 641) {
        return;
    }
    bitmap_font.images = host->sprite_images;
    bitmap_font.image_count = host->loaded_libraries->next_image;
    bitmap_font.first_image = menu_group_image(host, (uint32_t)font);
    bitmap_font.measure_space = font == 634 ? 4 :
        (font == 640 || font == 641) ? 10 : 6;
    bitmap_font.measure_gap = font == 634 || font >= 640 ? 1 : 0;
    rectangle.left = x + pharaoh_bitmap_text_width(&bitmap_font,
        (const uint8_t *)text);
    rectangle.top = y;
    rectangle.right = rectangle.left + 4;
    rectangle.bottom = y + 2;
    memset(&description, 0, sizeof(description));
    description.dwSize = sizeof(description);
    memset(&effects, 0, sizeof(effects));
    effects.dwSize = sizeof(effects);
    effects.dwFillColor = (DWORD)color;
    if (surface->lpVtbl->GetSurfaceDesc(surface, &description) == DD_OK &&
        description.ddpfPixelFormat.dwRGBBitCount == 32) {
        const uint32_t pixel = (uint16_t)color;
        const int32_t source_is_565 = host->sprite_pixels.pixel_mode ==
            PHARAOH_SPRITE_PIXEL_MODE_RGB565;
        const uint32_t red = (pixel >> (source_is_565 ? 11 : 10)) & 31u;
        const uint32_t green = (pixel >> 5) & (source_is_565 ? 63u : 31u);
        const uint32_t blue = pixel & 31u;
        const uint32_t red8 = (red << 3) | (red >> 2);
        const uint32_t green8 = source_is_565 ?
            (green << 2) | (green >> 4) : (green << 3) | (green >> 2);
        const uint32_t blue8 = (blue << 3) | (blue >> 2);
        effects.dwFillColor = (red8 << 16) | (green8 << 8) | blue8;
    }
    (void)surface->lpVtbl->Blt(surface, &rectangle, NULL, NULL,
        DDBLT_COLORFILL | DDBLT_WAIT, &effects);
}

static int32_t high_score_draw_number_end(void *user_data, int32_t value,
    int32_t prefix, int32_t x, int32_t y, int32_t font, int32_t color)
{
    PharaohWindowsStartupHost *const host = user_data;
    PharaohBitmapFont bitmap_font = { 0 };
    char text[48];
    int32_t offset = 0;
    if (prefix != 0) text[offset++] = (char)prefix;
    const int32_t digits = snprintf(text + offset, sizeof(text) -
        (size_t)offset, "%d", value);
    if (digits < 0 || (size_t)digits >= sizeof(text) - (size_t)offset) return x;
    offset += digits;
    /* 0x004CCE00 receives 0x0057C048, the native one-space suffix. */
    if ((size_t)offset + 1u >= sizeof(text)) return x;
    text[offset++] = ' ';
    text[offset] = '\0';
    high_score_draw_text(user_data, text, x, y, font, color);
    if (font < 634 || font > 641) return x;
    bitmap_font.images = host->sprite_images;
    bitmap_font.image_count = host->loaded_libraries->next_image;
    bitmap_font.first_image = menu_group_image(host, (uint32_t)font);
    bitmap_font.measure_space = font == 634 ? 4 :
        (font == 640 || font == 641) ? 10 : 6;
    bitmap_font.draw_space = font == 634 ? 4 :
        (font == 640 || font == 641) ? 8 : 6;
    bitmap_font.measure_gap = font == 634 || font >= 640 ? 1 : 0;
    if (bitmap_font.first_image == 0) return x;
    /* 0x004CC770 advances unmapped '@', the explicit suffix space, and
       one further space at the terminator; DAT_00E20778 includes all. */
    return x + pharaoh_bitmap_text_draw_advance(&bitmap_font,
        (const uint8_t *)text) + bitmap_font.draw_space;
}

static void high_score_draw_number(void *user_data, int32_t value,
    int32_t prefix, int32_t x, int32_t y, int32_t font, int32_t color)
{
    (void)high_score_draw_number_end(user_data, value, prefix, x, y,
        font, color);
}

static void high_score_draw_sprite(void *user_data, int32_t image,
    int32_t x, int32_t y)
{
    draw_menu_image(user_data, image, x, y);
}

void pharaoh_windows_startup_host_bind_high_scores(
    PharaohWindowsStartupHost *host, PharaohHighScoresOperations *operations)
{
    memset(operations, 0, sizeof(*operations));
    operations->user_data = host;
    operations->read_records = high_score_read_records;
    operations->draw_panel = high_score_draw_panel;
    operations->draw_inner_panel = high_score_draw_inner_panel;
    operations->draw_centered_language = high_score_draw_centered_language;
    operations->draw_language = high_score_draw_language;
    operations->draw_text = high_score_draw_text;
    operations->draw_number = high_score_draw_number;
    operations->draw_sprite = high_score_draw_sprite;
}

void pharaoh_windows_startup_host_draw_player_selection(
    PharaohWindowsStartupHost *host,
    const PharaohPlayerSelectionState *selection, int32_t focused_button)
{
    const int32_t origin_x = (host->graphics.render_width - 640) / 2;
    const int32_t origin_y = (host->graphics.render_height - 480) / 2;
    PharaohPlayerSelectionDrawOperations operations = { 0 };

    pharaoh_windows_startup_host_draw_boot_background(host, 0x2f);
    operations.user_data = host;
    operations.draw_language = draw_menu_language;
    operations.draw_text = high_score_draw_text;
    operations.draw_sprite = high_score_draw_sprite;
    operations.panel_image_base = (int32_t)menu_group_image(host,
        PHARAOH_IMAGE_GROUP_OUTER_PANEL);
    operations.inner_panel_image_base = (int32_t)menu_group_image(host,
        PHARAOH_IMAGE_GROUP_INNER_PANEL);
    operations.button_image_base = (int32_t)menu_group_image(host,
        PHARAOH_IMAGE_GROUP_BUTTON_BORDER);
    operations.arrow_image_base = (int32_t)menu_group_image(host, 365);
    operations.scrollbar_thumb_image =
        (int32_t)menu_group_image(host, PHARAOH_IMAGE_GROUP_PANEL_BUTTON) +
            0x27;
    operations.accept_image = (int32_t)menu_group_image(host, 461);
    pharaoh_player_selection_draw(selection, &operations,
        origin_x, origin_y, focused_button);
}

void pharaoh_windows_startup_host_draw_player_name_input(
    PharaohWindowsStartupHost *host,
    const PharaohPlayerSelectionState *selection,
    int32_t list_button_focused)
{
    const int32_t origin_x = (host->graphics.render_width - 640) / 2;
    const int32_t origin_y = (host->graphics.render_height - 480) / 2;
    PharaohPlayerSelectionDrawOperations operations = { 0 };

    /* 0x00520B60 uses the state-zero background image (group 14). */
    pharaoh_windows_startup_host_draw_boot_background(host, 0);
    operations.user_data = host;
    operations.draw_language = draw_menu_language;
    operations.draw_text = high_score_draw_text;
    operations.draw_caret = draw_name_input_caret;
    operations.draw_sprite = high_score_draw_sprite;
    operations.panel_image_base = (int32_t)menu_group_image(host,
        PHARAOH_IMAGE_GROUP_OUTER_PANEL);
    operations.inner_panel_image_base = (int32_t)menu_group_image(host,
        PHARAOH_IMAGE_GROUP_INNER_PANEL);
    operations.button_image_base = (int32_t)menu_group_image(host,
        PHARAOH_IMAGE_GROUP_BUTTON_BORDER);
    operations.accept_image = (int32_t)menu_group_image(host, 461);
    pharaoh_player_name_input_draw(selection, &operations,
        origin_x, origin_y, list_button_focused);
    host->graphics.redraw_requested = 0;
}

void pharaoh_windows_startup_host_draw_player_name_choices(
    PharaohWindowsStartupHost *host,
    const PharaohPlayerSelectionState *selection, int32_t focused_button)
{
    const int32_t origin_x = (host->graphics.render_width - 640) / 2;
    const int32_t origin_y = (host->graphics.render_height - 480) / 2;
    PharaohPlayerSelectionDrawOperations operations = { 0 };

    pharaoh_windows_startup_host_draw_boot_background(host, 0);
    operations.user_data = host;
    operations.draw_language = draw_menu_language;
    operations.draw_text = high_score_draw_text;
    operations.draw_sprite = high_score_draw_sprite;
    operations.panel_image_base = (int32_t)menu_group_image(host,
        PHARAOH_IMAGE_GROUP_OUTER_PANEL);
    operations.button_image_base = (int32_t)menu_group_image(host,
        PHARAOH_IMAGE_GROUP_BUTTON_BORDER);
    pharaoh_player_name_choices_draw(selection, &operations,
        origin_x, origin_y, focused_button);
    host->graphics.redraw_requested = 0;
}

void pharaoh_windows_startup_host_draw_family_message(
    PharaohWindowsStartupHost *host, int32_t text_entry,
    int32_t button_count, int32_t focused_button)
{
    const int32_t origin_x = (host->graphics.render_width - 640) / 2;
    const int32_t origin_y = (host->graphics.render_height - 480) / 2;
    const int32_t panel_x = origin_x + 80;
    const int32_t panel_y = origin_y + 80;
    const PharaohPanelFrameOperations frame = {
        host, draw_menu_image,
        (int32_t)menu_group_image(host, PHARAOH_IMAGE_GROUP_OUTER_PANEL), 0
    };
    const char *body;
    /* Native table 0x005c8168 stores button style 4, image group 365,
       then image offsets 0 and 4.  The group field is not SG3 group 4. */
    const int32_t image_base = (int32_t)menu_group_image(host, 365);

    pharaoh_panel_frame_draw(&frame, panel_x, panel_y, 30, 10);
    draw_menu_language(host, 5, text_entry, panel_x, panel_y + 20,
        480, 640, 0);
    pharaoh_text_select(&host->text_selection, 5, text_entry + 1);
    body = host->text_selection.current;
    if (body != NULL) {
        if (message_box_measure_text(host, body, 635) < 420) {
            high_score_draw_centered_text(host, body, panel_x,
                panel_y + 60, 480, 635, 0);
        } else {
            high_score_draw_wrapped_text(host, body, panel_x + 30,
                panel_y + 60, 420, 635, 0);
        }
    }
    draw_menu_image(host, image_base + (focused_button == 1),
        panel_x + 256, panel_y + 100);
    if (button_count == 2) {
        draw_menu_image(host, image_base + 4 + (focused_button == 2),
            panel_x + 192, panel_y + 100);
    }
    host->graphics.redraw_requested = 0;
}

void pharaoh_windows_startup_host_draw_custom_map_list(
    PharaohWindowsStartupHost *host,
    const PharaohCustomMapSelectionState *selection, int32_t campaign_mode,
    int32_t returns_to_front_menu, int32_t map_mode,
    int32_t focused_button, int32_t hovered_row)
{
    const int32_t origin_x = (host->graphics.render_width - 640) / 2;
    const int32_t origin_y = (host->graphics.render_height - 480) / 2;
    const uint32_t background = menu_group_image(host,
        returns_to_front_menu != 0 ? 15u : campaign_mode != 0 ? 33u : 32u);
    const int32_t rows = campaign_mode != 0 ? 13 : 15;
    const int32_t vertical_shift = campaign_mode != 0 ? 32 : 0;
    const PharaohPanelFrameOperations inner = {
        host, draw_menu_image, 0,
        (int32_t)menu_group_image(host, PHARAOH_IMAGE_GROUP_INNER_PANEL)
    };
    const uint8_t *const metadata = selection->preview.metadata;

    (void)host->graphics_operations.fill_surface(host,
        host->graphics.back_buffer_surface, DDBLT_COLORFILL | DDBLT_WAIT, 0);
    draw_menu_image(host, (int32_t)background,
        returns_to_front_menu != 0 ? 0 : origin_x - 192,
        returns_to_front_menu != 0 ? 0 : origin_y - 144);
    /* 0041F220 / 0041E1B0: list well and scrollbar well precede text. */
    pharaoh_inner_panel_frame_draw(&inner, origin_x + 15,
        origin_y + 0xcd + vertical_shift, 16, rows + 1);
    pharaoh_inner_panel_frame_draw(&inner, origin_x + 0x11e,
        origin_y + 0xf2 + vertical_shift, 2, rows - 3);
    for (int32_t row = 0; row < rows; ++row) {
        const int32_t index = selection->first_visible_file + row;
        const char *const name = pharaoh_file_list_name(selection->files,
            index);
        if (name != NULL) {
            int32_t font;
            /* 0x0041F220: 637 marks the hovered row, or the retained
               selection while no row is hovered.  Normal rows use 636 and
               unavailable kind-2 entries use 638. */
            if (row == hovered_row ||
                (hovered_row < 0 && index == selection->selected_file)) {
                font = 637;
            } else if (pharaoh_map_selection_entry_kind(selection,
                    index) == 2) {
                font = 638;
            } else {
                font = 636;
            }
            high_score_draw_text(host, name, origin_x + 24,
                origin_y + 220 + vertical_shift + row * 16, font, 0);
        }
    }
    if (campaign_mode != 0) {
        draw_menu_language(host, 293, 6, origin_x + 20, origin_y + 17,
            600, 640, 0);
        main_menu_draw_border(host, 0, origin_x + 32, origin_y + 208,
            160, 25, 1);
        draw_menu_language(host, 294, 38, origin_x + 36, origin_y + 215,
            152, 635, 0);
        main_menu_draw_border(host, 0, origin_x + 195, origin_y + 208,
            93, 25, focused_button == 8);
        draw_menu_language(host, 294, 39, origin_x + 199, origin_y + 215,
            85, 635, 0);
    }
    if (selection->preview.valid != 0) {
        int16_t preview_image;
        int32_t climate;
        int32_t duration;
        memcpy(&preview_image, metadata + 0x286, sizeof(preview_image));
        if (preview_image >= 0) {
            const uint32_t preview_group = preview_image < 19 ? 28u : 748u;
            const int32_t image_offset = preview_image < 19 ?
                preview_image : preview_image - 19;
            draw_menu_image(host,
                (int32_t)menu_group_image(host, preview_group) + image_offset,
                origin_x + 78, origin_y + (campaign_mode != 0 ? 56 : 36));
        }
        high_score_draw_centered_text(host, selection->selected_name,
            origin_x + 335, origin_y + 65, 260, 636, 0);
        high_score_draw_centered_text(host,
            (const char *)(metadata + 0x3c), origin_x + 335,
            origin_y + 85, 260, 637, 0);
        {
            int16_t start_year;
            memcpy(&start_year, metadata, sizeof(start_year));
            if (start_year < 0) {
                const int32_t end_x = high_score_draw_number_end(host,
                    -(int32_t)start_year, 0x20, origin_x + 410,
                    origin_y + 105, 636, 0);
                (void)high_score_draw_language(host, 20, 0, end_x - 8,
                    origin_y + 105, 636, 0);
            } else {
                const int32_t end_x = high_score_draw_language(host, 20, 1,
                    origin_x + 410, origin_y + 105, 636, 0);
                high_score_draw_number(host, start_year, 0x20, end_x,
                    origin_y + 105, 636, 0);
            }
        }
        if (map_mode == 3) {
            PharaohMapSelectionRatingInput rating_input;
            int32_t event_count;
            int32_t rating;
            int32_t rating_entry;
            int32_t invasion_entry;
            memcpy(&climate, metadata + 0x3d8, sizeof(climate));
            memcpy(&duration, metadata + 0x2c, sizeof(duration));
            draw_menu_language(host, 44, (climate & 0xff) + 0x4d,
                origin_x + 335, origin_y + 125, 260, 636, 0);
            switch (duration) {
            case 0x38: duration = 0x79; break;
            case 0x54: duration = 0x7a; break;
            case 0x70: duration = 0x7b; break;
            case 0x8c: duration = 0x7c; break;
            case 0xaa: duration = 0x7d; break;
            default: duration = 0x7e; break;
            }
            draw_menu_language(host, 44, duration, origin_x + 335,
                origin_y + 145, 260, 636, 0);
            invasion_entry = selection->preview_invasion_count < 1 ? 0x70 :
                selection->preview_invasion_count < 3 ? 0x71 :
                selection->preview_invasion_count < 5 ? 0x72 :
                selection->preview_invasion_count < 11 ? 0x73 : 0x74;
            draw_menu_language(host, 44, invasion_entry, origin_x + 335,
                origin_y + 165, 260, 636, 0);

            memset(&rating_input, 0, sizeof(rating_input));
            memcpy(rating_input.rating_terms, metadata + 0x34c,
                sizeof(rating_input.rating_terms));
            memcpy(&rating_input.population_rating_enabled,
                metadata + 0x380, sizeof(int32_t));
            memcpy(&rating_input.population, metadata + 0x384,
                sizeof(int32_t));
            rating_input.invasion_count = selection->preview_invasion_count;
            memcpy(&event_count, selection->preview.events,
                sizeof(event_count));
            if (event_count < 0) event_count = 0;
            if (event_count > 150) event_count = 150;
            rating_input.events =
                (const PharaohMapSelectionRatingEvent *)(const void *)
                    (selection->preview.events + sizeof(int32_t));
            rating_input.event_count = event_count;
            rating_input.god_settings =
                (const int16_t *)(const void *)(metadata + 0x0a);
            rating = pharaoh_map_selection_rating(&rating_input);
            rating_entry = rating < 101 ? 0 : rating < 201 ? 1 :
                rating < 401 ? 2 : rating < 601 ? 3 : rating < 901 ? 4 :
                rating < 1301 ? 5 : rating < 1701 ? 6 :
                rating < 2101 ? 7 : rating <= 2500 ? 8 : 9;
            draw_menu_language(host, 32, rating_entry + 0x16,
                origin_x + 335, origin_y + 185, 260, 636, 0);
            {
                static const size_t enabled_offsets[] = {
                    0x364, 0x365, 0x367, 0x380, 0x35c, 0x378, 0x370
                };
                static const size_t value_offsets[] = {
                    0x34c, 0x350, 0x358, 0x384, 0x35c, 0x37c, 0x374
                };
                static const int32_t label_entries[] = {
                    0x81, 0x82, 0x84, 0x85, 0, 0x87, 0x86
                };
                static const int32_t row_offsets[] = {
                    0xf5, 0x105, 0x115, 0x125, 0x135, 0x145, 0x145
                };
                uint16_t objective_mode;
                memcpy(&objective_mode, metadata + 0x288,
                    sizeof(objective_mode));
                if (objective_mode == 0) {
                    draw_menu_language(host, 44, 0x7f, origin_x + 335,
                        origin_y + 0xd9, 260, 637, 0);
                    for (size_t index = 0;
                         index < sizeof(enabled_offsets) /
                            sizeof(enabled_offsets[0]); ++index) {
                        int32_t enabled;
                        int32_t value;
                        int32_t end_x;
                        memcpy(&enabled, metadata + enabled_offsets[index],
                            sizeof(enabled));
                        if ((enabled & 0xff) == 0) continue;
                        memcpy(&value, metadata + value_offsets[index],
                            sizeof(value));
                        end_x = high_score_draw_number_end(host, value, 0x40,
                            origin_x + 410, origin_y + row_offsets[index],
                            636, 0);
                        if (index == 4) {
                            (void)high_score_draw_language(host, 29,
                                (enabled < 2 ? 0 : 20) +
                                    (metadata[0x360] & 0xff),
                                end_x, origin_y + row_offsets[index],
                                636, 0);
                        } else {
                            (void)high_score_draw_language(host, 44,
                                label_entries[index], end_x,
                                origin_y + row_offsets[index], 636, 0);
                        }
                    }
                    for (size_t index = 0; index < 3; ++index) {
                        int16_t text_entry;
                        memcpy(&text_entry, metadata + 0x508 +
                            index * sizeof(text_entry), sizeof(text_entry));
                        if (text_entry != 0) {
                            draw_menu_language(host, 198, text_entry,
                                origin_x + 350,
                                origin_y + 0x155 + (int32_t)index * 16,
                                260, 636, 0);
                        }
                    }
                } else {
                    pharaoh_text_select(&host->text_selection, 145, 0);
                    high_score_draw_wrapped_text(host,
                        host->text_selection.current, origin_x + 345,
                        origin_y + 0xe1, 260, 636, 0);
                }
            }
            if (returns_to_front_menu == 0) {
                const int32_t end_x = high_score_draw_language(host, 44,
                    0xd8, origin_x + 410, origin_y + 392, 636, 0);
                (void)high_score_draw_language(host, 153,
                    host->settings.difficulty + 1, end_x,
                    origin_y + 392, 636, 0);
            }
        } else if (campaign_mode != 0) {
            const int32_t mission = selection->warning_text_id;
            if (mission >= 0 && mission < PHARAOH_CAMPAIGN_INDEX_CAPACITY) {
                const PharaohCampaignMissionRecord *const record =
                    &host->campaign.mission_records[mission];
                if (record->score == 0) {
                    draw_menu_language(host, 305, 0, origin_x + 345,
                        origin_y + 125, 260, 636, 0);
                } else {
                    pharaoh_text_select(&host->text_selection, 297, mission);
                    high_score_draw_wrapped_text(host,
                        host->text_selection.current, origin_x + 345,
                        origin_y + 125, 270, 636, 0);
                    if (metadata[0x364] != 0) {
                        draw_language_number_language(host,
                            record->rating_culture, 298, 0, -1, 0,
                            origin_x + 345, origin_y + 285, 270, 636, 0);
                    }
                    if (metadata[0x365] != 0) {
                        draw_language_number_language(host,
                            record->rating_prosperity, 298, 1, -1, 0,
                            origin_x + 345, origin_y + 301, 270, 636, 0);
                    }
                    if (metadata[0x367] != 0) {
                        draw_language_number_language(host,
                            record->rating_kingdom, 298, 3, -1, 0,
                            origin_x + 345, origin_y + 317, 270, 636, 0);
                    }
                    {
                        int32_t population_enabled;
                        memcpy(&population_enabled, metadata + 0x380,
                            sizeof(population_enabled));
                        if (population_enabled != 0) {
                            draw_language_number_language(host,
                                record->population, 298, 4, -1, 0,
                                origin_x + 345, origin_y + 333, 270, 636, 0);
                        }
                    }
                    draw_language_number_language(host, record->treasury,
                        298, 5, -1, 0, origin_x + 345, origin_y + 349,
                        270, 636, 0);
                    draw_two_languages(host, 298, 7, 153,
                        record->difficulty + 1, origin_x + 335,
                        origin_y + 365, 270, 636, 0);
                    draw_language_number_language(host,
                        record->months / 12, 298, 6, 298, 9,
                        origin_x + 345, origin_y + 381, 270, 636, 0);
                    draw_language_number_language(host, record->score,
                        298, 8, -1, 0, origin_x + 345, origin_y + 397,
                        270, 637, 0);
                }
            }
        }
    }
    {
        int32_t position;
        if (pharaoh_map_selection_scroll_thumb_offset(
                selection, &position) != 0) {
        draw_menu_image(host,
            (int32_t)menu_group_image(host, PHARAOH_IMAGE_GROUP_PANEL_BUTTON) + 0x27,
            origin_x + 0x121, origin_y + 0xf5 + vertical_shift + position);
        }
    }
    /* 0x57CC30/0x57CCC0 are the native four ImageButton records for
       scroll-up, scroll-down, back and start. Their second table moves the
       upper arrow down by 32 pixels in campaign mode. */
    draw_menu_image(host,
        (int32_t)menu_group_image(host, 365) + 8 +
            (focused_button == 1),
        origin_x + 282, origin_y + (campaign_mode != 0 ? 247 : 215));
    draw_menu_image(host,
        (int32_t)menu_group_image(host, 365) + 12 +
            (focused_button == 2),
        origin_x + 282, origin_y + 435);
    draw_menu_image(host,
        (int32_t)menu_group_image(host, 365) + 4 +
            (focused_button == 3),
        origin_x + 335, origin_y + 440);
    if (pharaoh_map_selection_image_button_count(selection) == 4) {
        draw_menu_image(host,
            (int32_t)menu_group_image(host, 461) + (focused_button == 4),
            origin_x + 600, origin_y + 440);
    }
    /* The custom-map table also exposes the native difficulty decrement and
       increment image buttons; campaign selection mode 2 suppresses them. */
    if (pharaoh_map_selection_difficulty_buttons_visible(
            returns_to_front_menu, campaign_mode, map_mode) != 0) {
        draw_menu_image(host,
            (int32_t)menu_group_image(host, 481) + (focused_button == 5),
            origin_x + 368,
            origin_y + 392);
        draw_menu_image(host,
            (int32_t)menu_group_image(host, 285) + (focused_button == 6),
            origin_x + 385,
            origin_y + 392);
    }
    if (pharaoh_map_selection_entry_kind(selection,
            selection->selected_file) == 2) {
        (void)high_score_draw_language(host, 44, 0x15,
            origin_x + 352, origin_y + 446, 636, 0);
    } else if (returns_to_front_menu == 0) {
        if (campaign_mode != 0) {
            main_menu_draw_border(host, 0, origin_x + 350,
                origin_y + 410, 250, 23, focused_button == 9);
            (void)high_score_draw_language(host, 44, 0xd9,
                origin_x + 380, origin_y + 446, 635, 0);
            (void)high_score_draw_language(host, 44, 0xd7,
                origin_x + 460, origin_y + 446, 635, 0);
            draw_menu_language(host, 44, map_mode != 3 ? 0xdd : 0xdc,
                origin_x + 354, origin_y + 414, 250, 636, 0);
        } else {
            (void)high_score_draw_language(host, 44, 0x88,
                origin_x + 505, origin_y + 446, 635, 0);
        }
    } else {
        (void)high_score_draw_language(host, 44, 0xd6,
            origin_x + 490, origin_y + 446, 635, 0);
    }
    host->graphics.redraw_requested = 0;
}

void pharaoh_windows_startup_host_draw_campaign_menu(
    PharaohWindowsStartupHost *host, int32_t selected_period,
    int32_t latest_period, int32_t campaign_mode,
    int32_t focused_button)
{
    static const int16_t period_buttons[9][4] = {
        { 20, 291, 164, 316 }, { 20, 321, 164, 346 },
        { 172, 261, 316, 286 }, { 172, 291, 316, 316 },
        { 172, 321, 316, 346 }, { 20, 391, 164, 416 },
        { 20, 421, 164, 446 }, { 172, 391, 316, 416 },
        { 172, 421, 316, 446 }
    };
    const int32_t origin_x = (host->graphics.render_width - 640) / 2;
    const int32_t origin_y = (host->graphics.render_height - 480) / 2;
    int32_t selected_enabled = 0;

    /* Complete background behavior of native 0x0041C2D0. */
    (void)host->graphics_operations.fill_surface(host,
        host->graphics.back_buffer_surface, DDBLT_COLORFILL | DDBLT_WAIT, 0);
    draw_menu_image(host, (int32_t)menu_group_image(host, 33),
        origin_x - 192, origin_y - 144);

    if (selected_period >= 0 && selected_period < 9) {
        int32_t preview_group;
        int32_t preview_offset;
        if (selected_period < 5) {
            preview_group = 28;
            preview_offset = selected_period;
        } else {
            static const int8_t offsets[4] = { 4, 2, 5, 7 };
            preview_group = 748;
            preview_offset = offsets[selected_period - 5];
        }
        draw_menu_image(host,
            (int32_t)menu_group_image(host, (uint32_t)preview_group) +
                preview_offset,
            origin_x + 78, origin_y + (campaign_mode == 0 ? 60 : 56));
    }

    /* Native 0x0041BE10 uses text 293:5/6 for the window title. */
    draw_menu_language(host, 293, campaign_mode == 0 ? 5 : 6,
        origin_x + 20, origin_y + 17, 600, 640, 0);
    if (selected_period >= 0 && selected_period < 9) {
        const int32_t description_variant = campaign_mode != 0 ? 2 :
            selected_period < latest_period ? 1 :
            selected_period == latest_period ? 2 : 3;
        draw_menu_language(host, 294, selected_period * 4,
            origin_x + 339, origin_y + 60, 283, 641, 0);
        pharaoh_text_select(&host->text_selection, 294,
            selected_period * 4 + description_variant);
        high_score_draw_wrapped_text(host, host->text_selection.current,
            origin_x + 347, origin_y + 116, 269, 636, 0);
    }

    for (int32_t index = 0; index < 9; ++index) {
        const int32_t x = origin_x + period_buttons[index][0];
        const int32_t y = origin_y + period_buttons[index][1];
        const int32_t width = period_buttons[index][2] -
            period_buttons[index][0];
        const int32_t height = period_buttons[index][3] -
            period_buttons[index][1];
        const int32_t disabled = campaign_mode == 0 && index != latest_period;
        const int32_t hovered = !disabled && focused_button == index + 1;
        if (index == selected_period && !disabled) selected_enabled = 1;
        (void)height;
        draw_large_label_static(host, x, y, width / 16, hovered);
        draw_menu_language(host, 27, index, x, y + 6, width,
            disabled ? 638 : 635, 0);
    }
    draw_menu_language(host, 294, 0x29, origin_x + 30,
        origin_y + 266, 144, 635, 0);
    draw_menu_language(host, 294, 0x2a, origin_x + 30,
        origin_y + 366, 144, 635, 0);

    if (campaign_mode == 1) {
        main_menu_draw_border(host, 0, origin_x + 32, origin_y + 208,
            160, 25, focused_button == 10);
        draw_menu_language(host, 294, 0x26, origin_x + 36,
            origin_y + 215, 152, 635, 0);
        /* Campaigns is the selected native tab (0041BE10). */
        main_menu_draw_border(host, 0, origin_x + 195, origin_y + 208,
            93, 25, 1);
        draw_menu_language(host, 294, 0x27, origin_x + 199,
            origin_y + 215, 85, 635, 0);
    }

    if (selected_enabled) {
        draw_menu_image(host, (int32_t)menu_group_image(host, 461) +
            (focused_button == 12), origin_x + 600, origin_y + 440);
        if (campaign_mode == 0) {
            (void)high_score_draw_language(host, 294, 0x24,
                origin_x + 420, origin_y + 445, 635, 0);
        } else {
            draw_menu_image(host, (int32_t)menu_group_image(host, 365) + 4 +
                (focused_button == 13), origin_x + 335, origin_y + 440);
            (void)high_score_draw_language(host, 294, 0x25,
                origin_x + 550, origin_y + 445, 635, 0);
            (void)high_score_draw_language(host, 44, 0xd9,
                origin_x + 380, origin_y + 445, 635, 0);
        }
    }
    host->graphics.redraw_requested = 0;
}

void pharaoh_windows_startup_host_draw_saved_game_list(
    PharaohWindowsStartupHost *host, const PharaohFileList *files,
    int32_t first_visible, int32_t selected_row)
{
    enum { VISIBLE_ROWS = 12, SCROLL_RANGE = 0x82 };
    const int32_t origin_x = (host->graphics.render_width - 640) / 2;
    const int32_t origin_y = (host->graphics.render_height - 480) / 2;
    const uint32_t background = menu_group_image(host, 31);
    const PharaohPanelFrameOperations panel = {
        host, draw_menu_image,
        (int32_t)menu_group_image(host, PHARAOH_IMAGE_GROUP_OUTER_PANEL), 0
    };
    (void)selected_row;

    (void)host->graphics_operations.fill_surface(host,
        host->graphics.back_buffer_surface, DDBLT_COLORFILL | DDBLT_WAIT, 0);
    draw_menu_image(host, (int32_t)background, origin_x - 192,
        origin_y - 144);
    pharaoh_panel_frame_draw(&panel, origin_x + 128, origin_y + 40, 24, 21);
    draw_menu_language(host, 43, 1, origin_x + 160, origin_y + 50,
        304, 640, 0xffff);
    for (int32_t row = 0; row < VISIBLE_ROWS; ++row) {
        const int32_t index = first_visible + row;
        const char *const name = pharaoh_file_list_name(files, index);
        if (name != NULL) {
            high_score_draw_text(host, name, origin_x + 160,
                origin_y + 114 + row * 16, 635, 0xffff);
        }
    }
    draw_menu_language(host, 43, 5, origin_x + 224, origin_y + 342,
        192, 635, 0xf800);
    if (files->count > VISIBLE_ROWS) {
        int32_t percent = 0;
        if (first_visible > 0) {
            const int32_t last = files->count - VISIBLE_ROWS;
            percent = first_visible >= last ? 100 :
                first_visible * 100 / last;
        }
        draw_menu_image(host,
            (int32_t)menu_group_image(host, PHARAOH_IMAGE_GROUP_PANEL_BUTTON) + 39,
            origin_x + 0x1d8, origin_y + 0x91 + SCROLL_RANGE * percent / 100);
    }
    host->graphics.redraw_requested = 0;
}

static int32_t message_box_measure_text(void *user_data, const char *text,
    int32_t font)
{
    PharaohWindowsStartupHost *const host = user_data;
    PharaohBitmapFont bitmap_font = { 0 };
    const int32_t is_small_font = font == 634;
    const int32_t is_large_font = font == 640 || font == 641;
    if (font < 634 || font > 641 || text == NULL) return 0;
    bitmap_font.images = host->sprite_images;
    bitmap_font.image_count = host->loaded_libraries->next_image;
    bitmap_font.first_image = menu_group_image(host, (uint32_t)font);
    bitmap_font.measure_space = is_small_font ? 4 : is_large_font ? 10 : 6;
    bitmap_font.draw_space = is_small_font ? 4 : is_large_font ? 8 : 6;
    bitmap_font.measure_gap = is_small_font || is_large_font ? 1 : 0;
    bitmap_font.draw_gap = is_small_font ? 1 : 0;
    bitmap_font.baseline = is_small_font ? 9 : font == 640 ? 23 : font == 641 ? 24 : 11;
    bitmap_font.user_data = host;
    bitmap_font.draw_glyph = draw_font_glyph;
    return bitmap_font.first_image == 0 ? 0 :
        pharaoh_bitmap_text_width(&bitmap_font, (const uint8_t *)text);
}

static void message_box_draw_panel(void *user_data, int32_t x, int32_t y,
    int32_t width_blocks, int32_t height_blocks)
{
    const PharaohPanelFrameOperations frame = {
        user_data, draw_menu_image,
        general_local_image(user_data,
            PHARAOH_GENERAL_OUTER_PANEL_LOCAL_IMAGE), 0
    };
    pharaoh_panel_frame_draw(&frame, x, y, width_blocks, height_blocks);
}

static void message_box_draw_inner_panel(void *user_data, int32_t x,
    int32_t y, int32_t width_blocks, int32_t height_blocks)
{
    const PharaohPanelFrameOperations frame = {
        user_data, draw_menu_image, 0,
        general_local_image(user_data,
            PHARAOH_GENERAL_INNER_PANEL_LOCAL_IMAGE)
    };
    pharaoh_inner_panel_frame_draw(&frame, x, y, width_blocks, height_blocks);
}

static void message_box_draw_centered_text(void *user_data, const char *text,
    int32_t x, int32_t y, int32_t width, int32_t font)
{
    PharaohWindowsStartupHost *const host = user_data;
    PharaohBitmapFont bitmap_font = { 0 };
    const int32_t is_small_font = font == 634;
    const int32_t is_large_font = font == 640 || font == 641;
    if (font < 634 || font > 641 || text == NULL) return;
    bitmap_font.images = host->sprite_images;
    bitmap_font.image_count = host->loaded_libraries->next_image;
    bitmap_font.first_image = menu_group_image(host, (uint32_t)font);
    bitmap_font.measure_space = is_small_font ? 4 : is_large_font ? 10 : 6;
    bitmap_font.draw_space = is_small_font ? 4 : is_large_font ? 8 : 6;
    bitmap_font.measure_gap = is_small_font || is_large_font ? 1 : 0;
    bitmap_font.draw_gap = is_small_font ? 1 : 0;
    bitmap_font.baseline = is_small_font ? 9 : font == 640 ? 23 : font == 641 ? 24 : 11;
    bitmap_font.user_data = host;
    bitmap_font.draw_glyph = draw_font_glyph;
    if (bitmap_font.first_image != 0) {
        pharaoh_bitmap_text_draw_centered(&bitmap_font,
            (const uint8_t *)text, x, y, width, 0);
    }
}

static void message_box_draw_button_label(void *user_data, int32_t group,
    int32_t entry, int32_t x, int32_t y, int32_t width, int32_t font)
{
    draw_menu_language(user_data, group, entry, x, y, width, font, 0);
}

static void message_box_draw_button(void *user_data, int32_t x, int32_t y,
    int32_t width, int32_t height, uint32_t color)
{
    PharaohMenuButtonFrameOperations frame = { user_data, draw_menu_image };
    pharaoh_menu_button_frame_draw(&frame,
        /* FUN_00536c30 reads DAT_00f5698c: the first image of native
           image group 443.  The SG3 source number 715 is one-based before
           the 200 system images are skipped, so it is not a runtime id. */
        (int32_t)menu_group_image(user_data, 443),
        x, y, width, height, color != 0);
}

int32_t pharaoh_windows_startup_host_draw_message_box(
    PharaohWindowsStartupHost *host, PharaohMessageBoxState *state,
    const char *title, const char *body, uint32_t type)
{
    const PharaohMessageBoxOperations operations = {
        host,
        message_box_measure_text,
        message_box_draw_panel,
        message_box_draw_inner_panel,
        message_box_draw_centered_text,
        message_box_draw_button_label,
        message_box_draw_button
    };
    return pharaoh_message_box_draw(state, &operations, title, body, type);
}

void pharaoh_windows_startup_host_draw_boot_foreground(
    PharaohWindowsStartupHost *host, int32_t focused_button)
{
    const int32_t origin_x = (host->graphics.render_width - 640) / 2;
    const int32_t origin_y = (host->graphics.render_height - 480) / 2;
    if (host->current_window_id == 0x3c) {
        /* LogoWindowDraw: language 13/7, centered width 320 at UI y+462. */
        draw_menu_language(host, 13, 7,
            (host->graphics.render_width - 320) / 2,
            origin_y + 462, 320, 634, 0xffff);
    } else if (host->current_window_id == 0) {
        /* Native loading writes 100 to F53F28, the label slot consumed here. */
        int32_t animations[7] = { 0 };
        animations[6] = host->asset_state.loading_progress_percent;
        PharaohLabelRenderer labels = { 0 };
        PharaohMenuRenderer renderer = { 0 };
        PharaohMainMenuState menu = { 0 };
        labels.origin_x = origin_x;
        labels.origin_y = origin_y;
        labels.block_size = 16;
        labels.opening_animation = 5;
        labels.growing_animation = 15;
        labels.blocks_offset = 13;
        labels.pieces_per_style = 3;
        labels.panel_button_images = (int32_t)menu_group_image(host, 284);
        labels.animations = animations;
        labels.draw_image = draw_menu_image;
        labels.user_data = host;
        renderer.labels = &labels;
        renderer.context = host;
        renderer.draw_centered = draw_menu_language;
        menu.focused_button = focused_button;
        menu.font = 635;
        pharaoh_front_menu_draw(&renderer, &menu,
            pharaoh_front_menu_rects, pharaoh_front_menu_labels);
    }
}

void pharaoh_windows_startup_host_draw_main_menu(
    PharaohWindowsStartupHost *host, const PharaohMainMenuWindowState *state,
    const char *player_name, int32_t focused_button)
{
    const int32_t origin_x = (host->graphics.render_width - 640) / 2;
    const int32_t origin_y = (host->graphics.render_height - 480) / 2;
    const uint32_t background = menu_group_image(host, 31);
    const PharaohPanelFrameOperations panel = {
        host, draw_menu_image,
        (int32_t)menu_group_image(host, PHARAOH_IMAGE_GROUP_OUTER_PANEL), 0
    };
    PharaohLabelRenderer labels = { 0 };
    PharaohMenuRenderer renderer = { 0 };
    PharaohMainMenuState menu = { 0 };
    const PharaohMainMenuSound sound = {
        host, main_menu_sound_update, main_menu_sound_is_playing,
        main_menu_sound_play
    };
    char title_template[200];
    char title[200];
    const char *token;

    (void)host->graphics_operations.fill_surface(host,
        host->graphics.back_buffer_surface, DDBLT_COLORFILL | DDBLT_WAIT, 0);
    draw_menu_image(host, (int32_t)background, origin_x, origin_y);
    pharaoh_panel_frame_draw(&panel, origin_x + 128, origin_y + 40, 24, 21);
    pharaoh_text_select(&host->text_selection, 293, 5);
    title[0] = '\0';
    if (host->text_selection.current != NULL) {
        (void)strncpy_s(title_template, sizeof(title_template),
            host->text_selection.current, _TRUNCATE);
        token = strstr(title_template, "[player_name]");
        if (token != NULL) {
            const int32_t prefix_length = (int32_t)(token - title_template);
            (void)snprintf(title, sizeof(title), "%.*s%s%s", prefix_length,
                title_template, player_name != NULL ? player_name : "",
                token + 13);
        } else {
            (void)strncpy_s(title, sizeof(title), title_template, _TRUNCATE);
        }
    }
    high_score_draw_centered_text(host, title, origin_x + 140,
        origin_y + 60, 368, 640, 0);

    labels.origin_x = origin_x;
    labels.origin_y = origin_y;
    labels.panel_button_images = (int32_t)menu_group_image(host, 284);
    labels.draw_image = draw_menu_image;
    labels.user_data = host;
    renderer.labels = &labels;
    renderer.context = host;
    renderer.draw_centered = draw_menu_language;
    renderer.draw_border = main_menu_draw_border;
    menu.focused_button = focused_button;
    menu.font = 635;
    menu.digital_sound_ok = host->sound.ready;
    menu.music_enabled = host->settings.sound_music_enabled;
    menu.can_start = state->campaign_period_open;
    menu.has_saves = state->save_directory_has_files;
    menu.can_continue = state->selected_save_available;
    pharaoh_main_menu_draw_foreground(&renderer, &menu,
        &sound, pharaoh_game_menu_rects);
    host->graphics.redraw_requested = 0;
}

static void draw_loading_status_text(void *user_data, const char *text)
{
    PharaohWindowsStartupHost *const host = user_data;
    LPDIRECTDRAWSURFACE surface =
        (LPDIRECTDRAWSURFACE)host->graphics.back_buffer_surface;
    HDC device_context = NULL;
    SIZE extent;
    RECT background;
    RECT border;
    HBRUSH black_brush;
    HBRUSH white_brush;
    const int32_t text_length = lstrlenA(text);
    int32_t text_x;
    int32_t text_y;

    if (host->startup_video_started != 0 || text_length == 0 || surface->lpVtbl->GetDC(surface,
            &device_context) != DD_OK) {
        return;
    }
    if (GetTextExtentPoint32A(device_context, text, text_length, &extent) != 0) {
        text_x = (host->graphics.render_width - extent.cx) / 2;
        text_y = ((host->graphics.render_height - extent.cy) / 2) * 3 / 2;
        background.left = text_x - 26;
        background.top = text_y - 11;
        background.right = text_x + extent.cx + 26;
        background.bottom = text_y + extent.cy + 11;
        border.left = background.left + 1;
        border.top = background.top + 1;
        border.right = background.right - 1;
        border.bottom = background.bottom - 1;
        black_brush = CreateSolidBrush(RGB(0, 0, 0));
        white_brush = CreateSolidBrush(RGB(255, 255, 255));
        if (black_brush != NULL && white_brush != NULL) {
            (void)FillRect(device_context, &background, black_brush);
            (void)FillRect(device_context, &border, white_brush);
            (void)SetBkMode(device_context, TRANSPARENT);
            (void)SetTextColor(device_context, RGB(0, 0, 0));
            (void)TextOutA(device_context, text_x, text_y, text, text_length);
        }
        if (black_brush != NULL) {
            (void)DeleteObject(black_brush);
        }
        if (white_brush != NULL) {
            (void)DeleteObject(white_brush);
        }
    }
    (void)surface->lpVtbl->ReleaseDC(surface, device_context);
}

static void present_loading_surface(PharaohWindowsStartupHost *host)
{
    RECT destination;
    RECT source;
    if (pharaoh_windows_startup_host_refresh_video_bounds(host) == 0) {
        return;
    }
    destination.left = host->video_screen_bounds.left;
    destination.top = host->video_screen_bounds.top;
    destination.right = host->video_screen_bounds.right;
    destination.bottom = host->video_screen_bounds.bottom;
    source.left = 0;
    source.top = 0;
    source.right = host->graphics.render_width;
    source.bottom = host->graphics.render_height;
    (void)((LPDIRECTDRAWSURFACE)host->graphics.primary_surface)->lpVtbl->Blt(
        host->graphics.primary_surface, &destination,
        host->graphics.back_buffer_surface, &source, DDBLT_WAIT, NULL);
}

void pharaoh_windows_startup_host_present_loading(
    PharaohWindowsStartupHost *host)
{
    if (host->startup_video_started == 0) {
        pharaoh_windows_startup_host_present_frame(host);
    }
}

void pharaoh_windows_startup_host_present_frame(
    PharaohWindowsStartupHost *host)
{
    if (host->graphics.primary_surface != NULL &&
        host->graphics.back_buffer_surface != NULL) {
        present_loading_surface(host);
    }
}

int32_t pharaoh_windows_startup_host_capture_frame(
    PharaohWindowsStartupHost *host, const char *path)
{
    LPDIRECTDRAWSURFACE surface =
        (LPDIRECTDRAWSURFACE)host->graphics.back_buffer_surface;
    DDSURFACEDESC description;
    BITMAPFILEHEADER file_header;
    BITMAPINFOHEADER info_header;
    DWORD masks[3];
    FILE *file;
    uint8_t padding[3] = { 0, 0, 0 };
    uint32_t row_bytes, stored_row_bytes, image_bytes;
    uint32_t bytes_per_pixel;
    HRESULT result;

    if (surface == NULL || path == NULL) return 0;
    memset(&description, 0, sizeof(description));
    description.dwSize = sizeof(description);
    result = surface->lpVtbl->Lock(surface, NULL, &description,
        DDLOCK_WAIT, NULL);
    if (result != DD_OK) return 0;

    bytes_per_pixel = (uint32_t)description.lPitch / description.dwWidth;
    if (bytes_per_pixel != 2u && bytes_per_pixel != 4u) {
        (void)surface->lpVtbl->Unlock(surface, NULL);
        return 0;
    }
    row_bytes = description.dwWidth * bytes_per_pixel;
    stored_row_bytes = (row_bytes + 3u) & ~3u;
    image_bytes = stored_row_bytes * description.dwHeight;
    memset(&file_header, 0, sizeof(file_header));
    memset(&info_header, 0, sizeof(info_header));
    file_header.bfType = 0x4d42;
    file_header.bfOffBits = sizeof(file_header) + sizeof(info_header) +
        sizeof(masks);
    file_header.bfSize = file_header.bfOffBits + image_bytes;
    info_header.biSize = sizeof(info_header);
    info_header.biWidth = (LONG)description.dwWidth;
    info_header.biHeight = (LONG)description.dwHeight;
    info_header.biPlanes = 1;
    info_header.biBitCount = (WORD)(bytes_per_pixel * 8u);
    info_header.biCompression = BI_BITFIELDS;
    info_header.biSizeImage = image_bytes;
    if (bytes_per_pixel == 4u) {
        masks[0] = 0x00ff0000u;
        masks[1] = 0x0000ff00u;
        masks[2] = 0x000000ffu;
    } else {
        masks[0] = host->graphics.surface_context.pixel_format == 1 ?
            0xf800u : 0x7c00u;
        masks[1] = host->graphics.surface_context.pixel_format == 1 ?
            0x07e0u : 0x03e0u;
        masks[2] = 0x001fu;
    }

    file = _fsopen(path, "wb", _SH_DENYNO);
    if (file == NULL) {
        (void)surface->lpVtbl->Unlock(surface, NULL);
        return 0;
    }
    (void)fwrite(&file_header, sizeof(file_header), 1, file);
    (void)fwrite(&info_header, sizeof(info_header), 1, file);
    (void)fwrite(masks, sizeof(masks), 1, file);
    for (uint32_t row = description.dwHeight; row > 0; --row) {
        const uint8_t *source = (const uint8_t *)description.lpSurface +
            (size_t)(row - 1) * (size_t)description.lPitch;
        (void)fwrite(source, row_bytes, 1, file);
        (void)fwrite(padding, stored_row_bytes - row_bytes, 1, file);
    }
    (void)fclose(file);
    (void)surface->lpVtbl->Unlock(surface, NULL);
    return 1;
}

void pharaoh_windows_startup_host_finish_loading(
    PharaohWindowsStartupHost *host)
{
    host->startup_video_started = 1;
    /* The EXE calls 00426F60 before Bink: UpdateWindow then black-fill both
       surfaces. Disable loading paints before that potentially reentrant call. */
    clear_video_surfaces(host);
}

static void process_loading_updates(void *user_data, int32_t force_update)
{
    PharaohWindowsStartupHost *const host = user_data;
    MSG message;
    (void)force_update;
    if (host->startup_video_started != 0) {
        return;
    }
    while (PeekMessageA(&message, NULL, 0, 0, PM_REMOVE) != 0) {
        if (message.message == WM_QUIT) {
            PostQuitMessage((int)message.wParam);
            host->shutdown_requested = 1;
            break;
        }
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    present_loading_surface(host);
}

void pharaoh_windows_startup_host_draw_boot_background(
    PharaohWindowsStartupHost *host, uint8_t window_id)
{
    /* Native group.firstImage base is 00F55BB4, stride 8. Window zero
       draws 00F55C24: group 14 (31 belongs to the in-game menu). */
    enum {
        LOGO_IMAGE_GROUP = 7,
        FRONT_MENU_IMAGE_GROUP = 14
    };
    const uint32_t group = window_id == 0x3c ? LOGO_IMAGE_GROUP :
        window_id == 0x2f ? PHARAOH_IMAGE_GROUP_PLAYER_BACKGROUND :
        FRONT_MENU_IMAGE_GROUP;
    (void)host->graphics_operations.fill_surface(host,
        host->graphics.back_buffer_surface, DDBLT_COLORFILL | DDBLT_WAIT, 0);
    draw_loading_sprite(host, host->sprite_tags[group].image_index,
        (host->graphics.screen_width - 640) / 2,
        (host->graphics.screen_height - 480) / 2);
    host->graphics.redraw_requested = 0;
}

static void write_startup_status(void *user_data, const char *text)
{
    PharaohWindowsStartupHost *const host = user_data;
    pharaoh_status_log_format(text, NULL, 0, &host->log_format_state,
        &host->log_format_operations);
}

PharaohApplicationConfigurationResult pharaoh_windows_startup_host_load(
    PharaohWindowsStartupHost *host)
{
    memset(host, 0, sizeof(*host));
    pharaoh_windows_startup_file_operations_bind(
        &host->file_operations, &host->file_size_operations,
        &host->text_operations,
        &host->ini_operations, &host->media_operations);
    pharaoh_windows_status_log_append_operations_initialize(
        &host->log_append_operations);
    pharaoh_windows_status_log_format_operations_initialize(
        &host->log_format_operations, &host->log_append_operations);
    host->log_format_state.append_state = &host->log_append_state;
    host->file_status.state = &host->log_format_state;
    host->file_status.operations = &host->log_format_operations;

    host->files.operations = &host->file_operations;
    host->file_size.operations = &host->file_size_operations;
    host->file_size.user_data = host;
    host->settings_context.ini_operations = &host->ini_operations;
    host->settings_context.files = &host->files;
    host->settings_context.ini_user_data = &host->file_status;
    host->settings_context.settings = &host->settings;
    host->settings_context.menu_indexes = &host->menu_indexes;
    host->settings_context.campaign.index = &host->campaign_index;
    host->settings_context.campaign.starting_favor_by_difficulty =
        starting_favor_by_difficulty;
    host->settings_context.campaign.city_state_words =
        host->city_state_words;
    host->settings_context.campaign.city_state_slot = 0;
    host->settings_context.campaign.campaign_status =
        &host->campaign_status;
    host->settings_context.startup_finalization_flag =
        &host->startup_finalization_flag;

    host->text_context.operations = &host->text_operations;
    host->text_context.catalog = &host->text_catalog;
    host->text_selection.catalog = &host->text_catalog;
    host->default_names.group_9_entry_5 = host->default_name_1;
    host->default_names.group_9_entry_6_first_copy = host->default_name_2;
    host->default_names.group_9_entry_6_second_copy = host->default_name_3;
    host->default_names.group_9_entry_7 = host->default_name_4;

    host->configuration.settings = &host->settings_context;
    host->configuration.texts = &host->text_context;
    host->configuration.media_drive_state = &host->media_drive;
    host->configuration.media_probe_operations = &host->media_operations;
    host->configuration.detected_drive_letter =
        &host->detected_drive_letter;
    host->configuration.text_selection = &host->text_selection;
    host->configuration.default_names = &host->default_names;
    host->configuration.primary_text_filename = "Pharaoh_Text.eng";
    host->configuration.media_text_filename = "Pharaoh_MM.eng";
    return pharaoh_application_load_configuration(&host->configuration);
}

int32_t pharaoh_windows_startup_host_open_window(
    PharaohWindowsStartupHost *host, void *instance,
    PharaohApplicationWindowProcedure window_procedure)
{
    PharaohApplicationWindowStartupRequest request;

    memset(&host->graphics_operations, 0, sizeof(host->graphics_operations));
    pharaoh_windows_graphics_bind_platform_operations(
        &host->graphics_operations);
    host->graphics_operations.user_data = host;
    host->graphics_operations.get_current_window_id =
        get_current_window_id;
    host->graphics_operations.report_status = report_graphics_status;
    host->graphics_operations.dispatch_message = dispatch_graphics_message;
    pharaoh_windows_application_directory_operations_initialize(
        &host->directory_operations);
    pharaoh_windows_application_window_operations_initialize(
        &host->window_operations);
    pharaoh_windows_application_window_startup_operations_initialize(
        &host->window_startup_operations, &host->directory_operations,
        &host->graphics_operations, &host->window_operations);

    host->graphics.color_mode_selection = 0;
    host->graphics.keep_windowed_mode = 1;
    request.instance = instance;
    request.settings_display_mode = host->settings.display_mode;
    request.window_title_selection = &host->text_selection;
    request.window_procedure = window_procedure;
    return pharaoh_application_window_initialize(&host->window_startup,
        &host->graphics, &request, &host->window_startup_operations);
}

int32_t pharaoh_windows_startup_host_load_assets(
    PharaohWindowsStartupHost *host)
{
    host->game_data.files = &host->files;
    host->game_data.settings = &host->settings;
    host->game_data.text_selection = &host->text_selection;
    host->game_data.state = &host->game_data_state;
    memcpy(host->game_data_state.fallback_path, "C:\\", 4);
    host->asset_state.map_info_file_buffer = host->map_info;

    host->loaded_libraries = calloc(1, sizeof(*host->loaded_libraries));
    host->sprite_images = calloc(PHARAOH_WINDOWS_STARTUP_IMAGE_CAPACITY,
        sizeof(*host->sprite_images));
    host->sprite_tags = calloc(PHARAOH_WINDOWS_STARTUP_TAG_CAPACITY,
        sizeof(*host->sprite_tags));
    if (host->loaded_libraries == NULL || host->sprite_images == NULL ||
        host->sprite_tags == NULL) {
        return 0;
    }

    host->sprite_memory_operations.allocate = allocate_sprite_memory;
    host->sprite_memory_operations.release = release_sprite_memory;
    host->sprite_pixels.game_data = &host->game_data;
    host->sprite_pixels.memory_operations = &host->sprite_memory_operations;
    host->sprite_pixels.pixel_mode = host->graphics.surface_context.pixel_format == 1 ?
        PHARAOH_SPRITE_PIXEL_MODE_RGB565 : 0;
    host->sprite_reader.game_data = &host->game_data;
    host->sprite_reader.pixels = &host->sprite_pixels;
    host->sprite_reader.pixel_cursor = &host->loaded_libraries->pixel_bytes;
    host->sprite_libraries.libraries = host->loaded_libraries;
    host->sprite_libraries.reader = &host->sprite_reader;
    host->sprite_libraries.images = host->sprite_images;
    host->sprite_libraries.tags = host->sprite_tags;
    host->sprite_libraries.show_tag_sync_warning = report_sprite_tag_mismatch;
    host->sprite_libraries.warning_user_data = host;
    pharaoh_sprite_libraries_reset(host->loaded_libraries,
        &host->sprite_pixels);

    host->startup_asset_operations.user_data = host;
    host->startup_asset_operations.create_directory =
        create_startup_directory;
    host->startup_asset_operations.allocate = allocate_startup_pixels;
    host->startup_asset_operations.clear_loading_surface =
        clear_loading_surface;
    host->startup_asset_operations.draw_sprite = draw_loading_sprite;
    host->startup_asset_operations.draw_status_text =
        draw_loading_status_text;
    host->startup_asset_operations.process_loading_updates =
        process_loading_updates;
    host->startup_asset_operations.write_startup_status =
        write_startup_status;
    host->startup_asset_context.game_data = &host->game_data;
    host->startup_asset_context.sprite_libraries = &host->sprite_libraries;
    host->startup_asset_context.text_selection = &host->text_selection;
    host->startup_asset_context.state = &host->asset_state;
    host->startup_asset_context.operations = &host->startup_asset_operations;
    host->startup_asset_context.screen_width =
        (uint32_t)host->graphics.render_width;
    host->startup_asset_context.screen_height =
        (uint32_t)host->graphics.render_height;
    /* Native 00520B20/00520AF0 use the centered 640x480 UI origin. */
    host->startup_asset_context.loading_image_x =
        (host->graphics.render_width - 640) / 2;
    host->startup_asset_context.loading_image_y =
        (host->graphics.render_height - 480) / 2;
    /* Native image ID 509 is Load1: its one-based SG3 record number 509 is
       zero-based table entry 508, loaded at first_image 1 + entry 508. */
    host->startup_asset_context.loading_stage_image_base =
        PHARAOH_STARTUP_LOADING_FIRST_RUNTIME_IMAGE;
    host->startup_asset_context.loading_background_image =
        PHARAOH_STARTUP_EXPANSION_LOGO_RUNTIME_IMAGE;
    return pharaoh_startup_asset_setup(&host->startup_asset_context) == 0 &&
        host->shutdown_requested == 0;
}

int32_t pharaoh_windows_startup_host_load_pre_intro_data(
    PharaohWindowsStartupHost *host)
{
    const PharaohModelDifficulty difficulty =
        (PharaohModelDifficulty)host->settings.difficulty;
    void *const scratch = host->asset_state.screen_pixel_buffer;

    host->building_model_operations.user_data = host;
    host->building_model_operations.clear_bytes = clear_startup_model_bytes;
    host->building_model_operations.load_text = load_startup_model_text;
    host->building_model_operations.text_length = startup_model_text_length;
    host->building_model_operations.continue_after_count_mismatch =
        continue_after_building_model_mismatch;
    host->building_model_operations.report_status =
        report_building_model_status;
    host->building_model_context.operations =
        &host->building_model_operations;
    host->building_model_context.spare_screen = scratch;
    host->building_model_context.difficulty = difficulty;
    host->building_model_context.building_models = host->building_models;
    host->building_model_context.house_models = host->house_models;
    if (pharaoh_building_model_load(&host->building_model_context) == 0) {
        return 0;
    }

    host->figure_model_operations.user_data = host;
    host->figure_model_operations.clear_bytes = clear_startup_model_bytes;
    host->figure_model_operations.load_text = load_startup_model_text;
    host->figure_model_operations.text_length = startup_model_text_length;
    host->figure_model_operations.continue_after_count_mismatch =
        continue_after_figure_model_mismatch;
    host->figure_model_operations.report_status = report_figure_model_status;
    host->figure_model_context.operations = &host->figure_model_operations;
    host->figure_model_context.spare_screen = scratch;
    host->figure_model_context.difficulty = difficulty;
    host->figure_model_context.figure_models = &host->figure_models;
    host->figure_model_context.enemy_models = &host->enemy_models;
    if (pharaoh_figure_model_load(&host->figure_model_context) == 0) {
        return 0;
    }

    host->tax_model_operations.user_data = host;
    host->tax_model_operations.clear_bytes = clear_startup_model_bytes;
    host->tax_model_operations.load_text = load_startup_model_text;
    host->tax_model_operations.text_length = startup_model_text_length;
    host->tax_model_operations.continue_after_count_mismatch =
        continue_after_tax_model_mismatch;
    host->tax_model_operations.report_status = report_tax_model_status;
    host->tax_model_context.operations = &host->tax_model_operations;
    host->tax_model_context.spare_screen = scratch;
    host->tax_model_context.difficulty = difficulty;
    host->tax_model_context.model = &host->tax_model;
    if (pharaoh_tax_sentiment_model_load(&host->tax_model_context) == 0) {
        return 0;
    }

    host->campaign_file_operations.user_data = host;
    host->campaign_file_operations.open = open_campaign_file;
    host->campaign_file_operations.read_line = read_campaign_line;
    host->campaign_file_operations.close = close_campaign_file;
    host->campaign_file_operations.report_message = report_campaign_message;
    if (pharaoh_campaign_load(&host->campaign, "campaign.txt",
            &host->campaign_file_operations) == 0) {
        return 0;
    }
    host->campaign_index = host->campaign.index;

    host->trade_model_operations.user_data = host;
    host->trade_model_operations.clear_bytes = clear_startup_model_bytes;
    host->trade_model_operations.load_text = load_startup_model_text;
    host->trade_model_operations.text_length = startup_model_text_length;
    host->trade_model_operations.continue_after_count_mismatch =
        continue_after_trade_model_mismatch;
    host->trade_model_operations.report_status = report_trade_model_status;
    host->trade_model_context.operations = &host->trade_model_operations;
    host->trade_model_context.spare_screen = scratch;
    if (pharaoh_trade_recommendations_load(&host->trade_model_context) == 0) {
        return 0;
    }

    host->event_reason_operations.user_data = host;
    host->event_reason_operations.load_text_file = load_startup_reason_text;
    host->event_reason_operations.text_file_size = startup_reason_text_size;
    host->event_reason_operations.show_yes_no_prompt =
        show_startup_reason_prompt;
    host->event_reason_operations.report_status =
        report_startup_reason_status;
    host->event_reason_context.text_buffer = (char *)scratch;
    host->event_reason_context.text_buffer_capacity =
        PHARAOH_EVENT_REASON_TEXT_BUFFER_BYTES;
    host->event_reason_context.owner_window =
        host->window_startup.window.window;
    host->event_reason_context.table = &host->event_reason_table;
    host->event_reason_context.operations = &host->event_reason_operations;
    if (pharaoh_event_reason_phrases_load(
            &host->event_reason_context) == 0) {
        return 0;
    }

    process_loading_updates(host, 1);
    return host->shutdown_requested == 0;
}

int32_t pharaoh_windows_startup_host_play_intro(
    PharaohWindowsStartupHost *host)
{
    PharaohVideoPlayer *const player = host->window_startup.video_player;
    const char *filename;

    if (host->shutdown_requested != 0 || player == NULL ||
        pharaoh_windows_startup_host_refresh_video_bounds(host) == 0 ||
        pharaoh_windows_bink_open_library(&host->bink) == 0) {
        return 0;
    }
    memset(&host->video_loop, 0, sizeof(host->video_loop));
    memset(&host->video_open, 0, sizeof(host->video_open));
    pharaoh_windows_bink_bind_video_operations(&host->bink,
        &host->bink_player, &host->video_loop, &host->video_open,
        &host->video_blit);
    pharaoh_windows_video_io_bind(&host->video_loop, &host->video_open);
    host->video_loop.user_data = host;
    host->video_loop.clear_surfaces = clear_video_surfaces;
    host->video_loop.report_bink_error = report_bink_error;
    host->video_open.user_data = host;
    host->video_open.sound_manager_active = host->sound.ready;
    host->video_open.sound_manager = &host->sound.state;
    host->video_open.sound_operations = &host->sound.stream_operations;

    host->current_window_id = 0x44;
    host->startup_video_started = 1;
    host->video_runtime.display_mode = &host->current_window_id;
    host->video_runtime.redraw_requested = &host->graphics.redraw_requested;
    host->video_runtime.display_transition_active =
        &host->video_transition_active;
    host->video_runtime.video_system_active = 1;
    host->video_runtime.switch_display_mode = 0;
    host->video_runtime.previous_display_mode =
        (uint32_t)host->graphics.display_mode_selection;
    host->video_runtime.screen_width = (uint32_t)host->graphics.screen_width;
    host->video_runtime.screen_height = (uint32_t)host->graphics.screen_height;
    host->video_runtime.movie_drive_letter = host->detected_drive_letter;
    host->video_blit.primary_surface = host->graphics.primary_surface;
    /* Decode at (0,0) into the client-sized offscreen surface. The existing
       present adapter centers using the movie's actual dimensions and the
       refreshed client rectangle, including after a window move. */
    host->video_blit.video_surface = host->graphics.back_buffer_surface;
    host->video_blit.video_surface_width =
        (uint32_t)host->graphics.render_width;
    host->video_blit.video_surface_height =
        (uint32_t)host->graphics.render_height;
    host->video_blit.screen_bounds = &host->video_screen_bounds;
    host->video_blit.screen_bounds_user_data = host;
    host->video_blit.refresh_screen_bounds = refresh_video_screen_bounds;
    host->video_blit.write_to_primary_active = &host->video_write_to_primary;
    host->video_blit.status_state = &host->log_format_state;
    host->video_blit.status_operations = &host->log_format_operations;

    filename = host->graphics.screen_height < 0xe9 ?
        "Binks\\low\\intro.bik" : "Binks\\high\\intro_big.bik";
    const int32_t x = (host->graphics.screen_width - 0x1e0) / 2;
    const int32_t y = (host->graphics.screen_height - 0x11e) / 2;
    const int32_t played = pharaoh_video_player_play(player,
        &host->video_runtime, filename, x, y, &host->video_open);
    pharaoh_windows_bink_close_library(&host->bink);
    return played;
}

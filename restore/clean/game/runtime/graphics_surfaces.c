#include "pharaoh/runtime/graphics_startup.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

enum {
    PRIMARY_SURFACE_DESCRIPTION_SIZE = 0x6c,
    PRIMARY_SURFACE_DESCRIPTION_FLAGS = 0x01,
    PRIMARY_SURFACE_CAPABILITY = 0x0200,
    OFFSCREEN_SURFACE_DESCRIPTION_FLAGS = 0x07,
    OFFSCREEN_SURFACE_CAPABILITY = 0x0040,
    SYSTEM_MEMORY_SURFACE_CAPABILITY = 0x0800,
    SURFACE_LOCK_WAIT = 0x01,
    PIXEL_MASK_RGB_655_RED = 0xfc00,
    PIXEL_MASK_RGB_565_GREEN = 0x07e0,
    PIXEL_MASK_RGB_556_BLUE = 0x003f,
    PIXEL_FORMAT_RGB_655 = 0x0655,
    PIXEL_FORMAT_RGB_565 = 0x0565,
    PIXEL_FORMAT_RGB_556 = 0x0556,
    PIXEL_FORMAT_RGB_555 = 0x0555,
    PIXEL_FORMAT_SELECTION_655 = 3,
    PIXEL_FORMAT_SELECTION_565 = 2,
    PIXEL_FORMAT_SELECTION_556 = 1,
    PIXEL_FORMAT_SELECTION_555 = 0
};

static const char status_create_one_vram_and_system[] =
    "OK :DD Creating one Vram and one system surface.";
static const char status_create_one_vram[] = "OK :DD Creating one Vram only.";
static const char error_create_surface_windowed[] =
    "ERR:DD CreateSurface failed3:";
static const char error_create_surface_fullscreen[] =
    "ERR:DD CreateSurface failed2:";
static const char status_primary_lock_ok[] =
    "OK :DD can lock primary surface ";
static const char error_primary_lock[] = "ERR:DD surface lock error:";
static const char error_create_backbuffer[] =
    "ERR:DD Cant create(W) backbuffer1.";
static const char error_create_clipper[] = "ERR:DD Cant create clipper.";
static const char error_set_clipper_window[] =
    "ERR:DD Cant set clipper window handle.";
static const char error_attach_clipper[] =
    "ERR:DD Cant attach clipper to front buffer.";
static const char status_surfaces_clipper_ready[] =
    "OK :DD Surfaces created and clipper attached.";
static const char status_surface_ready[] = "OK :DD Surface created.";
static const char status_rgb_655[] = "OK :DD surface_format is RGB 655.";
static const char status_rgb_565[] = "OK :DD surface_format is RGB 565.";
static const char status_rgb_556[] = "OK :DD surface_format is RGB 556.";
static const char status_rgb_555[] = "OK :DD surface_format is RGB 555.";
static const char error_unsupported_rgb[] =
    "ERR:DD unsupported RGB format, trying RGB565 format";
static const char format_changed[] =
    "ERR: 16-bit surface format changed from %d to %d";
static const char error_extra_surface[] = "ERR:DD ExtraSurface failed.";

typedef struct DirectDrawErrorCharacter {
    uint32_t result;
    uint8_t character;
} DirectDrawErrorCharacter;

static const DirectDrawErrorCharacter direct_draw_error_characters[] = {
    { UINT32_C(0x88760005), UINT8_C(1) },
    { UINT32_C(0x8876000a), UINT8_C(2) },
    { UINT32_C(0x88760014), UINT8_C(3) },
    { UINT32_C(0x88760028), UINT8_C(4) },
    { UINT32_C(0x88760037), UINT8_C(5) },
    { UINT32_C(0x80004005), UINT8_C(6) },
    { UINT32_C(0x8876005a), UINT8_C(7) },
    { UINT32_C(0x8876005f), UINT8_C(8) },
    { UINT32_C(0x88760064), UINT8_C(9) },
    { UINT32_C(0x8876006e), UINT8_C(10) },
    { UINT32_C(0x88760078), UINT8_C(11) },
    { UINT32_C(0x88760082), UINT8_C(12) },
    { UINT32_C(0x80070057), UINT8_C(13) },
    { UINT32_C(0x88760091), UINT8_C(14) },
    { UINT32_C(0x88760096), UINT8_C(15) },
    { UINT32_C(0x887600a0), UINT8_C(16) },
    { UINT32_C(0x887600aa), UINT8_C(17) },
    { UINT32_C(0x887600b4), UINT8_C(18) },
    { UINT32_C(0x887600cd), UINT8_C(19) },
    { UINT32_C(0x887600d2), UINT8_C(20) },
    { UINT32_C(0x887600d4), UINT8_C(21) },
    { UINT32_C(0x887600d7), UINT8_C(22) },
    { UINT32_C(0x887600dc), UINT8_C(23) },
    { UINT32_C(0x887600de), UINT8_C(24) },
    { UINT32_C(0x887600e1), UINT8_C(25) },
    { UINT32_C(0x887600e6), UINT8_C(26) },
    { UINT32_C(0x887600f0), UINT8_C(27) },
    { UINT32_C(0x887600fa), UINT8_C(28) },
    { UINT32_C(0x887600ff), UINT8_C(29) },
    { UINT32_C(0x88760104), UINT8_C(30) },
    { UINT32_C(0x88760118), UINT8_C(31) },
    { UINT32_C(0x88760122), UINT8_C(32) },
    { UINT32_C(0x88760136), UINT8_C(33) },
    { UINT32_C(0x8876013c), UINT8_C(34) },
    { UINT32_C(0x8876013d), UINT8_C(35) },
    { UINT32_C(0x88760140), UINT8_C(36) },
    { UINT32_C(0x8876014a), UINT8_C(37) },
    { UINT32_C(0x8876014f), UINT8_C(38) },
    { UINT32_C(0x88760154), UINT8_C(39) },
    { UINT32_C(0x8876015e), UINT8_C(40) },
    { UINT32_C(0x88760168), UINT8_C(41) },
    { UINT32_C(0x8007000e), UINT8_C(42) },
    { UINT32_C(0x8876017c), UINT8_C(43) },
    { UINT32_C(0x8876017e), UINT8_C(44) },
    { UINT32_C(0x88760180), UINT8_C(45) },
    { UINT32_C(0x88760183), UINT8_C(46) },
    { UINT32_C(0x88760190), UINT8_C(47) },
    { UINT32_C(0x8876019a), UINT8_C(48) },
    { UINT32_C(0x887601a4), UINT8_C(49) },
    { UINT32_C(0x887601ae), UINT8_C(50) },
    { UINT32_C(0x887601b3), UINT8_C(51) },
    { UINT32_C(0x887601b8), UINT8_C(52) },
    { UINT32_C(0x887601c2), UINT8_C(53) },
    { UINT32_C(0x887601cc), UINT8_C(54) },
    { UINT32_C(0x887601d6), UINT8_C(55) },
    { UINT32_C(0x887601e0), UINT8_C(56) },
    { UINT32_C(0x887601ea), UINT8_C(57) },
    { UINT32_C(0x80004001), UINT8_C(58) },
    { UINT32_C(0x887601fe), UINT8_C(59) },
    { UINT32_C(0x88760208), UINT8_C(60) },
    { UINT32_C(0x88760219), UINT8_C(61) },
    { UINT32_C(0x8876021c), UINT8_C(62) },
    { UINT32_C(0x88760230), UINT8_C(63) },
    { UINT32_C(0x88760231), UINT8_C(64) },
    { UINT32_C(0x88760232), UINT8_C(65) },
    { UINT32_C(0x88760233), UINT8_C(66) },
    { UINT32_C(0x88760234), UINT8_C(67) },
    { UINT32_C(0x88760235), UINT8_C(68) },
    { UINT32_C(0x88760236), UINT8_C(69) },
    { UINT32_C(0x88760237), UINT8_C(70) },
    { UINT32_C(0x88760238), UINT8_C(71) },
    { UINT32_C(0x88760239), UINT8_C(72) },
    { UINT32_C(0x8876023a), UINT8_C(73) },
    { UINT32_C(0x8876023b), UINT8_C(74) },
    { UINT32_C(0x8876023c), UINT8_C(75) },
    { UINT32_C(0x8876023d), UINT8_C(76) },
    { UINT32_C(0x8876023e), UINT8_C(77) },
    { UINT32_C(0x8876023f), UINT8_C(78) },
    { UINT32_C(0x88760240), UINT8_C(79) },
    { UINT32_C(0x88760241), UINT8_C(80) },
    { UINT32_C(0x88760242), UINT8_C(81) },
    { UINT32_C(0x88760243), UINT8_C(82) },
    { UINT32_C(0x88760244), UINT8_C(83) },
    { UINT32_C(0x88760245), UINT8_C(84) },
    { UINT32_C(0x88760246), UINT8_C(85) },
    { UINT32_C(0x88760247), UINT8_C(86) },
    { UINT32_C(0x88760248), UINT8_C(87) },
    { UINT32_C(0x88760249), UINT8_C(88) },
    { UINT32_C(0x8876024a), UINT8_C(89) },
    { UINT32_C(0x8876024b), UINT8_C(90) },
    { UINT32_C(0x8876024c), UINT8_C(91) },
    { UINT32_C(0x8876024d), UINT8_C(92) },
    { UINT32_C(0x8876024e), UINT8_C(93) },
    { UINT32_C(0x8876024f), UINT8_C(94) },
    { UINT32_C(0x88760250), UINT8_C(95) },
    { UINT32_C(0x8876026c), UINT8_C(96) },
    { UINT32_C(0x88760280), UINT8_C(97) },
    { UINT32_C(0x88760294), UINT8_C(98) },
};

char pharaoh_graphics_hresult_error_character(int32_t result)
{
    const uint32_t result_bits = (uint32_t)result;
    for (size_t index = 0;
         index < sizeof direct_draw_error_characters /
             sizeof direct_draw_error_characters[0];
         ++index) {
        if (direct_draw_error_characters[index].result == result_bits) {
            return (char)direct_draw_error_characters[index].character;
        }
    }
    return result == -INT32_C(0x7789fd58) ? 'c' : 'd';
}

void *pharaoh_graphics_offscreen_surface_create(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations,
    uint32_t width, uint32_t height, int32_t system_memory)
{
    const PharaohDirectDrawSurfaceDescription description = {
        PRIMARY_SURFACE_DESCRIPTION_SIZE,
        OFFSCREEN_SURFACE_DESCRIPTION_FLAGS,
        width,
        height,
        OFFSCREEN_SURFACE_CAPABILITY |
            (system_memory != 0 ? SYSTEM_MEMORY_SURFACE_CAPABILITY : 0)
    };
    void *surface = NULL;
    const int32_t result = operations->create_surface(operations->user_data,
        state->direct_draw_device, &description, &surface);

    if (result != 0) {
        operations->report_status(operations->user_data,
            error_extra_surface, 0, 0);
        return NULL;
    }
    return surface;
}

int32_t pharaoh_graphics_surfaces_create(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations)
{
    const int32_t previous_pixel_format =
        state->surface_pixel_format_selection;
    const int32_t windowed = state->color_mode_selection == 0;
    const PharaohDirectDrawSurfaceDescription primary_description = {
        PRIMARY_SURFACE_DESCRIPTION_SIZE,
        PRIMARY_SURFACE_DESCRIPTION_FLAGS,
        0,
        0,
        PRIMARY_SURFACE_CAPABILITY
    };
    PharaohDirectDrawSurfaceLockResult primary_lock = { 0 };
    PharaohDirectDrawSurfaceLockResult back_buffer_lock = { 0 };
    PharaohDirectDrawPixelMasks masks = { 0, 0, 0 };
    int32_t result;

    operations->report_status(operations->user_data,
        windowed ? status_create_one_vram_and_system : status_create_one_vram,
        0, 0);
    result = operations->create_surface(operations->user_data,
        state->direct_draw_device, &primary_description,
        &state->primary_surface);
    if (result != 0) {
        operations->report_status(operations->user_data,
            windowed ? error_create_surface_windowed :
                error_create_surface_fullscreen,
            0, result);
        return 0;
    }

    state->primary_surface_lock_status = 0;
    result = operations->lock_surface(operations->user_data,
        state->primary_surface, SURFACE_LOCK_WAIT, &primary_lock);
    if (result == 0) {
        operations->report_status(operations->user_data,
            status_primary_lock_ok, 0, 0);
        state->primary_surface_lock_status = 0;
        operations->unlock_surface(operations->user_data,
            state->primary_surface);
    } else {
        operations->report_status(operations->user_data, error_primary_lock,
            0, (int32_t)pharaoh_graphics_hresult_error_character(result) + 1);
    }

    state->back_buffer_surface = pharaoh_graphics_offscreen_surface_create(
        state, operations, (uint32_t)state->render_width,
        (uint32_t)state->render_height, 1);
    if (state->back_buffer_surface == NULL) {
        operations->report_status(operations->user_data,
            error_create_backbuffer, 0, 0);
        return 0;
    }

    result = operations->lock_surface(operations->user_data,
        state->back_buffer_surface, SURFACE_LOCK_WAIT, &back_buffer_lock);
    if (result == 0) {
        state->back_buffer_pixels = back_buffer_lock.pixels;
        operations->unlock_surface(operations->user_data,
            state->back_buffer_surface);
    }

    if (windowed != 0) {
        result = operations->create_clipper(operations->user_data,
            state->direct_draw_device, &state->graphics_clipper);
        if (result != 0) {
            operations->report_status(operations->user_data,
                error_create_clipper, 0, 0);
            return 0;
        }
        result = operations->set_clipper_window(operations->user_data,
            state->graphics_clipper, state->main_window);
        if (result != 0) {
            operations->report_status(operations->user_data,
                error_set_clipper_window, 0, 0);
            return 0;
        }
        result = operations->attach_surface_clipper(operations->user_data,
            state->primary_surface, state->graphics_clipper);
        if (result != 0) {
            operations->report_status(operations->user_data,
                error_attach_clipper, 0, 0);
            return 0;
        }
        operations->report_status(operations->user_data,
            status_surfaces_clipper_ready, 0, 0);
    } else {
        operations->report_status(operations->user_data,
            status_surface_ready, 0, 0);
    }

    (void)operations->get_surface_pixel_masks(operations->user_data,
        state->primary_surface, &masks);
    if (masks.red == PIXEL_MASK_RGB_655_RED) {
        state->color_mode_code = PIXEL_FORMAT_RGB_655;
        state->surface_pixel_format_selection =
            PIXEL_FORMAT_SELECTION_655;
        operations->report_status(operations->user_data, status_rgb_655,
            0, 0);
    } else if (masks.green == PIXEL_MASK_RGB_565_GREEN) {
        state->color_mode_code = PIXEL_FORMAT_RGB_565;
        state->surface_pixel_format_selection =
            PIXEL_FORMAT_SELECTION_565;
        operations->report_status(operations->user_data, status_rgb_565,
            0, 0);
    } else if (masks.blue == PIXEL_MASK_RGB_556_BLUE) {
        state->color_mode_code = PIXEL_FORMAT_RGB_556;
        state->surface_pixel_format_selection =
            PIXEL_FORMAT_SELECTION_556;
        operations->report_status(operations->user_data, status_rgb_556,
            0, 0);
    } else {
        state->color_mode_code = PIXEL_FORMAT_RGB_555;
        state->surface_pixel_format_selection =
            PIXEL_FORMAT_SELECTION_555;
        operations->report_status(operations->user_data, status_rgb_555,
            0, 0);
    }

    if (state->surface_pixel_format_selection ==
            PIXEL_FORMAT_SELECTION_556 ||
        state->surface_pixel_format_selection ==
            PIXEL_FORMAT_SELECTION_655) {
        operations->report_status(operations->user_data,
            error_unsupported_rgb, 0, 0);
        state->color_mode_code = PIXEL_FORMAT_RGB_565;
        state->surface_pixel_format_selection =
            PIXEL_FORMAT_SELECTION_565;
    }

    if (operations->get_current_window_id(operations->user_data) != 0 &&
        state->surface_pixel_format_selection != previous_pixel_format) {
        char message[100];
        (void)snprintf(message, sizeof message, format_changed,
            previous_pixel_format, state->surface_pixel_format_selection);
        operations->report_status(operations->user_data, message, 0, 0);
        state->surface_pixel_format_selection = previous_pixel_format;
    }
    return 1;
}


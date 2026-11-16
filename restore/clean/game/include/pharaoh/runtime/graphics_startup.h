#ifndef PHARAOH_RUNTIME_GRAPHICS_STARTUP_H
#define PHARAOH_RUNTIME_GRAPHICS_STARTUP_H

#include <stdint.h>

typedef struct PharaohGraphicsPaletteEntry {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t flags;
} PharaohGraphicsPaletteEntry;

typedef struct PharaohGraphicsWindowRect {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
} PharaohGraphicsWindowRect;

typedef struct PharaohGraphicsStartupState {
    uint32_t display_initialization_in_progress;
    int32_t display_session_active;
    uint32_t frame_rendering_enabled;
    uint8_t redraw_requested;
    void *graphics_interface;
    void *graphics_clipper;
    void *back_buffer_surface;
    void *primary_surface;
    void *back_buffer_pixels;
    void *direct_draw_device;
    int32_t display_mode_selection;
    int32_t color_mode_selection;
    int32_t previous_display_mode;
    int32_t previous_color_mode;
    int32_t screen_width;
    int32_t screen_height;
    int32_t color_mode_code;
    uint8_t mode_status_byte;
    void *main_window;
    int32_t render_width;
    int32_t render_height;
    int32_t desktop_width;
    int32_t desktop_height;
    uint32_t desktop_color_depth;
    int32_t window_offset_x;
    int32_t window_offset_y;
    PharaohGraphicsPaletteEntry palette[256];
} PharaohGraphicsStartupState;

typedef struct PharaohGraphicsStartupOperations {
    void *user_data;
    uint32_t (*release_graphics_interface)(void *user_data,
                                           void *graphics_interface);
    int32_t (*create_flipping_surfaces)(void *user_data,
                                        PharaohGraphicsStartupState *state);
    void (*change_display_mode)(void *user_data, int32_t display_mode);
    void (*attach_surface_context)(void *user_data, void *primary_surface,
                                   void *back_buffer_surface);
    void (*output_surface_debug)(void *user_data, const char *format,
                                 const void *surface);
    int32_t (*clear_display_surfaces)(void *user_data);
    void (*report_status)(void *user_data, const char *message,
                          int32_t argument_1, int32_t argument_2);
    void *(*get_device_context)(void *user_data, void *window);
    int32_t (*get_device_capability)(void *user_data, void *device_context,
                                    int32_t capability);
    void (*release_device_context)(void *user_data, void *window,
                                   void *device_context);
    int32_t (*get_system_metric)(void *user_data, int32_t metric);
    uint32_t (*get_palette_entries)(void *user_data, void *palette,
                                    uint32_t first_entry, uint32_t entry_count,
                                    PharaohGraphicsPaletteEntry *entries);
    int32_t (*create_direct_draw)(void *user_data, const void *driver_guid,
                                  void **direct_draw_device,
                                  void *outer_unknown);
    int32_t (*set_cooperative_level)(void *user_data,
                                     void *direct_draw_device,
                                     void *main_window, uint32_t flags);
    int32_t (*get_window_long)(void *user_data, void *window,
                               int32_t index);
    void (*set_window_long)(void *user_data, void *window, int32_t index,
                            uint32_t value);
    void *(*get_window_menu)(void *user_data, void *window);
    int32_t (*adjust_window_rect)(void *user_data,
                                  PharaohGraphicsWindowRect *rect,
                                  uint32_t style, int32_t has_menu,
                                  uint32_t extended_style);
    int32_t (*set_window_position)(void *user_data, void *window,
                                   int32_t insert_after, int32_t x,
                                   int32_t y, int32_t width, int32_t height,
                                   uint32_t flags);
    int32_t (*get_work_area)(void *user_data,
                             PharaohGraphicsWindowRect *rect);
    int32_t (*get_window_rect)(void *user_data, void *window,
                               PharaohGraphicsWindowRect *rect);
    int32_t (*set_display_mode)(void *user_data, void *direct_draw_device,
                                uint32_t width, uint32_t height,
                                uint32_t bits_per_pixel);
    void (*prepare_display_mode_retry)(void *user_data, uint32_t mode);
    void (*resize_graphics_screen)(void *user_data);
    void (*clear_city_warnings)(void *user_data);
    void (*dispatch_message)(void *user_data, uint32_t message,
                             int32_t argument_1, int32_t argument_2);
} PharaohGraphicsStartupOperations;

/* Original entry 0x00426EE0. Releases clipper/surfaces in native order,
   clears the mapped back-buffer pixel pointer, and optionally releases the
   DirectDraw device. */
int32_t pharaoh_graphics_resources_release(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations,
    int32_t release_device);

/* Original entry 0x00426B40. Selects the windowed/full-screen path from the
   desktop and configured render dimensions, then initializes DirectDraw. */
int32_t pharaoh_graphics_enable_direct_draw(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations);

/* Original entry 0x004D0060. Handles DirectDraw enable, flip-surface fallback,
   persisted mode values, surface-context setup, and debug output. */
int32_t pharaoh_graphics_device_initialize(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations);

/* Original entry 0x004D01E0. Releases the auxiliary interface when present;
   the display session is released only when active. */
void pharaoh_graphics_session_release(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations,
    int32_t release_device);

/* Original entry 0x004D0220. The native routine ignores the clear result and
   leaves display_initialization_in_progress set when device initialization
   fails. */
int32_t pharaoh_graphics_session_begin(
    PharaohGraphicsStartupState *state,
    const PharaohGraphicsStartupOperations *operations);

#endif

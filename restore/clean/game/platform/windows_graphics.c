#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>

#include <stdio.h>
#include <string.h>

#include "pharaoh/platform/windows_graphics.h"

static uint32_t release_graphics_interface(void *user_data,
    void *graphics_interface)
{
    IUnknown *const unknown = (IUnknown *)graphics_interface;
    (void)user_data;
    return unknown->lpVtbl->Release(unknown);
}

static int32_t create_surface(void *user_data, void *direct_draw_device,
    const PharaohDirectDrawSurfaceDescription *source, void **surface)
{
    DDSURFACEDESC description;
    LPDIRECTDRAWSURFACE created = NULL;
    LPDIRECTDRAW draw = (LPDIRECTDRAW)direct_draw_device;
    (void)user_data;

    memset(&description, 0, sizeof(description));
    description.dwSize = sizeof(description);
    description.dwFlags = source->flags;
    description.dwWidth = source->width;
    description.dwHeight = source->height;
    description.ddsCaps.dwCaps = source->capabilities;
    const HRESULT result = draw->lpVtbl->CreateSurface(draw, &description,
        &created, NULL);
    *surface = created;
    return (int32_t)result;
}

static int32_t lock_surface(void *user_data, void *surface, uint32_t flags,
    PharaohDirectDrawSurfaceLockResult *result)
{
    DDSURFACEDESC description;
    LPDIRECTDRAWSURFACE draw_surface = (LPDIRECTDRAWSURFACE)surface;
    (void)user_data;

    memset(&description, 0, sizeof(description));
    description.dwSize = sizeof(description);
    const HRESULT status = draw_surface->lpVtbl->Lock(draw_surface, NULL,
        &description, flags, NULL);
    if (status == DD_OK) {
        result->pixels = description.lpSurface;
        result->width = description.dwWidth;
        result->height = description.dwHeight;
        result->pixel_masks.red = description.ddpfPixelFormat.dwRBitMask;
        result->pixel_masks.green = description.ddpfPixelFormat.dwGBitMask;
        result->pixel_masks.blue = description.ddpfPixelFormat.dwBBitMask;
    }
    return (int32_t)status;
}

static void unlock_surface(void *user_data, void *surface)
{
    LPDIRECTDRAWSURFACE draw_surface = (LPDIRECTDRAWSURFACE)surface;
    (void)user_data;
    (void)draw_surface->lpVtbl->Unlock(draw_surface, NULL);
}

static int32_t create_clipper(void *user_data, void *direct_draw_device,
    void **clipper)
{
    LPDIRECTDRAWCLIPPER created = NULL;
    LPDIRECTDRAW draw = (LPDIRECTDRAW)direct_draw_device;
    (void)user_data;
    const HRESULT result = draw->lpVtbl->CreateClipper(draw, 0, &created,
        NULL);
    *clipper = created;
    return (int32_t)result;
}

static int32_t set_clipper_window(void *user_data, void *clipper,
    void *window)
{
    LPDIRECTDRAWCLIPPER draw_clipper = (LPDIRECTDRAWCLIPPER)clipper;
    (void)user_data;
    return (int32_t)draw_clipper->lpVtbl->SetHWnd(draw_clipper, 0,
        (HWND)window);
}

static int32_t attach_surface_clipper(void *user_data, void *surface,
    void *clipper)
{
    LPDIRECTDRAWSURFACE draw_surface = (LPDIRECTDRAWSURFACE)surface;
    (void)user_data;
    return (int32_t)draw_surface->lpVtbl->SetClipper(draw_surface,
        (LPDIRECTDRAWCLIPPER)clipper);
}

static int32_t get_surface_pixel_masks(void *user_data, void *surface,
    PharaohDirectDrawPixelMasks *masks)
{
    DDSURFACEDESC description;
    LPDIRECTDRAWSURFACE draw_surface = (LPDIRECTDRAWSURFACE)surface;
    (void)user_data;

    memset(&description, 0, sizeof(description));
    description.dwSize = sizeof(description);
    description.dwFlags = DDSD_PIXELFORMAT;
    description.ddpfPixelFormat.dwSize = sizeof(description.ddpfPixelFormat);
    const HRESULT result = draw_surface->lpVtbl->GetSurfaceDesc(draw_surface,
        &description);
    if (result == DD_OK) {
        masks->red = description.ddpfPixelFormat.dwRBitMask;
        masks->green = description.ddpfPixelFormat.dwGBitMask;
        masks->blue = description.ddpfPixelFormat.dwBBitMask;
    }
    return (int32_t)result;
}

static void update_window(void *user_data, void *window)
{
    (void)user_data;
    (void)UpdateWindow((HWND)window);
}

static int32_t fill_surface(void *user_data, void *surface, uint32_t flags,
    uint32_t fill_color)
{
    DDBLTFX effects;
    LPDIRECTDRAWSURFACE draw_surface = (LPDIRECTDRAWSURFACE)surface;
    (void)user_data;
    memset(&effects, 0, sizeof(effects));
    effects.dwSize = sizeof(effects);
    effects.dwFillColor = fill_color;
    return (int32_t)draw_surface->lpVtbl->Blt(draw_surface, NULL, NULL, NULL,
        flags, &effects);
}

static void output_surface_debug(void *user_data, const char *format,
    const void *surface)
{
    char text[160];
    (void)user_data;
    (void)snprintf(text, sizeof(text), format,
        (unsigned int)(uintptr_t)surface);
    OutputDebugStringA(text);
}

static void *get_device_context(void *user_data, void *window)
{
    (void)user_data;
    return GetDC((HWND)window);
}

static int32_t get_device_capability(void *user_data, void *device_context,
    int32_t capability)
{
    (void)user_data;
    return GetDeviceCaps((HDC)device_context, capability);
}

static void release_device_context(void *user_data, void *window,
    void *device_context)
{
    (void)user_data;
    (void)ReleaseDC((HWND)window, (HDC)device_context);
}

static int32_t get_system_metric(void *user_data, int32_t metric)
{
    (void)user_data;
    return GetSystemMetrics(metric);
}

static uint32_t get_palette_entries(void *user_data, void *palette,
    uint32_t first_entry, uint32_t entry_count,
    PharaohGraphicsPaletteEntry *entries)
{
    (void)user_data;
    return GetPaletteEntries((HPALETTE)palette, first_entry, entry_count,
        (LPPALETTEENTRY)entries);
}

static int32_t create_direct_draw(void *user_data, const void *driver_guid,
    void **direct_draw_device, void *outer_unknown)
{
    LPDIRECTDRAW draw = NULL;
    (void)user_data;
    const HRESULT result = DirectDrawCreate((GUID *)driver_guid, &draw,
        (IUnknown *)outer_unknown);
    *direct_draw_device = draw;
    return (int32_t)result;
}

static int32_t set_cooperative_level(void *user_data,
    void *direct_draw_device, void *main_window, uint32_t flags)
{
    LPDIRECTDRAW draw = (LPDIRECTDRAW)direct_draw_device;
    (void)user_data;
    return (int32_t)draw->lpVtbl->SetCooperativeLevel(draw,
        (HWND)main_window, flags);
}

static int32_t get_window_long(void *user_data, void *window, int32_t index)
{
    (void)user_data;
    return (int32_t)GetWindowLongA((HWND)window, index);
}

static void set_window_long(void *user_data, void *window, int32_t index,
    uint32_t value)
{
    (void)user_data;
    (void)SetWindowLongA((HWND)window, index, (LONG)value);
}

static void *get_window_menu(void *user_data, void *window)
{
    (void)user_data;
    return GetMenu((HWND)window);
}

static int32_t adjust_window_rect(void *user_data,
    PharaohGraphicsWindowRect *rect, uint32_t style, int32_t has_menu,
    uint32_t extended_style)
{
    RECT native_rect = { rect->left, rect->top, rect->right, rect->bottom };
    (void)user_data;
    const BOOL result = AdjustWindowRectEx(&native_rect, style,
        has_menu != 0, extended_style);
    rect->left = native_rect.left;
    rect->top = native_rect.top;
    rect->right = native_rect.right;
    rect->bottom = native_rect.bottom;
    return result != 0;
}

static int32_t set_window_position(void *user_data, void *window,
    int32_t insert_after, int32_t x, int32_t y, int32_t width,
    int32_t height, uint32_t flags)
{
    (void)user_data;
    return SetWindowPos((HWND)window, (HWND)(INT_PTR)insert_after, x, y,
        width, height, flags) != 0;
}

static int32_t get_work_area(void *user_data, PharaohGraphicsWindowRect *rect)
{
    RECT native_rect;
    (void)user_data;
    const BOOL result = SystemParametersInfoA(SPI_GETWORKAREA, 0,
        &native_rect, 0);
    if (result != 0) {
        rect->left = native_rect.left;
        rect->top = native_rect.top;
        rect->right = native_rect.right;
        rect->bottom = native_rect.bottom;
    }
    return result != 0;
}

static int32_t get_window_rect(void *user_data, void *window,
    PharaohGraphicsWindowRect *rect)
{
    RECT native_rect;
    (void)user_data;
    const BOOL result = GetWindowRect((HWND)window, &native_rect);
    if (result != 0) {
        rect->left = native_rect.left;
        rect->top = native_rect.top;
        rect->right = native_rect.right;
        rect->bottom = native_rect.bottom;
    }
    return result != 0;
}

static int32_t set_display_mode(void *user_data, void *direct_draw_device,
    uint32_t width, uint32_t height, uint32_t bits_per_pixel)
{
    LPDIRECTDRAW draw = (LPDIRECTDRAW)direct_draw_device;
    (void)user_data;
    return (int32_t)draw->lpVtbl->SetDisplayMode(draw, width, height,
        bits_per_pixel);
}

static void set_focus(void *user_data, void *window)
{
    (void)user_data;
    (void)SetFocus((HWND)window);
}

void pharaoh_windows_graphics_bind_platform_operations(
    PharaohGraphicsStartupOperations *operations)
{
    operations->release_graphics_interface = release_graphics_interface;
    operations->create_surface = create_surface;
    operations->lock_surface = lock_surface;
    operations->unlock_surface = unlock_surface;
    operations->create_clipper = create_clipper;
    operations->set_clipper_window = set_clipper_window;
    operations->attach_surface_clipper = attach_surface_clipper;
    operations->get_surface_pixel_masks = get_surface_pixel_masks;
    operations->update_window = update_window;
    operations->fill_surface = fill_surface;
    operations->output_surface_debug = output_surface_debug;
    operations->get_device_context = get_device_context;
    operations->get_device_capability = get_device_capability;
    operations->release_device_context = release_device_context;
    operations->get_system_metric = get_system_metric;
    operations->get_palette_entries = get_palette_entries;
    operations->create_direct_draw = create_direct_draw;
    operations->set_cooperative_level = set_cooperative_level;
    operations->get_window_long = get_window_long;
    operations->set_window_long = set_window_long;
    operations->get_window_menu = get_window_menu;
    operations->adjust_window_rect = adjust_window_rect;
    operations->set_window_position = set_window_position;
    operations->get_work_area = get_work_area;
    operations->get_window_rect = get_window_rect;
    operations->set_display_mode = set_display_mode;
    operations->set_focus = set_focus;
}

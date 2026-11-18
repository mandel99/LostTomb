#include "pharaoh/platform/windows_frame_gate.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ddraw.h>

#include "pharaoh/runtime/application_window_messages.h"

static int32_t restore_surface(void *user_data, void *surface)
{
    LPDIRECTDRAWSURFACE draw_surface = (LPDIRECTDRAWSURFACE)surface;
    (void)user_data;
    return (int32_t)draw_surface->lpVtbl->Restore(draw_surface);
}

static bool surface_is_lost(void *user_data, PharaohDisplaySurface surface)
{
    const PharaohWindowsFrameGate *const context =
        (const PharaohWindowsFrameGate *)user_data;
    LPDIRECTDRAWSURFACE draw_surface = (LPDIRECTDRAWSURFACE)
        (surface == PHARAOH_DISPLAY_SURFACE_FRONT ?
            context->restore.front_surface : context->restore.back_surface);
    return draw_surface->lpVtbl->IsLost(draw_surface) == DDERR_SURFACELOST;
}

static bool restore_surfaces(void *user_data)
{
    const PharaohWindowsFrameGate *const context =
        (const PharaohWindowsFrameGate *)user_data;
    return pharaoh_graphics_frame_restore_surfaces(&context->restore) != 0;
}

static void pause_game(void *user_data)
{
    const PharaohWindowsFrameGate *const context =
        (const PharaohWindowsFrameGate *)user_data;
    pharaoh_application_window_pause_if_minimized(context->window,
        context->paused_state, context->report_user_data,
        context->report_paused);
}

static bool quit_requested(void *user_data)
{
    const PharaohWindowsFrameGate *const context =
        (const PharaohWindowsFrameGate *)user_data;
    return *context->quit_requested_state == 1;
}

static void dispatch_frame(void *user_data)
{
    const PharaohWindowsFrameGate *const context =
        (const PharaohWindowsFrameGate *)user_data;
    context->dispatch_frame(context->dispatch_user_data);
}

void pharaoh_windows_frame_gate_bind(PharaohWindowsFrameGate *context,
    PharaohFrameGateState *state, PharaohFrameGateHooks *hooks)
{
    context->restore_operations.user_data = NULL;
    context->restore_operations.restore_surface = restore_surface;
    context->restore.operations = &context->restore_operations;
    state->front_surface_available = context->restore.front_surface != NULL;
    state->back_surface_available = context->restore.back_surface != NULL;
    hooks->context = context;
    hooks->surface_is_lost = surface_is_lost;
    hooks->restore_surfaces = restore_surfaces;
    hooks->pause_game = pause_game;
    hooks->quit_requested = quit_requested;
    hooks->dispatch_frame = dispatch_frame;
}

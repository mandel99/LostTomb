#ifndef PHARAOH_RUNTIME_APPLICATION_DISPLAY_STARTUP_H
#define PHARAOH_RUNTIME_APPLICATION_DISPLAY_STARTUP_H

#include "pharaoh/runtime/graphics_startup.h"
#include "pharaoh/runtime/window_geometry.h"

typedef struct PharaohApplicationDisplayStartup {
    PharaohGraphicsStartupState *graphics;
    const PharaohGraphicsStartupOperations *graphics_operations;
    PharaohInitialWindowGeometry geometry;
} PharaohApplicationDisplayStartup;

/* Prepares native screen dimensions and desktop metrics before the window is
 * created, matching the order inside 0x004CF960. */
void pharaoh_application_display_prepare(
    PharaohApplicationDisplayStartup *startup, int32_t settings_display_mode);

/* Enters the existing 0x004D0220 graphics-session owner after window creation. */
int32_t pharaoh_application_display_begin_session(
    PharaohApplicationDisplayStartup *startup);

/* Convenience composition for callers whose window already exists. */
int32_t pharaoh_application_display_startup(
    PharaohApplicationDisplayStartup *startup, int32_t settings_display_mode);

#endif

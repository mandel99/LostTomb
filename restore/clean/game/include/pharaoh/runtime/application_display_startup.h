#ifndef PHARAOH_RUNTIME_APPLICATION_DISPLAY_STARTUP_H
#define PHARAOH_RUNTIME_APPLICATION_DISPLAY_STARTUP_H

#include "pharaoh/runtime/graphics_startup.h"
#include "pharaoh/runtime/window_geometry.h"

typedef struct PharaohApplicationDisplayStartup {
    PharaohGraphicsStartupState *graphics;
    const PharaohGraphicsStartupOperations *graphics_operations;
    PharaohInitialWindowGeometry geometry;
} PharaohApplicationDisplayStartup;

/* Prepare native screen dimensions and enter the existing 0x004D0220
 * graphics-session owner. Window creation and COM are still caller phases. */
int32_t pharaoh_application_display_startup(
    PharaohApplicationDisplayStartup *startup, int32_t settings_display_mode);

#endif

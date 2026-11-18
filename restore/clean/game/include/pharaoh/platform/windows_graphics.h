#ifndef PHARAOH_PLATFORM_WINDOWS_GRAPHICS_H
#define PHARAOH_PLATFORM_WINDOWS_GRAPHICS_H

#include "pharaoh/runtime/graphics_startup.h"

/* Installs the Win32, GDI and DirectDraw operations. Game-owned operations
   such as status reporting, display-mode state changes and warning resets are
   left for the startup host context to bind. */
void pharaoh_windows_graphics_bind_platform_operations(
    PharaohGraphicsStartupOperations *operations);

#endif

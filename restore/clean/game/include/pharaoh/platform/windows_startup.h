#ifndef PHARAOH_PLATFORM_WINDOWS_STARTUP_H
#define PHARAOH_PLATFORM_WINDOWS_STARTUP_H

#include "pharaoh/runtime/application_shutdown.h"
#include "pharaoh/runtime/application_window_startup.h"

void pharaoh_windows_application_directory_operations_initialize(
    PharaohApplicationDirectoryOperations *operations);

void pharaoh_windows_application_window_startup_operations_initialize(
    PharaohApplicationWindowStartupOperations *operations,
    const PharaohApplicationDirectoryOperations *directory_operations,
    const PharaohGraphicsStartupOperations *graphics_operations,
    const PharaohApplicationWindowOperations *window_operations);

/* Installs allocation and COM teardown. The game-owned normal-exit report is
   deliberately left for the startup host context. */
void pharaoh_windows_application_shutdown_operations_bind(
    PharaohApplicationShutdownOperations *operations);

#endif

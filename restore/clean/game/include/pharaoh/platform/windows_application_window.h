#ifndef PHARAOH_PLATFORM_WINDOWS_APPLICATION_WINDOW_H
#define PHARAOH_PLATFORM_WINDOWS_APPLICATION_WINDOW_H

#include "pharaoh/runtime/application_window.h"

/* Binds the typed application-window owner to the ANSI Win32 APIs used by the
   original x86 executable. */
void pharaoh_windows_application_window_operations_initialize(
    PharaohApplicationWindowOperations *operations);

#endif

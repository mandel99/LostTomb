#include "graphics_session.h"
#include <mmsystem.h>
static const RestoreGraphicsApi api={
    GetDC,GetDeviceCaps,ReleaseDC,GetSystemMetrics,GetPaletteEntries,DirectDrawCreate,
    GetWindowLongA,SetWindowLongA,GetMenu,AdjustWindowRectEx,SetWindowPos,
    SystemParametersInfoA,GetWindowRect,UpdateWindow,OutputDebugStringA,timeGetTime
};
const RestoreGraphicsApi *restore_graphics_platform(void) {return &api;}

#ifndef PHARAOH_PLATFORM_WINDOWS_VIDEO_IO_H
#define PHARAOH_PLATFORM_WINDOWS_VIDEO_IO_H

#include "pharaoh/runtime/video_player.h"

void pharaoh_windows_video_io_bind(
    PharaohVideoLoopOperations *loop, PharaohVideoOpenOperations *open);

#endif

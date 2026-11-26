#ifndef PHARAOH_PLATFORM_WINDOWS_DEBUG_FRAME_H
#define PHARAOH_PLATFORM_WINDOWS_DEBUG_FRAME_H

#include "pharaoh/rendering/image_draw.h"
#include "pharaoh/rendering/sprite_cycle.h"

typedef struct PharaohWindowsDebugFrameContext {
    uint32_t *current_time, *previous_time;
    const uint32_t *keys_enabled;
    PharaohSpriteCycleClock *clock;
    PharaohSpriteCycle **object_sprite, **frame_sprite;
    PharaohImageDrawContext *drawing;
} PharaohWindowsDebugFrameContext;

/* Native 004E61F0. The two slots contain actual C objects and methods.
 * The virtual draw and orientation implementations require separate owners. */
void pharaoh_windows_debug_frame_update(
    const PharaohWindowsDebugFrameContext *context);

#endif

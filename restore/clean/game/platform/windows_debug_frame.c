#include "pharaoh/platform/windows_debug_frame.h"
#include "pharaoh/core/x87_numeric.h"
#include "pharaoh/rendering/sprite_rle_blit.h"

#include <windows.h>
#include <mmsystem.h>
#include <string.h>

typedef struct DebugDrawMethods {
    uint32_t reserved_00[6];
    void (*draw)(PharaohSpriteCycle *self, int32_t x, int32_t y);
} DebugDrawMethods;

typedef struct DebugDrawFrame {
    uint8_t reserved_00, has_offset;
    int16_t width, height;
    uint8_t reserved_06[2];
    union { int16_t x; const int16_t *offset; } origin;
    const uint8_t *pixels;
} DebugDrawFrame;

_Static_assert(sizeof(DebugDrawFrame) == 16 &&
    offsetof(DebugDrawFrame, origin) == 8 &&
    offsetof(DebugDrawFrame, pixels) == 12 &&
    offsetof(DebugDrawMethods, draw) == 0x18,
    "native debug drawing views");

static void clock_adjust(float *rate, int subtract)
{
    static const float one = 1.0f, minimum = 10.0f, maximum = 80.0f;
    uint16_t status;
    /* Keep the extended intermediate after the native binary32 store. */
    if (subtract) {
        __asm {
            mov eax, rate
            fld dword ptr [eax]
            fsub one
            fst dword ptr [eax]
            fcomp minimum
            fnstsw status
        }
        if (status & 0x100u) *rate = minimum;
    } else {
        __asm {
            mov eax, rate
            fld dword ptr [eax]
            fadd one
            fst dword ptr [eax]
            fcomp maximum
            fnstsw status
        }
        if ((status & 0x4100u) == 0) *rate = maximum;
    }
}

static int16_t frame_index(float value)
{
    PharaohFloat80 extended;
    __asm {
        fld value
        fstp tbyte ptr extended
    }
    return (int16_t)(uint16_t)pharaoh_x87_truncate80(&extended);
}

void pharaoh_windows_debug_frame_update(const PharaohWindowsDebugFrameContext *c)
{
    const uint32_t now = timeGetTime();
    const uint32_t previous = *c->previous_time;
    *c->current_time = now;
    *c->previous_time = now;
    const uint32_t enabled = *c->keys_enabled;
    c->clock->elapsed = now - previous;
    if (enabled != 0) {
        if (((uint16_t)GetAsyncKeyState(VK_F2) & 0x8001u) == 0x8001u)
            c->clock->timed_mode = c->clock->timed_mode == 0;
        if (((uint16_t)GetAsyncKeyState(VK_F5) & 0x8001u) == 0x8001u)
            clock_adjust(&c->clock->frames_per_second, 1);
        if (((uint16_t)GetAsyncKeyState(VK_F6) & 0x8001u) == 0x8001u)
            clock_adjust(&c->clock->frames_per_second, 0);
    }
    if (*c->object_sprite != NULL) {
        (void)pharaoh_sprite_cycle_dispatch(*c->object_sprite, c->clock);
        PharaohSpriteCycle *const self = *c->object_sprite;
        const DebugDrawMethods *methods;
        memcpy(&methods, self, sizeof(methods));
        methods->draw(self, 350, 300);
    }
    PharaohSpriteCycle *sprite = *c->frame_sprite;
    if (sprite == NULL || sprite->source == NULL || sprite->source->data == NULL)
        return;
    (void)pharaoh_sprite_cycle_dispatch(sprite, c->clock);
    sprite = *c->frame_sprite;
    const DebugDrawFrame *const frames =
        (const void *)sprite->current_variant->images;
    const DebugDrawFrame *const frame = &frames[frame_index(sprite->frame)];
    const int16_t dx = frame->has_offset ? frame->origin.offset[1] : frame->origin.x;
    PharaohSpriteClipState *const clip = c->drawing->clip;
    clip->image_width = frame->width;
    clip->image_height = frame->height;
    clip->draw_x = (int32_t)dx + 450;
    clip->draw_y = 300;
    pharaoh_graphics_clip_sprite_horizontal(clip);
    pharaoh_graphics_clip_sprite_vertical(clip);
    if (clip->vertical_clip == 5) return;
    const PharaohSpriteRleBlitTarget target = {
        c->drawing->surface_pixels, c->drawing->pitch_bytes,
        clip->draw_x, clip->draw_y, clip->image_width, clip->visible_height,
        clip->clipped_top, clip->destination_row_skip, clip->clipped_left * 2,
        clip->clipped_right, *c->drawing->solid_color
    };
    if (clip->horizontal_clip == 1)
        pharaoh_image_blit_compressed_left(&target, frame->pixels);
    else if (clip->horizontal_clip == 2)
        pharaoh_image_blit_compressed_right(&target, frame->pixels);
    else pharaoh_image_blit_compressed(&target, frame->pixels);
}

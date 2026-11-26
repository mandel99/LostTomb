#include "pharaoh/rendering/debug_sprite.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohDebugSpriteDirection) == 8 &&
    offsetof(PharaohDebugSpriteDirection, frame_count) == 2,
    "debug sprite direction layout");
_Static_assert(offsetof(PharaohDebugSpriteAnimation, directions) == 0x0c,
    "debug sprite animation layout");
_Static_assert(offsetof(PharaohDebugSprite, animation) == 0x40 &&
    offsetof(PharaohDebugSprite, direction) == 0x44 &&
    offsetof(PharaohDebugSprite, frame) == 0x48,
    "debug sprite field layout");

/* Complete C translation of Pharaoh.exe 0x0055D290. */
void pharaoh_debug_sprite_set_frame(PharaohDebugSprite *sprite, int32_t input)
{
    int16_t frame = (int16_t)input;
    if (sprite->direction == NULL) return;
    const uint16_t count = sprite->direction->frame_count;
    while (frame < 0) frame = (int16_t)((uint16_t)frame + count);
    while ((int32_t)count <= (int32_t)frame) {
        frame = (int16_t)((int32_t)frame % (int32_t)count);
    }
    sprite->frame = (float)(int32_t)frame;
}

/* Complete C translation of Pharaoh.exe 0x0055D2E0. */
void pharaoh_debug_sprite_set_direction(
    PharaohDebugSprite *sprite, uint32_t direction)
{
    if (sprite->animation != NULL) {
        sprite->direction = sprite->animation->directions +
            (direction & UINT32_C(0xffff));
    }
}

/* Complete C translation of Pharaoh.exe 0x0055D300. */
void pharaoh_debug_sprite_set_direction_frame(
    PharaohDebugSprite *sprite, uint32_t direction, int32_t input)
{
    if (sprite->animation != NULL) {
        pharaoh_debug_sprite_set_direction(sprite, direction);
        sprite->frame = (float)(int32_t)(int16_t)input;
    }
}

/* Complete C translation of Pharaoh.exe 0x0055D750. */
int16_t pharaoh_debug_sprite_first_frame(void)
{
    return 0;
}

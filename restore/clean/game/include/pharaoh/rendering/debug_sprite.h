#ifndef PHARAOH_RENDERING_DEBUG_SPRITE_H
#define PHARAOH_RENDERING_DEBUG_SPRITE_H

#include <stdint.h>

typedef struct PharaohDebugSpriteDirection {
    uint16_t image_index;
    uint16_t frame_count;
    const void *frames;
} PharaohDebugSpriteDirection;

typedef struct PharaohDebugSpriteAnimation {
    uint8_t reserved_00[12];
    const PharaohDebugSpriteDirection *directions;
} PharaohDebugSpriteAnimation;

typedef struct PharaohDebugSprite {
    uint8_t reserved_00[0x40];
    const PharaohDebugSpriteAnimation *animation;
    const PharaohDebugSpriteDirection *direction;
    float frame;
} PharaohDebugSprite;

void pharaoh_debug_sprite_set_frame(PharaohDebugSprite *sprite, int32_t frame);
void pharaoh_debug_sprite_set_direction(
    PharaohDebugSprite *sprite, uint32_t direction);
void pharaoh_debug_sprite_set_direction_frame(
    PharaohDebugSprite *sprite, uint32_t direction, int32_t frame);
int16_t pharaoh_debug_sprite_first_frame(void);

#endif

#ifndef RESTORE_SPRITE_DIRECTION_H
#define RESTORE_SPRITE_DIRECTION_H
#include <stdint.h>
#include <stddef.h>
typedef struct RestoreSpriteAction {
    uint8_t mode,flags,rate,repeats,next_action;
    uint8_t reserved5[6];
    uint8_t directions;
    uint32_t variants;
} RestoreSpriteAction;
typedef struct RestoreSpriteDirection {
    uint16_t index,inherited_padding;
    uint32_t mirrored;
} RestoreSpriteDirection;
typedef char restore_sprite_action_directions_offset[offsetof(RestoreSpriteAction,directions)==11?1:-1];
typedef char restore_sprite_action_size[sizeof(RestoreSpriteAction)==16?1:-1];
typedef char restore_sprite_direction_size[sizeof(RestoreSpriteDirection)==8?1:-1];
/* Original output bytes2/3 come from an uninitialized private stack WORD.
   New C callers pass that input explicitly; the legacy bridge captures it.
   Last parameter optionally receives the two mutated native argument slots;
   ordinary C callers pass NULL. */
RestoreSpriteDirection *restore_sprite_direction_from_angle(
    RestoreSpriteDirection *,const RestoreSpriteAction *,float,uint32_t,uint32_t *);
#endif

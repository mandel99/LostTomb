#ifndef PHARAOH_RENDERING_SPRITE_CYCLE_H
#define PHARAOH_RENDERING_SPRITE_CYCLE_H

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohSpriteCycleImage {
    uint16_t reserved_00;
    uint16_t extent;
    uint16_t anchor;
} PharaohSpriteCycleImage;

typedef struct PharaohSpriteCycleVariant {
    uint16_t reserved_00;
    uint16_t frame_count;
    const PharaohSpriteCycleImage *images;
} PharaohSpriteCycleVariant;

typedef struct PharaohSpriteCycleAction {
    uint8_t mode, flags, rate, repeats, next_action;
    uint8_t reserved_05[6];
    uint8_t directions;
    const PharaohSpriteCycleVariant *variants;
} PharaohSpriteCycleAction;

typedef struct PharaohSpriteCycleSourceData {
    uint8_t reserved_00[0x1a];
    uint16_t action_count;
    const PharaohSpriteCycleAction *actions;
} PharaohSpriteCycleSourceData;

typedef struct PharaohSpriteCycleSource {
    uint8_t reserved_00[8];
    const PharaohSpriteCycleSourceData *data;
} PharaohSpriteCycleSource;

typedef struct PharaohSpriteCycleOrientation PharaohSpriteCycleOrientation;
typedef struct PharaohSpriteCycleOrientationMethods {
    float (*angle)(PharaohSpriteCycleOrientation *orientation);
} PharaohSpriteCycleOrientationMethods;

/* Native embedded subobject at cycle+4; the C virtual interface passes an
 * explicit receiver (it is not a binary-compatible native method table).
 * The native first virtual method returns ST0.
 * Actual implementations must provide this method; no default is supplied. */
struct PharaohSpriteCycleOrientation {
    const PharaohSpriteCycleOrientationMethods *methods;
};

typedef struct PharaohSpriteCycleClock {
    uint32_t timed_mode;       /* Native DWORD at 0x006044B0. */
    uint32_t elapsed;          /* Native unsigned DWORD at 0x012651A0. */
    float frames_per_second;  /* Native binary32 at 0x006044AC. */
} PharaohSpriteCycleClock;

typedef struct PharaohSpriteDirection {
    uint16_t index;
    uint32_t mirrored;
} PharaohSpriteDirection;

typedef struct PharaohSpriteCycle {
    uint32_t reserved_00;
    PharaohSpriteCycleOrientation orientation;
    uint8_t reserved_08[0x10];
    int32_t width, height, anchor;
    uint8_t reserved_24[0x10];
    uint32_t flags;
    const PharaohSpriteCycleSource *source;
    uint16_t action;
    uint16_t reserved_3e;
    union {
        const uint8_t *descriptor;
        const PharaohSpriteCycleAction *current_action;
    };
    union {
        const uint8_t *variant;
        const PharaohSpriteCycleVariant *current_variant;
    };
    float frame;
    float playback_rate;
    float action_rate;
    uint8_t reserved_54[0x24];
    uint16_t repeats_remaining;
} PharaohSpriteCycle;

_Static_assert(offsetof(PharaohSpriteCycle, descriptor) == 0x40 &&
    offsetof(PharaohSpriteCycle, variant) == 0x44 &&
    offsetof(PharaohSpriteCycle, frame) == 0x48 &&
    offsetof(PharaohSpriteCycle, repeats_remaining) == 0x78,
    "native sprite cycle layout");
_Static_assert(sizeof(PharaohSpriteCycleAction) == 16 &&
    offsetof(PharaohSpriteCycleAction, directions) == 11 &&
    sizeof(PharaohSpriteCycleVariant) == 8 &&
    offsetof(PharaohSpriteCycleSourceData, actions) == 0x1c &&
    offsetof(PharaohSpriteCycle, width) == 0x18 &&
    offsetof(PharaohSpriteCycle, orientation) == 4,
    "native sprite controller data layout");

/* Native 0x0055D330/460. frame=1000 keeps the existing frame. The unused
 * native middle argument and private by-value argument-slot writes are absent. */
void pharaoh_sprite_cycle_select_action(PharaohSpriteCycle *cycle,
    uint16_t action, int16_t frame);
void pharaoh_sprite_cycle_set_action(PharaohSpriteCycle *cycle,
    uint16_t action);
/* Native 0x0055D4F0/6D0/760/780/7B0/810. */
uint32_t pharaoh_sprite_cycle_tick(PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock);
uint32_t pharaoh_sprite_cycle_dispatch(PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock);
uint32_t pharaoh_sprite_cycle_finished(PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock);
uint32_t pharaoh_sprite_cycle_stop_when_finished(PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock);
uint32_t pharaoh_sprite_cycle_hold_last_frame(PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock);
uint32_t pharaoh_sprite_cycle_next_action(PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock);
/* Native 0x0055D840/9C0. Only the logical WORD index and mirror flag are
 * exposed; native private stack padding is outside these interfaces. */
uint16_t pharaoh_sprite_cycle_direction(PharaohSpriteCycle *cycle);
PharaohSpriteDirection *pharaoh_sprite_direction_from_angle(
    PharaohSpriteDirection *output, const PharaohSpriteCycleAction *action,
    float angle);

/* Native entries 0x0055D480, 0x0055D4C0, 0x0055D590 and 0x0055D630. */
uint32_t pharaoh_sprite_cycle_add_forward(
    PharaohSpriteCycle *cycle, float step);
uint32_t pharaoh_sprite_cycle_add_reverse(
    PharaohSpriteCycle *cycle, float step);
uint32_t pharaoh_sprite_cycle_wrap_forward(PharaohSpriteCycle *cycle);
uint32_t pharaoh_sprite_cycle_wrap_reverse(PharaohSpriteCycle *cycle);

#endif

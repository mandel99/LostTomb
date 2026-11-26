#include "pharaoh/rendering/sprite_cycle.h"
#include "pharaoh/rendering/debug_sprite.h"
#include "pharaoh/core/x87_numeric.h"

#include <math.h>
#include <string.h>

static uint16_t read_u16(const void *source)
{
    uint16_t value;
    memcpy(&value, source, sizeof(value));
    return value;
}

static float cycle_length(const PharaohSpriteCycle *cycle)
{
    return (float)read_u16(cycle->variant + 2);
}

static uint16_t cycle_repeat_count(const PharaohSpriteCycle *cycle)
{
    const uint32_t shift = (cycle->descriptor[0] & 7u) == 4u;
    return (uint16_t)((uint32_t)cycle->descriptor[3] << shift);
}

/* Returns the native x87 C0 bit: set for less-than and unordered. */
static uint32_t cycle_less(const PharaohSpriteCycle *cycle, float limit)
{
#if defined(_MSC_VER) && defined(_M_IX86)
    uint16_t status;
    __asm {
        mov eax, cycle
        fld dword ptr [eax+48h]
        fcomp limit
        fnstsw status
    }
    return (uint32_t)(status & 0x100u);
#else
    return (uint32_t)(isnan(cycle->frame) || isnan(limit) ||
        cycle->frame < limit) << 8;
#endif
}

/* Stores binary32 but compares the still-extended x87 result, as native. */
static uint32_t cycle_sum(PharaohSpriteCycle *cycle, float step,
    int subtract, float limit)
{
#if defined(_MSC_VER) && defined(_M_IX86)
    uint16_t status;
    if (subtract != 0) {
        __asm {
            mov eax, cycle
            fld dword ptr [eax+48h]
            fsub step
            fst dword ptr [eax+48h]
            fcomp limit
            fnstsw status
        }
    } else {
        __asm {
            mov eax, cycle
            fld step
            fadd dword ptr [eax+48h]
            fst dword ptr [eax+48h]
            fcomp limit
            fnstsw status
        }
    }
    return (uint32_t)(status & 0x100u);
#else
    const float result = subtract != 0 ? cycle->frame - step :
        cycle->frame + step;
    cycle->frame = result;
    return (uint32_t)(isnan(result) || isnan(limit) || result < limit) << 8;
#endif
}

static void cycle_set_last_frame(PharaohSpriteCycle *cycle, float count_value)
{
#if defined(_MSC_VER) && defined(_M_IX86)
    static const float one = 1.0f;
    __asm {
        mov eax, cycle
        fld count_value
        fsub dword ptr one
        fstp dword ptr [eax+48h]
    }
#else
    cycle->frame = count_value - 1.0f;
#endif
}

uint32_t pharaoh_sprite_cycle_add_forward(
    PharaohSpriteCycle *cycle, float step)
{
    return cycle_sum(cycle, step, 0, cycle_length(cycle)) == 0u;
}

uint32_t pharaoh_sprite_cycle_add_reverse(
    PharaohSpriteCycle *cycle, float step)
{
    return cycle_sum(cycle, step, 0, 0.0f) != 0u;
}

uint32_t pharaoh_sprite_cycle_wrap_forward(PharaohSpriteCycle *cycle)
{
    const float length = cycle_length(cycle);
    uint32_t finished = 0;

    while (cycle_less(cycle, length) == 0u && finished == 0u) {
        const uint32_t less = cycle_sum(cycle, length, 1, length);
        if (cycle->repeats_remaining != 0u) {
            --cycle->repeats_remaining;
            if (cycle->repeats_remaining == 0u) {
                if (less == 0u) cycle_set_last_frame(cycle, length);
                cycle->repeats_remaining = cycle_repeat_count(cycle);
                finished = 1;
            }
        }
    }
    return finished;
}

uint32_t pharaoh_sprite_cycle_wrap_reverse(PharaohSpriteCycle *cycle)
{
    const float length = cycle_length(cycle);
    uint32_t finished = 0;

    while (cycle_less(cycle, 0.0f) != 0u && finished == 0u) {
        const uint32_t less = cycle_sum(cycle, length, 0, 0.0f);
        if (cycle->repeats_remaining != 0u) {
            --cycle->repeats_remaining;
            if (cycle->repeats_remaining == 0u) {
                if (less != 0u) cycle->frame = 0.0f;
                cycle->repeats_remaining = cycle_repeat_count(cycle);
                finished = 1;
            }
        }
    }
    return finished;
}

/* The already-owned 55D290/2E0/300/750 methods operate on this same native
 * object. Keep their canonical owners and use the established typed overlay. */
_Static_assert(offsetof(PharaohSpriteCycle, current_action) ==
    offsetof(PharaohDebugSprite, animation) &&
    offsetof(PharaohSpriteCycle, current_variant) ==
    offsetof(PharaohDebugSprite, direction) &&
    offsetof(PharaohSpriteCycle, frame) == offsetof(PharaohDebugSprite, frame) &&
    sizeof(PharaohSpriteCycleVariant) == sizeof(PharaohDebugSpriteDirection),
    "shared debug sprite member view");

/* These small arithmetic boundaries retain native inherited x87 PC/RC,
 * extended intermediates and the two intentional binary32 ratio spills. */
static const float direction_full_turn = 360.0f;
static const float direction_half_turn = 180.0f;
static const float direction_unmirrored_origin = 225.0f;
static const float direction_mirrored_origin = 135.0f;
static const float direction_half = 0.5f;
static const float direction_one = 1.0f;
static const float direction_zero = 0.0f;

static void direction_origin(PharaohFloat80 *value, int32_t sector_offset,
    float angle, int mirrored)
{
    __asm { fild sector_offset }
    __asm { fld angle }
    if (mirrored != 0) { __asm { fsub direction_mirrored_origin } }
    else { __asm { fadd direction_unmirrored_origin } }
    __asm { fsubr direction_full_turn }
    __asm { faddp st(1), st(0) }
    __asm { mov eax, value }
    __asm { fstp tbyte ptr [eax] }
}

static uint16_t direction_compare(const PharaohFloat80 *value, float limit)
{
    uint16_t status;
    __asm {
        mov eax, value
        fld tbyte ptr [eax]
        fcomp limit
        fnstsw status
    }
    return status;
}

static void direction_adjust(PharaohFloat80 *value, int subtract)
{
    __asm { mov eax, value }
    __asm { fld tbyte ptr [eax] }
    if (subtract != 0) { __asm { fsub direction_full_turn } }
    else { __asm { fadd direction_full_turn } }
    __asm { mov eax, value }
    __asm { fstp tbyte ptr [eax] }
}

static uint16_t direction_mirrored_half(const PharaohFloat80 *value,
    int32_t steps, float *step_size, float *rounded_ratio)
{
    uint16_t status;
    __asm {
        mov eax, value
        fld tbyte ptr [eax]
        fild steps
        fld direction_full_turn
        fdiv st(0), st(1)
        mov eax, step_size
        fstp dword ptr [eax]
        fld st(1)
        fdiv dword ptr [eax]
        mov eax, rounded_ratio
        fst dword ptr [eax]
        fxch st(1)
        fmul direction_half
        fxch st(1)
        fcompp
        fnstsw status
        fstp st(0)
    }
    return status;
}

static void direction_reflected_ratio(PharaohFloat80 *result,
    const PharaohFloat80 *value, float step_size)
{
    __asm {
        mov eax, value
        fld tbyte ptr [eax]
        fld direction_full_turn
        fsub st(0), st(1)
        fdiv step_size
        fadd direction_one
        mov eax, result
        fstp tbyte ptr [eax]
        fstp st(0)
    }
}

static void direction_unmirrored_ratio(PharaohFloat80 *result,
    const PharaohFloat80 *value, int32_t count)
{
    __asm {
        mov eax, value
        fld tbyte ptr [eax]
        fild count
        fdivr direction_full_turn
        fdivr st(0), st(1)
        mov eax, result
        fstp tbyte ptr [eax]
        fstp st(0)
    }
}

static void direction_float80(PharaohFloat80 *result, float value)
{
    __asm {
        fld value
        mov eax, result
        fstp tbyte ptr [eax]
    }
}

PharaohSpriteDirection *pharaoh_sprite_direction_from_angle(
    PharaohSpriteDirection *output, const PharaohSpriteCycleAction *action,
    float angle)
{
    PharaohFloat80 value, ratio;
    uint16_t index = 0, status;
    uint32_t mirrored = 0;
    int32_t count = action->directions, steps;
    float step_size, rounded_ratio;

    if (count <= 1) {
        direction_float80(&value, angle);
        mirrored = (direction_compare(&value, direction_half_turn) & 0x100u) == 0;
    } else if ((action->flags & 0x80u) != 0) {
        steps = count * 2 - 2;
        direction_origin(&value, 360 / (steps * 2), angle, 1);
        while ((direction_compare(&value, direction_zero) & 0x100u) != 0)
            direction_adjust(&value, 0);
        while ((direction_compare(&value, direction_full_turn) & 0x100u) == 0)
            direction_adjust(&value, 1);
        status = direction_mirrored_half(&value, steps, &step_size,
            &rounded_ratio);
        if ((status & 0x4100u) != 0) {
            direction_float80(&ratio, rounded_ratio);
        } else {
            direction_reflected_ratio(&ratio, &value, step_size);
            mirrored = 1;
        }
        index = (uint16_t)(uint32_t)pharaoh_x87_truncate80(&ratio);
    } else {
        direction_origin(&value, 360 / (count * 2), angle, 0);
        /* Native performs one correction here, unlike the mirrored loops. */
        if ((direction_compare(&value, direction_zero) & 0x100u) != 0)
            direction_adjust(&value, 0);
        else if ((direction_compare(&value, direction_full_turn) & 0x100u) == 0)
            direction_adjust(&value, 1);
        direction_unmirrored_ratio(&ratio, &value, count);
        index = (uint16_t)(uint32_t)pharaoh_x87_truncate80(&ratio);
    }
    output->index = index;
    output->mirrored = mirrored;
    return output;
}

uint16_t pharaoh_sprite_cycle_direction(PharaohSpriteCycle *cycle)
{
    const PharaohSpriteCycleAction *action;
    PharaohSpriteDirection output;
    volatile float angle;

    if (cycle->source == NULL || cycle->source->data == NULL) return 0;
    action = cycle->current_action;
    /* The real embedded object's virtual method is called. Its extended ST0
     * result crosses the native binary32 spill before the direction mapper. */
    angle = cycle->orientation.methods->angle(&cycle->orientation);
    pharaoh_sprite_direction_from_angle(&output, action, angle);
    if (output.mirrored != 0) cycle->flags |= 1u;
    else cycle->flags &= ~1u;
    return output.index;
}

static uint16_t cycle_count_compare(uint32_t count,
    const PharaohSpriteCycle *cycle)
{
    uint16_t status;
    __asm {
        fild count
        mov eax, cycle
        fcomp dword ptr [eax+48h]
        fnstsw status
    }
    return status;
}

static void cycle_action_rate(PharaohSpriteCycle *cycle, uint32_t rate)
{
    __asm {
        fild rate
        mov eax, cycle
        fstp dword ptr [eax+50h]
    }
}

void pharaoh_sprite_cycle_select_action(PharaohSpriteCycle *cycle,
    uint16_t action, int16_t frame)
{
    const PharaohSpriteCycleSourceData *data;
    const PharaohSpriteCycleAction *descriptor;
    const PharaohSpriteCycleVariant *variants;
    const PharaohSpriteCycleImage *first, *middle;
    uint16_t variant, status;
    uint32_t count, directions;

    if (cycle->source == NULL) return;
    if (action != 0) {
        data = cycle->source->data;
        if (action >= data->action_count)
            action = (uint16_t)(data->action_count - 1u);
    }
    cycle->flags &= ~0x100u;
    cycle->action = action;
    data = cycle->source != NULL ? cycle->source->data : NULL;
    cycle->current_action = data->actions + action;
    variant = pharaoh_sprite_cycle_direction(cycle);
    if (frame == 1000)
        pharaoh_debug_sprite_set_direction((PharaohDebugSprite *)(void *)cycle,
            variant);
    else
        pharaoh_debug_sprite_set_direction_frame(
            (PharaohDebugSprite *)(void *)cycle, variant, frame);

    descriptor = cycle->current_action;
    cycle->repeats_remaining = (uint16_t)((uint32_t)descriptor->repeats <<
        ((descriptor->mode & 7u) == 4u));
    if (frame == 0)
        pharaoh_debug_sprite_set_frame((PharaohDebugSprite *)(void *)cycle,
            pharaoh_debug_sprite_first_frame());
    count = cycle->current_variant->frame_count;
    status = cycle_count_compare(count, cycle);
    if ((status & 0x4100u) != 0)
        pharaoh_debug_sprite_set_frame((PharaohDebugSprite *)(void *)cycle,
            (int16_t)(count - 1u));
    descriptor = cycle->current_action;
    cycle_action_rate(cycle, descriptor->rate);
    directions = descriptor->directions;
    if (directions > 1u) {
        variants = descriptor->variants;
        directions = descriptor->directions;
        middle = variants[directions >> 1].images;
        first = variants[0].images;
        cycle->width = first->extent;
        cycle->height = middle->extent;
        cycle->anchor = (int32_t)middle->anchor - cycle->width / 2;
    }
}

void pharaoh_sprite_cycle_set_action(PharaohSpriteCycle *cycle,
    uint16_t action)
{
    pharaoh_sprite_cycle_select_action(cycle, action, 0);
}

/* Native constants at 0x005702A8/AC: bits 447A0000/392EC33E. */
static const float cycle_milliseconds = 1000.0f;
static const float cycle_step_scale = 0.000166666664881631731987f;

static uint32_t cycle_scaled_step(const PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock, float *output)
{
    PharaohFloat80 delta, ratio;
    uint64_t elapsed;
    int32_t divisor;
    float step, fps;
    uint16_t status;

    __asm {
        mov eax, cycle
        fld dword ptr [eax+50h]
        fmul dword ptr [eax+4ch]
        fmul cycle_step_scale
        fst dword ptr step
        fstp tbyte ptr delta
    }
    if (clock->timed_mode != 0) {
        elapsed = clock->elapsed;
        fps = clock->frames_per_second;
        __asm {
            fld cycle_milliseconds
            fdiv fps
            fstp tbyte ptr ratio
        }
        /* FTOL returns signed64; FIDIV consumes its signed low DWORD. */
        divisor = (int32_t)(uint32_t)pharaoh_x87_truncate80(&ratio);
        __asm {
            fld tbyte ptr delta
            fild qword ptr elapsed
            fidiv divisor
            fmulp st(1), st(0)
            fstp tbyte ptr delta
        }
    }
    __asm {
        fld tbyte ptr delta
        fst dword ptr step
        fcomp direction_zero
        fnstsw status
    }
    *output = step;
    return (status & 0x100u) != 0;
}

uint32_t pharaoh_sprite_cycle_tick(PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock)
{
    float step;
    const uint32_t negative = cycle_scaled_step(cycle, clock, &step);
    if (negative != 0) {
        if (pharaoh_sprite_cycle_add_reverse(cycle, step) != 0)
            return pharaoh_sprite_cycle_wrap_reverse(cycle);
    } else if (pharaoh_sprite_cycle_add_forward(cycle, step) != 0) {
        return pharaoh_sprite_cycle_wrap_forward(cycle);
    }
    return 0;
}

uint32_t pharaoh_sprite_cycle_finished(PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock)
{
    return pharaoh_sprite_cycle_tick(cycle, clock) != 0;
}

uint32_t pharaoh_sprite_cycle_stop_when_finished(PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock)
{
    if (pharaoh_sprite_cycle_tick(cycle, clock) == 0) return 0;
    cycle->flags |= 8u;
    return 1;
}

uint32_t pharaoh_sprite_cycle_hold_last_frame(PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock)
{
    PharaohFloat80 frame80;
    int32_t frame;
    direction_float80(&frame80, cycle->frame);
    frame = (int16_t)(uint32_t)pharaoh_x87_truncate80(&frame80);
    if (frame == (int32_t)cycle->current_variant->frame_count - 1) {
        cycle->flags |= 0x100u;
        return 1;
    }
    if (pharaoh_sprite_cycle_tick(cycle, clock) == 0) return 0;
    pharaoh_debug_sprite_set_frame((PharaohDebugSprite *)(void *)cycle,
        (int16_t)(cycle->current_variant->frame_count - 1u));
    return 1;
}

uint32_t pharaoh_sprite_cycle_next_action(PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock)
{
    if (pharaoh_sprite_cycle_tick(cycle, clock) == 0) return 0;
    pharaoh_sprite_cycle_set_action(cycle, cycle->current_action->next_action);
    return 1;
}

uint32_t pharaoh_sprite_cycle_dispatch(PharaohSpriteCycle *cycle,
    const PharaohSpriteCycleClock *clock)
{
    uint32_t result;
    if (cycle->source == NULL) return 0;
    switch (cycle->current_action->mode & 7u) {
    case 0:
    case 4:
        result = pharaoh_sprite_cycle_finished(cycle, clock);
        break;
    case 1:
        /* Native returns directly without the common completion-flag store. */
        return pharaoh_sprite_cycle_hold_last_frame(cycle, clock);
    case 2:
        result = pharaoh_sprite_cycle_stop_when_finished(cycle, clock);
        break;
    case 3:
        result = pharaoh_sprite_cycle_next_action(cycle, clock);
        break;
    default:
        result = 0;
        break;
    }
    if (result != 0) cycle->flags |= 0x100u;
    else cycle->flags &= ~0x100u;
    return result;
}

#include "route_approach_candidates.h"

#define RA_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define RA_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define RA_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define RA_H(a) ((int32_t)(int16_t)RA_W(a))

typedef char route_approach_requires_pointer32[sizeof(void *) == 4 ? 1 : -1];
typedef void (__stdcall *route_debug_output)(const char *);

/* Bridges retain the physical ECX receiver and full explicit stack words.
   Their children remain the existing RAW providers, outside this C block. */
extern uint32_t restore_route_member_approach(uint32_t entry, uint32_t receiver,
    uint32_t x, uint32_t y, int32_t actor);
extern uint32_t restore_route_member_building_accept(uint32_t receiver,
    int32_t building, int32_t actor, uint32_t mode);
extern uint32_t restore_route_advance_travel(int32_t actor, uint32_t mode);
extern uint32_t FUN_0051cba0(int32_t x, int32_t y, int32_t tx, int32_t ty);
extern uint32_t FUN_004f01e0(uint32_t x, uint32_t y, uint32_t actor);
extern int32_t FUN_00420350(uint32_t x, uint32_t y, int32_t actor);
extern int32_t FUN_004e5c00(uint32_t x, uint32_t y, int32_t actor);
extern uint32_t restore_route_approach_00542a80(uint32_t x, uint32_t y, int32_t actor);
extern uint32_t restore_route_approach_004e0c00(uint32_t x, uint32_t y, int32_t actor);
extern void FUN_004b5420(int32_t actor);
extern void FUN_004b52f0(int32_t actor);

enum route_actor_field {
    RA_RENDER_STATE = 1, RA_KIND = 10, RA_DIRECTION = 17,
    RA_PREVIOUS_DIRECTION = 18, RA_X = 20, RA_Y = 22,
    RA_PREVIOUS_X = 24, RA_PREVIOUS_Y = 26, RA_CELL = 32,
    RA_TARGET_X = 36, RA_TARGET_Y = 38, RA_GOAL_X = 44, RA_GOAL_Y = 46, RA_PHASE = 57,
    RA_ON_ROAD = 65, RA_TURN_STEP = 72, RA_TURN_TICKS = 73,
    RA_FROM_BUILDING = 74, RA_FROM_POSITION = 76,
    RA_TO_BUILDING = 78, RA_TO_POSITION = 80,
    RA_TRAVEL_PROGRESS = 82, RA_TRAVEL_LENGTH = 84,
    RA_AT_DESTINATION = 101, RA_FOLLOWER_LINK = 104,
    RA_HEIGHT_TICKS = 119, RA_HEIGHT = 120, RA_TARGET_HEIGHT = 121,
    RA_TRAVEL_ACTIVE = 288
};

static uint32_t route_actor(int32_t actor)
{ return 0xbcd5f8u + (uint32_t)actor * 388u; }

/* 4B21D0: choose the nearer cardinal road direction around the current tile.
   Both searches read the original DWORD direction-offset table in order. */
__declspec(noinline) void restore_actor_choose_nearby_road_direction(int32_t actor)
{
    uint32_t record = route_actor(actor), cell, direction, clockwise, counterclockwise;
    uint32_t offset;
    int32_t y, x, target_y, target_x, clockwise_steps, counterclockwise_steps;
    y = RA_H(record + RA_Y);
    x = RA_H(record + RA_X);
    cell = (uint32_t)x + (uint32_t)y * 228u;
    cell += RA_U(0xbca2d8u);
    target_y = RA_H(record + RA_TARGET_Y);
    target_x = RA_H(record + RA_TARGET_X);
    direction = FUN_0051cba0(x, y, target_x, target_y) & 255u;
    if (direction >= 8u) direction = 0;
    clockwise = direction;
    for (clockwise_steps = 0; clockwise_steps < 8; ++clockwise_steps) {
        if ((clockwise & 1u) == 0) {
            offset = RA_U(0x5f80fcu + clockwise * 4u);
            if ((RA_B(0xaf598cu + (offset + cell) * 4u) & 64u) != 0) break;
        }
        clockwise += 1u;
        if (clockwise > 7u) clockwise = 0;
    }
    if (clockwise_steps == 8) clockwise = 0;
    counterclockwise = direction;
    for (counterclockwise_steps = 0; counterclockwise_steps < 8; ++counterclockwise_steps) {
        if ((counterclockwise & 1u) == 0) {
            offset = RA_U(0x5f80fcu + counterclockwise * 4u);
            if ((RA_B(0xaf598cu + (offset + cell) * 4u) & 64u) != 0) break;
        }
        counterclockwise -= 1u;
        if ((int32_t)counterclockwise < 0) counterclockwise = 7;
    }
    if (counterclockwise_steps == 8) counterclockwise = 4;
    if (clockwise_steps <= counterclockwise_steps) {
        RA_B(record + RA_DIRECTION) = (uint8_t)clockwise;
        RA_B(record + RA_TURN_STEP) = 2;
    } else {
        RA_B(record + RA_DIRECTION) = (uint8_t)counterclockwise;
        RA_B(record + RA_TURN_STEP) = 254;
    }
    RA_B(record + RA_TURN_TICKS) = 5;
}

/* 4A34F0: up to eight linked actors; even the final failed pass reads its link.
   The output pointer is volatile and may alias an actor field. */
__declspec(noinline) int32_t restore_actor_find_linked_kind85(int32_t actor, int32_t *length)
{
    int32_t linked = RA_H(route_actor(actor) + RA_FOLLOWER_LINK);
    uint32_t record, count, pass;
    uint32_t output = (uint32_t)(uintptr_t)length;
    RA_U(output) = 1;
    for (pass = 0; pass < 8u; ++pass) {
        record = route_actor(linked);
        if (RA_B(record + RA_KIND) == 85u) return linked;
        count = RA_U(output);
        RA_U(output) = count + 1u;
        linked = RA_H(record + RA_FOLLOWER_LINK);
    }
    return 0;
}

/* 4BC140: every native path leaves the selected child's full EAX.
   The existing public int32 facade can delegate to this named full-result API. */
__declspec(noinline) uint32_t restore_actor_approach_destination_result(
    uint32_t x, uint32_t y, int32_t actor)
{
    uint32_t result = FUN_004f01e0(x, y, (uint32_t)actor);
    if (result != 0) return result;
    result = (uint32_t)FUN_00420350(x, y, actor);
    if (result != 0) return result;
    result = (uint32_t)FUN_004e5c00(x, y, actor);
    if (result != 0) return result;
    result = restore_route_member_approach(0x4178e0u, 0x6fd368u, x, y, actor);
    if (result != 0) return result;
    result = restore_route_member_approach(0x4e6e10u, 0x6de548u, x, y, actor);
    if (result != 0) return result;
    result = restore_route_approach_00542a80(x, y, actor);
    if (result != 0) return result;
    result = restore_route_member_approach(0x40f070u, 0x6fd5d8u, x, y, actor);
    if (result != 0) return result;
    result = restore_route_member_approach(0x401f30u, 0x6de650u, x, y, actor);
    if (result != 0) return result;
    return restore_route_approach_004e0c00(x, y, actor);
}

static int32_t route_find_building_state(int32_t actor, uint32_t state)
{
    uint32_t type_field = 0x799af0u, receiver;
    uint16_t kind;
    int32_t building = 1;
    do {
        if (RA_B(type_field - 16u) == 1u) {
            kind = RA_W(type_field);
            switch (kind) {
            case 229: receiver = 0x6f9320u; break;
            case 234: receiver = 0x6f5648u; break;
            case 235: receiver = 0x6de9e0u; break;
            case 236: receiver = 0x6e9188u; break;
            default: receiver = 0; break;
            }
            if (receiver != 0 && RA_U(type_field + 80u) == state) {
                if ((uint8_t)restore_route_member_building_accept(receiver, building, actor, 1u) != 0)
                    return building;
            }
        }
        type_field += 264u;
        building += 1;
    } while (type_field < 0x89b6e8u);
    return 0;
}

__declspec(noinline) int32_t restore_actor_find_approach_building_state2(uint32_t x, uint32_t y, int32_t actor)
{ (void)x; (void)y; return route_find_building_state(actor, 2u); }

__declspec(noinline) int32_t restore_actor_find_approach_building_state3(uint32_t x, uint32_t y, int32_t actor)
{ (void)x; (void)y; return route_find_building_state(actor, 3u); }

/* 4B23C0: closely follow another actor, preserving travel-state handoff and
   tile crossing before heading and terrain-height interpolation. */
__declspec(noinline) void restore_actor_follow_leader_closely(int32_t actor, int32_t leader,
    uint32_t mode, int32_t distance)
{
    uint32_t record = route_actor(actor), other, debug_callback, difference, cell, terrain;
    int32_t old_x, old_y, new_y;
    uint16_t value, current_destination;
    uint8_t phase, height, target_height, ticks, terrain_kind;
    debug_callback = RA_U(0x56f0f8u);
    for (;;) {
        value = RA_W(record + RA_X);
        if (value == RA_W(record + RA_GOAL_X)) {
            value = RA_W(record + RA_Y);
            if (value == RA_W(record + RA_GOAL_Y)) RA_B(record + RA_AT_DESTINATION) = 1;
        }
        if (RA_W(record + RA_TRAVEL_ACTIVE) == 0) break;
        other = route_actor(leader);
        if (RA_W(other + RA_TRAVEL_ACTIVE) != 0) {
            current_destination = RA_W(record + RA_TO_BUILDING);
            if (current_destination == RA_W(other + RA_TO_BUILDING)) {
                difference = (uint32_t)RA_H(record + RA_TRAVEL_PROGRESS);
                value = RA_W(other + RA_TRAVEL_PROGRESS);
                difference += (uint32_t)distance;
                if ((int32_t)difference < (int32_t)(int16_t)value)
                    (void)restore_route_advance_travel(actor, mode);
                goto copy_render_state;
            }
            if (current_destination != RA_W(other + RA_FROM_BUILDING)) goto copy_travel;
            if ((uint8_t)restore_route_advance_travel(actor, mode) != 0) goto copy_travel;
            goto copy_render_state;
        }
        ((route_debug_output)(uintptr_t)debug_callback)((const char *)(uintptr_t)0x5df72cu);
        RA_W(record + RA_TRAVEL_ACTIVE) = 0;
    }
    other = route_actor(leader);
    if (RA_W(other + RA_TRAVEL_ACTIVE) != 0) {
        phase = (uint8_t)(RA_B(record + RA_PHASE) + 1u);
        RA_B(record + RA_PHASE) = phase;
        if ((int8_t)phase <= 8) goto copy_render_state;
        RA_B(record + RA_PHASE) = 0;
        goto copy_travel;
    }
    difference = (uint32_t)(int32_t)(int8_t)RA_B(other + RA_PHASE) - (uint32_t)distance;
    if ((int32_t)difference < 0) difference = (difference & 0xffffff00u) | (uint8_t)(difference + 15u);
    phase = (uint8_t)difference;
    RA_B(record + RA_PHASE) = phase;
    if (phase == 8u) {
        FUN_004b5420(actor);
        value = RA_W(other + RA_X);
        cell = RA_U(0xbca2d8u);
        old_x = RA_H(record + RA_X);
        old_y = RA_H(record + RA_Y);
        RA_W(record + RA_X) = value;
        value = RA_W(other + RA_Y);
        RA_W(record + RA_Y) = value;
        new_y = (int32_t)(int16_t)value;
        cell += (uint32_t)RA_H(record + RA_X) + (uint32_t)new_y * 228u;
        RA_U(record + RA_CELL) = cell;
        if (RA_H(record + RA_X) != old_x || RA_H(record + RA_Y) != old_y) {
            RA_W(record + RA_PREVIOUS_X) = (uint16_t)old_x;
            RA_W(record + RA_PREVIOUS_Y) = (uint16_t)old_y;
        }
        FUN_004b52f0(actor);
        cell = RA_U(record + RA_CELL);
        terrain = RA_U(0xaf598cu + cell * 4u);
        if ((terrain & 0x40000040u) == 0) {
            RA_B(record + RA_ON_ROAD) = 0;
        } else {
            RA_B(record + RA_ON_ROAD) = 1;
            if ((terrain & 4u) != 0) {
                terrain_kind = RA_B(0xadc36cu + cell);
                RA_B(record + RA_HEIGHT_TICKS) = 18;
                switch (terrain_kind) {
                case 0: target_height = 0; break;
                case 1: case 4: target_height = 15; break;
                case 2: target_height = 27; break;
                case 3: case 6: case 7: case 8: case 9: case 10: target_height = 14; break;
                case 5: case 13: target_height = 30; break;
                default: target_height = 36; break;
                }
                RA_B(record + RA_TARGET_HEIGHT) = target_height;
            }
        }
    }
    if (RA_B(record + RA_PHASE) == 0) {
        phase = RA_B(record + RA_DIRECTION);
        RA_B(record + RA_PREVIOUS_DIRECTION) = phase;
        phase = RA_B(other + RA_DIRECTION);
        RA_B(record + RA_DIRECTION) = phase;
    }
    ticks = RA_B(record + RA_HEIGHT_TICKS);
    if (ticks != 0) {
        ticks = (uint8_t)(ticks - 1u);
        RA_B(record + RA_HEIGHT_TICKS) = ticks;
        if (ticks == 0) goto clear_arrival;
        RA_B(record + RA_AT_DESTINATION) = 1;
        height = RA_B(record + RA_HEIGHT);
        if (height < RA_B(record + RA_TARGET_HEIGHT)) RA_B(record + RA_HEIGHT) = (uint8_t)(height + 1u);
        height = RA_B(record + RA_HEIGHT);
        target_height = RA_B(record + RA_TARGET_HEIGHT);
        if (height > target_height) RA_B(record + RA_HEIGHT) = (uint8_t)(height - 1u);
        goto copy_render_state;
    }
    height = RA_B(record + RA_HEIGHT);
    if (height != 0) {
        RA_B(record + RA_HEIGHT) = (uint8_t)(height - 1u);
        goto copy_render_state;
    }
clear_arrival:
    RA_B(record + RA_AT_DESTINATION) = 0;
    goto copy_render_state;
copy_travel:
    value = RA_W(other + RA_TRAVEL_ACTIVE);
    RA_W(record + RA_TRAVEL_ACTIVE) = value;
    value = RA_W(other + RA_TRAVEL_LENGTH);
    RA_W(record + RA_TRAVEL_LENGTH) = value;
    difference = (uint32_t)RA_H(other + RA_TRAVEL_PROGRESS) - (uint32_t)distance;
    /* SETS/DEC/AND uses the wrapped result's sign, even on signed overflow. */
    RA_W(record + RA_TRAVEL_PROGRESS) = (uint16_t)((int32_t)difference < 0 ? 0u : difference);
    value = RA_W(other + RA_FROM_BUILDING);
    RA_W(record + RA_FROM_BUILDING) = value;
    value = RA_W(other + RA_TO_BUILDING);
    RA_W(record + RA_TO_BUILDING) = value;
    value = RA_W(other + RA_FROM_POSITION);
    RA_W(record + RA_FROM_POSITION) = value;
    value = RA_W(other + RA_TO_POSITION);
    RA_W(record + RA_TO_POSITION) = value;
    phase = RA_B(other + RA_DIRECTION);
    RA_B(record + RA_DIRECTION) = phase;
    (void)restore_route_advance_travel(actor, 0);
copy_render_state:
    phase = RA_B(other + RA_RENDER_STATE);
    RA_B(record + RA_RENDER_STATE) = phase;
}

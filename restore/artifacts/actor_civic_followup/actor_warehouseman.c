#include <stdint.h>

#define WU(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define WW(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define WB(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define WH(a) ((int32_t)(int16_t)WW(a))
typedef char warehouseman_requires_pointer32[sizeof(void *) == 4 ? 1 : -1];

extern void FUN_004b17d0(int32_t), FUN_004b1d50(int32_t), FUN_004b3250(int32_t);
extern void FUN_0048b4a0(int32_t);
extern void FUN_004a5da0(int32_t, uint32_t);
extern uint32_t FUN_004a35e0(int32_t), FUN_00475b50(uint32_t);
extern void FUN_0045b880(int32_t, int32_t *, int32_t *, int32_t *, int32_t *, int32_t *, int32_t *);
extern int32_t FUN_0045bb50(int32_t, int32_t, char);
extern int32_t FUN_0045de80(int32_t), FUN_0045dcc0(int32_t, int32_t, uint32_t);
extern int32_t FUN_0045e150(int32_t, int32_t), FUN_0045e220(int32_t, int32_t, int32_t, int32_t);
extern int32_t FUN_0045e360(int32_t, int32_t, int32_t, int32_t);
extern int32_t FUN_0045b2a0(int32_t, int32_t, int32_t, int32_t);
extern int32_t FUN_0045b570(int32_t, int32_t, int32_t, int32_t);
extern int32_t FUN_0045fe20(int32_t, int32_t, int32_t), FUN_00460890(int32_t, int32_t, int32_t);
extern int32_t FUN_0045eab0(uint32_t, int32_t, uint32_t), FUN_0045f450(uint32_t, int32_t, uint32_t);
extern int32_t FUN_0045f7b0(uint32_t, uint32_t, int32_t, uint32_t);
extern uint32_t FUN_0045acf0(int32_t, uint32_t, int32_t, uint32_t);
extern int32_t FUN_0045d9e0(int32_t, int32_t, int32_t, uint32_t);

static uint32_t warehouse_actor(uint32_t id)
{ return 0xbcd600u + id * 388u; }
static uint32_t warehouse_building(int32_t id)
{ return 0x7999d8u + (uint32_t)id * 264u; }

/* Native48AE50. The existing canonical void facade must not be sampled for
   physical EAX: this API computes and returns the real full query result. */
__declspec(noinline) int32_t restore_actor_find_remaining_delivery_destination(
    int32_t id, uint32_t network)
{
    uint32_t actor = warehouse_actor((uint32_t)id);
    int32_t y, resource, x, result;
    y = WH(actor + 14u);
    resource = WB(actor + 3u);
    x = WH(actor + 12u);
    result = FUN_0045f7b0((uint32_t)x, (uint32_t)y, resource, network);
    if (result == 0) {
        resource = WB(actor + 3u);
        result = FUN_0045eab0((uint32_t)id, resource, network);
        if (result == 0) {
            resource = WB(actor + 3u);
            result = (int32_t)FUN_0045acf0(0, (uint32_t)id, resource, network);
            if (result == 0) {
                resource = WB(actor + 3u);
                result = FUN_0045d9e0(0, id, resource, network);
                if (result == 0) {
                    resource = WB(actor + 3u);
                    result = FUN_0045d9e0(1, id, resource, network);
                }
            }
        }
    }
    WW(actor + 0x56u) = (uint16_t)result;
    return result;
}

static uint32_t warehouseman_reset_animation(uint32_t id)
{
    WB(warehouse_actor(id) - 7u) = 0;
    FUN_0048b4a0((int32_t)id);
    return WU(0xd056f8u);
}

static void warehouseman_query_target(uint32_t id, int32_t destination, uint8_t state)
{
    uint32_t actor = warehouse_actor(id);
    uint16_t x;
    WW(actor + 0x56u) = (uint16_t)destination;
    WB(actor + 0x30u) = state;
    WW(actor + 0x2eu) = 0;
    x = WW(0xd3b4e4u);
    WW(actor + 0x1cu) = x;
    WW(actor + 0x1eu) = WW(0xd3b4e0u);
}

/* This helper only preserves the repeated child/refresh phase, not a new
   native owner. Movement's incidental physical result is never sampled. */
static uint32_t warehouseman_advance(uint32_t id, uint32_t speed)
{
    FUN_0048b4a0((int32_t)id);
    id = WU(0xd056f8u);
    FUN_004a5da0((int32_t)id, speed);
    return WU(0xd056f8u);
}

static uint32_t warehouseman_direction_end(uint32_t id, uint8_t direction)
{
    if (direction == 9u) {
        FUN_004b3250((int32_t)id);
        id = WU(0xd056f8u);
    } else if (direction == 10u) {
        WB(warehouse_actor(id) + 6u) = 2;
    }
    return id;
}

static uint32_t warehouseman_return_path(uint32_t actor, uint32_t route_actor)
{
    uint16_t x = WW(actor + 0x24u);
    WW(actor + 0x1cu) = x;
    WW(actor + 0x1eu) = WW(actor + 0x26u);
    FUN_004b3250((int32_t)route_actor);
    return WU(0xd056f8u);
}

static int32_t warehouseman_batch_size(uint8_t resource)
{
    switch (resource) {
    case 10: case 12: case 24: case 25: case 26: case 28: case 30: case 35:
        return 100;
    default: return 400;
    }
}

static int warehouseman_reverse_offset(uint8_t resource)
{
    switch (resource) {
    case 12: case 24: case 25: case 26: case 30: case 35: return 1;
    default: return 0;
    }
}

static uint32_t warehouseman_render(uint32_t id)
{
    uint32_t actor = warehouse_actor(id), direction, base, image, overlay, prefix, facing;
    uint8_t current = WB(actor + 9u), state, frame, resource, offset_x, offset_y;
    int32_t selected = (int8_t)current;
    if (selected >= 8) selected = (int8_t)WB(actor + 10u);
    direction = (uint32_t)selected - WU(0xbca2e0u);
    WU(0xe94e00u) = direction;
    if ((int32_t)direction < 0) {
        direction += 8u;
        WU(0xe94e00u) = direction;
    }
    state = WB(actor + 0x30u);
    if (state == 4u) {
        int32_t half = WH(actor + 0x2eu) / 2;
        frame = WB(0x5d048cu + (uint32_t)half);
        base = WU(0xf55e4cu);
        WU(actor - 4u) = base + (uint32_t)frame;
        WU(actor + 0x178u) = 0;
    } else if (state != 6u) {
        base = WU(0xf55e44u);
        frame = WB(actor - 7u);
        image = base + (uint32_t)frame * 8u + direction;
        WU(actor - 4u) = image;
    }
    overlay = WU(actor + 0x178u);
    if (overlay == 0u) return 0;
    overlay += direction;
    facing = direction;
    WU(actor + 0x178u) = overlay;
    prefix = overlay & UINT32_C(0xffffff00);
    resource = WB(actor + 3u);
    if (resource == 0u) resource = WB(actor + 0x72u);
    if (warehouseman_reverse_offset(resource)) {
        facing += 4u;
        if ((int32_t)facing >= 8) facing -= 8u;
    }
    /* Absolute native table addressing preserves out-of-range reads; no
       private eight-element array or invented heading clamp is introduced. */
    offset_x = WB(0x5dc23cu + facing * 2u);
    offset_y = WB(0x5dc23du + facing * 2u);
    WB(actor + 0x64u) = offset_x;
    WB(actor + 0x65u) = offset_y;
    return prefix | (uint32_t)offset_x;
}

/* Returns0 only for the native early-RET path, before any rendering. */
static int warehouseman_idle_step(uint32_t *id_io, uint32_t network)
{
    uint32_t id = *id_io, actor = warehouse_actor(id), building, slot;
    int32_t home = WH(actor + 0x52u), kind, destination, resource, x, y;
    uint8_t carried, policy, selected_state;
    uint16_t target_x;
    building = warehouse_building(home);
    if (WB(building) != 1u ||
        ((uint32_t)WH(building + 0x2au) != id &&
         (uint32_t)WH(building + 0x2cu) != id &&
         (uint32_t)WH(building + 0xb6u) != id))
        WB(actor + 6u) = 2;
    WW(actor + 0x2eu) = (uint16_t)(WW(actor + 0x2eu) + 1u);
    if (WH(actor + 0x2eu) <= 2) {
        *id_io = warehouseman_reset_animation(id);
        return 1;
    }
    kind = WH(building + 0x10u);
    carried = WB(actor + 3u);
    if (kind != 71 && kind != 191) {
        if (carried == 0u) {
            int32_t first_resource = 0, second_resource = 0;
            int32_t first_percent = 100, second_percent = 100;
            int32_t first_source = 0, second_source = 0;
            FUN_0045b880(home, &first_resource, &second_resource,
                &first_percent, &second_percent, &first_source, &second_source);
            x = WH(building + 0xb6u);
            id = WU(0xd056f8u);
            resource = ((uint32_t)x == id && second_resource != 0) ? second_resource : first_resource;
            destination = FUN_0045bb50(home, resource, 0);
            if (destination == 0) {
                id = WU(0xd056f8u);
                WB(warehouse_actor(id) + 6u) = 2;
                *id_io = id;
                return 0;
            }
            id = WU(0xd056f8u);
            actor = warehouse_actor(id);
            kind = WH(warehouse_building(destination) + 0x10u);
            WB(actor + 0x72u) = (uint8_t)resource;
            WW(actor + 0x6cu) = 0;
            selected_state = kind == 72 ? 15u : 18u;
            WW(actor + 0x56u) = (uint16_t)destination;
            WB(actor + 0x30u) = selected_state;
            /* This native arm captures global X before resetting the timer;
               the other target arms deliberately reset it before that read. */
            target_x = WW(0xd3b4e4u);
            WW(actor + 0x2eu) = 0;
            WW(actor + 0x1cu) = target_x;
            WW(actor + 0x1eu) = WW(0xd3b4e0u);
            WB(actor + 0x6au) = 3;
        } else {
            y = WH(actor + 14u);
            x = WH(actor + 12u);
            destination = FUN_0045f7b0((uint32_t)x, (uint32_t)y, carried, network);
            if (destination == 0) {
                id = WU(0xd056f8u); carried = WB(warehouse_actor(id) + 3u);
                destination = FUN_0045eab0(id, carried, network);
            }
            if (destination == 0) {
                id = WU(0xd056f8u); carried = WB(warehouse_actor(id) + 3u);
                destination = FUN_0045d9e0(0, (int32_t)id, carried, network);
            }
            if (destination == 0) {
                id = WU(0xd056f8u); carried = WB(warehouse_actor(id) + 3u);
                destination = FUN_0045dcc0((int32_t)id, carried, network);
            }
            if (destination == 0) {
                id = WU(0xd056f8u); carried = WB(warehouse_actor(id) + 3u);
                destination = (int32_t)FUN_0045acf0(home, id, carried, network);
            }
            if (destination == 0) {
                id = WU(0xd056f8u); carried = WB(warehouse_actor(id) + 3u);
                destination = FUN_0045f450(id, carried, network);
            }
            if (destination != 0) {
                id = WU(0xd056f8u);
                warehouseman_query_target(id, destination, 9);
            } else {
                id = WU(0xd056f8u);
                FUN_0048b4a0((int32_t)id);
                id = WU(0xd056f8u); actor = warehouse_actor(id);
                slot = (uint32_t)WB(building + 0xc1u) * 196u;
                carried = WB(actor + 3u);
                policy = WB(0x7900c2u + slot + (uint32_t)carried);
                (void)policy; /* All native policy arms here set state8. */
                WB(actor + 0x30u) = 8;
            }
        }
    } else if (carried == 0u) {
        destination = FUN_0045de80(home);
        if (destination != 0) {
            id = WU(0xd056f8u);
            warehouseman_query_target(id, destination, 18);
        } else {
            id = WU(0xd056f8u); carried = WB(warehouse_actor(id) + 0x72u);
            destination = FUN_0045bb50(home, carried, 0);
            if (destination != 0) {
                id = WU(0xd056f8u);
                kind = WH(warehouse_building(destination) + 0x10u);
                selected_state = kind == 72 ? 20u : 18u;
                warehouseman_query_target(id, destination, selected_state);
            } else {
                id = WU(0xd056f8u);
                WB(warehouse_actor(id) + 0x30u) = 8;
                FUN_0048b4a0((int32_t)id);
                id = WU(0xd056f8u);
            }
        }
    } else {
        destination = FUN_0045d9e0(0, (int32_t)id, carried, network);
        if (destination == 0) {
            id = WU(0xd056f8u); carried = WB(warehouse_actor(id) + 3u);
            destination = (int32_t)FUN_0045acf0(0, id, carried, network);
        }
        if (destination == 0) {
            id = WU(0xd056f8u); carried = WB(warehouse_actor(id) + 3u);
            destination = FUN_0045d9e0(1, (int32_t)id, carried, network);
        }
        if (destination != 0) {
            id = WU(0xd056f8u);
            warehouseman_query_target(id, destination, 9);
        } else {
            id = WU(0xd056f8u);
            FUN_0048b4a0((int32_t)id);
            id = WU(0xd056f8u); actor = warehouse_actor(id);
            slot = (uint32_t)WB(building + 0xc1u) * 196u;
            carried = WB(actor + 3u);
            policy = WB(0x7900c2u + slot + (uint32_t)carried);
            if (policy == 3u || policy == 2u)
                WB(actor + 0x30u) = 8;
            else {
                target_x = WW(building + 6u);
                WW(actor + 0x56u) = (uint16_t)home;
                WB(actor + 0x30u) = 9;
                WW(actor + 0x2eu) = 0;
                WW(actor + 0x1cu) = target_x;
                WW(actor + 0x1eu) = WW(building + 8u);
            }
        }
    }
    *id_io = warehouseman_reset_animation(id);
    return 1;
}

static int warehouse_resource_booking_kind(int32_t kind)
{
    if (kind >= 110 && kind <= 114) return 1;
    switch (kind) {
    case 32: case 47: case 51: case 53: case 74: case 113:
    case 177: case 178: case 187: case 188: case 189: case 194:
    case 203: case 204: case 205: case 226: case 231: case 232: case 233:
        return 1;
    default: return 0;
    }
}

/* Authenticated26-entry subtype table at48CDC0; defaults include2 and19. */
static int32_t warehouse_resource_for_subtype(int32_t subtype)
{
    switch (subtype) {
    case 1:return 16; case 3:return 24; case 4:return 20; case 5:return 11;
    case 6:return 25; case 7:return 12; case 8:return 10; case 9:return 14;
    case 10:return 18; case 11:return 22; case 12:return 23; case 13:return 9;
    case 14:return 28; case 15:return 21; case 16:return 17; case 17:return 29;
    case 18:return 30; case 20:return 32; case 21:return 2; case 22:return 8;
    case 23:return 31; case 24:return 13; case 25:return 33; case 26:return 34;
    default:return -1;
    }
}

/* Native48CAA0. Four real stack arguments; the last consumes only a BYTE.
   Full return is0 on completion or the actual reroute result on partial cargo. */
__declspec(noinline) int32_t restore_actor_apply_warehouse_delivery(
    int32_t delivered, int32_t id, uint32_t network, uint8_t retire)
{
    uint32_t actor = warehouse_actor((uint32_t)id);
    int32_t linked = WH(actor + 0x56u), kind, resource, cargo, result;
    uint32_t building = warehouse_building(linked);
    uint16_t amount, target_x, target_y;
    kind = WH(building + 0x10u);
    if (warehouse_resource_booking_kind(kind)) {
        resource = warehouse_resource_for_subtype(WH(building + 0xceu));
        if (WW(building + 0x10u) == 204u && (int32_t)WB(actor + 3u) == resource)
            WW(building + 0x106u) = (uint16_t)(WW(building + 0x106u) - 1u);
        else
            WW(building + 0x104u) = (uint16_t)(WW(building + 0x104u) - 1u);
    } else if (WW(building + 0x10u) == 71u) {
        amount = WW(actor + 0x6cu);
        WW(building + 0x104u) = (uint16_t)(WW(building + 0x104u) - 1u);
        WW(building + 0x106u) = (uint16_t)(WW(building + 0x106u) - amount);
    }
    amount = WW(actor + 0x6cu);
    cargo = (int16_t)amount;
    if (delivered == cargo) {
        if (retire != 0u) {
            WB(actor + 6u) = 2;
            WW(actor + 0x2eu) = 0;
            return 0;
        }
        WB(actor + 0x30u) = 11;
        WW(actor + 0x2eu) = 0;
        target_x = WW(actor + 0x24u);
        WW(actor + 0x1cu) = target_x;
        target_y = WW(actor + 0x26u);
        WW(actor + 0x1eu) = target_y;
        WW(actor + 0x6cu) = 0;
        return 0;
    }
    WW(actor + 0x6cu) = (uint16_t)((uint32_t)amount - (uint32_t)delivered);
    result = restore_actor_find_remaining_delivery_destination(id, network);
    if (result != 0) {
        target_x = WW(0xd3b4e4u);
        target_y = WW(0xd3b4e0u);
        WB(actor + 0x30u) = 9;
        WW(actor + 0x2eu) = 0;
        WW(actor + 0x1cu) = target_x;
        WW(actor + 0x1eu) = target_y;
    } else {
        WB(actor + 0x30u) = 8;
        WB(actor + 0x5eu) = 2;
        WW(actor + 0x2eu) = 0;
    }
    return result;
}

/* Shipped ALL FIGURES row9 is Warehouseman, not Prefect. */
__declspec(noinline) uint32_t restore_actor_update_warehouseman(void)
{
    uint32_t id = WU(0xd056f8u), actor = warehouse_actor(id), kind;
    uint32_t speed, tile, network;
    uint8_t frame, state, direction, resource;
    int32_t home, linked, delivered, amount, batch;
    uint32_t player, base, old_direction, cached_actor;

    kind = WB(actor + 2u);
    WB(actor + 0x6au) = 1;
    frame = (uint8_t)(WB(actor - 7u) + 1u);
    speed = WU(0x7090ecu + kind * 56u);
    WB(actor - 7u) = frame;
    if (frame >= 12u) WB(actor - 7u) = 0;
    WU(actor + 0x178u) = 0;
    tile = WU(actor + 0x18u);
    network = WB(0xa76aecu + tile);
    state = WB(actor + 0x30u);

    switch (state) {
    case 5:
        FUN_004b17d0((int32_t)id);
        id = WU(0xd056f8u);
        break;
    case 4:
        FUN_004b1d50((int32_t)id);
        id = WU(0xd056f8u);
        break;
    case 8:
        if (!warehouseman_idle_step(&id, network)) return id;
        break;
    case 9:
        id = warehouseman_advance(id, speed);
        actor = warehouse_actor(id);
        direction = WB(actor + 9u);
        if (direction == 8u) WB(actor + 0x30u) = 10;
        else id = warehouseman_direction_end(id, direction);
        break;
    case 10:
        linked = WH(actor + 0x56u);
        (void)FUN_00475b50((uint32_t)linked);
        id = WU(0xd056f8u);
        FUN_0048b4a0((int32_t)id);
        id = WU(0xd056f8u); actor = warehouse_actor(id);
        WW(actor + 0x2eu) = (uint16_t)(WW(actor + 0x2eu) + 1u);
        if (WH(actor + 0x2eu) > 4) {
            int32_t building_kind;
            FUN_0048b4a0((int32_t)id);
            building_kind = WH(warehouse_building(linked) + 0x10u);
            id = WU(0xd056f8u); actor = warehouse_actor(id);
            if (building_kind == 72 || building_kind == 73 || building_kind == 193) {
                amount = WH(actor + 0x6cu); resource = WB(actor + 3u); player = WU(0xeaac28u);
                delivered = FUN_0045b2a0((int32_t)player, linked, resource, amount);
            } else if (building_kind == 71 || building_kind == 191) {
                amount = WH(actor + 0x6cu); resource = WB(actor + 3u);
                delivered = FUN_0045e220(linked, resource, 0, amount);
            } else if (building_kind == 95) {
                amount = WH(actor + 0x6cu); resource = WB(actor + 3u);
                delivered = FUN_00460890(linked, amount, resource);
            } else {
                resource = WB(actor + 3u); amount = WH(actor + 0x6cu);
                delivered = FUN_0045fe20(linked, amount, resource);
            }
            id = WU(0xd056f8u);
            (void)restore_actor_apply_warehouse_delivery(delivered, (int32_t)id, network, 0);
            id = WU(0xd056f8u);
        }
        WB(warehouse_actor(id) - 7u) = 0;
        break;
    case 11: case 12: case 18: case 20:
        id = warehouseman_advance(id, speed);
        actor = warehouse_actor(id); direction = WB(actor + 9u);
        if (direction == 8u) {
            if (state == 11u) WB(actor + 6u) = 2;
            else WB(actor + 0x30u) = state == 12u ? 13u : state == 18u ? 19u : 21u;
        } else id = warehouseman_direction_end(id, direction);
        break;
    case 13:
        home = WH(actor + 0x52u); linked = WH(actor + 0x56u);
        WW(actor + 0x2eu) = (uint16_t)(WW(actor + 0x2eu) + 1u);
        if (WH(actor + 0x2eu) > 4) {
            cached_actor = actor;
            amount = FUN_0045e150(home, linked);
            resource = WB(0x78d99cu);
            WW(cached_actor + 0x6cu) = (uint16_t)amount;
            WB(cached_actor + 3u) = resource;
            id = WU(0xd056f8u);
            WB(cached_actor + 0x30u) = 14;
            WW(cached_actor + 0x2eu) = 0;
            id = warehouseman_return_path(cached_actor, id);
        }
        WB(warehouse_actor(id) - 7u) = 0;
        break;
    case 19:
        FUN_0048b4a0((int32_t)id);
        id = WU(0xd056f8u); actor = warehouse_actor(id);
        home = WH(actor + 0x52u); linked = WH(actor + 0x56u);
        WW(actor + 0x2eu) = (uint16_t)(WW(actor + 0x2eu) + 1u);
        if (WH(actor + 0x2eu) > 4) {
            resource = WB(actor + 0x72u); player = WU(0xeaac28u);
            amount = FUN_0045e360((int32_t)player, linked, resource, 400);
            id = WU(0xd056f8u); actor = warehouse_actor(id);
            if (amount == 400) WW(actor + 0x6cu) = 0;
            else if (amount == 0) WW(actor + 0x6cu) = 400;
            else WW(actor + 0x6cu) = (uint16_t)amount;
            if (WW(actor + 0x6cu) == 0u) {
                WB(actor + 3u) = 0; WB(actor + 0x30u) = 8;
                FUN_004b3250((int32_t)id); id = WU(0xd056f8u);
            } else {
                resource = WB(actor + 0x72u); WB(actor + 3u) = resource;
                kind = WW(warehouse_building(home) + 0x10u);
                WB(actor + 0x30u) = kind == 71u ? 14u : 17u;
                WW(actor + 0x2eu) = 0;
                id = warehouseman_return_path(actor, id);
            }
        }
        WB(warehouse_actor(id) - 7u) = 0;
        break;
    case 21: case 16:
        if (state == 16u) WB(actor + 0x6au) = 3;
        linked = WH(actor + 0x56u);
        WW(actor + 0x2eu) = (uint16_t)(WW(actor + 0x2eu) + 1u);
        if (WH(actor + 0x2eu) > 4) {
            if (state == 16u) WW(actor + 0x6cu) = 0;
            resource = WB(actor + 0x72u);
            batch = warehouseman_batch_size(resource); player = WU(0xeaac28u);
            amount = FUN_0045b570((int32_t)player, linked, resource, batch);
            id = WU(0xd056f8u); actor = warehouse_actor(id);
            WW(actor + 0x6cu) = amount == batch ? 0u : (uint16_t)((uint32_t)batch - (uint32_t)amount);
            if (WW(actor + 0x6cu) == 0u) {
                WB(actor + 3u) = 0; WB(actor + 0x30u) = 8;
                FUN_004b3250((int32_t)id); id = WU(0xd056f8u);
            } else {
                resource = WB(actor + 0x72u); WB(actor + 3u) = resource;
                WB(actor + 0x30u) = state == 21u ? 14u : 17u;
                WW(actor + 0x2eu) = 0;
                id = warehouseman_return_path(actor, id);
            }
        }
        WB(warehouse_actor(id) - 7u) = 0;
        break;
    case 14: case 17:
        if (state == 17u) WB(actor + 0x6au) = 3;
        home = WH(actor + 0x52u);
        id = warehouseman_advance(id, speed);
        actor = warehouse_actor(id); direction = WB(actor + 9u);
        if (direction == 8u) {
            amount = WH(actor + 0x6cu); resource = WB(actor + 3u);
            if (state == 14u) delivered = FUN_0045e220(home, resource, 0, amount);
            else {
                player = WU(0xeaac28u);
                delivered = FUN_0045b2a0((int32_t)player, home, resource, amount);
            }
            id = WU(0xd056f8u);
            (void)restore_actor_apply_warehouse_delivery(delivered, (int32_t)id, network, 1);
            id = WU(0xd056f8u);
        } else id = warehouseman_direction_end(id, direction);
        break;
    case 15:
        WB(actor + 0x6au) = 3;
        id = warehouseman_advance(id, speed);
        actor = warehouse_actor(id); direction = WB(actor + 9u);
        if (direction == 8u) WB(actor + 0x30u) = 16;
        else id = warehouseman_direction_end(id, direction);
        break;
    case 7:
        WB(actor - 7u) = 0;
        FUN_0048b4a0((int32_t)id);
        id = WU(0xd056f8u);
        old_direction = WU(0xe94e00u); base = WU(0xf55e44u);
        actor = warehouse_actor(id);
        WU(actor - 4u) = base + old_direction + 1u;
        WW(actor + 0x2eu) = (uint16_t)(WW(actor + 0x2eu) + 1u);
        if (WH(actor + 0x2eu) > 100) {
            uint8_t found;
            WW(actor + 0x2eu) = 0;
            found = (uint8_t)FUN_004a35e0((int32_t)id);
            id = WU(0xd056f8u);
            if (found == 0u) WB(warehouse_actor(id) + 0x30u) = 8;
        }
        break;
    case 6:
        WU(actor - 4u) = 0;
        if (WU(actor + 0x178u) != 0u) WU(actor + 0x178u) = 0;
        break;
    default:
        WB(actor + 0x30u) = 8;
        break;
    }
    return warehouseman_render(id);
}

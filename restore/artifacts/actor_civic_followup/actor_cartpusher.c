#include <stdint.h>

#define CU(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define CW(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define CB(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define CH(a) ((int32_t)(int16_t)CW(a))

typedef char cartpusher_requires_pointer32[sizeof(void *) == 4 ? 1 : -1];

extern void FUN_004b17d0(int32_t), FUN_004b1d50(int32_t), FUN_004b3250(int32_t);
extern void FUN_004a5da0(int32_t, uint32_t);
extern uint32_t FUN_004a35e0(int32_t), FUN_00475b50(uint32_t);
extern uint32_t FUN_0048b320(uint32_t), FUN_00488030(uint32_t);
extern uint32_t FUN_004894a0(int32_t);
extern int32_t FUN_0045b2a0(int32_t, int32_t, int32_t, int32_t);
extern int32_t FUN_0045e220(int32_t, int32_t, int32_t, int32_t);
extern int32_t FUN_0045fe20(int32_t, int32_t, int32_t);
extern int32_t FUN_00460890(int32_t, int32_t, int32_t);
extern int32_t restore_actor_find_remaining_delivery_destination(int32_t, uint32_t);
/* Native48AEF0 has three stack arguments. It remains the existing RAW
   provider; the four-argument warehouse helper48CAA0 is a different owner. */
extern void FUN_0048aef0(int32_t, int32_t, uint32_t);

static uint32_t cart_actor(uint32_t id)
{ return 0xbcd600u + id * 388u; }

static uint32_t cart_building(int32_t id)
{ return 0x7999d8u + (uint32_t)id * 264u; }

static int cart_reverse_resource(uint8_t resource)
{
    switch (resource) {
    case 12: case 24: case 25: case 26: case 30: case 35: return 1;
    default: return 0;
    }
}

/* Native48B4A0..48B5D5. The helper has no logical return value. Keep the
   existing resource-to-image provider48B320 and absolute image-table reads. */
__declspec(noinline) void restore_actor_select_cargo_image(int32_t id)
{
    uint32_t actor = cart_actor((uint32_t)id), group;
    int32_t amount = CH(actor + 0x6cu), offset;
    uint8_t resource, state;
    if (amount == 0) offset = -1;
    else if (amount <= 200) offset = 0;
    else offset = amount <= 400 ? 8 : 16;
    resource = CB(actor + 3u);
    if (cart_reverse_resource(resource) && offset != -1) offset = 0;
    if (CB(actor + 2u) == 38u) {
        state = CB(actor + 0x30u);
        if (state != 16u && state != 12u &&
            (state != 7u || (CB(actor + 8u) != 16u && CB(actor + 8u) != 12u))) {
            offset = 0;
            group = FUN_0048b320(resource);
            CU(actor + 0x178u) = CU(0xf55bb4u + group * 8u) + (uint32_t)offset;
            return;
        }
    } else if (offset >= 0) {
        group = FUN_0048b320(resource);
        CU(actor + 0x178u) = CU(0xf55bb4u + group * 8u) + (uint32_t)offset;
        return;
    }
    if (cart_reverse_resource(resource)) {
        CU(actor + 0x178u) = CU(0xf5603cu);
    } else if (resource == 0u && cart_reverse_resource(CB(actor + 0x72u))) {
        CU(actor + 0x178u) = CU(0xf5603cu);
    } else {
        CU(actor + 0x178u) = CU(0xf55f54u);
    }
}

static uint32_t cart_advance(uint32_t id, uint32_t speed)
{
    restore_actor_select_cargo_image((int32_t)id);
    id = CU(0xd056f8u);
    FUN_004a5da0((int32_t)id, speed);
    return CU(0xd056f8u);
}

static uint32_t cart_clear_route(uint32_t id)
{
    FUN_004b3250((int32_t)id);
    return CU(0xd056f8u);
}

static uint32_t cart_retry_route(uint32_t id)
{
    uint32_t actor, tile;
    id = cart_clear_route(id);
    actor = cart_actor(id);
    tile = CU(actor + 0x18u);
    if (CB(0xee0c50u + tile) != 2u) CB(actor + 0x30u) = 8;
    CW(actor + 0x2eu) = 0;
    return id;
}

static uint32_t cart_render(uint32_t id)
{
    uint32_t actor = cart_actor(id), direction, base, overlay, prefix;
    int32_t selected = (int8_t)CB(actor + 9u);
    uint8_t state, frame, resource, offset_x, offset_y;
    if (selected >= 8) selected = (int8_t)CB(actor + 10u);
    direction = (uint32_t)selected - CU(0xbca2e0u);
    CU(0xe94e00u) = direction;
    if ((int32_t)direction < 0) {
        direction += 8u;
        CU(0xe94e00u) = direction;
    }
    state = CB(actor + 0x30u);
    if (state == 4u) {
        int32_t half = CH(actor + 0x2eu) / 2;
        frame = CB(0x5d048cu + (uint32_t)half);
        base = CU(0xf55e4cu);
        CU(actor - 4u) = base + (uint32_t)frame;
        CU(actor + 0x178u) = 0;
    } else if (state != 6u) {
        base = CU(0xf55e44u);
        frame = CB(actor - 7u);
        CU(actor - 4u) = base + (uint32_t)frame * 8u + direction;
    }
    overlay = CU(actor + 0x178u);
    if (overlay == 0u) return 0;
    overlay += direction;
    CU(actor + 0x178u) = overlay;
    prefix = overlay & UINT32_C(0xffffff00);
    resource = CB(actor + 3u);
    if (resource == 0u) resource = CB(actor + 0x72u);
    if (cart_reverse_resource(resource)) {
        direction += 4u;
        if ((int32_t)direction >= 8) direction -= 8u;
    }
    /* Native indexes the absolute table without a heading clamp. */
    offset_x = CB(0x5dc23cu + direction * 2u);
    offset_y = CB(0x5dc23du + direction * 2u);
    CB(actor + 0x64u) = offset_x;
    CB(actor + 0x65u) = offset_y;
    return prefix | offset_x;
}

/* FIG kind4. Only48B620..48BF2F is executable ownership; the remaining
   bytes before48BFF0 contain bounded switch tables, not another C body. */
__declspec(noinline) uint32_t restore_actor_update_cartpusher(void)
{
    uint32_t id = CU(0xd056f8u), actor = cart_actor(id), speed, tile, network;
    uint32_t building, owner, available, flag, old_direction, base, mode;
    int32_t home, destination, kind, resource, cargo, delivered, facing;
    uint8_t actor_kind = CB(actor + 2u), frame = CB(actor - 7u), state, direction;
    uint16_t target_x, policy;
    frame = (uint8_t)(frame + 1u);
    CB(actor - 7u) = frame;
    speed = CU(0x7090ecu + (uint32_t)actor_kind * 56u);
    if (frame >= 12u) CB(actor - 7u) = 0;
    CU(actor + 0x178u) = 0;
    tile = CU(actor + 0x18u);
    CB(actor + 0x6au) = 1;
    network = CB(0xa76aecu + tile);
    state = CB(actor + 0x30u);
    switch (state) {
    case 5:
        FUN_004b17d0((int32_t)id);
        id = CU(0xd056f8u);
        break;
    case 4:
        FUN_004b1d50((int32_t)id);
        id = CU(0xd056f8u);
        break;
    case 8:
        home = CH(actor + 0x52u);
        restore_actor_select_cargo_image((int32_t)id);
        direction = CB(0xee0c50u + tile);
        id = CU(0xd056f8u);
        if ((int8_t)direction < 0 || (int8_t)direction > 2)
            CB(cart_actor(id) + 6u) = 2;
        building = cart_building(home);
        if (CB(building) != 1u ||
            (CH(building + 0x2au) != (int32_t)id && CH(building + 0xb6u) != (int32_t)id))
            CB(cart_actor(id) + 6u) = 2;
        actor = cart_actor(id);
        CW(actor + 0x2eu) = (uint16_t)(CW(actor + 0x2eu) + 1u);
        if (CH(actor + 0x2eu) > 30) {
            CU(0xd05628u) = 0;
            resource = CB(building + 0x43u);
            owner = CU(0xeaac28u);
            policy = CW(0xea4096u + owner * 0x49d8u + (uint32_t)resource * 2u);
            flag = policy == 2u || policy == 4u;
            available = CU(0xea42f8u + owner * 0x49d8u + (uint32_t)resource * 4u);
            if (available == 0u && flag == 0u) id = cart_clear_route(id);
            destination = restore_actor_find_remaining_delivery_destination((int32_t)id, network);
            if (destination != 0) {
                kind = CH(cart_building(destination) + 0x10u);
                id = CU(0xd056f8u);
                actor = cart_actor(id);
                switch (kind) {
                case 71: case 191: CB(actor + 0x30u) = 10; break;
                case 72: case 73: case 193: CB(actor + 0x30u) = 9; break;
                case 95: CB(actor + 0x30u) = 16; break;
                default: CB(actor + 0x30u) = 11; break;
                }
                target_x = CW(0xd3b4e4u);
                CW(actor + 0x2eu) = 0;
                CW(actor + 0x1cu) = target_x;
                CW(actor + 0x1eu) = CW(0xd3b4e0u);
            } else {
                id = CU(0xd056f8u);
                available = CU(0xd05628u);
                actor = cart_actor(id);
                CW(actor + 0x2eu) = 0;
                CB(actor + 0x5eu) = available != 0u ? 2u : 1u;
            }
        }
        CB(cart_actor(id) - 7u) = 0;
        break;
    case 9: case 10:
        id = cart_advance(id, speed);
        actor = cart_actor(id);
        direction = CB(actor + 9u);
        if (direction == 8u) {
            CB(actor + 0x30u) = state == 9u ? 12u : 13u;
        } else if (direction == 9u) {
            id = cart_retry_route(id);
            actor = cart_actor(id);
        } else if (direction == 10u) {
            if (state == 9u) CB(actor + 6u) = 2;
            else {
                CB(actor + 0x30u) = 8;
                CW(actor + 0x2eu) = 0;
            }
        }
        destination = CH(actor + 0x56u);
        if (CB(cart_building(destination)) != 1u) CB(actor + 6u) = 2;
        break;
    case 11: case 16:
        id = cart_advance(id, speed);
        actor = cart_actor(id);
        direction = CB(actor + 9u);
        if (direction == 8u) CB(actor + 0x30u) = state == 11u ? 14u : 17u;
        else if (direction == 9u) id = cart_retry_route(id);
        else if (direction == 10u) CB(actor + 6u) = 2;
        break;
    case 12: case 13: case 14: case 17:
        restore_actor_select_cargo_image((int32_t)id);
        id = CU(0xd056f8u);
        actor = cart_actor(id);
        CW(actor + 0x2eu) = (uint16_t)(CW(actor + 0x2eu) + 1u);
        if (CH(actor + 0x2eu) > (state == 12u ? 10 : 5)) {
            if (state == 12u) {
                destination = CH(actor + 0x56u);
                (void)FUN_00475b50((uint32_t)destination);
                id = CU(0xd056f8u);
                actor = cart_actor(id);
                cargo = CH(actor + 0x6cu);
                resource = CB(actor + 3u);
                destination = CH(actor + 0x56u);
                facing = CB(actor + 7u);
                delivered = FUN_0045b2a0(facing, destination, resource, cargo);
            } else if (state == 13u) {
                cargo = CH(actor + 0x6cu);
                destination = CH(actor + 0x56u);
                resource = CB(actor + 3u);
                delivered = FUN_0045e220(destination, resource, 1, cargo);
            } else {
                cargo = CH(actor + 0x6cu);
                resource = CB(actor + 3u);
                destination = CH(actor + 0x56u);
                if (state == 14u) delivered = FUN_0045fe20(destination, cargo, resource);
                else delivered = FUN_00460890(destination, cargo, resource);
            }
            id = CU(0xd056f8u);
            FUN_0048aef0(delivered, (int32_t)id, network);
            id = CU(0xd056f8u);
        }
        CB(cart_actor(id) - 7u) = 0;
        break;
    case 15:
        home = CH(actor + 0x52u);
        if ((uint8_t)FUN_00488030((uint32_t)home) != 0u &&
            (uint8_t)FUN_004894a0(home) != 0u) {
            mode = CU(0xd386d4u);
            if (mode == 1u || mode == 2u || mode == 3u) {
                id = CU(0xd056f8u);
                id = cart_clear_route(id);
                CB(cart_actor(id) + 6u) = 2;
                return id; /* Native early RET skips direction and sprite updates. */
            }
        }
        id = CU(0xd056f8u);
        id = cart_advance(id, speed);
        actor = cart_actor(id);
        direction = CB(actor + 9u);
        if (direction == 8u) {
            CB(actor + 0x30u) = direction;
            CB(actor + 6u) = 2;
        } else if (direction == 9u) id = cart_clear_route(id);
        else if (direction == 10u) CB(actor + 6u) = 2;
        break;
    case 7:
        CB(actor - 7u) = 0;
        restore_actor_select_cargo_image((int32_t)id);
        id = CU(0xd056f8u);
        old_direction = CU(0xe94e00u);
        base = CU(0xf55e44u);
        actor = cart_actor(id);
        CU(actor - 4u) = base + old_direction + 1u;
        CW(actor + 0x2eu) = (uint16_t)(CW(actor + 0x2eu) + 1u);
        if (CH(actor + 0x2eu) > 100) {
            CW(actor + 0x2eu) = 0;
            flag = FUN_004a35e0((int32_t)id);
            id = CU(0xd056f8u);
            if ((uint8_t)flag == 0u) CB(cart_actor(id) + 0x30u) = 8;
        }
        break;
    case 6:
        CU(actor - 4u) = 0;
        if (CU(actor + 0x178u) != 0u) CU(actor + 0x178u) = 0;
        break;
    default:
        CB(actor + 0x30u) = 8;
        break;
    }
    return cart_render(id);
}

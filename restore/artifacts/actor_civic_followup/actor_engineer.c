#include <stdint.h>

#define EU(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define EW(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define EB(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define EH(a) ((int32_t)(int16_t)EW(a))
typedef char engineer_requires_pointer32[sizeof(void *) == 4 ? 1 : -1];

extern void FUN_004b17d0(int32_t actor), FUN_004b1d50(int32_t actor);
extern void FUN_004b1df0(int32_t actor), FUN_004b1ef0(int32_t actor, int32_t steps);
extern void FUN_004a5da0(int32_t actor, uint32_t speed);
extern int32_t FUN_004b3ff0(int32_t actor, int32_t mode);
extern uint32_t FUN_0047fe60(int32_t x, int32_t y, uint32_t size, int32_t radius);
extern void FUN_004b4220(int32_t actor, int32_t from_x, int32_t from_y,
    int32_t target_x, int32_t target_y, int32_t mode);

static uint32_t engineer_actor(uint32_t id)
{
    return 0xbcd600u + id * 388u;
}

static uint32_t engineer_building(int32_t id)
{
    return 0x7999d8u + (uint32_t)id * 264u;
}

/* Both outbound paths cache building X/Y before the state store. The Y scale
   uses that cached WORD; X is deliberately reloaded after both target stores. */
static uint32_t engineer_begin_pixel_path(uint32_t id, uint32_t x_address,
    uint32_t y_address)
{
    uint32_t actor = engineer_actor(id);
    uint16_t target_x = EW(x_address), target_y = EW(y_address);
    int32_t to_x, to_y, from_x, from_y;
    EB(actor + 0x30u) = 9;
    EW(actor + 0x1cu) = target_x;
    EW(actor + 0x1eu) = target_y;
    to_y = (int32_t)(int16_t)target_y * 15;
    to_x = EH(actor + 0x1cu) * 15;
    from_y = EH(actor + 0x44u);
    from_x = EH(actor + 0x42u);
    FUN_004b4220((int32_t)id, from_x, from_y, to_x, to_y, 0);
    id = EU(0xd056f8u);
    EW(engineer_actor(id) + 0x3cu) = 0;
    return id;
}

static uint32_t engineer_render(uint32_t id)
{
    uint32_t actor = engineer_actor(id), rotation = EU(0xbca2e0u);
    uint32_t direction, base;
    uint8_t frame;
    direction = (uint32_t)(int32_t)(int8_t)EB(actor + 9u) - rotation;
    EU(0xe94e00u) = direction;
    if ((int32_t)direction < 0) {
        direction += 8u;
        EU(0xe94e00u) = direction;
    }
    if (EB(actor + 0x30u) == 4u) {
        int32_t half = EH(actor + 0x2eu) / 2;
        frame = EB(0x5d048cu + (uint32_t)half);
        base = EU(0xf55d14u);
        EU(actor - 4u) = base + (uint32_t)frame;
        return base; /* Native EAX is the loaded base, not the stored image. */
    }
    base = EU(0xf55d0cu);
    frame = EB(actor - 7u);
    EU(actor - 4u) = base + (uint32_t)frame * 8u + direction;
    return direction;
}

/* FIG_ENGINEER, registry kind8. Owned native scope is48C6A0..48CA93;
   the1936-byte input window also contains a different helper at48CAA0. */
__declspec(noinline) uint32_t restore_actor_update_engineer(void)
{
    uint32_t id = EU(0xd056f8u), actor = engineer_actor(id);
    uint32_t kind = EB(actor + 2u), speed, building, found, tile;
    int32_t home, x, y, count, limit, grid_building;
    uint8_t frame, state, size, direction;
    uint16_t target_x;

    EB(actor + 0x6au) = 1;
    EB(actor + 4u) = 0;
    EW(actor + 0x3au) = 0x280u;
    home = EH(actor + 0x52u);
    speed = EU(0x7090ecu + kind * 56u);
    building = engineer_building(home);
    if (EB(building) != 1u || (uint32_t)EH(building + 0x2au) != id)
        EB(actor + 6u) = 2;
    frame = (uint8_t)(EB(actor - 7u) + 1u);
    EB(actor - 7u) = frame;
    if (frame >= 12u)
        EB(actor - 7u) = 0;
    state = EB(actor + 0x30u);

    switch (state) {
    case 5:
        FUN_004b17d0((int32_t)id);
        id = EU(0xd056f8u);
        break;
    case 4:
        FUN_004b1d50((int32_t)id);
        id = EU(0xd056f8u);
        break;
    case 8:
        EB(actor + 0x5du) = 1;
        EB(actor - 7u) = 0;
        EW(actor + 0x2eu) = (uint16_t)(EW(actor + 0x2eu) - 1u);
        if (EH(actor + 0x2eu) <= 0) {
            y = EH(building + 8u);
            x = EH(building + 6u);
            size = EB(building + 3u);
            found = FUN_0047fe60(x, y, size, 2);
            id = EU(0xd056f8u);
            if (found == 0u)
                EB(engineer_actor(id) + 6u) = 2;
            else
                id = engineer_begin_pixel_path(id, building + 0x26u,
                    building + 0x28u);
        }
        break;
    case 9:
        EB(actor + 4u) = 1;
        EB(actor + 0x5du) = 1;
        if (FUN_004b3ff0((int32_t)id, 1) != 1) {
            id = EU(0xd056f8u);
            break;
        }
        id = EU(0xd056f8u);
        actor = engineer_actor(id);
        tile = EU(actor + 0x18u);
        grid_building = EH(0xb4e6fcu + tile * 2u);
        if (grid_building == home)
            EB(actor + 6u) = 2;
        else {
            EB(actor + 0x30u) = 10;
            FUN_004b1df0((int32_t)id);
            id = EU(0xd056f8u);
            EW(engineer_actor(id) + 0x3cu) = 0;
        }
        break;
    case 10:
        EB(actor + 0x5du) = 0;
        EW(actor + 0x3cu) = (uint16_t)(EW(actor + 0x3cu) + 1u);
        count = EH(actor + 0x3cu);
        limit = EH(actor + 0x3au);
        if (count >= limit) {
            y = EH(building + 8u);
            x = EH(building + 6u);
            size = EB(building + 3u);
            found = FUN_0047fe60(x, y, size, 2);
            id = EU(0xd056f8u);
            actor = engineer_actor(id);
            if (found == 0u) {
                EB(actor + 6u) = 2;
                FUN_004b1ef0((int32_t)id, 1);
                id = EU(0xd056f8u);
                break;
            }
            target_x = EW(0xd3b4e4u);
            EB(actor + 0x30u) = 11;
            EW(actor + 0x1cu) = target_x;
            EW(actor + 0x1eu) = EW(0xd3b4e0u);
        }
        FUN_004b1ef0((int32_t)id, 1);
        id = EU(0xd056f8u);
        break;
    case 11:
        FUN_004a5da0((int32_t)id, speed);
        id = EU(0xd056f8u);
        actor = engineer_actor(id);
        direction = EB(actor + 9u);
        if (direction == 9u || direction == 10u)
            EB(actor + 6u) = 2;
        else if (direction == 8u)
            id = engineer_begin_pixel_path(id, building + 6u, building + 8u);
        break;
    default:
        EB(actor + 0x30u) = 8;
        EW(actor + 0x2eu) = 0;
        break;
    }
    return engineer_render(id);
}

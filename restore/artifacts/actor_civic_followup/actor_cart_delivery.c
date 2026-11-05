#include <stdint.h>

#define DU(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define DW(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define DB(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define DH(a) ((int32_t)(int16_t)DW(a))

typedef char cart_delivery_requires_pointer32[sizeof(void *) == 4 ? 1 : -1];

/* Delegating APIs reuse the existing correct warehouse booking helpers.
   Their algorithms are shared source, not additional reconstructed owners. */
extern int restore_actor_resource_booking_kind(int32_t kind);
extern int32_t restore_actor_resource_for_subtype(int32_t subtype);
extern int32_t restore_actor_find_remaining_delivery_destination(int32_t id, uint32_t network);

static uint32_t cart_delivery_actor(int32_t id)
{ return 0xbcd600u + (uint32_t)id * 388u; }

static uint32_t cart_delivery_building(int32_t id)
{ return 0x7999d8u + (uint32_t)id * 264u; }

/* Native48AEF0..48B157, three explicit stack arguments. This API preserves
   logical void behavior. Keep RAW for incidental physical EAX/EDX/flags.
   Native48CAA0 is a distinct four-argument warehouse completion owner. */
__declspec(noinline) void restore_actor_apply_cart_delivery(
    int32_t delivered, int32_t id, uint32_t network)
{
    uint32_t actor = cart_delivery_actor(id);
    uint32_t building = cart_delivery_building(DH(actor + 0x56u));
    int32_t kind = DH(building + 0x10u), resource, destination;
    uint16_t amount, target_x, target_y;
    if (restore_actor_resource_booking_kind(kind)) {
        resource = restore_actor_resource_for_subtype(DH(building + 0xceu));
        if (DW(building + 0x10u) == 204u && (int32_t)DB(actor + 3u) == resource)
            DW(building + 0x106u) = (uint16_t)(DW(building + 0x106u) - 1u);
        else
            DW(building + 0x104u) = (uint16_t)(DW(building + 0x104u) - 1u);
    } else if (DW(building + 0x10u) == 71u) {
        amount = DW(actor + 0x6cu);
        DW(building + 0x104u) = (uint16_t)(DW(building + 0x104u) - 1u);
        DW(building + 0x106u) = (uint16_t)(DW(building + 0x106u) - amount);
    }
    amount = DW(actor + 0x6cu);
    if (delivered == (int32_t)(int16_t)amount) {
        DB(actor + 0x30u) = 15;
        DW(actor + 0x2eu) = 0;
        target_x = DW(actor + 0x24u);
        DW(actor + 0x1cu) = target_x;
        target_y = DW(actor + 0x26u);
        DW(actor + 0x1eu) = target_y;
        DW(actor + 0x6cu) = 0;
        return;
    }
    DW(actor + 0x6cu) = (uint16_t)((uint32_t)amount - (uint32_t)delivered);
    destination = restore_actor_find_remaining_delivery_destination(id, network);
    /* Native retains the original actor argument and offset across the query;
       the mutable current-actor global is deliberately not reloaded here. */
    if (destination != 0) {
        kind = DH(cart_delivery_building(destination) + 0x10u);
        switch (kind) {
        case 71: case 191: DB(actor + 0x30u) = 10; break;
        case 72: case 73: case 193: DB(actor + 0x30u) = 9; break;
        case 95: DB(actor + 0x30u) = 16; break;
        default: DB(actor + 0x30u) = 11; break;
        }
        target_x = DW(0xd3b4e4u);
        DW(actor + 0x2eu) = 0;
        target_y = DW(0xd3b4e0u);
        DW(actor + 0x1cu) = target_x;
        DW(actor + 0x1eu) = target_y;
    } else {
        DB(actor + 0x30u) = 8;
        DW(actor + 0x2eu) = 0;
    }
}

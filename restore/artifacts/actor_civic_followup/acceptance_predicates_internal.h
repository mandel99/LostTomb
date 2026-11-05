#ifndef RESTORE_ACCEPTANCE_PREDICATES_INTERNAL_H
#define RESTORE_ACCEPTANCE_PREDICATES_INTERNAL_H
#include "acceptance_predicates.h"
#include "building_services.h"

#define AP_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define AP_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define AP_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define AP_S(a) ((int32_t)(int16_t)AP_W(a))

/* IDs and offsets use native modulo-2^32 address arithmetic. Relationship
   WORDs are sign-extended before conversion to that address domain. */
static uint32_t ap_building_offset(int32_t building)
{
    return (uint32_t)building * 264u;
}

static uint32_t ap_actor_offset(uint32_t actor)
{
    return actor * 388u;
}

/* Caller has already stored its state BYTE. The native writes the worker
   DWORD before loading both coordinate WORDs and the tile DWORD. */
static void ap_claim_destination(uint32_t building_offset, uint32_t actor_offset,
                                 int32_t actor)
{
    uint16_t x, y;
    uint32_t tile;
    AP_U(0x799a34u + building_offset) = (uint32_t)actor;
    x = AP_W(0x7999feu + building_offset);
    y = AP_W(0x799a00u + building_offset);
    tile = AP_U(0x7999e4u + building_offset);
    AP_W(0xbcd61cu + actor_offset) = x;
    AP_W(0xbcd61eu + actor_offset) = y;
    AP_U(0xbcd620u + actor_offset) = tile;
    AP_W(0xbcd63cu + actor_offset) = 0;
}

/* This validation may clear a stale reciprocal link even for a probe. */
static uint8_t ap_linked_laborer_blocks(uint32_t worker, uint8_t last_state)
{
    uint32_t worker_offset = ap_actor_offset(worker);
    int32_t linked = AP_S(0xbcd660u + worker_offset);
    uint32_t linked_offset;
    uint8_t state;
    if (linked == 0) return 0;
    linked_offset = ap_actor_offset((uint32_t)linked);
    if (AP_B(0xbcd606u + linked_offset) != 0 &&
        AP_B(0xbcd602u + linked_offset) == 0x55u) {
        state = AP_B(0xbcd630u + linked_offset);
        if (state == 6u || state == 7u || state == 15u ||
            state == 16u || state == last_state) {
            if ((uint32_t)AP_S(0xbcd660u + linked_offset) == worker)
                return 1;
        }
    }
    AP_W(0xbcd660u + worker_offset) = 0;
    return 0;
}

enum ap_source_coordinate_phase {
    AP_SOURCE_X_THEN_LATE_Y,
    AP_SOURCE_Y_THEN_LATE_X,
    AP_SOURCE_X_AND_Y_EARLY
};

/* Preserve the three observed global-coordinate read phases around the
   actor writes. The source selector's complete signed result is consumed. */
static void ap_commit_linked_delivery(int32_t building, int32_t actor,
                                      uint32_t worker,
                                      enum ap_source_coordinate_phase phase)
{
    int32_t source = FUN_0045bb50(building, 0x1e, 1);
    uint32_t actor_offset = ap_actor_offset((uint32_t)actor);
    uint16_t x, y;
    if (source == 0) {
        AP_B(0xbcd606u + actor_offset) = 2;
        return;
    }
    if (phase == AP_SOURCE_X_AND_Y_EARLY) {
        x = AP_W(0xd3b4e4u);
        y = AP_W(0xd3b4e0u);
        AP_B(0xbcd672u + actor_offset) = 0x1e;
        AP_W(0xbcd66cu + actor_offset) = 0;
        AP_W(0xbcd656u + actor_offset) = (uint16_t)source;
    } else {
        uint16_t early;
        if (phase == AP_SOURCE_Y_THEN_LATE_X) early = AP_W(0xd3b4e0u);
        else early = AP_W(0xd3b4e4u);
        AP_B(0xbcd672u + actor_offset) = 0x1e;
        AP_W(0xbcd66cu + actor_offset) = 0;
        AP_W(0xbcd656u + actor_offset) = (uint16_t)source;
        if (phase == AP_SOURCE_Y_THEN_LATE_X) {
            x = AP_W(0xd3b4e4u);
            y = early;
        } else {
            x = early;
            y = AP_W(0xd3b4e0u);
        }
    }
    AP_W(0xbcd660u + actor_offset) = (uint16_t)worker;
    AP_B(0xbcd630u + actor_offset) = 15;
    AP_W(0xbcd62eu + actor_offset) = 0;
    AP_W(0xbcd61cu + actor_offset) = x;
    AP_W(0xbcd61eu + actor_offset) = y;
    if (phase == AP_SOURCE_X_AND_Y_EARLY)
        AP_B(0xbcd66au + actor_offset) = 3;
    AP_W(0xbcd660u + ap_actor_offset(worker)) = (uint16_t)actor;
}

#endif

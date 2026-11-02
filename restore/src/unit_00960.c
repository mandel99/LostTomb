#include "building_allocator.h"

/* Whole46A280 is owned by the allocator: ten 28-byte repair slots, BYTE
   occupancy and a DWORD building id. Keep every caller on that provider. */
void FUN_0046a280(int32_t building)
{
    restore_building_repair_slot_0046a280(building);
}

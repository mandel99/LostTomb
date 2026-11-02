#ifndef RESTORE_BUILDING_ALLOCATOR_H
#define RESTORE_BUILDING_ALLOCATOR_H
#include <stdint.h>
/* Complete native46A400: six DWORDs, full EAX building index, RET0. */
int32_t restore_building_allocate_0046a400(uint32_t owner,uint32_t kind,int32_t x,int32_t y,int32_t contiguous,int32_t selected);
/* Native46A280 consumes one building DWORD; RET0, no return contract. */
void restore_building_repair_slot_0046a280(int32_t building);
/* Native-address aliases use the same complete C providers. */
int32_t FUN_0046a400(uint32_t owner,uint32_t kind,int32_t x,int32_t y,int32_t contiguous,int32_t selected);
void FUN_0046a280(int32_t building);
#endif

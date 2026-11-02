#include "actor_order_flood.h"
#define FUN_0040f8f0 restore_flood_fill_actual
#include FLOOD_FILL_SOURCE
#undef FUN_0040f8f0
void restore_flood_trace(uint32_t destination,uint32_t bytes,uint32_t value);
void FUN_0040f8f0(uint32_t *destination,uint32_t bytes,uint8_t value)
{
    restore_flood_trace((uint32_t)(uintptr_t)destination,bytes,value);
    restore_flood_fill_actual(destination,bytes,value);
}

#include "actor_city_refresh.h"
#define _DAT_0078d5c8 (*(volatile uint32_t *)(uintptr_t)0x0078d5c8u)
#define _DAT_0078d5cc (*(volatile uint32_t *)(uintptr_t)0x0078d5ccu)
#define _DAT_0078d5dc (*(volatile uint32_t *)(uintptr_t)0x0078d5dcu)
#define _DAT_0078d5e0 (*(volatile uint32_t *)(uintptr_t)0x0078d5e0u)

/* Whole46A060,6 instructions. The existing algorithm was already complete:
   retain all four DWORD stores and their order, with volatile ownership. */
void restore_city_clear_periodic_counters(void)
{
    _DAT_0078d5e0=0u;
    _DAT_0078d5dc=0u;
    _DAT_0078d5cc=0u;
    _DAT_0078d5c8=0u;
}

void FUN_0046a060(void)
{
    restore_city_clear_periodic_counters();
}

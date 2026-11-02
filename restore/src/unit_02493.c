#include <stdint.h>
/* Native0053b670: REP STOSD clears exactly12 dwords; the separate total is
   also a dword. The intervening slot f55b80 remains unchanged. */
void FUN_0053b670(void)
{
    uint32_t i;
    for(i=0;i<12;++i)*(volatile uint32_t *)(uintptr_t)(0x00f55b50u+i*4u)=0;
    *(volatile uint32_t *)(uintptr_t)0x00f55b84u=0;
}

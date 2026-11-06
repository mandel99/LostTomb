#include <stdint.h>
extern uint32_t FUN_004b5520(uint32_t cell);
uint32_t restore_record_grid_candidate_004d6aa0(int32_t index)
{
 uint32_t x,y,origin;
 if (*(volatile const uint32_t *)0xd05708u==0) return 0;
 if (*(volatile const uint8_t *)(uintptr_t)(0xbcd728u+(uint32_t)index*388u)!=0) return 0;
 y=*(volatile const uint32_t *)0xbca2ccu;
 x=*(volatile const uint32_t *)0xbca2c8u;
 origin=*(volatile const uint32_t *)0xbca2d8u;
 return FUN_004b5520(x+y*228u+origin)!=0 ? 1u : 0u;
}
uint32_t FUN_004d6aa0(int32_t index)
{ return restore_record_grid_candidate_004d6aa0(index); }

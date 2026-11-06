#include <stdint.h>
/* The record flag is a byte at index*388; the grid uses DWORD cells. */
uint32_t restore_record_grid_allows_004d6b00(int32_t index)
{
 uint32_t x,y,origin,position,cell;
 if (*(volatile const uint32_t *)0x00d05708u==0) return 0;
 if (*(volatile const uint8_t *)(uintptr_t)(0x00bcd728u+(uint32_t)index*388u)==0) return 0;
 y=*(volatile const uint32_t *)0x00bca2ccu;
 x=*(volatile const uint32_t *)0x00bca2c8u;
 origin=*(volatile const uint32_t *)0x00bca2d8u;
 position=x+y*228u+origin;
 cell=*(volatile const uint32_t *)(uintptr_t)(0x00af598cu+position*4u);
 return (cell&4u)==0 ? 1u : 0u;
}
uint32_t FUN_004d6b00(int32_t index)
{
 return restore_record_grid_allows_004d6b00(index);
}

#ifndef RESTORE_MAP_CELLS_PHASE_H
#define RESTORE_MAP_CELLS_PHASE_H
#include <stdint.h>
int32_t FUN_00486d30(int32_t x,int32_t y,uint32_t mask,int32_t radius_limit);
uint8_t FUN_00487260(int32_t tile);
uint32_t FUN_004872c0(int32_t tile,uint32_t mask);
uint32_t FUN_00487300(int32_t tile);
uint32_t FUN_00487390(int32_t tile,int32_t offset);
#ifdef MAP_CELLS_TRACE
uint32_t map_cells_phase_read(uint32_t address,uint32_t width);
#define MC_U(a) map_cells_phase_read((uint32_t)(a),4)
#define MC_B(a) map_cells_phase_read((uint32_t)(a),1)
#else
#define MC_U(a) (*(volatile const uint32_t *)(uintptr_t)(uint32_t)(a))
#define MC_B(a) (*(volatile const uint8_t *)(uintptr_t)(uint32_t)(a))
#endif
/* Native grids use a fixed physical stride; caller-owned padding is valid. */
static __inline uint32_t mc_terrain(uint32_t tile) {return 0xaf598cu+tile*4u;}
static __inline uint32_t mc_object(uint32_t tile) {return 0xd05720u+tile*4u;}
static __inline int32_t mc_add(int32_t a,int32_t b) {return (int32_t)((uint32_t)a+(uint32_t)b);}
static __inline int32_t mc_sub(int32_t a,int32_t b) {return (int32_t)((uint32_t)a-(uint32_t)b);}
static __inline int mc_bank(uint32_t address)
{
    uint32_t p=MC_U(address);
    return p!=0 && MC_B(p+24u)==0;
}
static __inline uint32_t mc_growth(uint32_t tile)
{
    uint32_t a=mc_terrain(tile),o=mc_object(tile);
    if((MC_B(a-912u)&4u)&&(MC_B(a+912u)&4u))return 100;
    if((MC_B(a-4u)&4u)&&(MC_B(a+4u)&4u))return 100;
    if(mc_bank(o-912u)&&mc_bank(o+912u))return 100;
    if(mc_bank(o-4u)&&mc_bank(o+4u))return 100;
    return MC_B(0xa509bcu+tile);
}
#endif

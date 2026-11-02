#ifndef RESTORE_MAP_NEIGHBORS_PHASE_H
#define RESTORE_MAP_NEIGHBORS_PHASE_H
#include <stdint.h>
void FUN_0047dbc0(uint32_t mask);
void FUN_0047dc80(uint32_t include,uint32_t exclude);
void FUN_0047ddd0(uint32_t mask,uint32_t shift);
void FUN_0047dee0(void);
uint32_t FUN_0047e000(void);
#ifdef MAP_NEIGHBORS_TRACE
uint32_t map_neighbors_read(uint32_t address,uint32_t width);
void map_neighbors_write(uint32_t address,uint32_t value);
#define MN_U(a) map_neighbors_read((uint32_t)(a),4)
#define MN_B(a) map_neighbors_read((uint32_t)(a),1)
#define MN_W(a,v) map_neighbors_write((uint32_t)(a),(uint32_t)(v))
#else
#define MN_U(a) (*(volatile const uint32_t *)(uintptr_t)(uint32_t)(a))
#define MN_B(a) (*(volatile const uint8_t *)(uintptr_t)(uint32_t)(a))
#define MN_W(a,v) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a)=(uint32_t)(v))
#endif
static __inline uint32_t mn_terrain(uint32_t tile,uint32_t direction)
{
    static const int32_t offsets[8]={-228,-227,1,229,228,227,-1,-229};
    return 0xaf598cu+(tile+(uint32_t)offsets[direction])*4u;
}
static __inline uint32_t mn_secondary(uint32_t tile,uint32_t direction)
{
    static const int32_t offsets[8]={-228,-227,1,229,228,227,-1,-229};
    return 0xb285ccu+tile+(uint32_t)offsets[direction];
}
static __inline uint32_t mn_slot(uint32_t direction) {return 0xbc97c0u+direction*4u;}
#endif

#include <stdint.h>
void FUN_00488e90(int32_t,int32_t,int32_t,int32_t,char);
#ifdef MAP_REFRESH_WHOLE_TRACE
uint64_t map_whole_read(uint32_t,uint32_t);
#define MWC_R(a) ((uint32_t)map_whole_read((a),4))
#else
#define MWC_R(a) (*(volatile uint32_t *)(uintptr_t)(a))
#endif
/* Whole canonical00489030; both dimensions are DWORD globals. */
void FUN_00489030(void)
{
 uint32_t height=MWC_R(0xbca2d4u)-1u,width=MWC_R(0xbca2d0u)-1u;
 FUN_00488e90(0,0,(int32_t)width,(int32_t)height,1);
}

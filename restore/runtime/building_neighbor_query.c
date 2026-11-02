#include "building_neighbor_query.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_00481000(int32_t x,int32_t y,uint32_t layout)
{
 uint32_t cell=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x,p=0x5cc9e0u+layout*96u,i,offset;
 for(i=0;i<24;++i,p+=4u){
  offset=U(p);if(!offset)return 0;
  if(*(volatile uint8_t *)(uintptr_t)(0xaf598cu+(cell+offset)*4u)&4u)return 1;
 }
 return 0;
}

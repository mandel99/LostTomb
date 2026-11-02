#include "building_nearest_search.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define S(a) (*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* This is ordinary private C factoring of the four repeated native blocks,
   not an external algorithm boundary. It performs the real grid/list scan. */
static uint8_t point(int32_t count,const uint32_t *types,uint32_t mask,uint32_t ox,uint32_t oy,int32_t excluded,uint32_t *result)
{
 uint32_t i,actor;
 if((int32_t)U(ox)<0||(int32_t)U(oy)<0||(int32_t)U(ox)>=(int32_t)U(0xbca2d0u)||(int32_t)U(oy)>=(int32_t)U(0xbca2d4u))return 0;
 for(i=0;(int32_t)i<count;++i){
  actor=(uint32_t)(int32_t)S(0xb350dcu+U(0xbc9768u)*2u);
  while(actor){
   if((uint32_t)B(0xbcd602u+actor*388u)==U((uint32_t)(uintptr_t)types+i*4u)&&actor!=(uint32_t)excluded){*result=actor;return 1;}
   actor=(uint32_t)(int32_t)S(0xbcd600u+actor*388u);
  }
 }
 if(!(U(0xaf598cu+U(0xbc9768u)*4u)&0x80000u)&&(mask&U(0xaf598cu+U(0xbc9768u)*4u))){*result=(uint32_t)count+1u;return 1;}
 return 0;
}
int32_t restore_building_nearest_search(int32_t x,int32_t y,int32_t type_count,const uint32_t *types,uint32_t mask,int32_t *output_x,int32_t *output_y,int32_t rings,int32_t excluded_actor)
{
 uint32_t base=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x,side=1,ring=0,direction,step,result;
 uint32_t ox=(uint32_t)(uintptr_t)output_x,oy=(uint32_t)(uintptr_t)output_y,axis,delta;
 U(ox)=(uint32_t)x;U(oy)=(uint32_t)y;
 for(;;){
  U(0xbc9768u)=base;
  for(direction=0;direction<4;++direction){
   axis=direction&1u?oy:ox;delta=direction==0?1u:direction==1?228u:direction==2?0xffffffffu:0xffffff1cu;
   for(step=0;step<side;++step){
    if(point(type_count,types,mask,ox,oy,excluded_actor,&result))return (int32_t)result;
    U(0xbc9768u)+=delta;U(axis)+=direction<2?1u:0xffffffffu;
   }
   U(0xbc9768u)-=delta;U(axis)-=direction<2?1u:0xffffffffu;
  }
  if(!rings)return 0;
  ++ring;side+=2u;base-=229u;U(ox)-=1u;U(oy)-=1u;rings=(int32_t)((uint32_t)rings-1u);
  if(ring>227u)return 0;
 }
}

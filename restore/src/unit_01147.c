#include "city_maintenance.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define I(a) ((int32_t)(uint32_t)(a))

/* Whole47E290. The native clear mask XORs with0000FFFF, so processing a
   nonzero group can discard bits outside the low16 range. Preserve this
   operation verbatim instead of replacing it with a conventional bit clear. */
void restore_city_decay_overlay_counters(void)
{
 uint32_t clock=U(0xbca344u)+1u,row=0u,start,end,count,address,value,shift,group,height;
 U(0xbca344u)=clock;if(I(clock)<20)return;
 height=U(0xbca2d4u);U(0xbca344u)=0;if(I(height)<=0)return;
 do{
  start=U(0xbc9f24u+row*4u);end=U(0xbc9b94u+row*4u);address=0x978d9cu+U(0xbc9804u+row*4u)*4u;
  if(I(start)<I(end)){
   count=end-start;
   do{
    value=U(address);
    if(value!=0u)for(shift=0;shift<30u;shift+=3u){
     group=(value>>shift)&7u;
     if(group!=0u)value=(value&((7u<<shift)^0xffffu))|((group-1u)<<shift);
    }
    U(address)=value;address+=4u;--count;
   }while(count!=0u);
  }
  ++row;
 }while(I(row)<I(U(0xbca2d4u)));
}

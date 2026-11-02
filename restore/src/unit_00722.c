#include "earthquake_event.h"
#include "simulation_tick.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole44BCA0. A successful event stops this tick's scan; failed calls can
   change the signed list count, which the native loop reloads each time. */
void restore_earthquake_list_tick(int32_t *list)
{
    uint32_t self=(uint32_t)(uintptr_t)list,record=self+4u,index=0;
    if(U(0x605a3cu)==1u||W(0x784c60u)==0xffffu||
       W(0x784c62u)==0xffffu||(int32_t)U(self)<=0)return;
    do{
        if(B(record+2u)==3u&&FUN_0044be30(record)){
            B(self+0x48b4u)=1;return;
        }
        ++index;record+=124u;
    }while((int32_t)index<(int32_t)U(self));
}
void FUN_0044bca0(int32_t *list){restore_earthquake_list_tick(list);}

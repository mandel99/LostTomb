#include "worker_query_candidate.h"
#define WQ_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define WQ_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define WQ_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

uint32_t restore_monument_worker_query_00401b40(uint32_t building,uint32_t incoming_eax)
{
 uint32_t slot,worker,actor,state,result=incoming_eax&0xffffff00u;uint8_t type;
 building-=(uint32_t)(int32_t)(int16_t)WQ_W(0x7999eau+building*264u);
 slot=0x799a34u+building*264u;worker=WQ_U(slot);
 if(!worker)return result;
 actor=worker*388u;
 if(!WQ_B(0xbcd606u+actor)){WQ_U(slot)=0;return result;}
 state=WQ_B(0xbcd630u+actor);
 if(state==6||state==7)state=WQ_B(0xbcd608u+actor);
 type=WQ_B(0xbcd602u+actor);
 if(type==0x4f)return result|(uint32_t)(state==9||state==10);
 if(type==0x51)return result|(uint32_t)(state==9||state==11);
 WQ_U(slot)=0;return result;
}

#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_00470990(uint32_t,int32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t);

/* Native485F70 rebuilds building footprints, then breaks actor chains that
   loop back to their starting actor. Record offsets are byte addresses. */
void FUN_00485f70(void)
{
    uint32_t record,id,size,previous;
    int32_t next,kind;
    for(id=1,record=0x799af0u;record<0x89b6e8u;record+=264u,++id){
        if(!B(record-16u))continue;
        kind=H(record);size=U(0x5d06dcu+(uint32_t)kind*20u);
        U(0xbc96d0u)=size;
        FUN_00470990(U(0xeaac28u),kind,(int32_t)id,H(record-10u),H(record-8u),size,0,0);
    }
    for(id=1,record=0xbcd784u;record<0xc8ad40u;record+=388u,++id){
        if(!B(record+6u))continue;
        previous=id;next=H(record);
        while(next){
            if((uint32_t)next==id){W(0xbcd600u+previous*388u)=0;break;}
            previous=(uint32_t)next;
            next=H(0xbcd600u+(uint32_t)next*388u);
        }
    }
}

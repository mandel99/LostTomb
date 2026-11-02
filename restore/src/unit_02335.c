#include "actor_order_flood.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
static void enqueue(uint32_t cell)
{
    uint32_t tail=U(0xeae008);
    U(0xf203a0u+tail*4u)=cell;++tail;U(0xeae008)=tail;
    if((int32_t)tail>51983)U(0xeae008)=0;
}
static void advance(void)
{
    uint32_t head=U(0xeae00c)+1u;U(0xeae00c)=head;
    if((int32_t)head>51983)U(0xeae00c)=0;
}
uint32_t FUN_0051ce90(int32_t x,int32_t y)
{
    uint32_t iterations=0,cell,next,cost,start;
    FUN_0040f8f0((uint32_t *)0xeed760u,0x32c40,0);
    start=U(0xbca2d8)+(uint32_t)y*228u+(uint32_t)x;U(0xeae004)=start;
    if(B(0xed4140u+start)==0xfc)return 0;
    FUN_0040f8f0((uint32_t *)0xeae010u,0xcb10,0);
    U(0xeed760u+start*4u)=1;U(0xf203a0)=start;U(0xeae00c)=0;U(0xeae008)=1;
    do{
        if(++iterations>50000u)return 1;
        cell=U(0xf203a0u+U(0xeae00c)*4u);
        if(B(0xed4140u+cell)==0xfe){
            int8_t old=*(int8_t *)(uintptr_t)(0xeae010u+cell);
            B(0xeae010u+cell)=(uint8_t)((uint8_t)old+1u);
            if(old<4){enqueue(cell);advance();continue;}
        }
        cost=U(0xeed760u+cell*4u)+1u;
        next=cell-228u;
        if(!U(0xeed760u+next*4u)&&B(0xed4140u+next)!=0xff&&B(0xed4140u+next)!=0xfd){
            U(0xeed760u+next*4u)=cost;
            if(B(0xed4140u+next)==0xfe)U(0xeed760u+next*4u)=cost+4u;
            enqueue(next);
        }
        next=cell+1u;
        if(!U(0xeed760u+next*4u)&&B(0xed4140u+next)!=0xff&&B(0xed4140u+next)!=0xfd){
            U(0xeed760u+next*4u)=cost;
            if(B(0xed4140u+next)==0xfe)U(0xeed760u+next*4u)=cost+4u;
            enqueue(next);
        }
        next=cell+228u;
        if(!U(0xeed760u+next*4u)&&B(0xed4140u+next)!=0xff&&B(0xed4140u+next)!=0xfd){
            U(0xeed760u+next*4u)=cost;
            if(B(0xed4140u+next)==0xfe)U(0xeed760u+next*4u)=cost+4u;
            enqueue(next);
        }
        next=cell-1u;
        if(!U(0xeed760u+next*4u)&&B(0xed4140u+next)!=0xff&&B(0xed4140u+next)!=0xfd){
            U(0xeed760u+next*4u)=cost;
            if(B(0xed4140u+next)==0xfe)U(0xeed760u+next*4u)=cost+4u;
            enqueue(next);
        }
        advance();
    }while(U(0xeae00c)!=U(0xeae008));
    return 1;
}

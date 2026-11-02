#include "actor_order_path.h"
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define S(a) (*(int8_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_00517a40(int32_t x,int32_t y,int32_t tx,int32_t ty)
{
    uint32_t start,target,current,next,distance,tail,head;
    FUN_0040f8f0((uint32_t *)0xeed760u,207936u,0);
    start=U(0xbca2d8)+(uint32_t)y*228u+(uint32_t)x;
    U(0xeae004)=start;U(0xeadfd4)=U(0xeadfd4)+1u;
    U(0xeed760u+start*4u)=1;U(0xf203a0)=U(0xeae004);
    head=0;tail=1;U(0xeae00c)=head;U(0xeae008)=tail;
    target=U(0xbca2d8)+(uint32_t)ty*228u+(uint32_t)tx;
    do{
        current=U(0xf203a0u+head*4u);if(current==target)break;
        distance=U(0xeed760u+current*4u)+1u;
        next=current-228u;
        if(U(0xeed760u+next*4u)==0&&S(0xee0c50u+next)>=0){
            U(0xeed760u+next*4u)=distance;tail=U(0xeae008);U(0xf203a0u+tail*4u)=next;++tail;U(0xeae008)=tail;if((int32_t)tail>51983){tail=0;U(0xeae008)=0;}
        }
        next=current+1u;
        if(U(0xeed760u+next*4u)==0&&S(0xee0c50u+next)>=0){
            U(0xeed760u+next*4u)=distance;tail=U(0xeae008);U(0xf203a0u+tail*4u)=next;++tail;U(0xeae008)=tail;if((int32_t)tail>51983){tail=0;U(0xeae008)=0;}
        }
        next=current+228u;
        if(U(0xeed760u+next*4u)==0&&S(0xee0c50u+next)>=0){
            U(0xeed760u+next*4u)=distance;tail=U(0xeae008);U(0xf203a0u+tail*4u)=next;++tail;U(0xeae008)=tail;if((int32_t)tail>51983){tail=0;U(0xeae008)=0;}
        }
        next=current-1u;
        if(U(0xeed760u+next*4u)==0&&S(0xee0c50u+next)>=0){
            U(0xeed760u+next*4u)=distance;tail=U(0xeae008);U(0xf203a0u+tail*4u)=next;++tail;U(0xeae008)=tail;if((int32_t)tail>51983){tail=0;U(0xeae008)=0;}
        }
        head=U(0xeae00c)+1u;U(0xeae00c)=head;if((int32_t)head>51983){head=0;U(0xeae00c)=0;}
    }while(head!=tail);
    return U(0xeed760u+target*4u)!=0;
}

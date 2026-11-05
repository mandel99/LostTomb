#include "actor_route_queries_internal.h"

/* Whole 51D160. DWORD costs and queue entries, BYTE classification, eight
   fixed neighbors in native order. The iteration limit succeeds after
   processing 50000 cells and before dequeuing the 50001st. */
uint32_t restore_actor_flood_unblocked_terrain(int32_t x,int32_t y)
{
    static const int32_t delta[8]={-228,1,228,-1,-227,229,227,-229};
    uint32_t cell,current,next,cost,head=0,tail=1,iterations=0,i;
    FUN_0040f8f0((uint32_t *)(uintptr_t)0xeed760u,207936,0);
    cell=RQ_U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
    RQ_U(0xeae004u)=cell;
    if(RQ_B(0xed4140u+cell)==255u)return 0;
    RQ_U(0xeed760u+cell*4u)=1;
    RQ_U(0xf203a0u)=RQ_U(0xeae004u);
    RQ_U(0xeae00cu)=0;RQ_U(0xeae008u)=1;
    do{
        if(++iterations>50000u)return 1;
        current=RQ_U(0xf203a0u+head*4u);
        cost=RQ_U(0xeed760u+current*4u)+1u;
        for(i=0;i<8u;++i){
            next=current+(uint32_t)delta[i];
            if(RQ_U(0xeed760u+next*4u)||RQ_B(0xed4140u+next)==255u)continue;
            RQ_U(0xeed760u+next*4u)=cost;
            tail=RQ_U(0xeae008u);RQ_U(0xf203a0u+tail*4u)=next;
            ++tail;RQ_U(0xeae008u)=tail;
            if(RQ_I(tail)>51983){tail=0;RQ_U(0xeae008u)=0;}
        }
        head=RQ_U(0xeae00cu)+1u;RQ_U(0xeae00cu)=head;
        if(RQ_I(head)>51983){head=0;RQ_U(0xeae00cu)=0;}
    }while(head!=tail);
    return 1;
}
uint32_t FUN_0051d160(int32_t x,int32_t y)
{return restore_actor_flood_unblocked_terrain(x,y);}

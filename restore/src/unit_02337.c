#define RESTORE_ROUTE_QUERY_CARDINAL
#include "actor_route_queries_internal.h"

/* Whole 51D420. Classification 6 is accepted along with values <=4 after
   negative-class filtering. The final XOR/SETNE defines all of EAX. */
uint32_t restore_actor_search_accessible_terrain(int32_t x,int32_t y,int32_t target_x,int32_t target_y)
{
    uint32_t start,target,current,head=0,tail=1,generation;
    FUN_0040f8f0((uint32_t *)(uintptr_t)0xeed760u,207936,0);
    generation=RQ_U(0xeadfd4u);
    start=RQ_U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
    RQ_U(0xeae004u)=start;RQ_U(0xeadfd4u)=generation+1u;
    RQ_U(0xeed760u+start*4u)=1;
    RQ_U(0xf203a0u)=RQ_U(0xeae004u);
    RQ_U(0xeae00cu)=0;RQ_U(0xeae008u)=1;
    target=RQ_U(0xbca2d8u)+(uint32_t)target_y*228u+(uint32_t)target_x;
    do{
        current=RQ_U(0xf203a0u+head*4u);if(current==target)break;
        tail=restore_route_query_expand_cardinal(current,tail);
        head=RQ_U(0xeae00cu)+1u;RQ_U(0xeae00cu)=head;
        if(RQ_I(head)>51983){head=0;RQ_U(0xeae00cu)=0;}
    }while(head!=tail);
    return RQ_U(0xeed760u+target*4u)!=0;
}
uint32_t FUN_0051d420(int32_t x,int32_t y,int32_t target_x,int32_t target_y)
{return restore_actor_search_accessible_terrain(x,y,target_x,target_y);}

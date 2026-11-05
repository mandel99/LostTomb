#define RESTORE_ROUTE_QUERY_CARDINAL
#include "actor_route_queries_internal.h"

/* Whole 51D6D0. Candidate tiles are scanned backwards at every dequeue;
   a matching nonnegative index returns index+1, and a negative match fails.
   The native nonzero count test is retained without a count clamp. */
int32_t restore_actor_search_accessible_candidates(int32_t x,int32_t y,const int32_t *candidate_tiles,int32_t candidate_count)
{
    uint32_t start,current,head,tail=1,generation,index,old_index;
    uint32_t candidates=(uint32_t)(uintptr_t)candidate_tiles;
    FUN_0040f8f0((uint32_t *)(uintptr_t)0xeed760u,207936,0);
    generation=RQ_U(0xeadfd4u);
    start=RQ_U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
    RQ_U(0xeae004u)=start;RQ_U(0xeadfd4u)=generation+1u;
    RQ_U(0xeed760u+start*4u)=1;
    RQ_U(0xf203a0u)=RQ_U(0xeae004u);
    RQ_U(0xeae00cu)=0;RQ_U(0xeae008u)=1;
    do{
        head=RQ_U(0xeae00cu);current=RQ_U(0xf203a0u+head*4u);
        if(candidate_count){
            index=(uint32_t)candidate_count-1u;
            do{
                if(RQ_U(candidates+index*4u)==current)return RQ_I(index)<0?0:RQ_I(index+1u);
                old_index=index;--index;
            }while(old_index);
        }
        tail=restore_route_query_expand_cardinal(current,tail);
        head=RQ_U(0xeae00cu)+1u;RQ_U(0xeae00cu)=head;
        if(RQ_I(head)>51983)RQ_U(0xeae00cu)=0;
    }while(RQ_U(0xeae00cu)!=tail);
    return 0;
}
int32_t FUN_0051d6d0(int32_t x,int32_t y,int32_t candidates,int32_t count)
{return restore_actor_search_accessible_candidates(x,y,(const int32_t *)(uintptr_t)(uint32_t)candidates,count);}

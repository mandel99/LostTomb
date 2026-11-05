#include "actor_route_queries_internal.h"

/* Whole 51DC10. The geometric query is called but its result is ignored.
   Equal costs prefer a transition away from bit16, then permit replacement
   when the sampled global low two bits equal the selected tile preference.
   Coordinates passed to the query remain the original x/y throughout. */
int32_t restore_actor_build_terrain_gradient_route(int32_t slot,int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t use_tile_preference)
{
    static const int32_t delta[8]={-228,-227,1,229,228,227,-1,-229};
    uint32_t preference=RQ_U(0xd3b374u)&3u;
    uint32_t cell=RQ_U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
    uint32_t count=0,best,weight,best_flag,next_flag,tile_preference,i,next;
    int32_t chosen,previous=-1;
    RQ_U(0xeae004u)=cell;best=RQ_U(0xeed760u+cell*4u);
    if(RQ_I(best)<=0)return 0;
    if(RQ_I(best)>1)do{
        best=RQ_U(0xeed760u+cell*4u);
        best_flag=(RQ_U(0xaf598cu+cell*4u)>>16)&1u;
        tile_preference=use_tile_preference==1?(uint32_t)RQ_B(0xacf85cu+cell)&3u:preference;
        (void)FUN_0051cba0(x,y,target_x,target_y);
        chosen=-1;
        for(i=0;i<8u;++i){
            if((int32_t)i==previous)continue;
            next=cell+RQ_U(0x5f80fcu+i*4u);
            weight=RQ_U(0xeed760u+next*4u);
            next_flag=(RQ_U(0xaf598cu+next*4u)>>16)&1u;
            if(weight&&(RQ_I(weight)<RQ_I(best)||
                (weight==best&&((best_flag&&!next_flag)||preference==tile_preference)))){
                best=weight;best_flag=next_flag;chosen=(int32_t)i;
            }
        }
        if(chosen==-1)return 0;
        cell+=(uint32_t)delta[chosen];previous=(chosen+4)&7;
        RQ_B(0xf533c8u+count)=(uint8_t)previous;
        if(++count>=500u)return 0;
    }while(RQ_I(best)>1);
    for(i=0;i<count;++i)RQ_B(0xc8ad38u+(uint32_t)slot*500u+i)=RQ_B(0xf533c7u+count-i);
    return (int32_t)count;
}
int32_t FUN_0051dc10(int32_t slot,int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t use_tile_preference)
{return restore_actor_build_terrain_gradient_route(slot,target_x,target_y,x,y,use_tile_preference);}

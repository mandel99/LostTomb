#include "actor_route_queries_internal.h"

/* Whole 51D980. The forced straight prefix uses the original coordinates
   while count<=straight_steps. The later gradient walk uses current x/y,
   skips the preceding reverse, and reads the live neighbor table. */
int32_t restore_actor_build_straight_gradient_route(int32_t slot,int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t straight_steps)
{
    static const int32_t delta[8]={-228,-227,1,229,228,227,-1,-229};
    static const int8_t dx[8]={0,1,1,1,0,-1,-1,-1};
    static const int8_t dy[8]={-1,-1,0,1,1,1,0,-1};
    uint32_t cell=RQ_U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
    uint32_t count=0,best,weight,preferred,i,next,walk_x=(uint32_t)x,walk_y=(uint32_t)y;
    int32_t chosen,previous=-1;
    RQ_U(0xeae004u)=cell;best=RQ_U(0xeed760u+cell*4u);
    if(RQ_I(best)<=0)return 0;
    if(RQ_I(best)>1)do{
        best=RQ_U(0xeed760u+cell*4u);
        preferred=FUN_0051cba0(RQ_I(count)>straight_steps?RQ_I(walk_x):x,
            RQ_I(count)>straight_steps?RQ_I(walk_y):y,target_x,target_y)&255u;
        chosen=-1;
        for(i=0;i<8u;++i){
            if((int32_t)i==previous)continue;
            next=cell+RQ_U(0x5f80fcu+i*4u);weight=RQ_U(0xeed760u+next*4u);
            if(!weight)continue;
            if(RQ_I(count)<=straight_steps){
                if(RQ_I(weight)<=RQ_I(best)&&i==preferred){best=weight;chosen=(int32_t)i;}
            }else if(RQ_I(weight)<RQ_I(best)||
                (weight==best&&(i==preferred||chosen==-1))){best=weight;chosen=(int32_t)i;}
        }
        if(chosen==-1)return 0;
        cell+=(uint32_t)delta[chosen];
        walk_x+=(uint32_t)(int32_t)dx[chosen];walk_y+=(uint32_t)(int32_t)dy[chosen];
        previous=(chosen+4)&7;RQ_B(0xf533c8u+count)=(uint8_t)previous;
        if(++count>=500u)return 0;
    }while(RQ_I(best)>1);
    for(i=0;i<count;++i)RQ_B(0xc8ad38u+(uint32_t)slot*500u+i)=RQ_B(0xf533c7u+count-i);
    return (int32_t)count;
}
int32_t FUN_0051d980(int32_t slot,int32_t target_x,int32_t target_y,int32_t x,int32_t y,int32_t straight_steps)
{return restore_actor_build_straight_gradient_route(slot,target_x,target_y,x,y,straight_steps);}

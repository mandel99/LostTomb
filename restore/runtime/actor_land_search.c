#include "actor_routing.h"
#include "actor_order_path.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define C(a) ((int32_t)*(volatile int8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

enum search_rule {
    FREE_LAND,ANIMAL,UNRESTRICTED,FREE_EXCEPT3,NONNEG_EXCEPT3,
    ROAD_OR5,ROAD,SHORE_EMPTY,OTHER_ROAD,DESTRUCTIBLE
};
static int allowed(enum search_rule rule,uint32_t tile)
{
    int32_t classification;
    if(rule==UNRESTRICTED)return 1;
    classification=C((rule==OTHER_ROAD?0xebab20u:rule==DESTRUCTIBLE?0xec7630u:0xee0c50u)+tile);
    switch(rule){
    case ANIMAL:return (classification>=0||classification==-3)&&classification!=1&&classification!=3;
    case NONNEG_EXCEPT3:return classification>=0&&classification!=3;
    case ROAD_OR5:return classification>=0&&(classification<=2||classification==5);
    case ROAD:case OTHER_ROAD:return classification>=0&&classification<=2;
    case DESTRUCTIBLE:return classification>=0;
    case SHORE_EMPTY:
        return classification>=0||(classification==-1&&(U(0xaf598cu+tile*4u)&0x10004u)&&!H(0xb4e6fcu+tile*2u));
    default:
        if(classification<0&&classification!=-3&&
           (classification!=-1||!(U(0xaf598cu+tile*4u)&0x1000u)))return 0;
        return classification<=4&&(rule!=FREE_EXCEPT3||classification!=3);
    }
}

/* The ten complete native producers share the four-neighbor ring traversal.
   Occupancy calls differ: all four, west only, or none. Cache the native tail
   between neighbors, but reload it on a failed checked candidate. Helpers can
   change queue globals or distance cells; do not recheck a visited flag after
   a callback. Publish the pre-wrap tail/head before publishing zero. */
static uint32_t search(int32_t x,int32_t y,int32_t tx,int32_t ty,enum search_rule rule,uint32_t checked)
{
    static const int32_t delta[4]={-228,1,228,-1};
    uint32_t start,target,current,next,cost,head=0,tail=1,i;
    FUN_0040f8f0((uint32_t *)(uintptr_t)0xeed760u,207936,0);
    start=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
    U(0xeae004u)=start;U(0xeadfd4u)=U(0xeadfd4u)+1u;
    U(0xeed760u+start*4u)=1;U(0xf203a0u)=U(0xeae004u);
    U(0xeae00cu)=0;U(0xeae008u)=1;
    target=U(0xbca2d8u)+(uint32_t)ty*228u+(uint32_t)tx;
    do{
        current=U(0xf203a0u+head*4u);if(current==target)break;
        cost=U(0xeed760u+current*4u)+1u;
        for(i=0;i<4u;++i){
            next=current+(uint32_t)delta[i];
            if(U(0xeed760u+next*4u))continue;
            if((!(checked&(1u<<i))||FUN_0051aff0(I(next)))&&allowed(rule,next)){
                U(0xeed760u+next*4u)=cost;
                tail=U(0xeae008u);U(0xf203a0u+tail*4u)=next;
                ++tail;U(0xeae008u)=tail;
                if(I(tail)>51983){tail=0;U(0xeae008u)=0;}
            }else if(checked&(1u<<i))tail=U(0xeae008u);
        }
        head=U(0xeae00cu)+1u;U(0xeae00cu)=head;
        if(I(head)>51983){head=0;U(0xeae00cu)=0;}
    }while(head!=tail);
    return U(0xeed760u+target*4u)!=0;
}

#define QUERY(address,rule,mask) \
uint32_t FUN_##address(int32_t x,int32_t y,int32_t tx,int32_t ty) \
{return search(x,y,tx,ty,rule,mask);}
QUERY(00517f20,FREE_LAND,15)
QUERY(005181c0,ANIMAL,8)
QUERY(005186d0,UNRESTRICTED,0)
QUERY(00518880,FREE_EXCEPT3,15)
QUERY(00519390,NONNEG_EXCEPT3,15)
QUERY(005195e0,ROAD_OR5,15)
QUERY(00519840,ROAD,0)
QUERY(00519be0,SHORE_EMPTY,0)
QUERY(0051a030,OTHER_ROAD,0)
QUERY(0051b040,DESTRUCTIBLE,0)
#undef QUERY

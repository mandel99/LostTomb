#include "actor_routing.h"
#include "actor_transport.h"
#include "actor_order_path.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define C(a) ((int32_t)*(volatile int8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

static uint32_t queued_cost(uint32_t index)
{return U(0xeed760u+U(0xf203a0u+index*4u)*4u);}

/* Native selection is a stateful scan, not a heap or relaxing Dijkstra.
   The first scan stops on an improvement at/below the previous minimum;
   the wrapped low segment scans through its complete remaining interval. */
static uint32_t take_best(uint32_t tail,uint32_t *minimum)
{
    uint32_t head=U(0xeae00cu),chosen=head,best=queued_cost(head),index,value,tile;
    if(I(best)>I(*minimum)){
        if(I(tail)>I(head)){
            for(index=head+1u;I(index)<I(tail);++index){
                value=queued_cost(index);
                if(I(value)<I(best)){
                    chosen=index;best=value;if(I(value)<=I(*minimum))goto selected;
                }
            }
        }else{
            for(index=head+1u;I(index)<=51983;++index){
                value=queued_cost(index);
                if(I(value)<I(best)){
                    chosen=index;best=value;if(I(value)<=I(*minimum))goto selected;
                }
            }
            if(I(best)>I(*minimum))for(index=0;I(index)<I(tail);++index){
                value=queued_cost(index);if(I(value)<I(best)){chosen=index;best=value;}
            }
        }
selected:
        *minimum=best;
    }
    head=U(0xeae00cu);tile=U(0xf203a0u+chosen*4u);
    if(chosen!=head)U(0xf203a0u+chosen*4u)=U(0xf203a0u+head*4u);
    return tile;
}

/* Read the target array in reverse on every selected cell. It may alias live
   queue/distance data. A match at a signed-negative index returns zero. */
static int target_index(uint32_t list,uint32_t count,uint32_t tile,uint32_t *result)
{
    while(count){
        --count;
        if(U(list+count*4u)==tile){*result=I(count)<0?0u:count+1u;return 1;}
    }
    return 0;
}

static uint32_t expand(uint32_t tile,uint32_t tail,int32_t owner)
{
    static const int32_t delta[4]={-228,1,228,-1};
    uint32_t cost=U(0xeed760u+tile*4u)+1u,next,i,extra;int32_t classification;
    for(i=0;i<4u;++i){
        next=tile+(uint32_t)delta[i];
        if(U(0xeed760u+next*4u))continue;
        if(FUN_0051afa0(I(next))){
            classification=C(0xec7630u+next);
            if(owner){
                if(classification!=0&&classification!=2&&
                   (classification!=1||H(0xb4e6fcu+next*2u)!=owner))goto rejected;
                extra=0;
            }else{
                if(classification<0||classification>=5)goto rejected;
                extra=classification==3?40u:classification==1?10u:0u;
            }
            U(0xeed760u+next*4u)=cost+extra;
            tail=U(0xeae008u);U(0xf203a0u+tail*4u)=next;
            ++tail;U(0xeae008u)=tail;
            if(I(tail)>51983){tail=0;U(0xeae008u)=0;}
            continue;
        }
rejected:
        tail=U(0xeae008u);
    }
    return tail;
}

/* Whole51A420/51A9D0. Nonzero owner mode uses FIFO and ignores limit.
   Zero owner uses native weighted selection and signed expansion limits.
   A single target returns its discovered distance flag even at the limit;
   the multiple-target producer succeeds only when a selected cell matches. */
static uint32_t search(int32_t x,int32_t y,int32_t tx,int32_t ty,uint32_t targets,
                       uint32_t target_count,int32_t owner,int32_t limit,int multiple)
{
    uint32_t start,target=0,tail=1,head=0,minimum=0,iterations=0,tile,result;
    FUN_0040f8f0((uint32_t *)(uintptr_t)0xeed760u,207936,0);
    start=U(0xbca2d8u)+(uint32_t)y*228u+(uint32_t)x;
    U(0xeae004u)=start;U(0xeadfd4u)=U(0xeadfd4u)+1u;U(0xeadfd8u)=U(0xeadfd8u)+1u;
    U(0xeed760u+start*4u)=1;U(0xf203a0u)=U(0xeae004u);
    U(0xeae00cu)=0;U(0xeae008u)=1;
    if(!multiple)target=U(0xbca2d8u)+(uint32_t)ty*228u+(uint32_t)tx;
    do{
        tile=owner?U(0xf203a0u+head*4u):take_best(tail,&minimum);
        if(multiple){if(target_index(targets,target_count,tile,&result))return result;}
        else if(tile==target)break;
        if(!owner){++iterations;if(I(iterations)>limit)break;}
        tail=expand(tile,tail,owner);
        head=U(0xeae00cu)+1u;U(0xeae00cu)=head;
        if(I(head)>51983){head=0;U(0xeae00cu)=0;}
    }while(head!=tail);
    return multiple?0u:U(0xeed760u+target*4u)!=0;
}

uint32_t FUN_0051a420(int32_t x,int32_t y,int32_t tx,int32_t ty,int32_t owner,int32_t limit)
{return search(x,y,tx,ty,0,0,owner,limit,0);}

int32_t FUN_0051a9d0(int32_t x,int32_t y,int32_t targets,int32_t count,int32_t owner,int32_t limit)
{return I(search(x,y,0,0,(uint32_t)targets,(uint32_t)count,owner,limit,1));}

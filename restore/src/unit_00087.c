#include "actor_order_los.h"
#include "actor_order_flood.h"
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define I(a) (*(int32_t *)(uintptr_t)(a))
static int32_t add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}
static int32_t sign(int32_t value){return value<0?-1:value>0?1:0;}
static uint32_t magnitude(int32_t value){uint32_t mask=value<0?0xffffffffu:0;return ((uint32_t)value^mask)-mask;}
static int32_t square_sum(int32_t x,int32_t y){return (int32_t)((uint32_t)x*(uint32_t)x+(uint32_t)y*(uint32_t)y);}
static int reachable(uint32_t cell)
{
    if(!(B(0xaf598cu+cell*4u)&4u))return 0;
    if(!(B(0xaf55fcu+cell*4u)&4u))return 0;
    if(!(B(0xaf5d1cu+cell*4u)&4u))return 0;
    if(!(B(0xaf5990u+cell*4u)&4u))return 0;
    if(!(B(0xaf5988u+cell*4u)&4u))return 0;
    if(B(0xed4140u+cell)==0xff)return 0;
    return I(0xeed760u+cell*4u)>0;
}
static int32_t intersection(int32_t origin,int32_t delta,const uint8_t ratio[10])
{uint8_t value[10];restore_los_intersection80(origin,delta,ratio,value);return (int32_t)(uint32_t)restore_los_trunc80(value);}
int32_t FUN_0040c180(int32_t x,int32_t y,int32_t tx,int32_t ty,int32_t *out_x,int32_t *out_y)
{
    int32_t dx,dy,step_x,step_y,best=0,best_x=x,best_y=y,working,remaining,candidate,metric;
    uint32_t nx,ny,row;uint8_t slope[10],inverse[10];
    if(x==tx&&y==ty){*out_x=x;*out_y=y;return 0;}
    (void)FUN_0051ce90(x,y);
    dx=sub(tx,x);dy=sub(ty,y);step_x=sign(dx);step_y=sign(dy);nx=magnitude(dx);ny=magnitude(dy);
    restore_los_slopes80(dx,dy,slope,inverse);
    row=U(0xbca2d8)+(uint32_t)y*228u;working=y;
    for(remaining=(int32_t)ny;remaining>0;--remaining){
        if(!restore_los_equal_max80(inverse)){
            row+=(uint32_t)step_y*228u;working=add(working,step_y);candidate=intersection(x,sub(working,y),inverse);
            if(reachable(row+(uint32_t)candidate)){
                metric=square_sum(sub(working,y),sub(candidate,x));
                if(metric>=best){best=metric;best_x=candidate;best_y=working;}
            }
        }
    }
    working=x;
    for(remaining=(int32_t)nx;remaining>0;--remaining){
        if(!restore_los_equal_max80(slope)){
            working=add(working,step_x);candidate=intersection(y,sub(working,x),slope);
            if(reachable(U(0xbca2d8)+(uint32_t)candidate*228u+(uint32_t)working)){
                metric=square_sum(sub(working,x),sub(candidate,y));
                if(metric>=best){best=metric;best_x=working;best_y=candidate;}
            }
        }
    }
    *out_x=best_x;*out_y=best_y;return best;
}

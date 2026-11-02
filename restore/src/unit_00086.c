#include "actor_order_los.h"
#define B(a) (*(uint8_t *)(uintptr_t)(a))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
static int32_t add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}
static int32_t sign(int32_t value){return value<0?-1:value>0?1:0;}
static uint32_t magnitude(int32_t value){uint32_t mask=value<0?0xffffffffu:0;return ((uint32_t)value^mask)-mask;}
static int32_t square_sum(int32_t x,int32_t y){return (int32_t)((uint32_t)x*(uint32_t)x+(uint32_t)y*(uint32_t)y);}
static uint32_t flag_address(uint32_t cell){return 0xaf598cu+cell*4u;}
static int clear(uint32_t cell)
{
    uint32_t flags=U(flag_address(cell));
    if(flags&0x40u)return 1;
    if(flags&4u)return 0;
    if(B(flag_address(cell-228u))&4u)return 0;
    if(B(flag_address(cell+228u))&4u)return 0;
    if(B(flag_address(cell+1u))&4u)return 0;
    if(B(flag_address(cell-1u))&4u)return 0;
    if(B(flag_address(cell-227u))&4u)return 0;
    if(B(flag_address(cell-229u))&4u)return 0;
    if(B(flag_address(cell+229u))&4u)return 0;
    if(B(flag_address(cell+227u))&4u)return 0;
    return 1;
}
static int32_t intersection(int32_t origin,int32_t delta,const uint8_t ratio[10])
{uint8_t value[10];restore_los_intersection80(origin,delta,ratio,value);return (int32_t)(uint32_t)restore_los_trunc80(value);}
int32_t FUN_0040bda0(int32_t x,int32_t y,int32_t tx,int32_t ty,int32_t *out_x,int32_t *out_y)
{
    int32_t dx=sub(tx,x),dy=sub(ty,y),step_x=sign(dx),step_y=sign(dy),best=0,best_x=x,best_y=y;
    uint32_t nx=magnitude(dx),ny=magnitude(dy),row;uint8_t slope[10],inverse[10];
    int32_t working,remaining,candidate,metric,actual_x;
    if(x==tx&&y==ty){*out_x=x;*out_y=y;return 0;}
    restore_los_slopes80(dx,dy,slope,inverse);
    row=U(0xbca2d8)+(uint32_t)y*228u;working=y;
    for(remaining=(int32_t)ny;remaining>0;--remaining){
        if(!restore_los_equal_max80(inverse)){
            working=add(working,step_y);row+=(uint32_t)step_y*228u;
            candidate=intersection(x,sub(working,y),inverse);
            if(clear(row+(uint32_t)candidate)){
                metric=square_sum(sub(candidate,x),sub(working,y));
                if(metric>=best){best=metric;best_x=candidate;best_y=working;}
            }
        }
    }
    working=x;
    for(remaining=(int32_t)nx;remaining>0;--remaining){
        if(!restore_los_equal_max80(slope)){
            working=add(working,step_x);candidate=intersection(y,sub(working,x),slope);
            if(clear(U(0xbca2d8)+(uint32_t)candidate*228u+(uint32_t)working)){
                metric=square_sum(sub(working,x),sub(candidate,y));
                if(metric>=best){best=metric;best_x=working;best_y=candidate;}
            }
        }
    }
    *out_x=best_x;*out_y=best_y;actual_x=*out_x;
    row=U(0xbca2d8)+(uint32_t)best_y*228u+(uint32_t)actual_x;
    if(actual_x!=x||best_y!=y)return best;
    if(!(B(flag_address(row-228u))&4u)){*out_y=add(best_y,-1);return best;}
    if(!(B(flag_address(row-227u))&4u)){*out_y=add(best_y,-1);*out_x=add(*out_x,1);return best;}
    if(!(B(flag_address(row+228u))&4u)){*out_y=add(best_y,1);return best;}
    if(!(B(flag_address(row-229u))&4u)){*out_y=add(best_y,-1);*out_x=add(*out_x,-1);return best;}
    if(!(B(flag_address(row+1u))&4u)){*out_x=add(actual_x,1);return best;}
    if(!(B(flag_address(row+229u))&4u)){*out_y=add(best_y,1);*out_x=add(*out_x,1);return best;}
    if(!(B(flag_address(row-1u))&4u)){*out_x=add(actual_x,-1);return best;}
    if(!(B(flag_address(row+227u))&4u)){*out_y=add(best_y,1);*out_x=add(*out_x,-1);}
    return best;
}

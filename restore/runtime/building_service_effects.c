#include "building_service_effects.h"
#include "house_services.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern uint32_t FUN_00475b50(uint32_t building);

/* Whole482DD0/482EF0: nonzero signed IDs, house kinds10..29, nonzero state.
   Subtraction wraps at32 bits before testing the result's sign. */
static uint32_t reduce_health(int32_t building,uint32_t amount,uint32_t field)
{
    uint32_t off=(uint32_t)building*264u,value;int32_t kind;
    if(building){
        kind=H(0x7999e8u+off);
        if(kind>=10&&kind<=29&&B(0x7999d8u+off)){
            value=U(field+off)-amount;U(field+off)=value;
            if(I(value)<0)U(field+off)=0;
        }
    }
    return 0;
}
static uint32_t apothecary(int32_t building,uint32_t amount)
{return reduce_health(building,amount,0x799ac0u);}
static uint32_t physician(int32_t building,uint32_t amount)
{return reduce_health(building,amount,0x799ad4u);}
uint32_t FUN_00482dd0(int32_t x,int32_t y,int32_t amount)
{
    (void)restore_house_service_scan(x,y,2u,apothecary,(uint32_t)amount,2u);return 0;
}
void FUN_00482ef0(int32_t x,int32_t y,int32_t amount)
{(void)restore_house_service_scan(x,y,2u,physician,(uint32_t)amount,0);}

typedef struct {uint32_t left,right,bottom,gap,tile;int valid;} RiskRectangle;
static RiskRectangle risk_rectangle(int32_t x,int32_t y)
{
    RiskRectangle r;uint32_t top=(uint32_t)y-2u,width,height;
    r.left=(uint32_t)x-2u;r.right=(uint32_t)x+2u;r.bottom=(uint32_t)y+2u;
    if(I(r.left)<0)r.left=0;if(I(top)<0)top=0;
    width=U(0xbca2d0u);if(I(r.right)>=I(width))r.right=width-1u;
    height=U(0xbca2d4u);if(I(r.bottom)>=I(height))r.bottom=height-1u;
    U(0xd3b4e0u)=top;r.tile=U(0xbca2d8u)+top*228u+r.left;
    r.gap=227u+r.left-r.right;U(0xbc9768u)=r.tile;r.valid=I(top)<=I(r.bottom);
    return r;
}
static void subtract_risk(uint32_t address,uint32_t amount)
{
    W(address)=(uint16_t)((uint32_t)W(address)-amount);
    if(H(address)<0)W(address)=0;
}

/* Whole482C10. Unlike ordinary coverage, this cursor is live across the
   root-building query. Terrain risk uses the last cached reduction/maximum;
   a positive cell ID reloads both from their globals after the query. */
int32_t FUN_00482c10(int32_t x,int32_t y)
{
    RiskRectangle r;uint32_t tile,next_x,next_y,off,count=0,step,maximum=1000;
    int32_t building,risk;
    U(0x9ab9e0u)=1000;r=risk_rectangle(x,y);if(!r.valid)return 0;
    tile=r.tile;step=U(0x706b70u);
    do{
        U(0xd3b4e4u)=r.left;
        if(I(r.left)<=I(r.right))do{
            building=H(0xb4e6fcu+tile*2u);
            if(building>0){
                off=FUN_00475b50((uint32_t)building)*264u;maximum=U(0x9ab9e0u);
                risk=H(0x799a1eu+off);
                if(risk>I(maximum)){maximum=(uint32_t)risk;U(0x9ab9e0u)=maximum;}
                step=U(0x706b70u);subtract_risk(0x799a1eu+off,step);
                if(B(0x7999ddu+off)&&H(0x7999f4u+off)>0)++count;
                tile=U(0xbc9768u);
            }else if(U(0xaf598cu+tile*4u)&0x4000u){
                risk=H(0xa9cc1cu+tile*2u);
                if(risk>I(maximum)){maximum=(uint32_t)risk;U(0x9ab9e0u)=maximum;}
                subtract_risk(0xa9cc1cu+tile*2u,step);
            }
            next_x=U(0xd3b4e4u)+1u;++tile;U(0xd3b4e4u)=next_x;U(0xbc9768u)=tile;
        }while(I(next_x)<=I(r.right));
        next_y=U(0xd3b4e0u)+1u;tile+=r.gap;U(0xbc9768u)=tile;U(0xd3b4e0u)=next_y;
    }while(I(next_y)<=I(r.bottom));
    return I(count);
}

/* Whole483000. Fire prevention queries any nonzero signed cell ID, reduces
   the resolved record even for ID0/negative, but counts positive roots only. */
int32_t FUN_00483000(int32_t x,int32_t y)
{
    RiskRectangle r;uint32_t tile,next_x,next_y,root,off,count=0;int32_t building,risk;
    U(0x9ab9e4u)=1000;r=risk_rectangle(x,y);if(!r.valid)return 0;tile=r.tile;
    do{
        U(0xd3b4e4u)=r.left;
        if(I(r.left)<=I(r.right))do{
            building=H(0xb4e6fcu+tile*2u);
            if(building){
                root=FUN_00475b50((uint32_t)building);off=root*264u;risk=H(0x799a20u+off);
                if(risk>I(U(0x9ab9e4u)))U(0x9ab9e4u)=(uint32_t)risk;
                subtract_risk(0x799a20u+off,U(0x7078e0u));
                if(I(root)>0&&B(0x7999ddu+off)&&H(0x7999f4u+off)>0)++count;
            }
            next_x=U(0xd3b4e4u)+1u;tile=U(0xbc9768u)+1u;
            U(0xd3b4e4u)=next_x;U(0xbc9768u)=tile;
        }while(I(next_x)<=I(r.right));
        next_y=U(0xd3b4e0u)+1u;tile+=r.gap;U(0xd3b4e0u)=next_y;U(0xbc9768u)=tile;
    }while(I(next_y)<=I(r.bottom));
    return I(count);
}

/* Native resource jump table483E00, including the index-1 fallback. */
static uint32_t market_food_index(uint32_t resource)
{
    static const int8_t slots[19]={0,1,2,3,4,5,6,7,-1,-1,-1,-1,9,-1,8,-1,10,-1,11};
    return resource-1u<19u?(uint32_t)(int32_t)slots[resource-1u]:UINT32_MAX;
}
static void supply_good(uint32_t house,uint32_t market,uint32_t model,
    uint32_t model_field,uint32_t house_field,uint32_t source_field,uint32_t timer)
{
    uint32_t quantity=U(model_field+model),demand;int32_t have,available;
    if(B(0x7999dbu+house)>1u)quantity*=4u;
    if(!quantity)return;
    W(timer+market)=10;available=H(source_field+market);if(available<=0)return;
    have=H(house_field+house);demand=quantity*2u-(uint32_t)have;
    if(I(demand)<=0)return;
    if(I(demand)>available){W(house_field+house)=(uint16_t)((uint32_t)have+(uint32_t)available);W(source_field+market)=0;}
    else{W(house_field+house)=(uint16_t)((uint32_t)have+demand);W(source_field+market)=(uint16_t)((uint32_t)available-demand);}
}

/* Whole483910: per-cell population demand, ordered food list, two separate
   food-transfer tables and four ordered goods transfers. In particular, a
   positive unmet food demand consumes a variety slot even for an unsupported
   inventory index. All stocks/destinations and demand timers are WORDs. */
static uint32_t supply_house(int32_t building,uint32_t market)
{
    uint32_t house=(uint32_t)building*264u,model,wanted,foods,i,resource,slot,address,demand;
    int32_t population,level,available,have;
    if(building<=0||!B(0x7999ddu+house)||(population=H(0x7999f4u+house))<=0)return 0;
    level=H(0x7999eau+house);if(level<19)++level;model=(uint32_t)level*88u;
    wanted=U(0x70cdf4u+model);if(I(wanted)>0&&I(wanted)<4)++wanted;
    foods=U(0xf53dd8u);
    if(I(foods)>0)for(i=0;i<foods;++i){
        resource=U(0xf53ddcu+i*4u);if(!resource)continue;
        slot=market_food_index(resource);address=0x799a2eu+market+slot*2u;
        available=H(address);if(available<=0||!wanted)continue;
        have=H(0x799a2cu+house+slot*2u);demand=(uint32_t)population*6u-(uint32_t)have;
        if(I(demand)>population*2)demand=(uint32_t)population*2u;
        if(I(demand)<=0)continue;
        if(slot<=7u){
            if(slot!=0u&&slot!=6u)demand>>=1;
            if(available>=I(demand)){
                W(0x799a2cu+house+slot*2u)=(uint16_t)((uint32_t)have+demand);
                W(address)=(uint16_t)((uint32_t)available-demand);
            }else{
                W(0x799a2cu+house+slot*2u)=(uint16_t)((uint32_t)have+(uint32_t)available);W(address)=0;
            }
        }
        --wanted;
    }
    supply_good(house,market,model,0x70ce04u,0x799a3cu,0x799a3eu,0x799a7cu);
    supply_good(house,market,model,0x70cdf8u,0x799a3eu,0x799a40u,0x799a76u);
    supply_good(house,market,model,0x70cdfcu,0x799a40u,0x799a42u,0x799a7au);
    supply_good(house,market,model,0x70ce00u,0x799a42u,0x799a44u,0x799a78u);
    return 1;
}
int32_t FUN_00483910(int32_t market,int32_t x,int32_t y)
{
    return restore_house_service_scan(x,y,2u,supply_house,(uint32_t)market*264u,3u);
}

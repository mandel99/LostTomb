#include "pharaoh/population/house_economy.h"
#include <stddef.h>

_Static_assert(sizeof(PharaohHouseEconomyRecord)==264,"native building stride");
_Static_assert(offsetof(PharaohHouseEconomyRecord,level)==0x12,"house level WORD");
_Static_assert(offsetof(PharaohHouseEconomyRecord,population)==0x1c,"population WORD");
_Static_assert(offsetof(PharaohHouseEconomyRecord,primary_input)==0x3c,"primary input WORD");
_Static_assert(offsetof(PharaohHouseEconomyRecord,well)==0x3f,"well BYTE");
_Static_assert(offsetof(PharaohHouseEconomyRecord,dentist)==0x4d,"dentist BYTE");
_Static_assert(offsetof(PharaohHouseEconomyRecord,food)==0x54,"food WORDs");
_Static_assert(offsetof(PharaohHouseEconomyRecord,beer)==0x64,"beer WORD");
_Static_assert(offsetof(PharaohHouseEconomyRecord,entertainment)==0x80,"signed coverage BYTEs");
_Static_assert(offsetof(PharaohHouseEconomyRecord,evolve_text)==0x85,"evolve reason BYTE");
_Static_assert(offsetof(PharaohHouseEconomyRecord,pottery_demand)==0x9e,"market timer WORDs");
_Static_assert(offsetof(PharaohHouseEconomyRecord,desirability)==0xbe,"desirability signed BYTE");
_Static_assert(offsetof(PharaohHouseEconomyRecord,secondary_input)==0xcc,"secondary input WORD");
_Static_assert(offsetof(PharaohHouseEconomyRecord,primary_orders)==0x104,"pending input WORDs");
_Static_assert(sizeof(PharaohHouseDemandBank)==0x49d8,"native player stride");
_Static_assert(sizeof(PharaohGranaryResourceBank)==0x49d8,"native resource bank stride");

enum HouseDemand {
    MISSING_WATER=0,MISSING_WELL=1,MORE_ENTERTAINMENT=2,MORE_EDUCATION=3,
    MISSING_EDUCATION=4,REQUIRES_SCHOOL=5,REQUIRES_LIBRARY=6,
    MISSING_DENTIST=8,MISSING_COURTHOUSE=9,MISSING_FOOD=10,
    MISSING_HEALTH=14,MORE_HEALTH=15,REQUIRES_DENTIST=16,
    REQUIRES_COURTHOUSE=17,REQUIRES_HEALTH=18,MISSING_RELIGION=19,
    SECOND_RELIGION=20,THIRD_RELIGION=21,REQUIRES_RELIGION=22,
    MISSING_ENTERTAINMENT=33,LUXURY_KINDS=0x3a0/4,SECOND_LUXURY=0x3f8/4,
    REQUIRES_MORE_COURTHOUSE=0x444/4,ANY_COURTHOUSE=0x448/4
};

static void demand_count(volatile PharaohHouseDemandBank *p,enum HouseDemand slot)
{ p->words[slot]=p->words[slot]+1u; }
static uint32_t demand_unmet(volatile PharaohHouseDemandBank *p,enum HouseDemand slot)
{ demand_count(p,slot);return 0; }
static volatile PharaohHouseDemandBank *demand_player(const PharaohHouseEconomyContext *c)
{ return &c->players[*c->current_player]; }

/* Whole existing HouseChecks53/HouseEvolution algorithm. Wrapped addition
 * precedes signed division, including the native minimum-one adjustment. */
static int32_t goods_required(int32_t q,uint8_t size)
{
    if(size==1&&q>0){q=(int32_t)((uint32_t)q+2u)/4;if(q<1)q=1;}
    return q;
}
static uint32_t food_index(uint32_t resource)
{
    static const int8_t slots[19]={0,1,2,3,4,5,6,7,-1,-1,-1,-1,9,-1,8,-1,10,-1,11};
    return resource-1u<19u?(uint32_t)(int32_t)slots[resource-1u]:UINT32_MAX;
}
static uint16_t food_stock(volatile PharaohHouseEconomyRecord *h,uint32_t slot)
{
    if(slot==UINT32_MAX)return h->unsupported_food;
    return (uint16_t)h->market_stock[slot];
}
static uint32_t food_varieties(const PharaohHouseEconomyContext *c,
    volatile PharaohHouseEconomyRecord *h)
{
    const int32_t total=*c->food_count;uint32_t count=0;
    if(total>0)for(uint32_t i=0;i<(uint32_t)total;++i){
        const uint32_t resource=(uint32_t)c->foods[i];
        if(resource&&food_stock(h,food_index(resource)))++count;
    }
    return count;
}

/* Whole00453B40. Cache the requirements before any player counter stores;
 * latch the player only after the three early water/culture failures. */
uint32_t pharaoh_house_check_requirements(const PharaohHouseEconomyContext *c,
    int32_t building,uint32_t next_level)
{
    volatile PharaohHouseEconomyRecord *h=&c->buildings[building];
    int32_t level=h->level;
    if(next_level)level=(int32_t)((uint32_t)level+1u);
    const uint8_t size=h->size;
    const volatile PharaohHouseModel *m=&c->models[level];
    const int32_t religion=m->religion,water=m->water,dentist=m->dentist;
    const int32_t courthouse=m->courthouse,health=m->health;
    const int32_t entertainment=m->entertainment,education=m->education,foods=m->food_types;
    const int32_t pottery=goods_required(m->pottery,size),linen=goods_required(m->linen,size);
    const int32_t jewelry=goods_required(m->jewelry,size),beer=goods_required(m->beer,size);
    if(!h->water){
        if(water>=2)return demand_unmet(demand_player(c),MISSING_WATER);
        if(water==1&&!h->well)return demand_unmet(demand_player(c),MISSING_WELL);
    }
    int32_t value=h->entertainment;
    if(value<entertainment)return demand_unmet(demand_player(c),value?MORE_ENTERTAINMENT:MISSING_ENTERTAINMENT);
    value=h->education;
    if(value<education)return demand_unmet(demand_player(c),value?MORE_EDUCATION:MISSING_EDUCATION);
    volatile PharaohHouseDemandBank *p=demand_player(c);
    if(education==2){demand_count(p,REQUIRES_SCHOOL);demand_count(p,REQUIRES_LIBRARY);}
    else if(education==1)demand_count(p,REQUIRES_SCHOOL);
    value=h->religion;
    if(value<religion){
        if(religion==1)return demand_unmet(p,MISSING_RELIGION);
        if(religion==2)return demand_unmet(p,SECOND_RELIGION);
        if(religion==3)return demand_unmet(p,THIRD_RELIGION);
    }else if(religion>0)demand_count(p,REQUIRES_RELIGION);
    if(h->dentist<dentist)return demand_unmet(p,MISSING_DENTIST);
    if(dentist==1)demand_count(p,REQUIRES_DENTIST);
    value=h->courthouse;
    if(value<courthouse)return demand_unmet(p,value?MISSING_COURTHOUSE:ANY_COURTHOUSE);
    if(courthouse==2){demand_count(p,REQUIRES_COURTHOUSE);demand_count(p,REQUIRES_MORE_COURTHOUSE);}
    else if(courthouse==1)demand_count(p,REQUIRES_COURTHOUSE);
    value=h->health;
    if(value<health)return demand_unmet(p,health>=2?MORE_HEALTH:MISSING_HEALTH);
    if(health>0)demand_count(p,REQUIRES_HEALTH);
    if((int32_t)food_varieties(c,h)<foods)return demand_unmet(p,MISSING_FOOD);
    if(h->pottery<pottery)return 0;
    if(beer&&h->beer<=0)return 0;
    if(h->linen<linen)return 0;
    if(h->jewelry<jewelry)return 0;
    if(jewelry>=2&&(int32_t)p->words[LUXURY_KINDS]<2)return demand_unmet(p,SECOND_LUXURY);
    return 1;
}

typedef struct PharaohHouseRequirements {
    int32_t entertainment,water,religion,education,dentist,courthouse,health,foods;
    int32_t pottery,linen,jewelry,beer;
} PharaohHouseRequirements;
static PharaohHouseRequirements house_requirements(const volatile PharaohHouseModel *m,
    volatile PharaohHouseEconomyRecord *h)
{
    PharaohHouseRequirements q;const uint8_t size=h->size;
    q.entertainment=m->entertainment;q.water=m->water;q.religion=m->religion;
    q.education=m->education;q.dentist=m->dentist;q.courthouse=m->courthouse;
    q.health=m->health;q.foods=m->food_types;
    q.pottery=goods_required(m->pottery,size);q.linen=goods_required(m->linen,size);
    q.jewelry=goods_required(m->jewelry,size);q.beer=goods_required(m->beer,size);
    return q;
}
static uint32_t house_unmet_reason(const PharaohHouseEconomyContext *c,
    volatile PharaohHouseEconomyRecord *h,const PharaohHouseRequirements *q,uint32_t foods,int next)
{
    const uint32_t offset=next?30u:0u;
    if(q->water==1&&!h->water&&!h->well)return 1u+offset;
    if(q->water==2&&!h->water)return 2u+offset;
    const int32_t entertainment=h->entertainment;
    if(entertainment<q->entertainment){
        if(!entertainment)return 3u+offset;
        return (q->entertainment<10?4u:q->entertainment<25?5u:q->entertainment<50?6u:q->entertainment<80?7u:8u)+offset;
    }
    if((int32_t)foods<q->foods&&q->foods>=1&&q->foods<=3)return (uint32_t)q->foods+8u+offset;
    if(h->education<q->education){
        if(q->education==1)return 14u+offset;
        if(q->education==2){if(h->school)return 15u+offset;if(h->library)return 16u+offset;}
        else if(q->education==3&&!next)return 17;
    }
    if(h->courthouse<q->courthouse)return 18u+offset;
    if(h->pottery<q->pottery)return 19u+offset;
    if(h->religion<q->religion&&q->religion>=1&&q->religion<=3)return (uint32_t)q->religion+19u+offset;
    if(h->dentist<q->dentist)return 23u+offset;
    if(h->health<q->health){
        if(q->health==1)return 24u+offset;
        return (h->physician?25u:26u)+offset;
    }
    if(h->beer<q->beer)return 29u+offset;
    if(h->linen<q->linen)return 27u+offset;
    if(h->jewelry<q->jewelry)return next?68u:72u;
    if(q->jewelry>1&&(int32_t)demand_player(c)->words[LUXURY_KINDS]<2)return next?74u:70u;
    return 0;
}

/* Whole00454190: store the reason BYTE; incidental EAX is not an API result. */
void pharaoh_house_update_evolve_text(const PharaohHouseEconomyContext *c,int32_t building)
{
    volatile PharaohHouseEconomyRecord *h=&c->buildings[building];
    const int32_t level=h->level,desire=h->desirability;
    const volatile PharaohHouseModel *m=&c->models[level];
    const int32_t mode=desire<=m->devolve_desirability?-1:desire>=m->evolve_desirability;
    PharaohHouseRequirements q=house_requirements(m,h);
    const uint32_t foods=food_varieties(c,h);uint32_t reason;
    if(mode==-1){h->evolve_text=0;return;}
    reason=house_unmet_reason(c,h,&q,foods,0);
    if(reason){h->evolve_text=(uint8_t)reason;return;}
    if(level>=19){h->evolve_text=60;return;}
    q=house_requirements(m+1,h);
    if(!mode){h->evolve_text=*c->negative_influence?62:30;return;}
    reason=house_unmet_reason(c,h,&q,foods,1);
    if(reason){h->evolve_text=(uint8_t)reason;return;}
    h->evolve_text=61;if(h->merge_state==1)h->evolve_text=64;
}

/* Whole existing SupplyGood/SupplyHouse algorithms from BuildingServiceEffects. */
static void market_supply_good(volatile PharaohHouseEconomyRecord *h,
    uint32_t quantity,
    volatile int16_t *destination,volatile int16_t *source,volatile int16_t *timer)
{
    if(h->size>1)quantity*=4u;
    if(!quantity)return;
    *timer=10;const int32_t available=*source;if(available<=0)return;
    const int32_t have=*destination;const uint32_t demand=quantity*2u-(uint32_t)have;
    if((int32_t)demand<=0)return;
    if((int32_t)demand>available){*destination=(int16_t)((uint32_t)have+(uint32_t)available);*source=0;}
    else{*destination=(int16_t)((uint32_t)have+demand);*source=(int16_t)((uint32_t)*source-demand);}
}
uint32_t pharaoh_house_market_supply(const PharaohHouseEconomyContext *c,int32_t house,uint32_t market)
{
    if(house<=0)return 0;
    volatile PharaohHouseEconomyRecord *h=&c->buildings[house];
    if(!h->house_size)return 0;
    const int32_t population=h->population;if(population<=0)return 0;
    int32_t level=h->level;if(level<19)++level;
    const volatile PharaohHouseModel *model=&c->models[level];
    uint32_t wanted=(uint32_t)model->food_types;
    if((int32_t)wanted>0&&(int32_t)wanted<4)++wanted;
    const int32_t count=*c->food_count;
    volatile PharaohHouseEconomyRecord *m=&c->buildings[market];
    if(count>0)for(uint32_t i=0;i<(uint32_t)count;++i){
        const uint32_t resource=(uint32_t)c->foods[i];if(!resource)continue;
        const uint32_t slot=food_index(resource);
        const int32_t available=m->market_stock[slot+1u];if(available<=0||!wanted)continue;
        const int32_t have=(int16_t)food_stock(h,slot);
        uint32_t demand=(uint32_t)population*6u-(uint32_t)have;
        if((int32_t)demand>population*2)demand=(uint32_t)population*2u;
        if((int32_t)demand<=0)continue;
        if(slot<=7u){
            if(slot!=0u&&slot!=6u)demand>>=1;
            if(available>=(int32_t)demand){
                h->market_stock[slot]=(int16_t)((uint32_t)have+demand);
                m->market_stock[slot+1u]=(int16_t)((uint32_t)m->market_stock[slot+1u]-demand);
            }else{
                h->market_stock[slot]=(int16_t)((uint32_t)have+(uint32_t)available);
                m->market_stock[slot+1u]=0;
            }
        }
        --wanted;
    }
    market_supply_good(h,(uint32_t)model->beer,&h->beer,&m->market_stock[9],&m->beer_demand);
    market_supply_good(h,(uint32_t)model->pottery,&h->pottery,&m->market_stock[10],&m->pottery_demand);
    market_supply_good(h,(uint32_t)model->linen,&h->linen,&m->market_stock[11],&m->linen_demand);
    market_supply_good(h,(uint32_t)model->jewelry,&h->jewelry,&m->market_stock[12],&m->jewelry_demand);
    return 1;
}

/* Whole0045E360. The exhausted branch captures the city total before stores;
 * the partial branch publishes stock, total, then free granary capacity. */
int32_t pharaoh_granary_remove_resource(volatile PharaohHouseEconomyRecord *buildings,
    volatile PharaohGranaryResourceBank *players,int32_t player,int32_t building,
    int32_t resource,int32_t amount)
{
    if(amount<=0)return 0;
    volatile PharaohHouseEconomyRecord *b=&buildings[building];
    const uint32_t offset=0x56u+(uint32_t)resource*2u;
    volatile int16_t *at=(volatile int16_t *)((volatile uint8_t *)b+offset);
    const int32_t stock=*at;
    volatile uint32_t *total=&players[player].words[resource];
    if(stock<=amount){
        const uint32_t remaining=(uint32_t)amount-(uint32_t)stock;
        const uint32_t value=*total-(uint32_t)stock;
        b->market_stock[1]=(int16_t)((uint32_t)b->market_stock[1]+(uint32_t)stock);
        *at=0;*total=value;return (int32_t)remaining;
    }
    *at=(int16_t)((uint32_t)stock-(uint32_t)amount);
    *total=*total-(uint32_t)amount;
    b->market_stock[1]=(int16_t)((uint32_t)b->market_stock[1]+(uint32_t)amount);
    return 0;
}
static int32_t industry_input_capacity(int16_t type)
{
    switch(type){
    case 32:case 51:return 300;
    case 47:case 53:return 500;
    case 84:case 85:case 86:case 87:case 184:case 187:case 188:case 189:return 100000;
    default:return 200;
    }
}
/* Whole00460890. Native returns/adds excess rather than available space. */
int32_t pharaoh_building_recruiter_add_input(volatile PharaohHouseEconomyRecord *buildings,
    int32_t building,int32_t amount,int32_t resource)
{
    if(building<1)return 1;
    volatile PharaohHouseEconomyRecord *b=&buildings[building];
    const int16_t type=b->type;
    const int32_t limit=industry_input_capacity(type);
    volatile int16_t *at=resource==10?&b->primary_input:&b->secondary_input;
    volatile int16_t *orders=resource==10?&b->primary_orders:&b->secondary_orders;
    const int32_t stock=*at;if(stock>=limit)return 0;
    if((int32_t)((uint32_t)stock+(uint32_t)amount)>limit)
        amount=(int32_t)((uint32_t)amount+(uint32_t)stock-(uint32_t)limit);
    *at=(int16_t)((uint32_t)stock+(uint32_t)amount);
    *orders=(int16_t)((uint16_t)*orders-1u);
    return amount;
}

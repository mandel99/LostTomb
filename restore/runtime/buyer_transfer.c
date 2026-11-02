#include "actor_transport.h"
#include "actor_callback_496140.h"
#include "actor_create.h"
#include "placement_support.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

static uint32_t resource_for(uint32_t category)
{
    static const uint32_t resources[12]={1,2,3,4,5,6,7,8,15,13,17,19};
    return category<12u?resources[category]:0xffffffffu;
}
static uint32_t category_for(uint32_t resource)
{
    if(resource>=1u&&resource<=8u)return resource-1u;
    switch(resource){case 15:return 8;case 13:return 9;case 17:return 10;case 19:return 11;default:return 0xffffffffu;}
}
static uint32_t remaining(int32_t bazaar,int32_t category,int32_t committed,uint32_t limit)
{return limit-(uint32_t)H(0x799a2eu+(uint32_t)bazaar*264u+(uint32_t)category*2u)-(uint32_t)committed;}
static int32_t loads_for(uint32_t need,int32_t stock,int32_t maximum)
{
    int32_t capacity=I(need)/100+1,loads;
    if(stock>=maximum*100)loads=maximum;
    else if(stock>=100)loads=stock/100;
    else loads=stock>0;
    if(loads>capacity)loads=capacity;
    return loads>0?loads:0;
}

/* The goods calculator skips availability when demand is nonpositive;
   warehouse food queries availability before reading demand instead. */
int32_t FUN_004963b0(int32_t resource,int32_t actor,int32_t bazaar,int32_t source,int32_t committed,int32_t category)
{
    uint32_t need=remaining(bazaar,category,committed,400);(void)actor;
    if(I(need)<=0)return 0;
    return loads_for(need,FUN_0045d510(source,resource),4);
}
int32_t FUN_00496460(int32_t actor,int32_t resource,int32_t bazaar,int32_t source,int32_t committed,int32_t category)
{
    int32_t stock=FUN_0045d510(source,resource);
    uint32_t need=remaining(bazaar,category,committed,category==0?800u:600u);(void)actor;
    if(I(need)<=0)return 0;
    return loads_for(need,stock,8);
}
int32_t FUN_00496550(int32_t actor,int32_t resource,int32_t bazaar,int32_t source,int32_t committed,int32_t category)
{
    uint32_t need=remaining(bazaar,category,committed,category==0?800u:600u);(void)actor;
    if(I(need)<=0)return 0;
    return loads_for(need,H(0x799a2eu+(uint32_t)source*264u+(uint32_t)resource*2u),8);
}

/* Native unrolled creation has a four/eight actor ceiling and creates its
   first actor even for a nonpositive direct-call load count. Failed creation
   still publishes the same metadata to actor0; no successful result is faked.
   The buyer offset stays cached, while owner/coordinates/home reload at each
   actual creation boundary. Returned withdrawal amounts are unused. */
static int32_t send_carriers(int32_t resource,int32_t loads,int32_t buyer,int32_t bazaar,int32_t source,int32_t predecessor,int food)
{
    uint32_t category=category_for((uint32_t)resource),amount=(uint32_t)loads*100u;
    uint32_t owner,b,child,previous=(uint32_t)predecessor,i=0,limit=food?8u:4u,kind;
    int32_t x,y;
    (void)bazaar;
    if(food){
        kind=W(0x7999e8u+(uint32_t)source*264u);owner=U(0xeaac28u);
        if(kind==71u||kind==191u)(void)FUN_0045e360(I(owner),source,resource,I(amount));
        else (void)FUN_0045b570(I(owner),source,resource,I(amount));
    }else{owner=U(0xeaac28u);(void)FUN_0045b570(I(owner),source,resource,I(amount));}
    b=(uint32_t)buyer*388u;
    do{
        if(!i){owner=U(0xeaac28u);y=H(0xbcd60eu+b);x=H(0xbcd60cu+b);}
        else{x=H(0xbcd60cu+b);owner=U(0xeaac28u);y=H(0xbcd60eu+b);}
        child=(uint32_t)FUN_004b4800((uint8_t)owner,66,x,y,0);
        W(0xbcd660u+child*388u)=(uint16_t)previous;
        B(0xbcd672u+child*388u)=(uint8_t)category;
        W(0xbcd652u+child*388u)=W(0xbcd652u+b);
        previous=child;++i;
    }while(i<limit&&I(i)<loads);
    return 1;
}
int32_t FUN_00496660(int32_t resource,int32_t loads,int32_t buyer,int32_t bazaar,int32_t source,int32_t predecessor)
{return send_carriers(resource,loads,buyer,bazaar,source,predecessor,0);}
int32_t FUN_004968a0(int32_t resource,int32_t loads,int32_t buyer,int32_t bazaar,int32_t source,int32_t predecessor)
{return send_carriers(resource,loads,buyer,bazaar,source,predecessor,1);}

static int32_t buy(int32_t buyer,int32_t bazaar,int32_t source,int32_t category,int32_t committed,int32_t slot,int food)
{
    uint32_t resource=resource_for((uint32_t)category),owner=U(0xeaac28u)*0x49d8u,at,kind;
    int32_t loads,predecessor;
    if(U(0xea42f8u+owner+resource*4u))return 0;
    if(!food)loads=FUN_004963b0(I(resource),buyer,bazaar,source,committed,category);
    else{
        kind=W(0x7999e8u+(uint32_t)source*264u);
        if(kind==71u||kind==191u)loads=FUN_00496550(buyer,I(resource),bazaar,source,committed,category);
        else loads=FUN_00496460(buyer,I(resource),bazaar,source,committed,category);
    }
    if(loads<=0)return 0;
    predecessor=FUN_00496640(buyer);
    if(food)(void)FUN_004968a0(I(resource),loads,buyer,bazaar,source,predecessor);
    else (void)FUN_00496660(I(resource),loads,buyer,bazaar,source,predecessor);
    at=0xbcd6b4u+(uint32_t)buyer*388u+(uint32_t)slot*4u;
    U(at)=U(at)+(uint32_t)loads*100u;
    return 1;
}
int32_t FUN_00496c70(int32_t buyer,int32_t bazaar,int32_t source,int32_t category,int32_t committed,int32_t slot)
{return buy(buyer,bazaar,source,category,committed,slot,1);}
int32_t FUN_00496de0(int32_t buyer,int32_t bazaar,int32_t source,int32_t category,int32_t committed,int32_t slot)
{return buy(buyer,bazaar,source,category,committed,slot,0);}

uint32_t FUN_00495cf0(int32_t buyer,uint32_t bazaar,uint32_t source,int32_t slot)
{
    uint32_t at=(uint32_t)buyer*388u+(uint32_t)slot*4u;
    int32_t committed=I(U(0xbcd6b4u+at)),category=I(U(0xbcd694u+at));
    return (uint32_t)FUN_00496c70(buyer,I(bazaar),I(source),category,committed,slot);
}
uint32_t FUN_00496010(uint32_t buyer,uint32_t bazaar,uint32_t source,int32_t slot)
{
    uint32_t at=U(0xd056f8u)*388u+(uint32_t)slot*4u;
    int32_t committed=I(U(0xbcd6b4u+at)),category=I(U(0xbcd694u+at));
    return (uint32_t)FUN_00496de0(I(buyer),I(bazaar),I(source),category,committed,slot);
}

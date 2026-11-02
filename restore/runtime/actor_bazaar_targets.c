#include "actor_bazaar_targets.h"
#include "actor_transport.h"
#include "actor_callback_496140.h"
#include "menu_prepare_integer.h"
#include "menu_resource_neighbors.h"
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

static const uint32_t goods_index[4]={9,11,10,8};
static const uint32_t goods_resource[4]={13,19,17,15};

static int nearby(uint32_t source,uint32_t target)
{
    int32_t dx=H(0x7999deu+source)-H(0x7999deu+target);
    int32_t dy=H(0x7999e0u+source)-H(0x7999e0u+target);
    if(dx<0)dx=-dx;if(dy<0)dy=-dy;
    U(0xd3b4fcu)=(uint32_t)dx;U(0xd3b4f8u)=(uint32_t)dy;
    return (dx>dy?dx:dy)<40;
}

static void append(uint32_t index,uint32_t source_id,uint32_t source,int granary)
{
    uint32_t count=U(0x96e644u+index*4u),slot=(index*4000u+count)*4u;
    uint32_t y=(uint32_t)H(0x799a00u+source),x,tile;
    U(0x904ec4u+slot)=source_id;
    x=(uint32_t)H(0x7999feu+source);tile=U(0xbca2d8u)+x+y*228u;
    if(granary)U(0x96e644u+index*4u)=count+1u;
    U(0x89b744u+slot)=tile;
    if(!granary)U(0x96e644u+index*4u)=count+1u;
}

/* Complete456760, including native jump-table arms omitted by the original
   decompiler. The owner/profile byte and target record offset stay cached;
   availability callbacks can change the live stock, owner and storage limit. */
void FUN_00456760(int32_t actor,int32_t building)
{
    uint32_t target=(uint32_t)building*264u,profile=B(0x799a99u+target),store=1;
    uint32_t source,id,kind,resource,index,owner,count,selected,i,home;
    memset((void *)(uintptr_t)0x96e644u,0,27u*4u);
    memset((void *)(uintptr_t)0x89b744u,0,108000u*4u);
    memset((void *)(uintptr_t)0x904ec4u,0,108000u*4u);
    memset((void *)(uintptr_t)0x89b6d8u,0,27u*4u);
    while(I(store)<=I(U(0x78d788u))){
        if(!B(0x7900c0u+store*196u))goto next_store;
        id=(uint32_t)H(0x7900bcu+store*196u);if(!id)goto next_store;
        source=id*264u;if(B(0x7999d8u+source)!=1u)goto next_store;
        kind=W(0x7999e8u+source);
        if(kind==71u||kind==191u){
            if(U(0x784bbcu)||!B(0x799a1cu+source)||H(0x7999f8u+source)<=0||
               B(0x7999ecu+source)!=B(0x7999ecu+target)||!nearby(source,target))goto next_store;
            for(resource=1;resource<=8u;++resource){
                index=resource-1u;
                if(W(0x799a2eu+source+resource*2u)&&
                   B(0x7900c2u+profile*196u+resource)!=1u&&
                   H(0x799a2eu+target+index*2u)<20000)append(index,id,source,1);
            }
        }else if(kind==72u||kind==193u){
            if(!B(0x799a1cu+source)||H(0x7999f8u+source)<=0||!nearby(source,target))goto next_store;
            for(resource=1;resource<=8u;++resource){
                index=resource-1u;owner=U(0xeaac28u)*0x49d8u;
                if(U(0xea42f8u+owner+resource*4u)||FUN_0045d510(I(id),I(resource))<=0)continue;
                if(B(0x7900c2u+profile*196u+resource)==1u||H(0x799a2eu+target+index*2u)>=20000)continue;
                owner=U(0xeaac28u)*0x49d8u;kind=W(0xea4096u+owner+resource*2u);
                if(kind==1u||kind==3u)append(index,id,source,0);
            }
            for(i=0;i<4u;++i){
                index=goods_index[i];resource=goods_resource[i];owner=U(0xeaac28u)*0x49d8u;
                if(U(0xea42f8u+owner+resource*4u)||FUN_0045d510(I(id),I(resource))<=0)continue;
                if(B(0x7900c2u+profile*196u+resource)!=1u&&
                   H(0x799a2eu+target+index*2u)<20000)append(index,id,source,0);
            }
        }
next_store:
        ++store;
    }
    for(index=0;index<27u;++index){
        count=U(0x96e644u+index*4u);if(!count)continue;
        selected=(uint32_t)FUN_00519e30(H(0x7999feu+target),H(0x799a00u+target),
            I(0x89b744u+index*16000u),I(count));
        if(selected){
            id=U(0x904ec0u+(selected+index*4000u)*4u);
            U(0x89b6d8u+index*4u)=FUN_00475b50(id);
        }
    }
    home=(uint32_t)H(0xbcd652u+(uint32_t)actor*388u)*264u;
    for(i=0;i<4u;++i){
        if(H(0x799a76u+home+i*2u)>0)--W(0x799a76u+home+i*2u);
        else{
            index=goods_index[i];U(0x96e644u+index*4u)=0;U(0x89b6d8u+index*4u)=0;
        }
    }
}

uint32_t FUN_00495c10(int32_t actor,int32_t building)
{
    uint32_t a=(uint32_t)actor*388u,b=(uint32_t)building*264u;
    B(0xbcd630u+a)=8;W(0xbcd656u+a)=(uint16_t)building;B(0xbcd609u+a)=0;
    if(!FUN_00480180(H(0x7999deu+b),H(0x7999e0u+b),B(0x7999dbu+b))&&
       !FUN_00480180(H(0x7999deu+b),H(0x7999e0u+b),3)){
        B(0xbcd630u+a)=9;W(0xbcd61cu+a)=W(0xbcd624u+a);W(0xbcd61eu+a)=W(0xbcd626u+a);
    }else{W(0xbcd61cu+a)=W(0xd3b4e4u);W(0xbcd61eu+a)=W(0xd3b4e0u);}
    return 0;
}

void FUN_00495d30(int32_t actor)
{
    uint32_t a=(uint32_t)actor*388u;
    B(0xbcd630u+a)=9;W(0xbcd61cu+a)=W(0xbcd624u+a);W(0xbcd61eu+a)=W(0xbcd626u+a);
    FUN_004b3250(actor);
}

/* Preserve the full pointer-derived EAX; the native caller consumes AL. */
uint32_t FUN_00495d70(int32_t actor)
{
    uint32_t p=0xbcd6b4u+(uint32_t)actor*388u,i;
    for(i=0;i<4u;++i,p+=4u)if(U(p))return (p&0xffffff00u)|1u;
    return p&0xffffff00u;
}

void FUN_00495de0(int32_t actor)
{
    if((uint8_t)FUN_00495d70(actor))FUN_00495d30(actor);
    else B(0xbcd606u+(uint32_t)actor*388u)=2;
}

static void retry(int32_t actor,int32_t building,uint32_t target,int food)
{
    uint32_t a=(uint32_t)actor*388u,b=(uint32_t)building*264u,i,resource,value;
    int32_t demand[4],selected;
    if(B(0xbcd68bu+a)>=3u){FUN_00495de0(actor);return;}
    for(i=0;i<4u;++i){
        resource=U(0xbcd694u+a+i*4u);demand[i]=0;
        if(resource!=0xffffffffu){
            value=food?(resource==0u?800u:600u):400u;
            value-=(uint32_t)H(0x799a2eu+b+resource*2u);value-=U(0xbcd6b4u+a+i*4u);
            demand[i]=I(value);
        }
    }
    selected=FUN_00495da0(demand);FUN_00456760(actor,I(target));
    while(selected!=-1){
        resource=U(0xbcd694u+a+(uint32_t)selected*4u);
        if(I(U(0x89b6d8u+resource*4u))>0){
            (void)FUN_00495c10(actor,I(U(0x89b6d8u+resource*4u)));return;
        }
        demand[selected]=0;selected=FUN_00495da0(demand);
    }
    FUN_00495de0(actor);
}
void FUN_00495e10(int32_t actor,int32_t building,uint32_t target){retry(actor,building,target,0);}
void FUN_00495f00(int32_t actor,int32_t building,uint32_t target){retry(actor,building,target,1);}

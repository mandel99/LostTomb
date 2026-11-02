#include "remaining_resource_actions.h"
#include "map_validation.h"
#include "menu_dialog.h"
#include "numeric_input.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* Existing whole443AD0 is a genuine200-record char-pointer98-byte scan,
   using actual443040. Its original unspecified declaration is not used. */
extern uint32_t FUN_00443ad0(uint32_t kind);

/* Whole4074E0,61 instructions. Keep each native publication and sequential
   signed wrap test, including unusual lower>upper values and invalid actions. */
void restore_resource_action_004074e0(void)
{
    uint32_t action=U(0xd3b198u),lower=U(0xf56a0cu),upper=U(0xf56a14u),value;
    uint32_t city;
    switch(action){
    case 0:value=U(0x775924u)+1u;break;
    case 1:value=U(0x775924u)-1u;break;
    case 2:value=lower;break;
    case 3:value=lower+(uint32_t)((int32_t)(upper-lower)/2);break;
    case 4:value=upper;break;
    default:value=U(0x775924u);break;
    }
    U(0x775924u)=value;
    if((int32_t)value>(int32_t)upper){value=lower;U(0x775924u)=value;}
    if((int32_t)value<(int32_t)lower){value=upper;U(0x775924u)=value;}
    if(U(0x775928u)==2u){
        city=0x77f748u+U(0x77591cu)*98u;
        if(B(city+1u)){
            W(city+12u)=(uint16_t)value;
            W(city+14u)=(uint16_t)(value-lower);
        }
    }
    B(0xe38dcau)=1;
}
/* Whole40CE60,3 instructions. */
void restore_resource_action_0040ce60(void)
{B(0xe38ddau)=0x4e;B(0xe38dcau)=1;}

/* Whole40CE70,40 instructions. Fresh primary/coordinates after actualqueries;
   the numeric destination is a DWORD, not a BYTE or WORD. */
void restore_resource_action_0040ce70(void)
{
    uint32_t resource=U(0x5760bcu+U(0xd3b194u)*4u),primary,target,x,y;
    if(!FUN_00443a80(resource)&&!FUN_00443ad0(resource)){
        FUN_004264d0(46,0x51f780u,1);
        B(0xe38dcau)=1;
        return;
    }
    primary=U(0xd3b198u);
    y=U(0xe38da4u)+45u;
    target=0x5d2c48u+(primary+resource*2u)*4u;
    x=U(0xe38da8u)+45u;
    FUN_00403790(x,y,4,0,0,target,0);
    B(0xe38dcau)=1;
}
/* Whole40CEF0,10 instructions. Copy all36 native lookup entries and retain
   paired read/store order, including resource0 and repeated mapping values. */
void restore_resource_action_0040cef0(void)
{
    uint32_t cursor=0x5760bcu,resource,value;
    do{
        resource=U(cursor);cursor+=4u;
        value=U(0x5d2d68u+resource*8u);
        U(0x5d2c48u+resource*8u)=value;
        value=U(0x5d2d6cu+resource*8u);
        U(0x5d2c4cu+resource*8u)=value;
    }while((int32_t)cursor<0x57614c);
}
int restore_remaining_resource_action(uint32_t token)
{
    switch(token){
    case 0x4074e0u:restore_resource_action_004074e0();break;
    case 0x40ce60u:restore_resource_action_0040ce60();break;
    case 0x40ce70u:restore_resource_action_0040ce70();break;
    case 0x40cef0u:restore_resource_action_0040cef0();break;
    default:return 0;
    }
    return 1;
}

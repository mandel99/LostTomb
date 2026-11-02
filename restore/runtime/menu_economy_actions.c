#include "menu_economy_actions.h"
#include "editor_city_route_actions.h"
#include "message_panel_input.h"
#include "training_queries.h"
#include "financial_updates.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* Existing whole559C60/21 instructions: its three-target/21-byte native
   remap agrees with unit02632, including signed-negative and default cases. */
extern int32_t FUN_00559c60(uint32_t resource,int32_t value);

/* Menu projection and startup use the same complete typed tax provider. */
static void tax_projection(void)
{FUN_00459d20();}
/* Menu and calendar callers share the canonical complete finance bodies. */
static void finance_totals(void)
{FUN_0045a990();}
static void finance_allowance(void)
{FUN_0045a7b0();}
/* Whole5599C0/46 instructions. Preserve the native partial-AX/AL result
   as well as all WORD state writes and fresh player loads after queries. */
static uint32_t validate_trade_mode(uint32_t resource)
{
    uint32_t eax=U(0xeaac28u),index=eax*0x24ecu+resource,result;
    uint16_t state=W(0xea4096u+index*2u);
    if(state==1u||state==3u){
        result=FUN_00443950((int32_t)resource);
        if(!result){
            index=U(0xeaac28u)*0x24ecu+resource;
            W(0xea4096u+index*2u)=(uint16_t)result;
            return (result&0xffffff00u)|1u;
        }
        eax=U(0xeaac28u);
    }
    index=eax*0x24ecu+resource;
    eax=(index&0xffff0000u)|W(0xea4096u+index*2u);
    state=(uint16_t)eax;
    if(state==2u||state==4u){
        eax=FUN_00443b20((int32_t)resource);
        if(!eax){
            index=U(0xeaac28u)*0x24ecu+resource;
            W(0xea4096u+index*2u)=0;
            return (index&0xffffff00u)|1u;
        }
    }
    return eax&0xffffff00u;
}
/* Whole559860/9 instructions: dirty store precedes the BYTE toggle. */
static void trade_filter(void)
{
    uint32_t player=U(0xeaac28u),field;
    B(0xe38dcau)=1;field=0xea623cu+player*0x49d8u;
    B(field)=(uint8_t)(B(field)==0);
}
/* Whole559890/3 instructions, sharing the already reconstructed409FA0. */
static void trade_back(void)
{restore_editor_city_enter_00409fa0();B(0xe38dcau)=1;}
/* Whole559820/13 instructions. The source includes the leading count
   DWORD; keep the actual index expression instead of offsetting the list. */
static void trade_select(void)
{
    uint32_t player=U(0xeaac28u),action=U(0xd3b198u),index,resource;
    index=U(0xea60a4u+player*0x49d8u)+action;
    resource=U(0xf53e6cu+index*4u);U(0x60598cu)=resource;
    (void)validate_trade_mode(resource);B(0xe38ddau)=40;B(0xe38dcau)=1;
}
/* Whole40A8A0/89 instructions. The table encodes all twelve original
   state/action branches. Invalid actions/states still reset selection. */
static void trade_direction(void)
{
    static const uint32_t next[6][2]={{2,5},{4,3},{0,4},{5,1},{1,2},{3,0}};
    uint32_t state=U(0x605a40u);
    if(state<=5u){uint32_t action=U(0xd3b198u);if(action<=1u)U(0x605a40u)=next[state][action];}
    U(0x5f825cu)=UINT32_MAX;B(0xe38dcau)=1;
}
/* Whole40A260/33 instructions; original signed clamp0..25. */
static void tax_rate(void)
{
    uint32_t action=U(0xd3b198u),player=U(0xeaac28u),field=0xea1878u+player*0x49d8u,v;
    if(action==1u)U(field)=U(field)-1u;
    else if(action==0u)U(field)=U(field)+1u;
    v=U(field);
    if((int32_t)v<0)U(field)=0;else if((int32_t)v>25)U(field)=25;
    tax_projection();finance_totals();finance_allowance();B(0xe38dcau)=1;
}
/* Whole559DE0/7 instructions, genuine three-DWORD message service. */
static void trade_help(void){FUN_004c5bf0(46,1,0);B(0xe38dcau)=1;}
/* Whole559CD0/41 instructions. WORD wrap precedes signed scaling and
   the actual559C60 limit; this callback has no native dirty store. */
static void trade_limit(void)
{
    uint32_t action=U(0xd3b198u),player=U(0xeaac28u),resource=U(0x60598cu);
    uint32_t field=0xea40deu+(player*0x24ecu+resource)*2u;
    int32_t price;
    if(action==1u)W(field)=(uint16_t)(W(field)-1u);
    else if(action==0u)W(field)=(uint16_t)(W(field)+1u);
    price=(int32_t)(int16_t)W(field)*100;
    price=FUN_00559c60(resource,price);
    player=U(0xeaac28u);price/=100;resource=U(0x60598cu);
    W(0xea40deu+(player*0x24ecu+resource)*2u)=(uint16_t)price;
}
/* Whole40AF90/17 instructions, preserving its three fresh amount reads. */
static void donate(void)
{
    uint32_t o=U(0xeaac28u)*0x49d8u,amount=U(0xea5cccu+o),value=U(0xea187cu+o);
    U(0xea187cu+o)=value+amount;
    amount=U(0xea5cccu+o);value=U(0xea5cc8u+o);U(0xea5cc8u+o)=value+amount;
    amount=U(0xea5cccu+o);U(0xea5cc0u+o)=U(0xea5cc0u+o)-amount;
    finance_totals();finance_allowance();B(0xe38ddau)=7;B(0xe38dcau)=1;
}
/* Whole40AFF0/45 instructions, both original sequential signed clamps. */
static void donation_amount(void)
{
    uint32_t action=U(0xd3b198u),o=U(0xeaac28u)*0x49d8u,field=0xea5cccu+o;
    int32_t available,amount;
    switch(action){case 0:U(field)=0;break;case 1:U(field)=500;break;
    case 2:U(field)=2000;break;case 3:U(field)=5000;break;case 4:U(field)=1000000;break;
    default:break;}
    available=(int32_t)U(0xea5cc0u+o);amount=(int32_t)U(field);
    if(amount>available)U(field)=(uint32_t)available;
    if((int32_t)U(field)<0)U(field)=0;
    B(0xe38dcau)=1;
}
/* Whole40B420/40 instructions. Keep the early publication of increment,
   DWORD reserves, signed division100, and the actual difference bound. */
static void resource_amount_up(void)
{
    uint32_t player=U(0xeaac28u),o=player*0x49d8u,amount=U(0xea41f4u+o);
    uint32_t resource=U(0xea41ecu+o),floor,ceiling,stock;
    int32_t maximum;
    ++amount;floor=U(0x784e78u+resource*4u);U(0xea41f4u+o)=amount;
    ceiling=U(0x784de8u+resource*4u);
    stock=U(0xea6160u+(player*0x1276u+resource)*4u);
    maximum=(int32_t)(ceiling-floor);
    if((int32_t)stock/100<maximum)maximum=(int32_t)stock/100;
    U(0xea41f4u+o)=(int32_t)amount<maximum?amount:(uint32_t)maximum;
    B(0xe38dcau)=1;
}
/* Whole40B4B0/13 instructions; original BYTE gate and signed positive test. */
static void resource_amount_down(void)
{
    if(B(0xe38e03u)){
        uint32_t field=0xea41f4u+U(0xeaac28u)*0x49d8u,value=U(field);
        if((int32_t)value>0)U(field)=value-1u;
    }
    B(0xe38dcau)=1;
}
void restore_economy_refresh_00459d20(void){tax_projection();}
uint32_t restore_economy_trade_validate_005599c0(uint32_t resource){return validate_trade_mode(resource);}
void restore_economy_refresh_0045a990(void){finance_totals();}
void restore_economy_refresh_0045a7b0(void){finance_allowance();}

int restore_menu_economy_action(uint32_t token)
{
    switch(token){
    case 0x559860u:trade_filter();break;case 0x559890u:trade_back();break;
    case 0x559820u:trade_select();break;case 0x40a8a0u:trade_direction();break;
    case 0x40a260u:tax_rate();break;case 0x559de0u:trade_help();break;
    case 0x559cd0u:trade_limit();break;case 0x40af90u:donate();break;
    case 0x40aff0u:donation_amount();break;case 0x40b420u:resource_amount_up();break;
    case 0x40b4b0u:resource_amount_down();break;default:return 0;
    }
    return 1;
}

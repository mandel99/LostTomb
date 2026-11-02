#include "late_menu_actions.h"
#include "training_queries.h"
#include "town_trade.h"
#include "text_selection.h"
#include "menu_keyboard_modal.h"
#include "pyramid_summary.h"
#include "monument_complex_queries.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

static uint32_t planned_cost(int32_t kind,int32_t resource)
{
    switch(kind){
    case 1:case 13:return resource==24?4800u:resource==25?19200u:0u;
    case 2:case 14:return resource==24?21600u:resource==25?43200u:0u;
    case 3:return resource==12?4800u:resource==25?19200u:0u;
    case 4:return resource==12?21600u:resource==25?43200u:0u;
    case 5:return resource==12?67200u:resource==25?76800u:0u;
    case 6:return resource==12?144000u:resource==25?120000u:0u;
    case 7:return resource==12?264000u:resource==25?160000u:0u;
    case 8:return resource==24?4800u:0u;
    case 9:return resource==24?21600u:0u;
    case 10:return resource==24?67200u:0u;
    case 11:return resource==24?144000u:0u;
    case 12:return resource==24?264000u:0u;
    case 15:return resource==24?67200u:resource==25?76800u:0u;
    case 16:return resource==24?144000u:resource==25?120000u:0u;
    case 17:return resource==24?264000u:resource==25?160000u:0u;
    case 18:return resource==12?10800u:0u;
    case 19:return resource==12?24000u:0u;
    case 20:return resource==12?42000u:0u;
    case 22:return resource==26?10000u:0u;
    case 23:return resource==26?20000u:0u;
    case 24:return resource==30?38000u:0u;
    case 25:case 26:case 27:return resource==30?49600u:0u;
    default:return 0;
    }
}
/* Whole4F57B0,237 instructions; actual DWORD player, WORD planned kind and
   WORD building kind. All three native slots are processed with fresh player
   loads. Unsigned totals preserve native32-bit wrap, including summed stages.
   Outputs are not initialized here. Actual4E0530/542110 AL1 implies the worker
   remains nonzero; without external concurrent writes the following slot
   reload cannot take DF040/542AD0's early exit. Thus both genuine providers
   write all48/56 output bytes. Actual4F1310 itself initializes all112 bytes. */
int32_t restore_late_menu_004f57b0(int32_t resource)
{
    uint32_t total=0,slot;
    for(slot=0;slot<3u;++slot){
        uint32_t player=U(0xeaac28u);
        uint32_t building=U(0xea5facu+(player*0x1276u+slot)*4u);
        if(building&&B(0x7999d8u+building*264u)==1u){
            int32_t kind=(int16_t)W(0x7999e8u+building*264u);
            if(kind==0xde){
                int32_t output[12];
                (void)FUN_004df040(building,output);
                if(resource==30)total+=(uint32_t)output[7];
            }else if(kind==0xd5){
                int32_t output[14];
                (void)FUN_00542ad0(building,output);
                if(resource==30)total+=(uint32_t)output[11];
            }else if(kind==0xb7){
                int32_t output[28];uint32_t mode,first,second;
                (void)FUN_004f1310(building,output);
                mode=U(0x5e4ef4u);first=(uint32_t)output[8];second=(uint32_t)output[9];
                switch(mode){
                case 1:case 4:
                    if(resource==25){if((int32_t)second>0)total+=second*400u;}
                    else if(resource==24&&(int32_t)first>0)total+=first*400u;
                    break;
                case 2:
                    if(resource==25){if((int32_t)second>0)total+=second*400u;}
                    else if(resource==12&&(int32_t)first>0)total+=first*400u;
                    break;
                case 3:
                    if(resource==24&&((int32_t)first>0||(int32_t)second>0))total+=(first+second)*400u;
                    break;
                case 5:
                    if(resource==12&&((int32_t)first>0||(int32_t)second>0))total+=(first+second)*400u;
                    break;
                default:break;
                }
            }
        }else total+=planned_cost((int16_t)W(0x784de0u+slot*2u),resource);
    }
    return (int32_t)total;
}
int32_t FUN_004f57b0(int32_t resource)
{return restore_late_menu_004f57b0(resource);}

/* Whole53D730,57 instructions. The scan compares each BYTE count with the
   current candidate INDEX, exactly as native; this is not a maximum scan. */
void restore_late_menu_0053d730(void)
{
    static const uint8_t image[8]={8,13,2,1,11,5,1,10};
    uint32_t selected=U(0xeadce4u),actor=U(0xeadd24u+selected*4u)*388u;
    int32_t old=(int8_t)B(0xbcd675u+actor);
    uint8_t candidate=0,index;
    if(old!=9){
        B(0xf55ba4u+(uint32_t)old)=0;
        for(index=0;index<8u;++index)
            if(B(0xf55ba4u+index)>candidate)candidate=index;
        if(B(0xf55ba4u+candidate)>1u){
            B(0xf55ba4u+candidate)=0;
            B(0xbcd675u+actor)=candidate;
            U(0xeadcdcu)=candidate;
            if(candidate!=9u&&candidate!=8u){
                uint8_t next=image[candidate];
                B(0xe38dcau)=1;
                B(0xbcd729u+actor)=next;
                return;
            }
            B(0xbcd729u+actor)=13;
        }
    }
    B(0xe38dcau)=1;
}

/* Whole5598A0,17 instructions. Resource9 uses availability slot1 but keeps
   its own WORD toggle. Nonpositive signed counts leave the WORD untouched. */
void restore_late_menu_005598a0(void)
{
    uint32_t resource=U(0x60598cu),slot=resource==9u?1u:resource,field;
    if((int32_t)U(0x78d6dcu+slot*4u)<=0)return;
    field=0xea4126u+(U(0xeaac28u)*0x24ecu+resource)*2u;
    W(field)=(uint16_t)(W(field)==0);
}

static void trade_error(int32_t title_entry,int32_t body_entry)
{
    uint32_t body,title;
    FUN_004cc110(5,title_entry);
    FUN_004cc160(5,body_entry);
    body=U(0xe20764u);title=U(0xe20768u);
    (void)FUN_00538cd0(title,body,0);
}
static void refresh_price(void)
{
    uint32_t resource,player;int32_t price;
    FUN_00443330();
    resource=U(0x60598cu);
    /* Native signed magic multiply/SAR/correction is truncating division100. */
    price=(int32_t)U(0x775810u+resource*4u)/100;
    player=U(0xeaac28u);
    W(0xea40deu+(player*0x24ecu+resource)*2u)=(uint16_t)price;
}

/* Whole559A60,59 instructions, including all five PE jump-table entries. */
void restore_late_menu_00559a60(void)
{
    uint32_t field,state,player,resource;
    if(!FUN_004439c0((int32_t)U(0x60598cu))){trade_error(112,113);return;}
    if(!FUN_00443950((int32_t)U(0x60598cu))){trade_error(116,118);return;}
    player=U(0xeaac28u);resource=U(0x60598cu);
    field=0xea4096u+(player*0x24ecu+resource)*2u;
    state=(uint32_t)(int32_t)(int16_t)W(field);
    switch(state){
    case 0:case 2:case 4:W(field)=3;break;
    case 1:W(field)=0;break;
    case 3:W(field)=1;refresh_price();break;
    default:break;
    }
}
/* Whole559B50,68 instructions. Auto-trade DWORD is cleared only by the
   native0/1/3 cases; native2 and4 retain it. Neither controller adds dirty. */
void restore_late_menu_00559b50(void)
{
    uint32_t field,state,player,resource;
    if(!FUN_00443a20((int32_t)U(0x60598cu))){trade_error(114,115);return;}
    if(!FUN_00443b20((int32_t)U(0x60598cu))){trade_error(116,117);return;}
    player=U(0xeaac28u);resource=U(0x60598cu);
    field=0xea4096u+(player*0x24ecu+resource)*2u;
    state=(uint32_t)(int32_t)(int16_t)W(field);
    switch(state){
    case 0:case 1:case 3:
        U(0xea42f8u+(player*0x1276u+resource)*4u)=0;
        W(field)=4;break;
    case 2:W(field)=0;break;
    case 4:W(field)=2;refresh_price();break;
    default:break;
    }
}

/* Whole559D70,26 instructions. Keep the DWORD store before reloading mode. */
void restore_late_menu_00559d70(void)
{
    uint32_t player=U(0xeaac28u),resource=U(0x60598cu);
    uint32_t automatic=0xea42f8u+(player*0x1276u+resource)*4u,field;
    uint16_t state;
    if(U(automatic)==0){
        field=0xea4096u+(player*0x24ecu+resource)*2u;
        U(automatic)=1;
        state=W(field);
        if(state==2u||state==4u)W(field)=0;
    }else U(automatic)=0;
    B(0xe38dcau)=1;
}
/* Whole559E00,3 instructions. */
void restore_late_menu_00559e00(void)
{B(0xe38ddau)=7;B(0xe38dcau)=1;}

int restore_late_menu_action(uint32_t token)
{
    switch(token){
    case 0x53d730u:restore_late_menu_0053d730();break;
    case 0x5598a0u:restore_late_menu_005598a0();break;
    case 0x559a60u:restore_late_menu_00559a60();break;
    case 0x559b50u:restore_late_menu_00559b50();break;
    case 0x559d70u:restore_late_menu_00559d70();break;
    case 0x559e00u:restore_late_menu_00559e00();break;
    default:return 0;
    }
    return 1;
}

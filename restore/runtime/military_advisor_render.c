#include "military_advisor_render.h"
#include "actor_panel_actions.h"
#include "town_menu.h"
#include "order_input.h"
#include "menu_building_overlay.h"
#include "menu_training_render.h"
#include "menu_tail_ui.h"
#include "sprite_render.h"
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6160(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6860(int32_t,uint32_t,const char *,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004ccc30(uint32_t,uint32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004cd1a0(uint32_t,int16_t *,uint32_t,int32_t,int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
static int32_t x(uint32_t dx){return I(U(0xf53f0cu)+dx);}
static int32_t y(uint32_t dy){return I(U(0xf53f08u)+dy);}
static void label(uint32_t group,uint32_t item,uint32_t dx,uint32_t dy,uint32_t font)
{FUN_004d62f0(group,item,x(dx),y(dy),U(font),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void sprite(uint32_t image,uint32_t dx,uint32_t dy)
{(void)FUN_0053e590(I(image),x(dx),y(dy));}
static void box(uint32_t dx,uint32_t dy,uint32_t selected)
{FUN_004d63d0(0,(uint32_t)x(dx),(uint32_t)y(dy),30,30,selected,U(0xe38da8u),U(0xe38da4u));}
static void initialize(void)
{
    U(0xf53f0cu)=U(0xe38da8u);U(0xf53f08u)=U(0xe38da4u);
    U(0xf53f04u)=40;U(0xf53f00u)=26;
}
static void empire_message(uint32_t dy)
{
    uint32_t offset=U(0xeaac28u)*0x49d8u,item;
    if(U(0xea5eb4u+offset))item=11;
    else if(U(0xea5ea4u+offset))item=10;
    else item=FUN_00446610()?9u:8u;
    label(0x33,item,70,dy,0xf56f8cu);
}

/* Whole521B50: native event classification, real formation ordinals and
  144-byte records. All displayed values retain their actual field widths. */
void FUN_00521b50(void)
{
    uint32_t phase=5u-U(0xf53f48u),item,ordinal,row,group,offset,kind;
    uint8_t first,second,dispatched_land=0,dispatched_sea=0,waiting_land=0,waiting_sea=0;
    int eligible;RestoreTownRecord *record;
    initialize();
    if(I(phase)>=5)return;
    if(I(phase)>0){
        FUN_004cd680(I(U(0xe38da8u)+phase*16u),I(U(0xe38da4u)+phase*8u),
                     I((40u-phase*2u)*16u),I((26u-phase*2u)*16u),0);return;
    }
    FUN_00536820(I(U(0xe38da8u)),I(U(0xe38da4u)),40,26);
    sprite(U(0xf5681cu)+1u,10,10);label(0x33,0,60,12,0xf56fb4u);
    label(0x33,1,425,43,0xf56f8cu);label(0x33,2,425,58,0xf56f8cu);
    label(0x33,3,490,43,0xf56f8cu);label(0x33,4,490,58,0xf56f8cu);
    label(0x33,17,350,43,0xf56f8cu);label(0x33,18,330,58,0xf56f8cu);
    label(0x8a,36,250,58,0xf56f8cu);
    U(0xf53dd4u)=(uint32_t)FUN_004b78f0();
    restore_actor_panel_requests_00521a60(&first,&second);
    for(record=restore_town_trade_next(RESTORE_TOWNS,NULL);record;
        record=restore_town_trade_next(RESTORE_TOWNS,(const int16_t *)record)){
        uint32_t p=(uint32_t)(uintptr_t)record;
        if(H(p+4u)==37){
            int32_t city=H(p+28u),mode=H(p+48u);
            int sea=city>=0&&city<50&&B(0x7770f0u+(uint32_t)city*324u)==2;
            if(mode==5){if(sea)waiting_sea=1;else waiting_land=1;}
            if(mode==6||mode==7){if(sea)dispatched_sea=1;else dispatched_land=1;}
        }
    }
    if(I(U(0xf53dd4u))>0){
        sprite(U(0xf5690cu),50,349);U(0xe20778u)=0;
        FUN_004ccc30(8,46,B(0xea5edfu+U(0xeaac28u)*0x49d8u),x(70),y(348),U(0xf56f8cu),0);
        label(0x33,7,U(0xe20778u)+70u,348,0xf56f8cu);
        FUN_004ccc30(8,48,B(0xea5edcu+U(0xeaac28u)*0x49d8u),
                     x(U(0xe20778u)+70u),y(348),U(0xf56f8cu),0);
        sprite(U(0xf5690cu),50,369);empire_message(368);
    }else{sprite(U(0xf5690cu),50,359);empire_message(358);}
    sprite(U(0xf5690cu),50,389);
    item=dispatched_land?15u:dispatched_sea?28u:waiting_land?14u:waiting_sea?27u:first?13u:second?26u:12u;
    label(0x33,item,70,388,0xf56f8cu);
    FUN_004d65b0(x(32),y(70),36,17,I(U(0xe38da8u)),I(U(0xe38da4u)));
    if(I(U(0xf53dd4u))<=0){
        FUN_004d6160(0x33,16,x(64),y(200),496,U(0xf56f94u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    }else for(ordinal=1,row=0;I(ordinal-1u)<I(U(0xf53dd4u));++ordinal,row+=44u){
        group=(uint32_t)FUN_004b78b0(I(ordinal));eligible=FUN_0040b5e0(I(group))!=0;offset=group*144u;
        sprite(U(eligible?0xf56434u:0xf56814u)+B(0xbcb9cau+offset),48,row+82u);
        label(0x8a,B(0xbcb9cau+offset),100,row+83u,0xf56f9cu);
        U(0xe20778u)=0;
        FUN_004d6860(B(0xbcb9f0u+offset),0x40,(const char *)(uintptr_t)0x57c048u,
                     x(100),y(row+100u),U(0xf56f94u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
        kind=(uint32_t)H(0xbcb9ccu+offset);
        label(0x8a,kind==13u?33u:kind==11u?35u:34u,U(0xe20778u)+100u,row+100u,0xf56f94u);
        FUN_004d6250(0x8a,(uint32_t)(H(0xbcb9f4u+offset)/5)+37u,x(200),y(row+91u),150,
                     U(0xf56f94u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
        if(H(0xbcba4au+offset)<0){
            box(440,row+83u,0);sprite(U(0xf56834u),443,row+86u);
            box(500,row+83u,0);sprite(U(0xf56834u)+(B(0xbcb9cbu+offset)?2u:1u),503,row+86u);
            box(560,row+83u,0);
            if(!(eligible?second:first)){
                B(0xbcba2fu+offset)=0;sprite(U(0xf56834u)+5u,563,row+86u);
            }else sprite(U(0xf56834u)+(B(0xbcba2fu+offset)?3u:4u),563,row+86u);
        }else label(0x33,29,500,row+86u,0xf56f94u);
        box(360,row+83u,0);
        sprite(U(0xf5642cu)+(uint32_t)((H(0xbcba42u+offset)+10)/20),363,row+86u);
    }
    sprite(U(0xf5643cu)+1u,532,347);
}

/* Whole522A60: native post-query target reload, three hover slots per row,
   and the shared dispatch/recall buttons' actual five arguments. */
void FUN_00522a60(void)
{
    uint32_t ordinal,row,slot,group,offset;uint8_t first,second;int any=0,enabled;
    if(I(5u-U(0xf53f48u))>0)return;
    initialize();U(0xf53dd4u)=(uint32_t)FUN_004b78f0();
    restore_actor_panel_requests_00521a60(&first,&second);
    for(ordinal=1,row=0,slot=2;I(ordinal-1u)<I(U(0xf53dd4u));++ordinal,row+=44u,slot+=3u){
        group=(uint32_t)FUN_004b78b0(I(ordinal));offset=group*144u;
        if(H(0xbcba4au+offset)<0){
            box(440,row+83u,U(0x7056c8u)==slot-1u);
            box(500,row+83u,U(0x7056c8u)==slot);
            enabled=FUN_0040b5e0(I(group))!=0;
            enabled=H(0xbcba4au+offset)<0&&(enabled?second:first)!=0;
            box(560,row+83u,enabled&&U(0x7056c8u)==slot+1u);
            if(B(0xbcba2fu+offset))any=1;
        }
    }
    FUN_004cd1a0(0,(int16_t *)(uintptr_t)0x5c4ec8u,U(0x7056c8u)==19u,0,0);
    if(any){
        FUN_004cd1a0(0,(int16_t *)(uintptr_t)0x5c4ee4u,U(0x7056c8u)==20u,0,0);
        label(0x33,30,550,40,0xf56f8cu);label(0x33,31,550,54,0xf56f8cu);B(0x5f8518u)=1;
    }else{
        label(0x33,5,550,43,0xf56f8cu);label(0x33,6,550,58,0xf56f8cu);B(0x5f8518u)=0;
    }
}

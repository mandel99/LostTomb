#include "fleet_advisor_render.h"
#include "actor_panel_actions.h"
#include "town_menu.h"
#include "map_commit_links.h"
#include "menu_building_overlay.h"
#include "menu_training_render.h"
#include "menu_tail_ui.h"
#include "sprite_render.h"
extern void FUN_00536820(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004d62f0(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6250(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004d6160(uint32_t,uint32_t,int32_t,int32_t,uint32_t,uint32_t,uint32_t,int32_t,int32_t);
extern void FUN_004ccc30(uint32_t,uint32_t,int32_t,int32_t,int32_t,uint32_t,uint32_t);
extern void FUN_004cd1a0(uint32_t,int16_t *,uint32_t,int32_t,int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
static int32_t x(uint32_t dx){return I(U(0xf53f0cu)+dx);}
static int32_t y(uint32_t dy){return I(U(0xf53f08u)+dy);}
static void label(uint32_t item,uint32_t dx,uint32_t dy,uint32_t font)
{FUN_004d62f0(0x33,item,x(dx),y(dy),U(font),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void centered(uint32_t item,uint32_t dx,uint32_t dy)
{FUN_004d6250(0xb8,item,x(dx),y(dy),150,U(0xf56f94u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));}
static void sprite(uint32_t image,uint32_t dx,uint32_t dy)
{(void)FUN_0053e590(I(image),x(dx),y(dy));}
static void box(uint32_t dx,uint32_t dy,uint32_t selected)
{FUN_004d63d0(0,(uint32_t)x(dx),(uint32_t)y(dy),30,30,selected,U(0xe38da8u),U(0xe38da4u));}
static void initialize(void)
{U(0xf53f0cu)=U(0xe38da8u);U(0xf53f08u)=U(0xe38da4u);U(0xf53f04u)=40;U(0xf53f00u)=26;}
static void empire_message(uint32_t dy)
{
    uint32_t offset=U(0xeaac28u)*0x49d8u,item;
    if(U(0xea5eb4u+offset))item=11;
    else if(U(0xea5ea4u+offset))item=10;
    else item=FUN_00446610()?9u:8u;
    label(item,70,dy,0xf56f8cu);
}

/* Whole522D80. Native consumes AL only. The geometry query may leave its
   output coordinates unchanged; compare those genuine values, then restore
   both full DWORD scratch coordinates on every path. */
uint8_t FUN_00522d80(int32_t actor)
{
    uint32_t a=(uint32_t)actor*388u,old_y=U(0xd3b4e0u),old_x=U(0xd3b4e4u);
    uint32_t building=(uint32_t)H(0xbcd652u+a)*264u;
    uint8_t result;
    (void)FUN_004806c0(H(0x7999deu+building),H(0x7999e0u+building),B(0x7999dbu+building));
    result=(uint8_t)((uint32_t)H(0xbcd61cu+a)==U(0xd3b4e4u)&&
                     (uint32_t)H(0xbcd61eu+a)==U(0xd3b4e0u));
    U(0xd3b4e4u)=old_x;U(0xd3b4e0u)=old_y;return result;
}

/* Whole522E10: original summary priorities and ship rows. Crew/damage
   classification, ordinal selection and dock geometry use real providers. */
void FUN_00522e10(void)
{
    uint32_t phase=5u-U(0xf53f48u),item,ordinal,row,actor,offset;
    uint8_t first,second,dispatched_land=0,dispatched_sea=0,waiting_land=0,waiting_sea=0;
    RestoreTownRecord *record;
    initialize();
    if(I(phase)>=5)return;
    if(I(phase)>0){
        FUN_004cd680(I(U(0xe38da8u)+phase*16u),I(U(0xe38da4u)+phase*8u),
                     I((40u-phase*2u)*16u),I((26u-phase*2u)*16u),0);return;
    }
    FUN_00536820(I(U(0xe38da8u)),I(U(0xe38da4u)),40,26);
    sprite(U(0xf5681cu)+1u,10,10);label(19,60,12,0xf56fb4u);
    label(1,423,43,0xf56f8cu);label(20,423,58,0xf56f8cu);
    label(3,480,43,0xf56f8cu);label(21,480,58,0xf56f8cu);
    label(22,315,43,0xf56f8cu);label(23,300,58,0xf56f8cu);label(24,170,58,0xf56f8cu);
    U(0xf53dd0u)=B(0xea5ed8u+U(0xeaac28u)*0x49d8u);
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
    if(I(U(0xf53dd0u))>0){
        sprite(U(0xf5690cu),50,349);U(0xe20778u)=0;
        FUN_004ccc30(8,50,B(0xea5ed8u+U(0xeaac28u)*0x49d8u),x(70),y(348),U(0xf56f8cu),0);
        if(B(0xea5ed4u+U(0xeaac28u)*0x49d8u)){
            label(32,U(0xe20778u)+70u,348,0xf56f8cu);
            FUN_004ccc30(8,58,B(0xea5ed4u+U(0xeaac28u)*0x49d8u),
                         x(U(0xe20778u)+70u),y(348),U(0xf56f8cu),0);
        }
        sprite(U(0xf5690cu),50,369);empire_message(368);
    }else{sprite(U(0xf5690cu),50,359);empire_message(358);}
    sprite(U(0xf5690cu),50,389);
    item=dispatched_land?15u:dispatched_sea?28u:waiting_land?14u:waiting_sea?27u:first?13u:second?26u:12u;
    label(item,70,388,0xf56f8cu);
    FUN_004d65b0(x(32),y(70),36,17,I(U(0xe38da8u)),I(U(0xe38da4u)));
    if(I(U(0xf53dd0u))<=0){
        FUN_004d6160(0x33,25,x(64),y(200),496,U(0xf56f94u),0,I(U(0xe38da8u)),I(U(0xe38da4u)));
    }else for(ordinal=1,row=0;I(ordinal-1u)<I(U(0xf53dd0u));++ordinal,row+=44u){
        actor=(uint32_t)FUN_004bc620(I(ordinal));
        sprite(U(0xf56694u),48,row+82u);label(20,100,row+83u,0xf56f9cu);
        centered((uint32_t)FUN_0050fc70(I(actor)),140,row+91u);offset=actor*388u;
        if(H(0xbcd72au+offset)<0){
            box(440,row+83u,0);sprite(U(0xf56834u),443,row+86u);
            box(500,row+83u,0);item=FUN_00522d80(I(actor))?2u:1u;
            sprite(U(0xf56834u)+item,503,row+86u);box(560,row+83u,0);
            if(!second){B(0xbcd5fau+offset)=0;sprite(U(0xf56834u)+5u,563,row+86u);}
            else sprite(U(0xf56834u)+(B(0xbcd5fau+offset)?3u:4u),563,row+86u);
        }else label(29,500,row+86u,0xf56f94u);
        centered((uint32_t)FUN_0050fc10(I(actor)),273,row+91u);
    }
    sprite(U(0xf5643cu),532,347);
}

/* Whole523BB0: selection is a DWORD, request outputs and assignment flags
   are BYTEs. Preserve the assignment read between the first two boxes. */
void FUN_00523bb0(void)
{
    uint32_t ordinal,row,slot,actor,offset;uint8_t first,second;int any=0;
    if(I(5u-U(0xf53f48u))>0)return;
    initialize();U(0xf53dd0u)=B(0xea5ed8u+U(0xeaac28u)*0x49d8u);
    restore_actor_panel_requests_00521a60(&first,&second);
    for(ordinal=1,row=0,slot=2;I(ordinal-1u)<I(U(0xf53dd0u));++ordinal,row+=44u,slot+=3u){
        actor=(uint32_t)FUN_004bc620(I(ordinal));offset=actor*388u;
        if(H(0xbcd72au+offset)<0){
            box(440,row+83u,U(0x7056c8u)==slot-1u);
            if(B(0xbcd5fau+offset))any=1;
            box(500,row+83u,U(0x7056c8u)==slot);
            box(560,row+83u,second&&U(0x7056c8u)==slot+1u);
        }
    }
    FUN_004cd1a0(0,(int16_t *)(uintptr_t)0x5c50f8u,U(0x7056c8u)==19u,0,0);
    if(any){
        FUN_004cd1a0(0,(int16_t *)(uintptr_t)0x5c5114u,U(0x7056c8u)==20u,0,0);
        label(30,550,40,0xf56f8cu);label(31,550,54,0xf56f8cu);B(0x5f8518u)=1;
    }else{
        label(5,550,43,0xf56f8cu);label(6,550,58,0xf56f8cu);B(0x5f8518u)=0;
    }
}

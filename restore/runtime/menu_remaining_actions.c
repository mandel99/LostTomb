#include "menu_remaining_actions.h"
#include "map_file_load.h"
#include "numeric_input.h"
#include "menu_dialog.h"
#include "menu_tool_return.h"
#include "tool_selector.h"
#include "map_commit_connect.h"
#include "camera_navigation.h"
#include "mission_unlock.h"
#include "actor_panel_actions.h"
#include "building_controllers.h"
#include "diplomacy_input_actions.h"
#include "remaining_editor_list_actions.h"
#include "pyramid_summary.h"
#include "monument_complex_queries.h"
#include "monument_base_queries.h"
#include "monument_progress.h"
#include "campaign_score_numeric.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004ddec0(void);
extern uint32_t FUN_0047d350(void);
extern int32_t FUN_004b78b0(int32_t ordinal);
extern int32_t FUN_0040b5e0(int32_t group);
extern void FUN_004b6c20(int32_t group,int32_t order);

/* Whole4EA770/27 instructions. Actual unsupported actions still set the
   continuation flag and return to the city, without adding years. */
static void victory_continue(void)
{
    uint32_t player=U(0xeaac28u),action=U(0xd3b198u),o=player*0x49d8u;
    U(0xea5d40u+o)=1;
    if(action==1u||action==2u){
        uint32_t years=action==1u?24u:60u;
        U(0xea5d44u+o)=U(0xea5d44u+o)+years;
        U(0xea5d48u+o)=years;U(0xea450cu+o)=0;U(0xea4508u+o)=0;
        restore_diplomacy_0045a970();
    }
    B(0xe38dd8u)=0;B(0xe38dd9u)=0;B(0xe38dd7u)=0;
    B(0xe38ddau)=1;B(0xe38dcau)=1;
}
/* Whole409430/20 instructions; signed-WORD wrap in the original0..26 range. */
static void scenario_image(void)
{
    uint32_t action=U(0xd3b198u);int16_t image;
    if(action==1u)W(0x784b5eu)=(uint16_t)(W(0x784b5eu)-1u);
    else if(action==0u)W(0x784b5eu)=(uint16_t)(W(0x784b5eu)+1u);
    image=(int16_t)W(0x784b5eu);
    if(image<0)W(0x784b5eu)=26;else if(image>26)W(0x784b5eu)=0;
    B(0xe38dcau)=1;
}
/* Whole409CF0/5 instructions. */
static void editor_preview(void)
{uint32_t action=U(0xd3b198u);U(0x6059e8u)=0;U(0x605a38u)=action;B(0xe38dcau)=1;}
/* Whole409AC0, including all9 jump targets and27 remap bytes. Categories
   taking the popup branch return before the later drag-state resets. */
static void editor_tool(void)
{
    uint32_t action,submenu=UINT32_MAX,count;
    if(U(0xd3b198u)!=8u){(void)FUN_00405ba0();action=U(0xd3b198u);}
    else action=8;
    switch(action){
    case 8:submenu=14;break;case 2:submenu=27;break;case 14:submenu=15;break;
    case 13:submenu=17;break;case 32:submenu=37;break;case 7:submenu=16;break;
    case 17:submenu=18;break;case 20:submenu=19;break;case 24:submenu=26;break;
    case 5:submenu=0;break;default:break;
    }
    if(submenu!=UINT32_MAX){
        uint32_t x,y;
        B(0xe38ddau)=6;U(0x7056ecu)=submenu;count=(uint32_t)FUN_00404120();
        x=U(0x5cf63cu+count*8u);y=U(0x5cf640u+count*8u);
        U(0x7056f0u)=count;U(0x7056e8u)=x;U(0x7056e4u)=y;B(0xe38dcau)=1;
        return;
    }
    U(0x605a30u)=action;
    switch(action){
    case 0:U(0x6059e8u)=0x74;break;case 1:U(0x6059e8u)=0x75;break;
    case 3:U(0x6059e8u)=0x77;break;case 4:U(0x6059e8u)=0x78;break;
    case 6:U(0x6059e8u)=0x7a;break;case 10:U(0x6059e8u)=5;break;
    case 13:U(0x6059e8u)=0x7e;break;
    case 26:U(0x6059e8u)=0x86;break;default:break;
    }
    /* Action13's0x7E case exists in the remap but is dominated by the
       earlier popup return. Keeping the original native branch ordering
       keeps that remap arm unreachable here as in the executable. */
    U(0x6059e4u)=0;U(0x6059dcu)=0;U(0x6059e0u)=0;
    U(0x6059d4u)=0;U(0x6059d8u)=0;
    if(U(0x6059e8u)==5u)FUN_00517370();B(0xe38dcau)=1;
}
/* Whole409D10/10 instructions.47D350 is the genuine XOR EAX,EAX;RET leaf;
   its original pushed value is not a consumed parameter. */
static void sidebar_collapse(void)
{
    U(0xf53f7cu)=0;B(0xe38ddau)=2;FUN_004ddec0();FUN_004de080();
    (void)FUN_0047d350();FUN_0053b090(6);
}
/* Whole409D40/15 instructions. Preserve publication order, no extra redraw. */
static void overlay_menu(void)
{
    B(0xe38ddau)=8;U(0x7056f0u)=20;U(0x7056e8u)=40;U(0x7056e4u)=12;
    U(0x7056e0u)=0;U(0x7056dcu)=0;U(0x7056d8u)=0;U(0x7056d4u)=0;
    U(0x7056d0u)=40;U(0x7056ccu)=12;B(0xe38dcau)=1;
}
/* Whole409410/5 instructions: original DWORD read followed by a WORD store. */
static void scenario_era(void)
{uint32_t year=U(0x7848d8u);B(0xe38dcau)=1;W(0x7848d8u)=(uint16_t)(0u-year);}
/* Whole4093E0/11 instructions. */
static void scenario_year(void)
{FUN_00403790(240,180,2,0,0x7848d8u,0,0);B(0xe38dcau)=1;}
/* Whole448520/20 instructions. No native dirty store. */
static void event_list_arrow(void)
{
    uint32_t action=U(0xd3b198u),index,count;
    if(action==0u){index=U(0x78a620u);if((int32_t)index>0)U(0x78a620u)=index-1u;return;}
    if(action!=1u)return;
    count=U(0x785d70u);index=U(0x78a620u);
    if((int32_t)index<(int32_t)(count-1u))U(0x78a620u)=index+1u;
}
/* Whole408A00/24 and408A60/24 instructions. Each active98-byte city record
   edits a distinct BYTE at+60/+61; native ordering clears mode last. */
static void empire_city_byte(uint32_t displacement)
{
    uint32_t record=0x77f748u+U(0x77591cu)*98u,y;
    if(!B(record+1u))return;
    y=U(0xe38dacu);B(record+51u)=10;y-=314u;U(0x5d2a2cu)=0;
    FUN_00403790(453,y,1,record+displacement,0,0,0);B(0xe38dcau)=1;
}
/* Whole409530/3 instructions. */
static void empire_open(void){B(0xe38ddau)=20;B(0xe38dcau)=1;}

/* Whole40CD30/22 instructions. Zero phase means count all native entries;
   the original one-based search includes resources1..35 without a new cap. */
static uint32_t monument_resource(uint32_t ordinal)
{
    uint32_t index=1,count=0,p=0x784decu;
    if(!restore_remaining_editor_count_0040cc40(0))return 0;
    do{
        if((int32_t)U(p)>0)++count;
        if(count==ordinal)return index;
        p+=4u;++index;
    }while((int32_t)p<0x784e78);
    return 0;
}
/* Whole40AE00/3 instructions, genuinely only two stores and RET0. */
static void resource_dialog(void){B(0xe38ddau)=76;B(0xe38dcau)=1;}
/* Whole40CD70. Exact two-target/12-byte resource remap; the native signed
   difference and signed stock threshold are retained. */
static void monument_send_resource(void)
{
    uint32_t resource=monument_resource(U(0xd3b198u)+1u),floor,ceiling,unit=100,player,o;
    if(!resource)return;
    floor=U(0x784e78u+resource*4u);ceiling=U(0x784de8u+resource*4u);
    switch(resource){case 24:case 25:case 26:case 30:case 35:case 28:case 10:case 36:unit=1;break;default:break;}
    if((int32_t)(ceiling-floor)<=0){FUN_004264d0(32,0x51f780u,0);return;}
    player=U(0xeaac28u);
    if((int32_t)U(0xea6160u+(player*0x1276u+resource)*4u)<(int32_t)unit){
        FUN_004264d0(30,0x51f780u,1);return;
    }
    o=player*0x49d8u;U(0xea41ecu+o)=resource;U(0xea41f0u+o)=floor;U(0xea41f4u+o)=0;
    resource_dialog();B(0xe38dcau)=1;
}
/* Whole40CF20. All14 jump targets/54 remap bytes; real explicit member
   receivers and all112 bytes of pyramid output remain owned by true APIs. */
static void monument_go_to(void)
{
    uint32_t action=U(0xd3b198u),player=U(0xeaac28u);
    uint32_t building=U(0xea5facu+(player*0x1276u+action)*4u),o=building*264u;
    int32_t kind=(int16_t)W(0x7999e8u+o),progress;
    switch(kind){
    case 183:{int32_t output[28];(void)FUN_004f1310(building,output);progress=output[13];break;}
    case 207:case 208:progress=B(0x799a2du+o);break;
    case 210:progress=(int16_t)W(0xea5baau+player*0x49d8u)>=16?100:0;break;
    case 213:progress=FUN_00542ad0(building,0);break;
    case 222:progress=FUN_004df040(building,0);break;
    case 225:progress=restore_monument_base_0040d940((void *)(uintptr_t)0x6fd5d8u,building,0);break;
    case 227:progress=restore_monument_base_00417050((void *)(uintptr_t)0x6fd368u,building,0);break;
    case 228:progress=restore_monument_base_004e6710((void *)(uintptr_t)0x6de548u,building,0);break;
    case 229:progress=restore_monument_progress_00545740((void *)(uintptr_t)0x6f9320u,building,0);break;
    case 230:progress=FUN_00401540(building,0);break;
    case 234:progress=restore_monument_progress_00558be0((void *)(uintptr_t)0x6f5648u,building,0);break;
    case 235:progress=restore_monument_progress_00555e80((void *)(uintptr_t)0x6de9e0u,building,0);break;
    case 236:progress=restore_monument_progress_0054f470((void *)(uintptr_t)0x6e9188u,building,0);break;
    default:return;
    }
    if(progress>=100)return;
    FUN_00430cd0(U(0x7999e4u+o));B(0xe38ddau)=1;B(0xe38dcau)=1;
}
/* Whole40D1A0/3 and40ACE0/6 instructions. */
static void window_80(void){B(0xe38ddau)=80;B(0xe38dcau)=1;}
static void advisor_choice(void)
{uint32_t player=U(0xeaac28u),action=U(0xd3b198u);B(0xe38dcau)=1;U(0xea5ca0u+player*0x49d8u)=action;}
/* Whole40A2F0/25 instructions. The map uses actual stride228 and signed
   WORD tile coordinates, including original32-bit address wrap. */
static void military_go_to(void)
{
    uint32_t group=(uint32_t)FUN_004b78b0((int32_t)U(0xd3b198u)),o,x,y,tile;
    if(!group)return;o=group*144u;if((int16_t)W(0xbcba4au+o)>=0)return;
    y=(uint32_t)(int32_t)(int16_t)W(0xbcb9f8u+o);
    x=(uint32_t)(int32_t)(int16_t)W(0xbcb9f6u+o);tile=U(0xbca2d8u)+x+y*228u;
    FUN_00430cd0(tile);B(0xe38ddau)=1;B(0xe38dcau)=1;
}
/* Whole40A350/16 instructions. */
static void military_return(void)
{
    int32_t group=FUN_004b78b0((int32_t)U(0xd3b198u));
    if(!group||(int16_t)W(0xbcba4au+(uint32_t)group*144u)>=0)return;
    FUN_004b6c20(group,14);B(0xe38dcau)=1;
}
/* Whole40A390/62 instructions. Both BYTE locals are written by the real
   521A60 before use. Errors38/36/40 and the full32-bit eligibility result
   follow the original branch order; no port-only fallback is used. */
static void military_empire_service(void)
{
    uint32_t group=(uint32_t)FUN_004b78b0((int32_t)U(0xd3b198u)),o;
    int32_t eligible;uint8_t first,second;uint32_t error;
    if(!group)return;
    restore_actor_panel_requests_00521a60(&first,&second);
    eligible=FUN_0040b5e0((int32_t)group);o=group*144u;
    if((int16_t)W(0xbcba4au+o)>=0)return;
    if((eligible!=0&&second!=0)||(eligible==0&&first!=0)){
        B(0xbcba2fu+o)=(uint8_t)(B(0xbcba2fu+o)==0);
        FUN_004b7c80();B(0xe38dcau)=1;return;
    }
    error=first?38u:second?36u:40u;
    FUN_004264d0(error,0x51f780u,1);B(0xbcba2fu+o)=0;FUN_004b7c80();B(0xe38dcau)=1;
}
int restore_menu_remaining_action(uint32_t token)
{
    switch(token){
    case 0x4ea770u:victory_continue();break;case 0x409430u:scenario_image();break;
    case 0x409cf0u:editor_preview();break;case 0x409ac0u:editor_tool();break;
    case 0x409d10u:sidebar_collapse();break;case 0x409d40u:overlay_menu();break;
    case 0x409410u:scenario_era();break;case 0x4093e0u:scenario_year();break;
    case 0x448520u:event_list_arrow();break;case 0x408a00u:empire_city_byte(60);break;
    case 0x408a60u:empire_city_byte(61);break;case 0x409530u:empire_open();break;
    case 0x4099b0u:FUN_004099b0();break;case 0x40cd70u:monument_send_resource();break;
    case 0x40cf20u:monument_go_to();break;case 0x40d1a0u:window_80();break;
    case 0x40ace0u:advisor_choice();break;case 0x40a2f0u:military_go_to();break;
    case 0x40a350u:military_return();break;case 0x40a390u:military_empire_service();break;
    default:return 0;
    }
    return 1;
}

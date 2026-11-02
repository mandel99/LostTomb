#include "menu_message_actions.h"
#include "message_panel_input.h"
#include "sidebar_message_actions.h"
#include "event_publisher.h"
#include "camera_navigation.h"
#include "remaining_selection_actions.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole4E2360/7 instructions. */
static void message_ten(void){FUN_004c7f30();FUN_004c5bf0(10,0,0);}
/* Whole4E2380/29 instructions. Group lookup only replaces a strictly
   positive destination. Map bounds and signed-WORD class are native. */
static void message_go_to(void)
{
    uint32_t message=U(0xd3b124u),selected=U(0xe951e4u),event=selected*48u;
    uint16_t category=W(0xe209bau+message*80u);
    uint32_t tile=U(0xe9522cu+event);
    if(category==7u){
        uint32_t group=U(0xe95228u+event);
        uint32_t located=restore_sidebar_group_tile_004b80b0((int32_t)group);
        if((int32_t)located>0)tile=located;
    }
    if((int32_t)tile>0&&(int32_t)tile<0xcb10)FUN_00430cd0(tile);
    B(0xe38ddau)=1;B(0xe38dcau)=1;
}
/* Whole4E21B0/7 instructions. */
static void message_help(void){FUN_004c5bf0(34,0,0);B(0xe38dcau)=1;}
/* Whole4E2160/19 instructions: the down limit is count itself, not count-1. */
static void message_scroll(void)
{
    uint32_t action=U(0xd3b198u),index,count;
    if(action==0u){index=U(0xe95120u);if((int32_t)index>0)U(0xe95120u)=index-1u;}
    else if(action==1u){
        index=U(0xe95120u);count=U(0xe9511cu);
        if((int32_t)index<(int32_t)count)U(0xe95120u)=index+1u;
    }
    U(0xe95118u)=0;B(0xe38dcau)=1;
}
/* Whole4E21D0/42 instructions. Native saves ECX then replaces only the
   bottom BYTE of that scratch DWORD before calling4C5BF0. That callee
   consumes just the BYTE third argument, so unknown high bytes are dead. */
static void message_open(void)
{
    uint32_t selected=U(0xe95120u),count,event;
    uint32_t kind,first,second,message;
    uint8_t source,templated;
    selected+=U(0xd3b198u);count=U(0xe951ecu);
    U(0xe951e8u)=selected;U(0xe951e4u)=selected;
    if((int32_t)selected>=(int32_t)count)return;
    event=selected*48u;B(0xe95236u+event)=1;source=B(0xe95237u+event);
    kind=(uint32_t)(int32_t)(int16_t)W(0xe95230u+event);U(0xd3b178u)=kind;
    first=U(0xe95228u+event);U(0xd3b174u)=source;second=U(0xe9522cu+event);
    message=(uint32_t)(int32_t)(int16_t)W(0xe95232u+event);
    U(0xd3b170u)=first;U(0xd3b16cu)=second;U(0xe951dcu)=message;
    FUN_004e14b0(message);selected=U(0xe951e8u);
    templated=(uint8_t)((int16_t)W(0xe95238u+selected*48u)>=0);
    message=U(0xe951dcu);FUN_004c5bf0((int32_t)message,0,templated);
}
/* Whole4E2300/22 instructions. After sorting, decrement the old scroll
   when outside the new count, then perform the separate negative clamp. */
static void message_delete(void)
{
    uint32_t selected=U(0xe95120u),count,scroll;
    selected+=U(0xd3b198u);count=U(0xe951ecu);
    U(0xe951e8u)=selected;if((int32_t)selected>=(int32_t)count)return;
    W(0xe95232u+selected*48u)=0;restore_sidebar_message_sort_004e1640();
    scroll=U(0xe95120u);count=U(0xe9511cu);
    if((int32_t)scroll>=(int32_t)count){--scroll;U(0xe95120u)=scroll;}
    if((int32_t)scroll<0)U(0xe95120u)=0;
    B(0xe38dcau)=1;
}
/* Whole4E2290/31 instructions. The native exclusive endEA0D82, signed
   message IDs, class4 exception and distinct count-1 clamp are retained. */
static void message_delete_read(void)
{
    uint32_t cursor=0xe95232u,scroll,count;
    do{
        int16_t message=(int16_t)W(cursor);
        if(message!=0&&B(cursor+4u)!=0&&
           W(0xe209bau+(uint32_t)(int32_t)message*80u)!=4u)W(cursor)=0;
        cursor+=48u;
    }while((int32_t)cursor<0xea0d82);
    restore_sidebar_message_sort_004e1640();scroll=U(0xe95120u);count=U(0xe9511cu);
    if((int32_t)scroll>=(int32_t)count){scroll=count-1u;U(0xe95120u)=scroll;}
    if((int32_t)scroll<0)U(0xe95120u)=0;
    B(0xe38dcau)=1;
}
/* Whole409E00/14 instructions including its native tail JMP547A60.
   A zero menu value retains the previous overlay; the final refresh is
   shared with the genuine whole547A60 implementation, not duplicated. */
static void overlay_select(void)
{
    uint32_t group=U(0x7056dcu),action=U(0xd3b198u);
    uint32_t value=U(0x5cf75cu+(group*30u+action)*4u);
    if(value){U(0xe94e10u)=0;U(0xe94e0cu)=value;}
    B(0xe38ddau)=1;B(0xe38dcau)=1;restore_remaining_selection_00547a60();
}
int restore_menu_message_action(uint32_t token)
{
    switch(token){
    case 0x4e2360u:message_ten();break;case 0x4e2380u:message_go_to();break;
    case 0x4e21b0u:message_help();break;case 0x4e2160u:message_scroll();break;
    case 0x4e21d0u:message_open();break;case 0x4e2300u:message_delete();break;
    case 0x4e2290u:message_delete_read();break;case 0x409e00u:overlay_select();break;
    default:return 0;
    }
    return 1;
}

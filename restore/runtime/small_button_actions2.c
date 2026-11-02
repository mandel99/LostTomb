#include "small_button_actions2.h"
#include "message_panel.h"
#include "menu_timers.h"
#include "map_view_commit.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Complete40AAC0:7 instructions, message29/background1/templated0. */
void restore_small_button2_0040aac0(void)
{
    FUN_004c5bf0(29,1,0);
    B(0xe38dcau)=1;
}
/* Complete40AAE0:8 instructions. NEG/SBB AL/AND2/ADD7 depends on the
   ENTIRE secondary DWORD, not its low byte. Dirty is stored before mode. */
void restore_small_button2_0040aae0(void)
{
    uint32_t secondary=U(0xd3b194u);
    B(0xe38dcau)=1;
    B(0xe38ddau)=secondary?9:7;
}
/* Complete409510:6 instructions. No extra native dirty store after callee. */
void restore_small_button2_00409510(void)
{
    FUN_004c5bf0(32,1,0);
}
/* Complete418390:17 instructions. The genuine timer provider includes its
   actual clock call and native first/mode/duration store order. */
void restore_small_button2_00418390(void)
{
    if(U(0xe38ebcu)!=0)return;
    FUN_0041aa20(0,59,1000);
    U(0xd3b130u)=0;
    U(0xd3b138u)=0;
    U(0xd3a6e4u)=0;
    U(0xd3b128u)=0;
    B(0xe38dcau)=1;
}
/* Complete511C50:15 instructions, including both native signed branches. */
void restore_small_button2_00511c50(void)
{
    uint32_t message=U(0xeadd54u);
    FUN_004c5bf0((int32_t)message>0?(int32_t)message:10,0,0);
    B(0xe38dcau)=1;
}
/* Original409520 is a single JMP40A090. Its accepted canonical C already
   composes the genuine442800 record/camera update; reuse it without a copy. */
void restore_small_button2_00409520(void)
{
    FUN_00409520();
}
int restore_small_button_action2(uint32_t token)
{
    switch(token){
    case 0x40aac0u:restore_small_button2_0040aac0();break;
    case 0x40aae0u:restore_small_button2_0040aae0();break;
    case 0x409510u:restore_small_button2_00409510();break;
    case 0x418390u:restore_small_button2_00418390();break;
    case 0x511c50u:restore_small_button2_00511c50();break;
    case 0x409520u:restore_small_button2_00409520();break;
    default:return 0;
    }
    return 1;
}

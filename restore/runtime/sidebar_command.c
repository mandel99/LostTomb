#include "sidebar_command.h"
#include "tool_selector.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Complete403D70..403DF8:34 instructions,137 pinned PE bytes.
   Actual36-byte sidebar buttons publish their primary DWORD toD3B198.
   Tables5C7370/count16 and5C75B0/count12 both use rows0..11.on_left.
   This body also retains all other native DWORD inputs without clamping. */
void restore_sidebar_command_00403d70(void)
{
    uint8_t mode=B(0xe38ddau);
    uint32_t category,count,width,height;
    if(mode==6&&U(0x605a3cu)==1)return;
    B(0xe38ddau)=6;
    category=U(0xd3b198u);
    U(0x7056ecu)=category;
    if(category==1&&U(0x978d70u)==1)U(0x7056ecu)=13;
    count=(uint32_t)FUN_00404120();
    width=U(0x5cf63cu+count*8u);
    height=U(0x5cf640u+count*8u);
    U(0x7056f0u)=count;
    category=U(0x7056ecu);
    U(0x7056e8u)=width;
    U(0x7056e4u)=height;
    if(category==2||category==3||category==1){
        U(0xd3b198u)=1;
        FUN_00405200();
    }
    B(0xe38dcau)=1;
}
int restore_sidebar_command_action(uint32_t token)
{
    if(token!=0x403d70u)return 0;
    restore_sidebar_command_00403d70();return 1;
}

#include "message_panel.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern void FUN_004cd240(int,int,const void *,int,int,int,int);
extern void FUN_004cd1a0(int,const void *,int,int,int);
void FUN_004c5ce0(void)
{
    if(B(0xd3b186)&4) {
        FUN_004cd240(44,207,(const void *)0x5e02f8u,U(0xf56f8c),0,restore_message_record()->x+4,restore_message_record()->y+4);
        FUN_004cd1a0(1,(const void *)0x5e02f8u,U(0x7056c8)==3,restore_message_record()->x,restore_message_record()->y);
    }
    if(B(0xd3b186)&1) {
        FUN_004cd240(44,208,(const void *)0x5e02c0u,U(0xf56f8c),0,restore_message_record()->x+4,restore_message_record()->y+4);
        FUN_004cd1a0(1,(const void *)0x5e02c0u,U(0x7056c8)==1,restore_message_record()->x,restore_message_record()->y);
    }
    if(B(0xd3b186)&2) {
        FUN_004cd240(44,209,(const void *)0x5e02dcu,U(0xf56f8c),0,restore_message_record()->x+4,restore_message_record()->y+4);
        FUN_004cd1a0(1,(const void *)0x5e02dcu,U(0x7056c8)==2,restore_message_record()->x,restore_message_record()->y);
    }
}

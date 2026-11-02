#include "command_dialog_actions.h"
#include "pending_message.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole 41D3A0..41D3C5, ten instructions, no incoming args/ECX.
   Original button57C560.on_left is41D3A0. Native's two extra zero message
   slots are unused by the actual typed4E2970 provider. Tail41D3F0 is real C. */
void FUN_0041d3a0(void)
{
 if(B(0x7027e0u))FUN_004e2970(218);
 B(0x7027e0u)=0;
 FUN_0041d3f0();
}

/* Whole 41D3D0..41D3ED, six instructions, cdecl0/RET0. Read previous mode
   after the native DWORD clear, then save its zero-extended BYTE value. */
void FUN_0041d3d0(void)
{
 uint32_t previous;
 U(0xe20998u)=0;
 previous=B(0xe38ddau);
 B(0xe38ddau)=0x55;
 U(0x7027e8u)=previous;
}

/* Add this resolver to the central menu callback router during integration.
   Unknown tokens stay unmatched; only this authenticated button is bound. */
int restore_command_dialog_action(uint32_t token)
{
 if(token!=0x41d3a0u)return 0;
 FUN_0041d3a0();return 1;
}

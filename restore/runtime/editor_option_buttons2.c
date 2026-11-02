#include "editor_option_buttons2.h"
#include "numeric_input.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Native409020..40904E:12 instructions. Only enabling this option clears
   its sibling; disabling it preserves the other option's DWORD. */
void restore_editor_option2_00409020(void)
{
 uint32_t enabled=U(0x784c48u);
 if(enabled){U(0x784c48u)=0;B(0xe38dcau)=1;return;}
 U(0x784c50u)=0;
 U(0x784c48u)=1;
 B(0xe38dcau)=1;
}
/* Native409050..409076:11. Seven genuine DWORD arguments, RET0. */
void restore_editor_option2_00409050(void)
{
 FUN_00403790(300,200,4,0,0,0x784c4cu,0);
 B(0xe38dcau)=1;
}
/* Native409080..4090AE:12. Symmetric exclusive option, exact store order. */
void restore_editor_option2_00409080(void)
{
 uint32_t enabled=U(0x784c50u);
 if(enabled){U(0x784c50u)=0;B(0xe38dcau)=1;return;}
 U(0x784c48u)=0;
 U(0x784c50u)=1;
 B(0xe38dcau)=1;
}
/* Native4090B0..4090D6:11. */
void restore_editor_option2_004090b0(void)
{
 FUN_00403790(300,200,4,0,0,0x784c54u,0);
 B(0xe38dcau)=1;
}
/* Native4090E0..4090F9:7. This flag is a full DWORD, not a BYTE. */
void restore_editor_option2_004090e0(void)
{
 uint32_t enabled=U(0x784c58u);
 U(0x784c58u)=enabled==0u?1u:0u;
 B(0xe38dcau)=1;
}
/* Native409100..409126:11. */
void restore_editor_option2_00409100(void)
{
 FUN_00403790(300,200,4,0,0,0x784c5cu,0);
 B(0xe38dcau)=1;
}
/* Original 28-byte table5C63D8/count18 rows8..13, trigger3,
   actual4261D0 call4D9841. Only these reviewed tokens are resolved. */
int restore_editor_option2_action(uint32_t token)
{
 switch(token){
 case 0x409020u:restore_editor_option2_00409020();return 1;
 case 0x409050u:restore_editor_option2_00409050();return 1;
 case 0x409080u:restore_editor_option2_00409080();return 1;
 case 0x4090b0u:restore_editor_option2_004090b0();return 1;
 case 0x4090e0u:restore_editor_option2_004090e0();return 1;
 case 0x409100u:restore_editor_option2_00409100();return 1;
 default:return 0;
 }
}

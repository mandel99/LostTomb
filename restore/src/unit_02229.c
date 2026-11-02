#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_007999f6 (*(undefined4 *)0x007999f6u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f5684c (*(undefined *)0x00f5684cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
extern void FUN_004d62f0();
extern void FUN_004d6860();
extern int FUN_0053e590();

void FUN_004fc150(int param_1)

{
  short sVar1;
  undefined4 uVar2;
  
  uVar2 = DAT_00f56f94;
  DAT_00e20778 = 0;
  FUN_0053e590(DAT_00f5684c + 0xd,DAT_00eadd90 + 0x22,param_1 + 4);
  param_1 = param_1 + 0xe;
  FUN_004d6860((int)(short)(&DAT_007999f4)[DAT_00eadd6c * 0x84],0x40,&DAT_0057c048,
               DAT_00eadd90 + 0x32 + DAT_00e20778,param_1,uVar2,0,DAT_00eadd90,DAT_00eadd8c);
  FUN_004d62f0(0x7f,0x14,DAT_00eadd90 + 0x32 + DAT_00e20778,param_1,uVar2,0,DAT_00eadd90,
               DAT_00eadd8c);
  sVar1 = (&DAT_007999f6)[DAT_00eadd6c * 0x84];
  if (sVar1 < 0) {
    FUN_004d6860(-(int)sVar1,0x40,&DAT_0057c048,DAT_00eadd90 + 0x32 + DAT_00e20778,param_1,uVar2,0,
                 DAT_00eadd90,DAT_00eadd8c);
    FUN_004d62f0(0x7f,0x15,DAT_00eadd90 + 0x32 + DAT_00e20778,param_1,uVar2,0,DAT_00eadd90,
                 DAT_00eadd8c);
    return;
  }
  if (0 < sVar1) {
    FUN_004d62f0(0x7f,0x16,DAT_00eadd90 + 0x32 + DAT_00e20778,param_1,uVar2,0,DAT_00eadd90,
                 DAT_00eadd8c);
    FUN_004d6860((int)(short)(&DAT_007999f6)[DAT_00eadd6c * 0x84],0x40,&DAT_0057c048,
                 DAT_00eadd90 + 0x32 + DAT_00e20778,param_1,uVar2,0,DAT_00eadd90,DAT_00eadd8c);
  }
  return;
}

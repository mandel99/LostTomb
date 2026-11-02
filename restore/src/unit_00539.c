#include "restore_types.h"
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00799a26 (*(undefined4 *)0x00799a26u)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
extern void FUN_00435de0();
extern void FUN_004361a0();
extern int FUN_0046f1c0();

void FUN_00435d60(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0') {
    FUN_004361a0(((uint)(byte)(&DAT_00799a26)[param_1 * 0x108] * 0x14) / 100,param_2,param_3,0);
    uVar1 = FUN_0046f1c0(param_1);
    FUN_00435de0(uVar1,param_2,param_3);
  }
  return;
}

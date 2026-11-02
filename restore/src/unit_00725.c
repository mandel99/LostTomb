#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern void FUN_004695d0();
extern void FUN_0046d100();
extern void FUN_00475720();
extern void FUN_00476530();
extern void FUN_004798e0();
extern void FUN_0047b570();
extern void FUN_0048c0e0();
extern void FUN_00516f90();
extern void FUN_005173a0();
extern void FUN_005178e0();
extern void FUN_0053b090();

void FUN_0044bd50(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  iVar1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  iVar2 = (int)*(short *)(&DAT_00b4e6fc + iVar1 * 2);
  if (iVar2 != 0) {
    FUN_0046d100(iVar2,0);
    FUN_004695d0(iVar2,1);
    FUN_0053b090(0x11);
  }
  if (*(short *)(&DAT_00b4e6fc + iVar1 * 2) != 0) {
    (&DAT_007999d8)[*(short *)(&DAT_00b4e6fc + iVar1 * 2) * 0x108] = 5;
  }
  (&DAT_00af598c)[iVar1] = 0;
  FUN_004798e0(param_1,param_2,1);
  FUN_00476530();
  FUN_0047b570();
  FUN_00475720(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,1);
  FUN_00516f90(param_1 + -1,param_2 + -1,param_1 + 1,param_2 + 1,1);
  FUN_005173a0(param_1 + -1,param_2 + -1,param_1 + 1,param_2 + 1);
  FUN_005178e0();
  FUN_0048c0e0(param_1,param_2,1,1);
  return;
}

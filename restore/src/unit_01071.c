#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern void FUN_00477dd0();
extern int FUN_004781a0();

uint FUN_00478270(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  
  param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  uVar2 = DAT_00bca2d8;
  if (((&DAT_00af598c)[param_1] & 0x10000) == 0) {
    iVar1 = FUN_004781a0(param_1,0x10000);
    uVar2 = 0;
    if (iVar1 != 0) {
      do {
        uVar2 = DAT_00bca2d8;
        iVar1 = iVar1 + param_1;
        (&DAT_00af598c)[iVar1] = (&DAT_00af598c)[iVar1] & 0xfffeffff | 0x80;
        iVar3 = (int)(iVar1 - uVar2) % 0xe4;
        iVar1 = (int)(iVar1 - uVar2) / 0xe4;
        FUN_00478270(iVar3,iVar1);
        FUN_00477dd0(iVar3,iVar1,1);
        iVar1 = FUN_004781a0(param_1,0x10000);
      } while (iVar1 != 0);
      return 1;
    }
  }
  return uVar2 & 0xffffff00;
}

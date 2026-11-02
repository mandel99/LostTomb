#include "restore_types.h"
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
extern void FUN_00475cc0();
extern void FUN_00475dc0();
extern void FUN_004767d0();
extern void FUN_004770f0();
extern void FUN_00477430();
extern void FUN_00477500();
extern void FUN_004776f0();
extern void FUN_004777c0();
extern void FUN_0047b590();
extern void FUN_0047bd50();

void FUN_00477dd0(int param_1,int param_2,int param_3)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  
  uVar1 = (param_1 - param_3 < 0) - 1 & param_1 - param_3;
  iVar3 = DAT_00bca2d0;
  if (param_1 + param_3 <= DAT_00bca2d0) {
    iVar3 = param_1 + param_3;
  }
  uVar2 = (param_2 - param_3 < 0) - 1 & param_2 - param_3;
  iVar4 = DAT_00bca2d4;
  if (param_3 + param_2 <= DAT_00bca2d4) {
    iVar4 = param_3 + param_2;
  }
  FUN_00475cc0(uVar1,uVar2,iVar3,iVar4,1);
  FUN_004776f0(uVar1,uVar2,iVar3,iVar4);
  FUN_004770f0(uVar1,uVar2,iVar3,iVar4);
  FUN_00475dc0();
  FUN_00477430(uVar1,uVar2,iVar3,iVar4);
  FUN_004767d0(uVar1,uVar2,iVar3,iVar4,0,1);
  FUN_004777c0(uVar1,uVar2,iVar3,iVar4);
  FUN_0047b590(uVar1 - 1,uVar2 - 1,iVar3 + 1,iVar4 + 1);
  FUN_0047bd50(uVar1 - 1,uVar2 - 1,iVar3 + 1,iVar4 + 1);
  FUN_00477500(uVar1,uVar2,iVar3,iVar4);
  return;
}

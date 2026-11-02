#include "restore_types.h"
#define DAT_00702798 (*(undefined *)0x00702798u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a44 (*(undefined4 *)0x00799a44u)
#define DAT_00799a48 (*(undefined4 *)0x00799a48u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_0046a400();
extern undefined4 FUN_00485100();
extern void FUN_004ded50();
extern void FUN_004dfbb0();
extern int FUN_004f4f80();

void FUN_004df9d0(int param_1,int param_2,int param_3,char param_4)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 local_38;
  int local_34;
  int local_30;
  undefined1 local_2c;
  
  local_38 = 0;
  if (param_4 == '\0') {
    local_38 = FUN_004f4f80(param_3);
    uVar1 = DAT_00bca2e0;
  }
  else {
    uVar1 = (DAT_00702798 & 0xffff) >> 0xd;
  }
  FUN_004ded50(&local_34,0,DAT_00bca2e0,uVar1);
  iVar2 = param_3 * 0x108;
  *(short *)((int)&DAT_007999dc + iVar2 + 2) = (short)local_34 + (short)param_1;
  *(short *)(&DAT_007999e0 + iVar2) = (short)local_30 + (short)param_2;
  *(int *)(&DAT_007999e4 + iVar2) = DAT_00bca2d8 + (local_30 + param_2) * 0xe4 + local_34 + param_1;
  (&DAT_007999ea)[param_3 * 0x84] = 0;
  (&DAT_007999db)[iVar2] = local_2c;
  *(undefined2 *)(&DAT_00799a10 + iVar2) = 0;
  *(undefined4 *)(&DAT_00799a2c + param_3 * 0x84) = local_38;
  (&DAT_00799a30)[param_3 * 0x42] = 0;
  *(undefined4 *)(&DAT_00799a38 + iVar2) = 1;
  *(undefined4 *)(&DAT_00799a34 + iVar2) = 0;
  *(int *)(&DAT_00799a40 + param_3 * 0x84) = param_1;
  *(int *)(&DAT_00799a44 + param_3 * 0x84) = param_2;
  *(uint *)(&DAT_00799a48 + iVar2) = uVar1;
  FUN_004dfbb0(param_3);
  FUN_00485100(param_3);
  iVar2 = 1;
  do {
    FUN_004ded50(&local_34,iVar2,DAT_00bca2e0,uVar1);
    iVar3 = FUN_0046a400(DAT_00eaac28,0xde,local_34 + param_1,local_30 + param_2,0,param_3 + 1);
    iVar4 = iVar3 * 0x108;
    *(short *)(&DAT_00799a10 + iVar4) = (short)param_3;
    *(undefined2 *)(&DAT_00799a12 + iVar4) = 0;
    *(short *)(&DAT_00799a12 + param_3 * 0x108) = (short)iVar3;
    (&DAT_007999ea)[iVar3 * 0x84] = (short)iVar2;
    (&DAT_007999db)[iVar4] = local_2c;
    *(undefined4 *)(&DAT_00799a2c + iVar3 * 0x84) = local_38;
    (&DAT_00799a30)[iVar3 * 0x42] = 0;
    *(undefined4 *)(&DAT_00799a38 + iVar4) = 0;
    *(undefined4 *)(&DAT_00799a34 + iVar4) = 0;
    *(int *)(&DAT_00799a40 + iVar3 * 0x84) = param_1;
    *(int *)(&DAT_00799a44 + iVar3 * 0x84) = param_2;
    *(uint *)(&DAT_00799a48 + iVar4) = uVar1;
    FUN_004dfbb0(iVar3);
    FUN_00485100(iVar3);
    iVar2 = iVar2 + 1;
    param_3 = iVar3;
  } while (iVar2 < 0x74);
  return;
}

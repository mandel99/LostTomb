#include "restore_types.h"
#define DAT_00601dd8 (*(undefined *)0x00601dd8u)
#define DAT_00601ddc (*(undefined *)0x00601ddcu)
#define DAT_00601de0 (*(undefined *)0x00601de0u)
#define DAT_00601e10 (*(undefined4 *)0x00601e10u)
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
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_0046a400();
extern undefined4 FUN_00485100();
extern int FUN_004f4f80();
extern void FUN_00541920();

void FUN_005417b0(int param_1,int param_2,int param_3)

{
  int iVar1;
  undefined1 uVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  short sVar6;
  int *piVar7;
  
  uVar3 = FUN_004f4f80(param_3);
  iVar5 = DAT_00601ddc;
  iVar1 = DAT_00601dd8;
  iVar4 = param_3 * 0x108;
  *(short *)((int)&DAT_007999dc + iVar4 + 2) = (short)param_1 + (short)DAT_00601dd8;
  *(short *)(&DAT_007999e0 + iVar4) = (short)param_2 + (short)iVar5;
  uVar2 = DAT_00601de0;
  *(int *)(&DAT_007999e4 + iVar4) = DAT_00bca2d8 + (iVar5 + param_2) * 0xe4 + iVar1 + param_1;
  (&DAT_007999ea)[param_3 * 0x84] = 0;
  (&DAT_007999db)[iVar4] = uVar2;
  *(undefined2 *)(&DAT_00799a10 + iVar4) = 0;
  *(undefined4 *)(&DAT_00799a2c + param_3 * 0x84) = uVar3;
  (&DAT_00799a30)[param_3 * 0x42] = 0;
  *(undefined4 *)(&DAT_00799a38 + iVar4) = 1;
  *(undefined4 *)(&DAT_00799a34 + iVar4) = 0;
  FUN_00541920(param_3);
  FUN_00485100(param_3);
  sVar6 = 1;
  piVar7 = &DAT_00601e10;
  do {
    iVar5 = FUN_0046a400(DAT_00eaac28,0xd5,piVar7[-1] + param_1,*piVar7 + param_2,0,param_3 + 1);
    iVar4 = iVar5 * 0x108;
    *(short *)(&DAT_00799a10 + iVar4) = (short)param_3;
    *(short *)(&DAT_00799a12 + param_3 * 0x108) = (short)iVar5;
    iVar1 = piVar7[1];
    *(undefined2 *)(&DAT_00799a12 + iVar4) = 0;
    (&DAT_007999ea)[iVar5 * 0x84] = sVar6;
    (&DAT_007999db)[iVar4] = (char)iVar1;
    *(undefined4 *)(&DAT_00799a2c + iVar5 * 0x84) = uVar3;
    (&DAT_00799a30)[iVar5 * 0x42] = 0;
    *(undefined4 *)(&DAT_00799a38 + iVar4) = 0;
    *(undefined4 *)(&DAT_00799a34 + iVar4) = 0;
    FUN_00541920(iVar5);
    FUN_00485100(iVar5);
    piVar7 = piVar7 + 0xd;
    sVar6 = sVar6 + 1;
    param_3 = iVar5;
  } while ((int)piVar7 < 0x60322c);
  return;
}

#include "restore_types.h"
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
extern void FUN_004dea50();
extern int FUN_004f4f80();

void FUN_004e6840(int param_1,int param_2,int param_3,char param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *in_ECX;
  undefined2 unaff_BX;
  
  iVar1 = param_3;
  param_3 = 0;
  if (param_4 == '\0') {
    param_3 = FUN_004f4f80(iVar1);
  }
  iVar3 = in_ECX[0x13];
  iVar4 = in_ECX[0x14];
  iVar2 = iVar1 * 0x108;
  *(short *)((int)&DAT_007999dc + iVar2 + 2) = (short)param_1 + (short)iVar3;
  *(short *)(&DAT_007999e0 + iVar2) = (short)iVar4 + (short)param_2;
  *(int *)(&DAT_007999e4 + iVar2) = DAT_00bca2d8 + (iVar4 + param_2) * 0xe4 + iVar3 + param_1;
  (&DAT_007999ea)[iVar1 * 0x84] = 0;
  (&DAT_007999db)[iVar2] = (char)in_ECX[0x15];
  *(undefined2 *)(&DAT_00799a10 + iVar2) = 0;
  *(int *)(&DAT_00799a2c + iVar1 * 0x84) = param_3;
  (&DAT_00799a30)[iVar1 * 0x42] = 0;
  *(undefined4 *)(&DAT_00799a38 + iVar2) = 1;
  *(undefined4 *)(&DAT_00799a34 + iVar2) = 0;
  FUN_00485100(iVar1);
  (**(code **)(*in_ECX + 0x18))(iVar1);
  iVar3 = FUN_0046a400(DAT_00eaac28,0xe4,in_ECX[0x20] + param_1,in_ECX[0x21] + param_1,0,iVar1 + 1);
  iVar4 = iVar3 * 0x108;
  *(undefined2 *)(&DAT_00799a10 + iVar4) = unaff_BX;
  *(short *)(&DAT_00799a12 + iVar1) = (short)iVar3;
  *(undefined2 *)(&DAT_00799a12 + iVar4) = 0;
  (&DAT_007999ea)[iVar3 * 0x84] = (short)in_ECX[0x23];
  (&DAT_007999db)[iVar4] = (char)in_ECX[0x22];
  *(int *)(&DAT_00799a2c + iVar3 * 0x84) = param_2;
  (&DAT_00799a30)[iVar3 * 0x42] = 0;
  *(undefined4 *)(&DAT_00799a38 + iVar4) = 0;
  *(undefined4 *)(&DAT_00799a34 + iVar4) = 0;
  FUN_00485100(iVar3);
  (**(code **)(*in_ECX + 0x18))(iVar3);
  in_ECX[2] = 0;
  FUN_004dea50();
  return;
}

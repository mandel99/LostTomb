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
extern int FUN_004f4f80();

void FUN_00417180(int param_1,int param_2,int param_3,char param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *in_ECX;
  int *piVar4;
  int iVar5;
  int unaff_retaddr;
  
  iVar5 = param_3;
  param_3 = 0;
  if (param_4 == '\0') {
    param_3 = FUN_004f4f80(iVar5);
  }
  iVar2 = in_ECX[0x13];
  iVar3 = in_ECX[0x14];
  iVar1 = iVar5 * 0x108;
  *(short *)((int)&DAT_007999dc + iVar1 + 2) = (short)param_1 + (short)iVar2;
  *(short *)(&DAT_007999e0 + iVar1) = (short)param_2 + (short)iVar3;
  *(int *)(&DAT_007999e4 + iVar1) = DAT_00bca2d8 + (iVar3 + param_2) * 0xe4 + iVar2 + param_1;
  (&DAT_007999ea)[iVar5 * 0x84] = 0;
  (&DAT_007999db)[iVar1] = (char)in_ECX[0x15];
  *(undefined2 *)(&DAT_00799a10 + iVar1) = 0;
  *(int *)(&DAT_00799a2c + iVar5 * 0x84) = param_3;
  (&DAT_00799a30)[iVar5 * 0x42] = 0;
  *(undefined4 *)(&DAT_00799a38 + iVar1) = 1;
  *(undefined4 *)(&DAT_00799a34 + iVar1) = 0;
  (**(code **)(*in_ECX + 0x18))(iVar5);
  FUN_00485100(iVar5);
  in_ECX[2] = 0;
  param_3 = 1;
  piVar4 = in_ECX + 0x21;
  do {
    iVar2 = FUN_0046a400(DAT_00eaac28,0xe3,piVar4[-1] + unaff_retaddr,*piVar4 + param_1,0,iVar5 + 1)
    ;
    iVar3 = iVar2 * 0x108;
    *(short *)(&DAT_00799a10 + iVar3) = (short)iVar5;
    *(short *)(&DAT_00799a12 + iVar5 * 0x108) = (short)iVar2;
    *(undefined2 *)(&DAT_00799a12 + iVar3) = 0;
    (&DAT_007999ea)[iVar2 * 0x84] = (undefined2)param_3;
    (&DAT_007999db)[iVar3] = (char)piVar4[1];
    *(int *)(&DAT_00799a2c + iVar2 * 0x84) = param_2;
    (&DAT_00799a30)[iVar2 * 0x42] = 0;
    *(undefined4 *)(&DAT_00799a38 + iVar3) = 1;
    *(undefined4 *)(&DAT_00799a34 + iVar3) = 0;
    (**(code **)(*in_ECX + 0x18))(iVar2);
    FUN_00485100(iVar2);
    param_3 = param_3 + 1;
    piVar4 = piVar4 + 0xd;
    iVar5 = iVar2;
  } while (param_3 < 9);
  return;
}

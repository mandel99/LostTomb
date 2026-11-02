#include "restore_types.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a10 (*(undefined4 *)0x00799a10u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a39 (*(undefined4 *)0x00799a39u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_0046a400();
extern undefined4 FUN_00485100();
extern void FUN_004dea50();
extern int FUN_004f4f80();
#define stack0x00000000 (restore_stack[32768])

void FUN_004015a0(int param_1,int param_2,int param_3,int param_4)

{
  unsigned char restore_stack[65536];
  int iVar1;
  int *in_ECX;
  int unaff_EBX;
  int *piVar2;
  undefined2 unaff_BP;
  int unaff_ESI;
  int iVar3;
  int iVar4;
  undefined2 unaff_retaddr;
  int iStack_8;
  int local_4;
  
  iVar1 = param_3;
  local_4 = param_3;
  if ((char)param_4 == '\0') {
    FUN_004f4f80(param_3);
  }
  param_4 = in_ECX[0x13];
  param_3 = in_ECX[0x14];
  iVar3 = iVar1 * 0x108;
  *(short *)((int)&DAT_007999dc + iVar3 + 2) = (short)param_1 + (short)param_4;
  *(short *)(&DAT_007999e0 + iVar3) = (short)param_2 + (short)param_3;
  *(int *)(&DAT_007999e4 + iVar3) = DAT_00bca2d8 + (param_3 + param_2) * 0xe4 + param_4 + param_1;
  (&DAT_007999ea)[iVar1 * 0x84] = 0;
  (&DAT_007999db)[iVar3] = (char)in_ECX[0x15];
  *(undefined2 *)(&DAT_00799a10 + iVar3) = 0;
  *(undefined4 *)(&DAT_00799a2c + iVar1 * 0x84) = 0;
  *(undefined4 *)(&DAT_00799a34 + iVar3) = 0;
  (&DAT_00799a30)[iVar1 * 0x42] = 0;
  (**(code **)(*in_ECX + 0x20))(iVar1,&param_4,&param_3);
  (&DAT_007999fe)[iVar1 * 0x84] = (short)param_1;
  (&DAT_00799a00)[iVar1 * 0x84] = unaff_retaddr;
  *(undefined4 *)(&DAT_00799a39 + iVar3) = 0;
  *(undefined4 *)((int)&DAT_00799a3c + iVar3 + 1) = 0;
  *(undefined4 *)((int)&DAT_00799a40 + iVar3 + 1) = 0;
  (**(code **)(*in_ECX + 0x18))(iVar1);
  FUN_00485100(iVar1);
  in_ECX[2] = 0;
  iVar3 = 1;
  piVar2 = in_ECX + 0x21;
  do {
    local_4 = *piVar2;
    iVar1 = FUN_0046a400(DAT_00eaac28,0xe6,piVar2[-1] + unaff_EBX,local_4 + iStack_8,0,iVar1 + 1);
    iVar4 = iVar1 * 0x108;
    *(short *)(&DAT_00799a10 + iVar4) = (short)unaff_ESI;
    *(short *)(&DAT_00799a12 + unaff_ESI * 0x108) = (short)iVar1;
    *(undefined2 *)(&DAT_00799a12 + iVar4) = 0;
    (&DAT_007999ea)[iVar1 * 0x84] = (short)iVar3;
    (&DAT_007999db)[iVar4] = (char)piVar2[1];
    *(undefined4 *)(&DAT_00799a2c + iVar1 * 0x84) = 0;
    *(undefined4 *)(&DAT_00799a34 + iVar4) = 0;
    (&DAT_00799a30)[iVar1 * 0x42] = 0;
    (**(code **)(*in_ECX + 0x20))(iVar1,&stack0x00000000,&local_4);
    (&DAT_007999fe)[iVar1 * 0x84] = (short)unaff_EBX;
    (&DAT_00799a00)[iVar1 * 0x84] = unaff_BP;
    *(undefined4 *)(&DAT_00799a39 + iVar4) = 0;
    *(undefined4 *)((int)&DAT_00799a3c + iVar4 + 1) = 0;
    *(undefined4 *)((int)&DAT_00799a40 + iVar4 + 1) = 0;
    (**(code **)(*in_ECX + 0x18))(iVar1);
    FUN_00485100(iVar1);
    iVar3 = iVar3 + 1;
    piVar2 = piVar2 + 0xd;
    unaff_ESI = iVar1;
  } while (iVar3 < 0x10);
  FUN_004dea50();
  return;
}

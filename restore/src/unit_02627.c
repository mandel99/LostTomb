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
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a44 (*(undefined4 *)0x00799a44u)
#define DAT_00799a5c (*(undefined4 *)0x00799a5cu)
#define DAT_00799a68 (*(undefined4 *)0x00799a68u)
#define DAT_00799a6a (*(undefined4 *)0x00799a6au)
#define DAT_00799a6e (*(undefined4 *)0x00799a6eu)
#define DAT_00799a74 (*(undefined4 *)0x00799a74u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_0046a400();
extern undefined4 FUN_00485100();
extern void FUN_004dea50();
extern int FUN_004f4f80();
#define _param_4 restore_param_4

void FUN_00559130(int param_1,int param_2,int *param_3,unsigned int restore_param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int *in_ECX;
  int *piVar5;
  int iVar6;
  int iVar7;
  undefined4 local_1c;
  int *local_18;
  int local_10;
  int *local_c;
  
  iVar1 = (int)param_3;
  iVar7 = 0;
  local_1c = 0;
  if (RESTORE_SLICE1(restore_param_4,0) == '\0') {
    local_1c = FUN_004f4f80(param_3);
  }
  iVar6 = in_ECX[0x72f];
  iVar3 = in_ECX[0x730];
  *(undefined1 *)(in_ECX + 0x13) = 0;
  iVar2 = (int)param_3 * 0x108;
  in_ECX[0x15] = 1;
  _param_4 = (undefined1 *)0x6;
  *(short *)((int)&DAT_007999dc + iVar2 + 2) = (short)param_1 + (short)iVar6;
  *(short *)(&DAT_007999e0 + iVar2) = (short)param_2 + (short)iVar3;
  *(int *)(&DAT_007999e4 + iVar2) = DAT_00bca2d8 + (iVar3 + param_2) * 0xe4 + iVar6 + param_1;
  (&DAT_007999ea)[(int)param_3 * 0x84] = (short)in_ECX[0x732];
  (&DAT_007999db)[iVar2] = (char)in_ECX[0x731];
  *(undefined2 *)(&DAT_00799a10 + iVar2) = 0;
  *(undefined4 *)(&DAT_00799a2c + (int)param_3 * 0x84) = local_1c;
  (&DAT_00799a30)[(int)param_3 * 0x42] = 0;
  *(undefined4 *)(&DAT_00799a38 + iVar2) = 1;
  *(undefined4 *)(&DAT_00799a34 + iVar2) = 0;
  (&DAT_00799a74)[iVar2] = *(undefined1 *)((int)in_ECX + 0x1c1e);
  *(undefined4 *)(&DAT_00799a3c + (int)param_3 * 0x84) = 1;
  *(int *)(&DAT_00799a68 + iVar2) = in_ECX[0x16];
  *(int *)(&DAT_00799a40 + (int)param_3 * 0x84) = in_ECX[0xb4];
  *(char *)((int)&DAT_00799a6a + iVar2 + 2) = (char)in_ECX[0x6e0];
  *(undefined4 *)((int)&DAT_00799a6e + iVar2 + 2) = 0;
  piVar5 = (int *)(&DAT_00799a44 + (int)param_3 * 0x84);
  piVar4 = in_ECX + 0x152;
  do {
    iVar6 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar5 = iVar6;
    piVar5 = piVar5 + 1;
    _param_4 = (undefined1 *)((int)_param_4 + -1);
  } while (_param_4 != (undefined1 *)0x0);
  piVar5 = (int *)(&DAT_00799a5c + iVar2);
  piVar4 = in_ECX + 0x506;
  iVar6 = 3;
  do {
    iVar3 = *piVar4;
    piVar4 = piVar4 + 1;
    *piVar5 = iVar3;
    piVar5 = piVar5 + 1;
    iVar6 = iVar6 + -1;
  } while (iVar6 != 0);
  FUN_00485100(param_3);
  _param_4 = (undefined1 *)((int)in_ECX + 0x1b81);
  local_18 = in_ECX + 0xb5;
  local_c = in_ECX + 0x509;
  in_ECX[2] = 0;
  piVar5 = in_ECX + 0x73d;
  iVar6 = (int)param_3;
  param_3 = in_ECX + 0x158;
  do {
    iVar3 = FUN_0046a400(DAT_00eaac28,0xea,piVar5[-1] + param_1,*piVar5 + param_2,0,iVar6 + 1);
    iVar2 = iVar3 * 0x108;
    local_10 = 6;
    *(short *)(&DAT_00799a10 + iVar2) = (short)iVar6;
    *(short *)(&DAT_00799a12 + iVar6 * 0x108) = (short)iVar3;
    *(undefined2 *)(&DAT_00799a12 + iVar2) = 0;
    (&DAT_007999ea)[iVar3 * 0x84] = (short)piVar5[2];
    (&DAT_007999db)[iVar2] = (char)piVar5[1];
    *(undefined4 *)(&DAT_00799a2c + iVar3 * 0x84) = local_1c;
    (&DAT_00799a30)[iVar3 * 0x42] = 0;
    *(undefined4 *)(&DAT_00799a38 + iVar2) = 0;
    *(undefined4 *)(&DAT_00799a34 + iVar2) = 0;
    (&DAT_00799a74)[iVar2] = ((unsigned short *)restore_param_4)[0x9e];
    *(int *)(&DAT_00799a68 + iVar2) = local_18[-0x9e];
    *(undefined4 *)(&DAT_00799a3c + iVar3 * 0x84) = 0;
    *(int *)(&DAT_00799a40 + iVar3 * 0x84) = *local_18;
    *(undefined1 *)((int)&DAT_00799a6a + iVar2 + 2) = *(unsigned short *)(restore_param_4);
    *(undefined4 *)((int)&DAT_00799a6e + iVar2 + 2) = 0;
    piVar4 = (int *)(&DAT_00799a44 + iVar3 * 0x84);
    do {
      *piVar4 = *param_3;
      param_3 = param_3 + 1;
      local_10 = local_10 + -1;
      piVar4 = piVar4 + 1;
    } while (local_10 != 0);
    piVar4 = (int *)(&DAT_00799a5c + iVar2);
    local_10 = 3;
    do {
      iVar6 = *local_c;
      local_c = local_c + 1;
      *piVar4 = iVar6;
      piVar4 = piVar4 + 1;
      local_10 = local_10 + -1;
    } while (local_10 != 0);
    FUN_00485100(iVar3);
    local_18 = local_18 + 1;
    piVar5 = piVar5 + 0xd;
    _param_4 = _param_4 + 1;
    iVar6 = iVar3;
  } while ((int)(_param_4 + (-0x1b80 - (int)in_ECX)) < 0x9e);
  do {
    (**(code **)(*in_ECX + 0x18))(iVar7 + iVar1);
    iVar7 = iVar7 + 1;
  } while (iVar7 < 0x9e);
  FUN_004dea50();
  return;
}

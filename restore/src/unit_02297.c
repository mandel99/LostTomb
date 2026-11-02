#include "restore_types.h"
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00b284e8 (*(undefined4 *)0x00b284e8u)
#define DAT_00b285cb (*(undefined4 *)0x00b285cbu)
#define DAT_00b285cd (*(undefined4 *)0x00b285cdu)
#define DAT_00b286b0 (*(undefined4 *)0x00b286b0u)
#define DAT_00b67c38 (*(undefined4 *)0x00b67c38u)
#define DAT_00b67d1b (*(undefined4 *)0x00b67d1bu)
#define DAT_00b67d1d (*(undefined4 *)0x00b67d1du)
#define DAT_00b67e00 (*(undefined4 *)0x00b67e00u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eadfd4 (*(undefined *)0x00eadfd4u)
#define DAT_00eae004 (*(undefined *)0x00eae004u)
#define DAT_00eae008 (*(undefined *)0x00eae008u)
#define DAT_00eae00c (*(undefined *)0x00eae00cu)
#define DAT_00ee0b6c (*(undefined4 *)0x00ee0b6cu)
#define DAT_00ee0c4f (*(undefined4 *)0x00ee0c4fu)
#define DAT_00ee0c50 (*(undefined4 *)0x00ee0c50u)
#define DAT_00ee0d34 (*(undefined4 *)0x00ee0d34u)
#define DAT_00eed75c (*(undefined4 *)0x00eed75cu)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define DAT_00f203a0 (*(undefined *)0x00f203a0u)
extern void FUN_0040f8f0();

bool FUN_00517c20(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  byte bVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  
  FUN_0040f8f0(&DAT_00eed760,0x32c40,0);
  DAT_00eae004 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  DAT_00eadfd4 = DAT_00eadfd4 + 1;
  (&DAT_00eed760)[DAT_00eae004] = 1;
  DAT_00f203a0 = DAT_00eae004;
  iVar5 = 1;
  DAT_00eae00c = 0;
  DAT_00eae008 = 1;
  param_3 = DAT_00bca2d8 + param_4 * 0xe4 + param_3;
  do {
    iVar3 = (&DAT_00f203a0)[DAT_00eae00c];
    if (iVar3 == param_3) break;
    iVar1 = iVar3 + -0xe4;
    iVar6 = (&DAT_00eed760)[iVar3] + 1;
    if ((&DAT_00eed760)[iVar1] == 0) {
      if ((char)(&DAT_00ee0b6c)[iVar3] < '\0') {
        if (((&DAT_00af598c)[iVar1] & 0x4120) != 0) {
          uVar4 = *(uint *)(&DAT_00af55fc + iVar3 * 4);
          bVar2 = (&DAT_00b284e8)[iVar3];
          (&DAT_00b67c38)[iVar3] = (&DAT_00b67c38)[iVar3] | 0x40;
          *(uint *)(&DAT_00af55fc + iVar3 * 4) = uVar4 & 0x3052e80;
          (&DAT_00b284e8)[iVar3] = bVar2 & 0xf0;
          return false;
        }
      }
      else {
        (&DAT_00eed760)[iVar1] = iVar6;
        (&DAT_00f203a0)[DAT_00eae008] = iVar1;
        iVar5 = DAT_00eae008 + 1;
        DAT_00eae008 = iVar5;
        if (0xcb0f < iVar5) {
          iVar5 = 0;
          DAT_00eae008 = 0;
        }
      }
    }
    iVar1 = iVar3 + 1;
    if ((&DAT_00eed760)[iVar1] == 0) {
      if (*(char *)((int)&DAT_00ee0c50 + iVar3 + 1) < '\0') {
        if ((*(uint *)(&DAT_00af5990 + iVar3 * 4) & 0x4120) != 0) {
          uVar4 = *(uint *)(&DAT_00af5990 + iVar3 * 4);
          bVar2 = (&DAT_00b285cd)[iVar3];
          (&DAT_00b67d1d)[iVar3] = (&DAT_00b67d1d)[iVar3] | 0x40;
          *(uint *)(&DAT_00af5990 + iVar3 * 4) = uVar4 & 0x3052e80;
          (&DAT_00b285cd)[iVar3] = bVar2 & 0xf0;
          return false;
        }
      }
      else {
        (&DAT_00eed760)[iVar1] = iVar6;
        (&DAT_00f203a0)[DAT_00eae008] = iVar1;
        iVar5 = DAT_00eae008 + 1;
        DAT_00eae008 = iVar5;
        if (0xcb0f < iVar5) {
          iVar5 = 0;
          DAT_00eae008 = 0;
        }
      }
    }
    iVar1 = iVar3 + 0xe4;
    if ((&DAT_00eed760)[iVar1] == 0) {
      if ((char)(&DAT_00ee0d34)[iVar3] < '\0') {
        if ((*(uint *)(&DAT_00af5d1c + iVar3 * 4) & 0x4120) != 0) {
          uVar4 = *(uint *)(&DAT_00af5d1c + iVar3 * 4);
          bVar2 = (&DAT_00b286b0)[iVar3];
          (&DAT_00b67e00)[iVar3] = (&DAT_00b67e00)[iVar3] | 0x40;
          *(uint *)(&DAT_00af5d1c + iVar3 * 4) = uVar4 & 0x3052e80;
          (&DAT_00b286b0)[iVar3] = bVar2 & 0xf0;
          return false;
        }
      }
      else {
        (&DAT_00eed760)[iVar1] = iVar6;
        (&DAT_00f203a0)[DAT_00eae008] = iVar1;
        iVar5 = DAT_00eae008 + 1;
        DAT_00eae008 = iVar5;
        if (0xcb0f < iVar5) {
          iVar5 = 0;
          DAT_00eae008 = 0;
        }
      }
    }
    if (*(int *)(&DAT_00eed75c + iVar3 * 4) == 0) {
      if ((char)(&DAT_00ee0c4f)[iVar3] < '\0') {
        if ((*(uint *)(&DAT_00af5988 + iVar3 * 4) & 0x4120) != 0) {
          uVar4 = *(uint *)(&DAT_00af5988 + iVar3 * 4);
          bVar2 = (&DAT_00b285cb)[iVar3];
          (&DAT_00b67d1b)[iVar3] = (&DAT_00b67d1b)[iVar3] | 0x40;
          *(uint *)(&DAT_00af5988 + iVar3 * 4) = uVar4 & 0x3052e80;
          (&DAT_00b285cb)[iVar3] = bVar2 & 0xf0;
          return false;
        }
      }
      else {
        *(int *)(&DAT_00eed75c + iVar3 * 4) = iVar6;
        (&DAT_00f203a0)[DAT_00eae008] = iVar3 + -1;
        iVar5 = DAT_00eae008 + 1;
        DAT_00eae008 = iVar5;
        if (0xcb0f < iVar5) {
          iVar5 = 0;
          DAT_00eae008 = 0;
        }
      }
    }
    DAT_00eae00c = DAT_00eae00c + 1;
    if (0xcb0f < DAT_00eae00c) {
      DAT_00eae00c = 0;
    }
  } while (DAT_00eae00c != iVar5);
  return (&DAT_00eed760)[param_3] != 0;
}

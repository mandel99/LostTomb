#include "restore_types.h"
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eae004 (*(undefined *)0x00eae004u)
#define DAT_00eae008 (*(undefined *)0x00eae008u)
#define DAT_00eae00c (*(undefined *)0x00eae00cu)
#define DAT_00ed405b (*(undefined4 *)0x00ed405bu)
#define DAT_00ed405c (*(undefined4 *)0x00ed405cu)
#define DAT_00ed405d (*(undefined4 *)0x00ed405du)
#define DAT_00ed413f (*(undefined4 *)0x00ed413fu)
#define DAT_00ed4140 (*(undefined4 *)0x00ed4140u)
#define DAT_00ed4141 (*(undefined4 *)0x00ed4141u)
#define DAT_00ed4223 (*(undefined4 *)0x00ed4223u)
#define DAT_00ed4224 (*(undefined4 *)0x00ed4224u)
#define DAT_00ed4225 (*(undefined4 *)0x00ed4225u)
#define DAT_00eed75c (*(undefined4 *)0x00eed75cu)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define DAT_00f203a0 (*(undefined *)0x00f203a0u)
extern void FUN_0040f8f0();

undefined4 FUN_0051d160(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = 0;
  FUN_0040f8f0(&DAT_00eed760,0x32c40,0);
  DAT_00eae004 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  if ((&DAT_00ed4140)[DAT_00eae004] == -1) {
    return 0;
  }
  (&DAT_00eed760)[DAT_00eae004] = 1;
  DAT_00f203a0 = DAT_00eae004;
  iVar3 = 1;
  DAT_00eae00c = 0;
  DAT_00eae008 = 1;
  do {
    iVar5 = iVar5 + 1;
    if (50000 < iVar5) {
      return 1;
    }
    iVar2 = (&DAT_00f203a0)[DAT_00eae00c];
    iVar1 = iVar2 + -0xe4;
    iVar4 = (&DAT_00eed760)[iVar2] + 1;
    if (((&DAT_00eed760)[iVar1] == 0) && ((&DAT_00ed405c)[iVar2] != -1)) {
      (&DAT_00eed760)[iVar1] = iVar4;
      (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      iVar3 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar3;
      if (0xcb0f < iVar3) {
        iVar3 = 0;
        DAT_00eae008 = 0;
      }
    }
    iVar1 = iVar2 + 1;
    if (((&DAT_00eed760)[iVar1] == 0) && ((&DAT_00ed4141)[iVar2] != -1)) {
      (&DAT_00eed760)[iVar1] = iVar4;
      (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      iVar3 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar3;
      if (0xcb0f < iVar3) {
        iVar3 = 0;
        DAT_00eae008 = 0;
      }
    }
    iVar1 = iVar2 + 0xe4;
    if (((&DAT_00eed760)[iVar1] == 0) && ((&DAT_00ed4224)[iVar2] != -1)) {
      (&DAT_00eed760)[iVar1] = iVar4;
      (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      iVar3 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar3;
      if (0xcb0f < iVar3) {
        iVar3 = 0;
        DAT_00eae008 = 0;
      }
    }
    if ((*(int *)(&DAT_00eed75c + iVar2 * 4) == 0) && ((&DAT_00ed413f)[iVar2] != -1)) {
      *(int *)(&DAT_00eed75c + iVar2 * 4) = iVar4;
      (&DAT_00f203a0)[DAT_00eae008] = iVar2 + -1;
      iVar3 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar3;
      if (0xcb0f < iVar3) {
        iVar3 = 0;
        DAT_00eae008 = 0;
      }
    }
    iVar1 = iVar2 + -0xe3;
    if (((&DAT_00eed760)[iVar1] == 0) && ((&DAT_00ed405d)[iVar2] != -1)) {
      (&DAT_00eed760)[iVar1] = iVar4;
      (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      iVar3 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar3;
      if (0xcb0f < iVar3) {
        iVar3 = 0;
        DAT_00eae008 = 0;
      }
    }
    iVar1 = iVar2 + 0xe5;
    if (((&DAT_00eed760)[iVar1] == 0) && ((&DAT_00ed4225)[iVar2] != -1)) {
      (&DAT_00eed760)[iVar1] = iVar4;
      (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      iVar3 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar3;
      if (0xcb0f < iVar3) {
        iVar3 = 0;
        DAT_00eae008 = 0;
      }
    }
    iVar1 = iVar2 + 0xe3;
    if (((&DAT_00eed760)[iVar1] == 0) && ((&DAT_00ed4223)[iVar2] != -1)) {
      (&DAT_00eed760)[iVar1] = iVar4;
      (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      iVar3 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar3;
      if (0xcb0f < iVar3) {
        iVar3 = 0;
        DAT_00eae008 = 0;
      }
    }
    iVar1 = iVar2 + -0xe5;
    if (((&DAT_00eed760)[iVar1] == 0) && ((&DAT_00ed405b)[iVar2] != -1)) {
      (&DAT_00eed760)[iVar1] = iVar4;
      (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      iVar3 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar3;
      if (0xcb0f < iVar3) {
        iVar3 = 0;
        DAT_00eae008 = 0;
      }
    }
    DAT_00eae00c = DAT_00eae00c + 1;
    if (0xcb0f < DAT_00eae00c) {
      DAT_00eae00c = 0;
    }
  } while (DAT_00eae00c != iVar3);
  return 1;
}

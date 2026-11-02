#include "restore_types.h"
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
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

undefined4 FUN_0051c790(undefined4 param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  FUN_0040f8f0(&DAT_00eed760,0x32c40,0);
  DAT_00eae004 = DAT_00bca2d8 + param_3 * 0xe4 + param_2;
  (&DAT_00eed760)[DAT_00eae004] = 1;
  DAT_00f203a0 = DAT_00eae004;
  iVar3 = 1;
  DAT_00eae00c = 0;
  DAT_00eae008 = 1;
  do {
    iVar2 = (&DAT_00f203a0)[DAT_00eae00c];
    iVar1 = iVar2 + -0xe4;
    iVar4 = (&DAT_00eed760)[iVar2] + 1;
    if (((&DAT_00eed760)[iVar1] == 0) && ((&DAT_00ee0b6c)[iVar2] == '\x04')) {
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
    if (((&DAT_00eed760)[iVar1] == 0) && (*(char *)((int)&DAT_00ee0c50 + iVar2 + 1) == '\x04')) {
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
    if (((&DAT_00eed760)[iVar1] == 0) && ((&DAT_00ee0d34)[iVar2] == '\x04')) {
      (&DAT_00eed760)[iVar1] = iVar4;
      (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      iVar3 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar3;
      if (0xcb0f < iVar3) {
        iVar3 = 0;
        DAT_00eae008 = 0;
      }
    }
    if ((*(int *)(&DAT_00eed75c + iVar2 * 4) == 0) && ((&DAT_00ee0c4f)[iVar2] == '\x04')) {
      *(int *)(&DAT_00eed75c + iVar2 * 4) = iVar4;
      (&DAT_00f203a0)[DAT_00eae008] = iVar2 + -1;
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

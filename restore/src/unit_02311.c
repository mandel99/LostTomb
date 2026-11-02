#include "restore_types.h"
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

int FUN_00519e30(int param_1,int param_2,int param_3,int param_4)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  bool bVar7;
  
  FUN_0040f8f0(&DAT_00eed760,0x32c40,0);
  DAT_00eae004 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  DAT_00eadfd4 = DAT_00eadfd4 + 1;
  (&DAT_00eed760)[DAT_00eae004] = 1;
  iVar3 = 1;
  DAT_00f203a0 = DAT_00eae004;
  DAT_00eae00c = 0;
  DAT_00eae008 = 1;
  do {
    iVar2 = (&DAT_00f203a0)[DAT_00eae00c];
    if (param_4 != 0) {
      piVar5 = (int *)(param_3 + (param_4 + -1) * 4);
      iVar4 = param_4 + -1;
      do {
        if (*piVar5 == iVar2) {
          if (iVar4 < 0) {
            return 0;
          }
          return iVar4 + 1;
        }
        piVar5 = piVar5 + -1;
        bVar7 = iVar4 != 0;
        iVar4 = iVar4 + -1;
      } while (bVar7);
    }
    iVar4 = iVar2 + -0xe4;
    iVar6 = (&DAT_00eed760)[iVar2] + 1;
    if ((((&DAT_00eed760)[iVar4] == 0) && (-1 < (char)(&DAT_00ee0b6c)[iVar2])) &&
       ((char)(&DAT_00ee0b6c)[iVar2] < '\x03')) {
      (&DAT_00eed760)[iVar4] = iVar6;
      (&DAT_00f203a0)[DAT_00eae008] = iVar4;
      iVar3 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar3;
      if (0xcb0f < iVar3) {
        iVar3 = 0;
        DAT_00eae008 = 0;
      }
    }
    iVar4 = iVar2 + 1;
    if ((((&DAT_00eed760)[iVar4] == 0) &&
        (cVar1 = *(char *)((int)&DAT_00ee0c50 + iVar2 + 1), -1 < cVar1)) && (cVar1 < '\x03')) {
      (&DAT_00eed760)[iVar4] = iVar6;
      (&DAT_00f203a0)[DAT_00eae008] = iVar4;
      iVar3 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar3;
      if (0xcb0f < iVar3) {
        iVar3 = 0;
        DAT_00eae008 = 0;
      }
    }
    iVar4 = iVar2 + 0xe4;
    if ((((&DAT_00eed760)[iVar4] == 0) && (-1 < (char)(&DAT_00ee0d34)[iVar2])) &&
       ((char)(&DAT_00ee0d34)[iVar2] < '\x03')) {
      (&DAT_00eed760)[iVar4] = iVar6;
      (&DAT_00f203a0)[DAT_00eae008] = iVar4;
      iVar3 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar3;
      if (0xcb0f < iVar3) {
        iVar3 = 0;
        DAT_00eae008 = 0;
      }
    }
    if (((*(int *)(&DAT_00eed75c + iVar2 * 4) == 0) && (-1 < (char)(&DAT_00ee0c4f)[iVar2])) &&
       ((char)(&DAT_00ee0c4f)[iVar2] < '\x03')) {
      *(int *)(&DAT_00eed75c + iVar2 * 4) = iVar6;
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
  return 0;
}

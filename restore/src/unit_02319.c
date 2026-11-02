#include "restore_types.h"
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eadfd4 (*(undefined *)0x00eadfd4u)
#define DAT_00eae004 (*(undefined *)0x00eae004u)
#define DAT_00eae008 (*(undefined *)0x00eae008u)
#define DAT_00eae00c (*(undefined *)0x00eae00cu)
#define DAT_00ec754c (*(undefined4 *)0x00ec754cu)
#define DAT_00ec762f (*(undefined4 *)0x00ec762fu)
#define DAT_00ec7631 (*(undefined4 *)0x00ec7631u)
#define DAT_00ec7714 (*(undefined4 *)0x00ec7714u)
#define DAT_00eed75c (*(undefined4 *)0x00eed75cu)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define DAT_00f203a0 (*(undefined *)0x00f203a0u)
extern void FUN_0040f8f0();

int FUN_0051b220(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  bool bVar6;
  
  FUN_0040f8f0(&DAT_00eed760,0x32c40,0);
  DAT_00eae004 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  DAT_00eadfd4 = DAT_00eadfd4 + 1;
  (&DAT_00eed760)[DAT_00eae004] = 1;
  DAT_00f203a0 = DAT_00eae004;
  iVar2 = 1;
  DAT_00eae00c = 0;
  DAT_00eae008 = 1;
  do {
    iVar1 = (&DAT_00f203a0)[DAT_00eae00c];
    if (param_4 != 0) {
      piVar4 = (int *)(param_3 + (param_4 + -1) * 4);
      iVar3 = param_4 + -1;
      do {
        if (*piVar4 == iVar1) {
          if (iVar3 < 0) {
            return 0;
          }
          return iVar3 + 1;
        }
        piVar4 = piVar4 + -1;
        bVar6 = iVar3 != 0;
        iVar3 = iVar3 + -1;
      } while (bVar6);
    }
    iVar3 = iVar1 + -0xe4;
    iVar5 = (&DAT_00eed760)[iVar1] + 1;
    if (((&DAT_00eed760)[iVar3] == 0) && (-1 < (char)(&DAT_00ec754c)[iVar1])) {
      (&DAT_00eed760)[iVar3] = iVar5;
      (&DAT_00f203a0)[DAT_00eae008] = iVar3;
      iVar2 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar2;
      if (0xcb0f < iVar2) {
        iVar2 = 0;
        DAT_00eae008 = 0;
      }
    }
    iVar3 = iVar1 + 1;
    if (((&DAT_00eed760)[iVar3] == 0) && (-1 < (char)(&DAT_00ec7631)[iVar1])) {
      (&DAT_00eed760)[iVar3] = iVar5;
      (&DAT_00f203a0)[DAT_00eae008] = iVar3;
      iVar2 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar2;
      if (0xcb0f < iVar2) {
        iVar2 = 0;
        DAT_00eae008 = 0;
      }
    }
    iVar3 = iVar1 + 0xe4;
    if (((&DAT_00eed760)[iVar3] == 0) && (-1 < (char)(&DAT_00ec7714)[iVar1])) {
      (&DAT_00eed760)[iVar3] = iVar5;
      (&DAT_00f203a0)[DAT_00eae008] = iVar3;
      iVar2 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar2;
      if (0xcb0f < iVar2) {
        iVar2 = 0;
        DAT_00eae008 = 0;
      }
    }
    if ((*(int *)(&DAT_00eed75c + iVar1 * 4) == 0) && (-1 < (char)(&DAT_00ec762f)[iVar1])) {
      *(int *)(&DAT_00eed75c + iVar1 * 4) = iVar5;
      (&DAT_00f203a0)[DAT_00eae008] = iVar1 + -1;
      iVar2 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar2;
      if (0xcb0f < iVar2) {
        iVar2 = 0;
        DAT_00eae008 = 0;
      }
    }
    DAT_00eae00c = DAT_00eae00c + 1;
    if (0xcb0f < DAT_00eae00c) {
      DAT_00eae00c = 0;
    }
  } while (DAT_00eae00c != iVar2);
  return 0;
}

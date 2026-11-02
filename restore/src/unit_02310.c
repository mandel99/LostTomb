#include "restore_types.h"
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00b4e6fa (*(undefined4 *)0x00b4e6fau)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b4e6fe (*(undefined4 *)0x00b4e6feu)
#define DAT_00b4e8c4 (*(undefined4 *)0x00b4e8c4u)
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

bool FUN_00519be0(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  FUN_0040f8f0(&DAT_00eed760,0x32c40,0);
  DAT_00eae004 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  DAT_00eadfd4 = DAT_00eadfd4 + 1;
  (&DAT_00eed760)[DAT_00eae004] = 1;
  DAT_00f203a0 = DAT_00eae004;
  iVar4 = 1;
  DAT_00eae00c = 0;
  DAT_00eae008 = 1;
  param_3 = DAT_00bca2d8 + param_4 * 0xe4 + param_3;
  do {
    iVar3 = (&DAT_00f203a0)[DAT_00eae00c];
    if (iVar3 == param_3) break;
    iVar1 = iVar3 + -0xe4;
    iVar5 = (&DAT_00eed760)[iVar3] + 1;
    if (((&DAT_00eed760)[iVar1] == 0) &&
       ((-1 < (char)(&DAT_00ee0b6c)[iVar3] ||
        ((((&DAT_00ee0b6c)[iVar3] == -1 && (((&DAT_00af598c)[iVar1] & 0x10004) != 0)) &&
         (*(short *)(&DAT_00b4e6fc + iVar1 * 2) == 0)))))) {
      (&DAT_00eed760)[iVar1] = iVar5;
      (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      iVar4 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar4;
      if (0xcb0f < iVar4) {
        iVar4 = 0;
        DAT_00eae008 = 0;
      }
    }
    iVar1 = iVar3 + 1;
    if (((&DAT_00eed760)[iVar1] == 0) &&
       ((cVar2 = *(char *)((int)&DAT_00ee0c50 + iVar3 + 1), -1 < cVar2 ||
        (((cVar2 == -1 && ((*(uint *)(&DAT_00af5990 + iVar3 * 4) & 0x10004) != 0)) &&
         (*(short *)(&DAT_00b4e6fe + iVar3 * 2) == 0)))))) {
      (&DAT_00eed760)[iVar1] = iVar5;
      (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      iVar4 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar4;
      if (0xcb0f < iVar4) {
        iVar4 = 0;
        DAT_00eae008 = 0;
      }
    }
    iVar1 = iVar3 + 0xe4;
    if (((&DAT_00eed760)[iVar1] == 0) &&
       ((-1 < (char)(&DAT_00ee0d34)[iVar3] ||
        ((((&DAT_00ee0d34)[iVar3] == -1 && ((*(uint *)(&DAT_00af5d1c + iVar3 * 4) & 0x10004) != 0))
         && (*(short *)(&DAT_00b4e8c4 + iVar3 * 2) == 0)))))) {
      (&DAT_00eed760)[iVar1] = iVar5;
      (&DAT_00f203a0)[DAT_00eae008] = iVar1;
      iVar4 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar4;
      if (0xcb0f < iVar4) {
        iVar4 = 0;
        DAT_00eae008 = 0;
      }
    }
    if ((*(int *)(&DAT_00eed75c + iVar3 * 4) == 0) &&
       ((-1 < (char)(&DAT_00ee0c4f)[iVar3] ||
        ((((&DAT_00ee0c4f)[iVar3] == -1 && ((*(uint *)(&DAT_00af5988 + iVar3 * 4) & 0x10004) != 0))
         && (*(short *)(&DAT_00b4e6fa + iVar3 * 2) == 0)))))) {
      *(int *)(&DAT_00eed75c + iVar3 * 4) = iVar5;
      (&DAT_00f203a0)[DAT_00eae008] = iVar3 + -1;
      iVar4 = DAT_00eae008 + 1;
      DAT_00eae008 = iVar4;
      if (0xcb0f < iVar4) {
        iVar4 = 0;
        DAT_00eae008 = 0;
      }
    }
    DAT_00eae00c = DAT_00eae00c + 1;
    if (0xcb0f < DAT_00eae00c) {
      DAT_00eae00c = 0;
    }
  } while (DAT_00eae00c != iVar4);
  return (&DAT_00eed760)[param_3] != 0;
}

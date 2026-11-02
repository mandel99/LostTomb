#include "restore_types.h"
#define DAT_00799a3e (*(undefined4 *)0x00799a3eu)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a42 (*(undefined4 *)0x00799a42u)
#define DAT_00799a44 (*(undefined4 *)0x00799a44u)
#define DAT_00799a84 (*(undefined4 *)0x00799a84u)

undefined4 FUN_004565f0(int param_1,int param_2,int param_3)

{
  short sVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  
  iVar4 = 0;
  sVar1 = (&DAT_00799a3e)[param_1 * 0x84];
  if ((sVar1 == 0) && (*(int *)(param_2 + 0x20) != 0)) {
    *(undefined1 *)(&DAT_00799a84 + param_1 * 0x42) = 8;
    return *(undefined4 *)(param_3 + 0x20);
  }
  sVar2 = (&DAT_00799a40)[param_1 * 0x84];
  if ((sVar2 == 0) && (*(int *)(param_2 + 0x24) != 0)) {
    *(undefined1 *)(&DAT_00799a84 + param_1 * 0x42) = 9;
    return *(undefined4 *)(param_3 + 0x24);
  }
  if (((&DAT_00799a42)[param_1 * 0x84] == 0) && (*(int *)(param_2 + 0x28) != 0)) {
    *(undefined1 *)(&DAT_00799a84 + param_1 * 0x42) = 10;
    return *(undefined4 *)(param_3 + 0x28);
  }
  if (((&DAT_00799a44)[param_1 * 0x84] == 0) && (*(int *)(param_2 + 0x2c) != 0)) {
    *(undefined1 *)(&DAT_00799a84 + param_1 * 0x42) = 0xb;
    return *(undefined4 *)(param_3 + 0x2c);
  }
  sVar3 = 100;
  if ((*(int *)(param_2 + 0x20) != 0) && (sVar1 < 100)) {
    iVar4 = 8;
    sVar3 = sVar1;
  }
  if ((*(int *)(param_2 + 0x24) != 0) && (sVar2 < sVar3)) {
    iVar4 = 9;
    sVar3 = sVar2;
  }
  if ((*(int *)(param_2 + 0x28) != 0) && ((short)(&DAT_00799a42)[param_1 * 0x84] < sVar3)) {
    iVar4 = 10;
    sVar3 = (&DAT_00799a42)[param_1 * 0x84];
  }
  if ((*(int *)(param_2 + 0x2c) == 0) || (sVar3 <= (short)(&DAT_00799a44)[param_1 * 0x84])) {
    if (iVar4 == 8) {
      *(undefined1 *)(&DAT_00799a84 + param_1 * 0x42) = 8;
      return *(undefined4 *)(param_3 + 0x20);
    }
    if (iVar4 == 9) {
      *(undefined1 *)(&DAT_00799a84 + param_1 * 0x42) = 9;
      return *(undefined4 *)(param_3 + 0x24);
    }
    if (iVar4 == 10) {
      *(undefined1 *)(&DAT_00799a84 + param_1 * 0x42) = 10;
      return *(undefined4 *)(param_3 + 0x28);
    }
    if (iVar4 != 0xb) {
      return 0;
    }
  }
  *(undefined1 *)(&DAT_00799a84 + param_1 * 0x42) = 0xb;
  return *(undefined4 *)(param_3 + 0x2c);
}

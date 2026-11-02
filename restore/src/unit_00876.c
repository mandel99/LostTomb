#include "restore_types.h"
#define DAT_00ea4488 (*(undefined4 *)0x00ea4488u)
#define DAT_00ea44bc (*(undefined4 *)0x00ea44bcu)
#define DAT_00ea44d4 (*(undefined4 *)0x00ea44d4u)
#define DAT_00ea44dc (*(undefined4 *)0x00ea44dcu)
#define DAT_00ea44ec (*(undefined4 *)0x00ea44ecu)
#define DAT_00ea44f4 (*(undefined4 *)0x00ea44f4u)
#define DAT_00ea44fc (*(undefined4 *)0x00ea44fcu)
#define DAT_00ea4504 (*(undefined4 *)0x00ea4504u)
#define DAT_00ea4514 (*(undefined4 *)0x00ea4514u)
#define DAT_00ea4518 (*(undefined4 *)0x00ea4518u)
#define DAT_00ea451c (*(undefined4 *)0x00ea451cu)
#define DAT_00ea4520 (*(undefined4 *)0x00ea4520u)
#define DAT_00ea4524 (*(undefined4 *)0x00ea4524u)
#define DAT_00ea4528 (*(undefined4 *)0x00ea4528u)
#define DAT_00ea452c (*(undefined4 *)0x00ea452cu)
#define DAT_00ea4530 (*(undefined4 *)0x00ea4530u)
#define DAT_00ea5cc8 (*(undefined4 *)0x00ea5cc8u)
#define DAT_00ea5fc8 (*(undefined4 *)0x00ea5fc8u)
#define DAT_00ea6020 (*(undefined4 *)0x00ea6020u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_0045a990(void)

{
  int iVar1;
  int iVar2;
  
  iVar1 = DAT_00eaac28;
  iVar2 = DAT_00eaac28 * 0x49d8;
  (&DAT_00ea4518)[DAT_00eaac28 * 0x1276] =
       *(int *)(&DAT_00ea6020 + iVar2) + (&DAT_00ea5cc8)[DAT_00eaac28 * 0x1276] +
       (&DAT_00ea44bc)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44d4)[DAT_00eaac28 * 0x1276];
  (&DAT_00ea4520)[iVar1 * 0x1276] =
       *(int *)(&DAT_00ea5fc8 + iVar2) + (&DAT_00ea44f4)[iVar1 * 0x1276] +
       (&DAT_00ea4504)[iVar1 * 0x1276] + (&DAT_00ea44ec)[iVar1 * 0x1276] +
       (&DAT_00ea44fc)[iVar1 * 0x1276] + (&DAT_00ea4488)[iVar1 * 0x1276] +
       (&DAT_00ea44dc)[iVar1 * 0x1276];
  (&DAT_00ea4524)[iVar1 * 0x1276] =
       (&DAT_00ea4514)[iVar1 * 0x1276] - (&DAT_00ea451c)[iVar1 * 0x1276];
  (&DAT_00ea4528)[iVar1 * 0x1276] =
       (&DAT_00ea4518)[iVar1 * 0x1276] - (&DAT_00ea4520)[iVar1 * 0x1276];
  (&DAT_00ea4530)[iVar1 * 0x1276] =
       (&DAT_00ea452c)[iVar1 * 0x1276] +
       ((&DAT_00ea4518)[iVar1 * 0x1276] - (&DAT_00ea4520)[iVar1 * 0x1276]);
  return;
}

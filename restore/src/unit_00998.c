#include "restore_types.h"
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_00784b60 (*(undefined *)0x00784b60u)
#define DAT_00784c24 (*(undefined *)0x00784c24u)
#define DAT_00784c28 (*(undefined *)0x00784c28u)
#define DAT_00784c2c (*(undefined *)0x00784c2cu)
#define DAT_00784c30 (*(undefined *)0x00784c30u)
#define DAT_00784c3c (*(undefined *)0x00784c3cu)
#define DAT_00784c3d (*(char *)0x00784c3du)
#define DAT_00784c3e (*(undefined1 *)0x00784c3eu)
#define DAT_00784c3f (*(char *)0x00784c3fu)
#define DAT_00784c58 (*(undefined *)0x00784c58u)
#define DAT_00784c5c (*(undefined *)0x00784c5cu)
#define DAT_00784c98 (*(undefined *)0x00784c98u)
#define DAT_00784c9c (*(undefined *)0x00784c9cu)
#define DAT_00784ca0 (*(undefined *)0x00784ca0u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea450c (*(undefined4 *)0x00ea450cu)
#define DAT_00ea4514 (*(undefined4 *)0x00ea4514u)
#define DAT_00ea451c (*(undefined4 *)0x00ea451cu)
#define DAT_00ea5b3c (*(undefined4 *)0x00ea5b3cu)
#define DAT_00ea5b40 (*(undefined4 *)0x00ea5b40u)
#define DAT_00ea5b44 (*(undefined4 *)0x00ea5b44u)
#define DAT_00ea5b48 (*(undefined4 *)0x00ea5b48u)
#define DAT_00ea5cbc (*(undefined4 *)0x00ea5cbcu)
#define DAT_00ea5d58 (*(undefined4 *)0x00ea5d58u)
#define DAT_00ea5d5c (*(undefined4 *)0x00ea5d5cu)
#define DAT_00ea5dcc (*(undefined4 *)0x00ea5dccu)
#define DAT_00ea5dd0 (*(undefined4 *)0x00ea5dd0u)
#define DAT_00ea5df0 (*(undefined4 *)0x00ea5df0u)
#define DAT_00ea5df4 (*(undefined4 *)0x00ea5df4u)
#define DAT_00ea5df8 (*(undefined4 *)0x00ea5df8u)
#define DAT_00ea5dfc (*(undefined4 *)0x00ea5dfcu)
#define DAT_00ea5e00 (*(undefined4 *)0x00ea5e00u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_0046e930();
extern void FUN_0046ec70();
extern int FUN_004cde40();

void FUN_0046e970(int param_1)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  
  iVar5 = DAT_00eaac28;
  if (DAT_00784b60 != 0) {
    (&DAT_00ea5b48)[DAT_00eaac28 * 0x1276] = 0x32;
    return;
  }
  iVar6 = DAT_00eaac28 * 0x49d8;
  iVar4 = (&DAT_00ea5dcc)[DAT_00eaac28 * 0x1276];
  (&DAT_00ea5dcc)[DAT_00eaac28 * 0x1276] = iVar4 + 1;
  if (0xb < iVar4 + 1) {
    (&DAT_00ea5dd0)[iVar5 * 0x1276] = 0;
  }
  cVar2 = DAT_00e38ea8;
  if (param_1 == 0) goto LAB_0046ec37;
  (&DAT_00ea5df0)[iVar5 * 0x1276] = 0;
  (&DAT_00ea5df4)[iVar5 * 0x1276] = 0;
  (&DAT_00ea5df8)[iVar5 * 0x1276] = 0;
  if (('\x01' < cVar2) || (DAT_00e38ebc != 0)) {
    (&DAT_00ea5b48)[iVar5 * 0x1276] = (&DAT_00ea5b48)[iVar5 * 0x1276] + -1;
  }
  if ((&DAT_00ea5d58)[iVar5 * 0x1276] == 0) {
    if ((int)(&DAT_00ea451c)[iVar5 * 0x1276] < (int)(&DAT_00ea4514)[iVar5 * 0x1276]) {
      iVar4 = (&DAT_00ea5b48)[iVar5 * 0x1276] + 1;
      goto LAB_0046ea36;
    }
  }
  else {
    if ((int)(&DAT_00ea5d5c)[iVar5 * 0x1276] < 2) {
      iVar4 = (&DAT_00ea5b48)[iVar5 * 0x1276] + -3;
    }
    else if ((int)(&DAT_00ea5d5c)[iVar5 * 0x1276] < 3) {
      iVar4 = (&DAT_00ea5b48)[iVar5 * 0x1276] + -5;
    }
    else {
      iVar4 = (&DAT_00ea5b48)[iVar5 * 0x1276] + -8;
    }
LAB_0046ea36: ;
    (&DAT_00ea5b48)[iVar5 * 0x1276] = iVar4;
  }
  iVar4 = (&DAT_00ea450c)[iVar5 * 0x1276] - (&DAT_00ea5cbc)[iVar5 * 0x1276];
  if ((&DAT_00ea5cbc)[iVar5 * 0x1276] == 0) {
    if (0 < iVar4) {
      (&DAT_00ea5b48)[iVar5 * 0x1276] = (&DAT_00ea5b48)[iVar5 * 0x1276] - iVar4;
      (&DAT_00ea5df0)[iVar5 * 0x1276] = iVar4;
    }
  }
  else if (iVar4 < 1) {
    if (iVar4 < 0) {
      (&DAT_00ea5b48)[iVar5 * 0x1276] = (&DAT_00ea5b48)[iVar5 * 0x1276] + 1;
    }
  }
  else {
    (&DAT_00ea5b48)[iVar5 * 0x1276] = (&DAT_00ea5b48)[iVar5 * 0x1276] - iVar4;
    (&DAT_00ea5df0)[iVar5 * 0x1276] = iVar4 + 1;
  }
  iVar5 = (int)DAT_007848d8;
  if (DAT_0078d7bc == iVar5 + DAT_00784c98) {
    uVar7 = 0x19;
LAB_0046ead6: ;
    bVar1 = true;
    if (DAT_00784c3c != '\0') {
      iVar5 = FUN_004cde40(DAT_00784c24,uVar7);
      iVar6 = DAT_00eaac28 * 0x49d8;
      if ((int)(&DAT_00ea5b3c)[DAT_00eaac28 * 0x1276] < iVar5) {
        bVar1 = false;
      }
    }
    if (DAT_00784c3d != '\0') {
      iVar5 = FUN_004cde40(DAT_00784c28,uVar7);
      iVar6 = DAT_00eaac28 * 0x49d8;
      if ((int)(&DAT_00ea5b40)[DAT_00eaac28 * 0x1276] < iVar5) {
        bVar1 = false;
      }
    }
    if (DAT_00784c58 != 0) {
      iVar5 = FUN_004cde40(DAT_00784c5c,uVar7);
      iVar6 = DAT_00eaac28 * 0x49d8;
      if (*(int *)(&DAT_00ea1894 + iVar6) < iVar5) {
        bVar1 = false;
      }
    }
    if (DAT_00784c3e != '\0') {
      iVar5 = FUN_004cde40(DAT_00784c2c,uVar7);
      iVar6 = DAT_00eaac28 * 0x49d8;
      if ((int)(&DAT_00ea5b44)[DAT_00eaac28 * 0x1276] < iVar5) {
        bVar1 = false;
      }
    }
    if (DAT_00784c3f == '\0') {
LAB_0046ebbe: ;
      if (bVar1) {
        *(int *)((int)&DAT_00ea5b48 + iVar6) = *(int *)((int)&DAT_00ea5b48 + iVar6) + 5;
        goto LAB_0046ebe4;
      }
    }
    else {
      iVar5 = FUN_004cde40(DAT_00784c30,uVar7);
      iVar6 = DAT_00eaac28 * 0x49d8;
      if (iVar5 <= (int)(&DAT_00ea5b48)[DAT_00eaac28 * 0x1276]) goto LAB_0046ebbe;
    }
    *(int *)((int)&DAT_00ea5b48 + iVar6) = *(int *)((int)&DAT_00ea5b48 + iVar6) + -2;
    *(undefined4 *)((int)&DAT_00ea5df4 + iVar6) = 2;
  }
  else {
    if (DAT_0078d7bc == DAT_00784c9c + iVar5) {
      uVar7 = 0x32;
      goto LAB_0046ead6;
    }
    if (DAT_0078d7bc == iVar5 + DAT_00784ca0) {
      uVar7 = 0x4b;
      goto LAB_0046ead6;
    }
  }
LAB_0046ebe4: ;
  FUN_0046e930();
  iVar3 = DAT_00eaac28;
  iVar6 = DAT_00eaac28 * 0x49d8;
  iVar5 = (&DAT_00ea5dfc)[DAT_00eaac28 * 0x1276];
  iVar4 = (&DAT_00ea5b48)[DAT_00eaac28 * 0x1276];
  if (iVar5 < iVar4) {
    (&DAT_00ea5e00)[DAT_00eaac28 * 0x1276] = 2;
  }
  else if (iVar5 == iVar4) {
    (&DAT_00ea5e00)[DAT_00eaac28 * 0x1276] = 1;
  }
  else if (iVar4 < iVar5) {
    (&DAT_00ea5e00)[DAT_00eaac28 * 0x1276] = 0;
  }
  (&DAT_00ea5dfc)[iVar3 * 0x1276] = (&DAT_00ea5b48)[iVar3 * 0x1276];
LAB_0046ec37: ;
  if (*(int *)((int)&DAT_00ea5b48 + iVar6) < 0) {
    *(undefined4 *)((int)&DAT_00ea5b48 + iVar6) = 0;
  }
  if (100 < *(int *)((int)&DAT_00ea5b48 + iVar6)) {
    *(undefined4 *)((int)&DAT_00ea5b48 + iVar6) = 100;
  }
  FUN_0046ec70();
  return;
}

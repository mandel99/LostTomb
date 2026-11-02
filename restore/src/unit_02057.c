#include "restore_types.h"
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea5bfc (*(undefined4 *)0x00ea5bfcu)
#define DAT_00ea5c0a (*(undefined4 *)0x00ea5c0au)
#define DAT_00ea5c50 (*(undefined4 *)0x00ea5c50u)
#define DAT_00ea5d68 (*(undefined4 *)0x00ea5d68u)
#define DAT_00ea5d6c (*(undefined4 *)0x00ea5d6cu)
#define DAT_00ea5d70 (*(undefined4 *)0x00ea5d70u)
#define DAT_00ea5d98 (*(undefined4 *)0x00ea5d98u)
#define DAT_00ea5d9c (*(undefined4 *)0x00ea5d9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00461950();
extern void FUN_004e0cf0();
extern void FUN_004e8e80();

void FUN_004ea000(void)

{
  char *pcVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  
  iVar3 = DAT_00eaac28;
  iVar4 = DAT_00eaac28 * 0x49d8;
  if ((&DAT_00ea5d98)[DAT_00eaac28 * 0x1276] != 0) {
    (&DAT_00ea5d98)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5d98)[DAT_00eaac28 * 0x1276] + -1;
  }
  if ((&DAT_00ea5d9c)[iVar3 * 0x1276] != 0) {
    (&DAT_00ea5d9c)[iVar3 * 0x1276] = (&DAT_00ea5d9c)[iVar3 * 0x1276] + -1;
  }
  if ((int)(&DAT_00ea5d68)[iVar3 * 0x1276] < 1) {
    return;
  }
  iVar2 = (&DAT_00ea5d6c)[iVar3 * 0x1276];
  (&DAT_00ea5d6c)[iVar3 * 0x1276] = iVar2 + -1;
  if (0 < iVar2 + -1) {
    return;
  }
  if ((int)(&DAT_00ea5d98)[iVar3 * 0x1276] < 1) {
    iVar3 = (&DAT_00ea5d68)[iVar3 * 0x1276];
    if (iVar3 == 1) {
      FUN_004e8e80(6);
      pcVar1 = (char *)((int)&DAT_00ea5c0a +
                       DAT_00eaac28 * 0x49d8 + (&DAT_00ea5d70)[DAT_00eaac28 * 0x1276]);
      *pcVar1 = *pcVar1 + '\x01';
    }
    else if (iVar3 == 2) {
      FUN_004e8e80(10);
      *(char *)((int)&DAT_00ea5c0a + DAT_00eaac28 * 0x49d8 + (&DAT_00ea5d70)[DAT_00eaac28 * 0x1276])
           = *(char *)((int)&DAT_00ea5c0a +
                      DAT_00eaac28 * 0x49d8 + (&DAT_00ea5d70)[DAT_00eaac28 * 0x1276]) + '\x02';
    }
    else if (iVar3 == 3) {
      FUN_004e8e80(0xf);
      pcVar1 = (char *)((int)&DAT_00ea5c0a +
                       DAT_00eaac28 * 0x49d8 + (&DAT_00ea5d70)[DAT_00eaac28 * 0x1276]);
      *pcVar1 = *pcVar1 + '\x03';
    }
    else if (iVar3 == 4) {
      FUN_004e8e80(0xf);
      iVar3 = DAT_00eaac28;
      iVar4 = 0;
      do {
        if (iVar4 != 4) {
          *(char *)((int)&DAT_00ea5c0a + iVar4 + iVar3 * 0x49d8) =
               *(char *)((int)&DAT_00ea5c0a + iVar4 + iVar3 * 0x49d8) + '\x03';
        }
        iVar4 = iVar4 + 1;
      } while (iVar4 < 5);
    }
  }
  else if ((int)(&DAT_00ea5d9c)[iVar3 * 0x1276] < 1) {
    iVar2 = (&DAT_00ea5d68)[iVar3 * 0x1276];
    if (iVar2 == 1) {
      uVar5 = 2;
      *(char *)((int)&DAT_00ea5c0a + iVar4 + (&DAT_00ea5d70)[iVar3 * 0x1276]) =
           *(char *)((int)&DAT_00ea5c0a + iVar4 + (&DAT_00ea5d70)[iVar3 * 0x1276]) + '\x01';
    }
    else if (iVar2 == 2) {
      uVar5 = 3;
      pcVar1 = (char *)((int)&DAT_00ea5c0a + iVar4 + (&DAT_00ea5d70)[iVar3 * 0x1276]);
      *pcVar1 = *pcVar1 + '\x02';
    }
    else {
      if (iVar2 != 3) goto LAB_004ea1b1;
      uVar5 = 5;
      *(char *)((int)&DAT_00ea5c0a + iVar4 + (&DAT_00ea5d70)[iVar3 * 0x1276]) =
           *(char *)((int)&DAT_00ea5c0a + iVar4 + (&DAT_00ea5d70)[iVar3 * 0x1276]) + '\x05';
    }
    FUN_004e8e80(uVar5);
  }
LAB_004ea1b1: ;
  iVar3 = DAT_00eaac28;
  DAT_00e950dc = 1;
  iVar4 = DAT_00eaac28 * 0x1276;
  (&DAT_00ea5bfc)[DAT_00eaac28 * 0x1276] = 1;
  (&DAT_00ea5c50)[(&DAT_00ea5d70)[iVar3 * 0x1276] + iVar4] = 0;
  iVar3 = (&DAT_00ea5d68)[iVar3 * 0x1276];
  if (iVar3 == 1) {
    uVar5 = 0x89;
  }
  else if (iVar3 == 2) {
    uVar5 = 0x8a;
  }
  else {
    if (iVar3 != 3) goto LAB_004ea234;
    uVar5 = 0x8b;
  }
  FUN_004e0cf0(uVar5,0,0,0,0);
LAB_004ea234: ;
  FUN_00461950((&DAT_00ea5d68)[DAT_00eaac28 * 0x1276] * 10);
  iVar3 = DAT_00eaac28;
  (&DAT_00ea5d68)[DAT_00eaac28 * 0x1276] = 0;
  (&DAT_00ea5d6c)[iVar3 * 0x1276] = 0;
  return;
}

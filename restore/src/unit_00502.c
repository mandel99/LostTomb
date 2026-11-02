#include "restore_types.h"
#define DAT_0070573c (*(undefined4 *)0x0070573cu)
#define DAT_00705c78 (*(undefined *)0x00705c78u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a5b (*(undefined4 *)0x00799a5bu)
extern undefined4 FUN_004c06e0();

int FUN_00434400(int param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  sVar1 = (&DAT_007999e8)[param_1 * 0x84];
  if (sVar1 == 0x62) {
    return 0;
  }
  if (sVar1 == 0x96) {
    return 0;
  }
  if (sVar1 == 0x3c) {
    return 0;
  }
  if (sVar1 == 0x3d) {
    return 0;
  }
  if (sVar1 == 0x3e) {
    return 0;
  }
  if (sVar1 == 0x3f) {
    return 0;
  }
  if (sVar1 == 0x40) {
    return 0;
  }
  if (sVar1 == 0x41) {
    return 0;
  }
  if (sVar1 == 0x42) {
    return 0;
  }
  if (sVar1 == 0x43) {
    return 0;
  }
  if (sVar1 == 0x44) {
    return 0;
  }
  if (sVar1 == 0x45) {
    return 0;
  }
  if (sVar1 == 0xd2) {
    return 0;
  }
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
    return 0;
  }
  iVar5 = 0;
  iVar3 = 0;
  do {
    iVar4 = 0;
    do {
      iVar2 = FUN_004c06e0(iVar3,iVar4);
      iVar5 = iVar5 + (&DAT_0070573c)[iVar2];
      iVar4 = iVar4 + 1;
    } while (iVar4 < 3);
    iVar3 = iVar3 + 1;
  } while (iVar3 < 5);
  if (DAT_00705c78 + iVar5 == 0) {
    return 0;
  }
  return ((char)(&DAT_00799a5b)[param_1 * 0x108] * 0x11) / 10;
}

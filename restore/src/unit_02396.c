#include "restore_types.h"
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea5bfc (*(undefined4 *)0x00ea5bfcu)
#define DAT_00ea5d68 (*(undefined4 *)0x00ea5d68u)
#define DAT_00ea5d6c (*(undefined4 *)0x00ea5d6cu)
#define DAT_00ea5d98 (*(undefined4 *)0x00ea5d98u)
#define DAT_00ea5d9c (*(undefined4 *)0x00ea5d9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f55c5c (*(undefined *)0x00f55c5cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
extern void FUN_004ccc30();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_005292b0();
extern int FUN_0053e590();

void FUN_00528f10(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  
  FUN_004d65b0(DAT_00f53f0c + 0x30,DAT_00f53f08 + 0x140,0x22,6,DAT_00e38da8,DAT_00e38da4);
  FUN_0053e590(DAT_00f55c5c + 0xf,DAT_00f53f0c + 0x1cc,DAT_00f53f08 + 0x143);
  DAT_00e20778 = 0;
  FUN_004ccc30(8,4,(&DAT_00ea5bfc)[DAT_00eaac28 * 0x1276],DAT_00f53f0c + 0x70,DAT_00f53f08 + 0x148,
               DAT_00f56f9c,0);
  FUN_004d62f0(0x3a,0xf,DAT_00e20778 + 0x70 + DAT_00f53f0c,DAT_00f53f08 + 0x148,DAT_00f56f9c,0,
               DAT_00e38da8,DAT_00e38da4);
  iVar2 = (&DAT_00ea5d68)[DAT_00eaac28 * 0x1276];
  if (iVar2 == 0) {
    if (((&DAT_00ea5d98)[DAT_00eaac28 * 0x1276] == 0) ||
       ((&DAT_00ea5d9c)[DAT_00eaac28 * 0x1276] == 0)) {
      FUN_004d6250(0x3a,0x10,DAT_00f53f0c + 0x66,DAT_00f53f08 + 0x160,300,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      iVar2 = (&DAT_00ea5bfc)[DAT_00eaac28 * 0x1276];
      if (iVar2 < 2) {
        iVar2 = 0;
      }
      else if (iVar2 < 7) {
        iVar2 = 1;
      }
      else if (iVar2 < 0xd) {
        iVar2 = 2;
      }
      else if (iVar2 < 0x13) {
        iVar2 = 3;
      }
      else if (iVar2 < 0x19) {
        iVar2 = 4;
      }
      else {
        iVar2 = (0x1e < iVar2) + 5;
      }
    }
    else {
      FUN_004d6250(0x3a,0x34,DAT_00f53f0c + 0x66,DAT_00f53f08 + 0x160,300,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      if ((int)(&DAT_00ea5bfc)[DAT_00eaac28 * 0x1276] < 2) {
        iVar2 = 7;
      }
      else {
        iVar2 = (6 < (int)(&DAT_00ea5bfc)[DAT_00eaac28 * 0x1276]) + 8;
      }
    }
  }
  else {
    if (iVar2 == 1) {
      iVar2 = 2;
      iVar3 = 10;
    }
    else if (iVar2 == 2) {
      iVar2 = 3;
      iVar3 = 0x14;
    }
    else {
      iVar2 = 4;
      iVar3 = 0x1f;
    }
    DAT_00e20778 = 0;
    if (iVar2 < (int)(&DAT_00ea5d6c)[DAT_00eaac28 * 0x1276]) {
      uVar4 = 0x35;
    }
    else {
      uVar4 = 0x22;
    }
    FUN_004d62f0(0x3a,uVar4,DAT_00f53f0c + 0x66,DAT_00f53f08 + 0x160,DAT_00f56f9c,0,DAT_00e38da8,
                 DAT_00e38da4);
    iVar1 = (&DAT_00ea5d6c)[DAT_00eaac28 * 0x1276];
    iVar2 = iVar3 + -1 + iVar1;
    if (iVar1 == 1) {
      FUN_004d62f0(0x3a,0x36,DAT_00e20778 + 0x66 + DAT_00f53f0c,DAT_00f53f08 + 0x160,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    else {
      FUN_004d62f0(0xa0,(iVar1 + DAT_0078d7c0) % 0xc,DAT_00e20778 + 0x66 + DAT_00f53f0c,
                   DAT_00f53f08 + 0x160,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
    }
  }
  FUN_004d6160(0x127,iVar2,DAT_00f53f0c + 0x38,DAT_00f53f08 + 0x175,0x17c,DAT_00f56f9c,0,
               DAT_00e38da8,DAT_00e38da4);
  FUN_005292b0();
  return;
}

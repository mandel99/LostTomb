#include "restore_types.h"
#define DAT_0070e440 (*(undefined *)0x0070e440u)
#define DAT_0070e444 (*(undefined *)0x0070e444u)
#define DAT_0070e448 (*(undefined *)0x0070e448u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e450 (*(undefined *)0x0070e450u)
#define DAT_0070e454 (*(undefined *)0x0070e454u)
#define DAT_0070e458 (*(undefined *)0x0070e458u)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
#define DAT_00f56b8c (*(undefined *)0x00f56b8cu)
#define DAT_00f56ca4 (*(undefined *)0x00f56ca4u)
extern int FUN_0045d510();
extern int FUN_00475b50();
extern int FUN_0053e590();
extern longlong FUN_0056133c();

void FUN_00434aa0(int param_1,int param_2,undefined4 param_3)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  byte bVar4;
  
  uVar1 = (&DAT_00af598c)[DAT_0070e488];
  iVar2 = (int)*(short *)(&DAT_00b4e6fc + DAT_0070e488 * 2);
  if (((uVar1 & 0x26341677) != 0) && ((uVar1 & 8) == 0)) {
    if (((&DAT_00b67d1c)[DAT_0070e488] & 0x40) != 0) {
      DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
      bVar4 = (&DAT_00b285cc)[DAT_0070e488] & 0xf;
      if (((&DAT_00b285cc)[DAT_0070e488] & 0xf) == 0) {
        FUN_0053e590(DAT_00d3b5c0,param_1,param_2);
        return;
      }
      if (bVar4 == 1) {
        FUN_0053e590(DAT_00d3b5c0,DAT_0070e45c + 1 + param_1,param_2 - DAT_0070e44c);
        return;
      }
      if (bVar4 == 2) {
        FUN_0053e590(DAT_00d3b5c0,DAT_0070e458 + 2 + param_1,param_2 - DAT_0070e448);
        return;
      }
      if (bVar4 == 3) {
        FUN_0053e590(DAT_00d3b5c0,DAT_0070e454 + 3 + param_1,param_2 - DAT_0070e444);
        return;
      }
      if (bVar4 == 4) {
        FUN_0053e590(DAT_00d3b5c0,DAT_0070e450 + 4 + param_1,param_2 - DAT_0070e440);
        return;
      }
      if (bVar4 == 5) {
        FUN_0053e590(DAT_00d3b5c0,DAT_0070e450 + DAT_0070e45c + 5 + param_1,
                     (param_2 - DAT_0070e440) - DAT_0070e44c);
        return;
      }
    }
    return;
  }
  if ((uVar1 & 0x4100) != 0) {
    DAT_00d3b5c0 = ((byte)(&DAT_00acf85c)[DAT_0070e488] & 7) + DAT_00f56b8c;
    FUN_0053e590(DAT_00d3b5c0,param_1,param_2);
    return;
  }
  if ((uVar1 & 8) == 0) {
    DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
    FUN_0053e590(DAT_00d3b5c0,param_1,param_2);
    return;
  }
  switch((&DAT_007999e8)[iVar2 * 0x84]) {
  case 0x46: ;
  case 0xbe: ;
    break;
  case 0x47: ;
  case 0xbf: ;
    break;
  case 0x48: ;
  case 0xc1: ;
    FUN_0045d510(iVar2,param_3);
    break;
  case 0x49: ;
    uVar3 = FUN_00475b50(iVar2,param_3);
    FUN_0045d510(uVar3);
    break;
default: ;
    if (*(char *)((int)&DAT_007999dc + iVar2 * 0x108 + 1) == '\0') {
      iVar2 = 0;
      goto LAB_00434d00;
    }
  }
  iVar2 = FUN_0056133c();
  iVar2 = 4 - iVar2;
  if (9 < iVar2) {
    iVar2 = 9;
  }
LAB_00434d00: ;
  DAT_00d3b5c0 = DAT_00f56ca4 + iVar2;
  FUN_0053e590(DAT_00d3b5c0,param_1,param_2);
  return;
}

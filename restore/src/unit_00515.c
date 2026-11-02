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
#define DAT_00a5d4cc (*(undefined4 *)0x00a5d4ccu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
#define DAT_00f56ca4 (*(undefined *)0x00f56ca4u)
extern int FUN_0053e590();

void FUN_00434e10(int param_1,int param_2)

{
  byte bVar1;
  
  bVar1 = (&DAT_00a5d4cc)[DAT_0070e488];
  if (bVar1 != 0) {
    if (bVar1 < 10) {
      DAT_00d3b5c0 = DAT_00f56ca4 + 1;
    }
    else if (bVar1 < 0x14) {
      DAT_00d3b5c0 = DAT_00f56ca4 + 2;
    }
    else if (bVar1 < 0x1e) {
      DAT_00d3b5c0 = DAT_00f56ca4 + 3;
    }
    else if (bVar1 < 0x28) {
      DAT_00d3b5c0 = DAT_00f56ca4 + 4;
    }
    else if (bVar1 < 0x32) {
      DAT_00d3b5c0 = DAT_00f56ca4 + 5;
    }
    else if (bVar1 < 0x3c) {
      DAT_00d3b5c0 = DAT_00f56ca4 + 6;
    }
    else if (bVar1 < 0x46) {
      DAT_00d3b5c0 = DAT_00f56ca4 + 7;
    }
    else if (bVar1 < 0x50) {
      DAT_00d3b5c0 = DAT_00f56ca4 + 8;
    }
    else if (bVar1 < 0x5a) {
      DAT_00d3b5c0 = DAT_00f56ca4 + 9;
    }
    else {
      DAT_00d3b5c0 = DAT_00f56ca4 + 9;
    }
    FUN_0053e590(DAT_00d3b5c0,param_1,param_2);
    return;
  }
  if ((((&DAT_00af598c)[DAT_0070e488] & 0x26341677) == 0) ||
     (((&DAT_00af598c)[DAT_0070e488] & 8) != 0)) {
    DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
    FUN_0053e590(DAT_00d3b5c0,param_1,param_2);
  }
  else if (((&DAT_00b67d1c)[DAT_0070e488] & 0x40) != 0) {
    DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
    bVar1 = (&DAT_00b285cc)[DAT_0070e488] & 0xf;
    if (((&DAT_00b285cc)[DAT_0070e488] & 0xf) == 0) {
      FUN_0053e590(DAT_00d3b5c0,param_1,param_2);
      return;
    }
    if (bVar1 == 1) {
      FUN_0053e590(DAT_00d3b5c0,DAT_0070e45c + 1 + param_1,param_2 - DAT_0070e44c);
      return;
    }
    if (bVar1 == 2) {
      FUN_0053e590(DAT_00d3b5c0,DAT_0070e458 + 2 + param_1,param_2 - DAT_0070e448);
      return;
    }
    if (bVar1 == 3) {
      FUN_0053e590(DAT_00d3b5c0,DAT_0070e454 + 3 + param_1,param_2 - DAT_0070e444);
      return;
    }
    if (bVar1 == 4) {
      FUN_0053e590(DAT_00d3b5c0,DAT_0070e450 + 4 + param_1,param_2 - DAT_0070e440);
      return;
    }
    if (bVar1 == 5) {
      FUN_0053e590(DAT_00d3b5c0,DAT_0070e450 + DAT_0070e45c + 5 + param_1,
                   (param_2 - DAT_0070e440) - DAT_0070e44c);
      return;
    }
  }
  return;
}

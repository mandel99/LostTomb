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
#define DAT_00ac2d4c (*(undefined4 *)0x00ac2d4cu)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
#define DAT_00f56b8c (*(undefined *)0x00f56b8cu)
extern void FUN_00433980();
extern void FUN_004339a0();

void FUN_00433760(int param_1,int param_2,char param_3)

{
  uint uVar1;
  int iVar2;
  byte bVar3;
  int iVar4;
  int iVar5;
  code *pcVar6;
  
  uVar1 = (&DAT_00af598c)[DAT_0070e488];
  pcVar6 = FUN_004339a0;
  iVar5 = (int)(char)(&DAT_00ac2d4c)[DAT_0070e488];
  if (param_3 == '\0') {
    pcVar6 = FUN_00433980;
  }
  if (((uVar1 & 0x26341777) == 0) || ((uVar1 & 8) != 0)) {
    if ((uVar1 & 0x4000) != 0) {
      DAT_00d3b5c0 = ((byte)(&DAT_00acf85c)[DAT_0070e488] & 7) + DAT_00f56b8c;
      (*pcVar6)(DAT_00d3b5c0,param_1,param_2);
      return;
    }
    if (((uVar1 & 0x28) == 0) && (iVar5 == 0)) {
      DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
      (*pcVar6)(DAT_00d3b5c0,param_1,param_2);
      return;
    }
    if ((*(short *)(&DAT_00b4e6fc + DAT_0070e488 * 2) == 0) ||
       (iVar4 = 0x22b,
       *(char *)((int)&DAT_007999dc + *(short *)(&DAT_00b4e6fc + DAT_0070e488 * 2) * 0x108 + 1) ==
       '\0')) {
      iVar4 = 0x22a;
    }
    iVar2 = 0;
    if (-0xb < iVar5) {
      if (iVar5 < 0x46) {
        iVar2 = iVar5 / 10 + 2;
      }
      else {
        iVar2 = 9;
      }
    }
    (*pcVar6)((&DAT_00f55bb4)[iVar4 * 2] + iVar2,param_1,param_2);
  }
  else if (((&DAT_00b67d1c)[DAT_0070e488] & 0x40) != 0) {
    DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
    bVar3 = (&DAT_00b285cc)[DAT_0070e488] & 0xf;
    if (((&DAT_00b285cc)[DAT_0070e488] & 0xf) == 0) {
      (*pcVar6)(DAT_00d3b5c0,param_1,param_2);
      return;
    }
    if (bVar3 == 1) {
      (*pcVar6)(DAT_00d3b5c0,DAT_0070e45c + 1 + param_1,param_2 - DAT_0070e44c);
      return;
    }
    if (bVar3 == 2) {
      (*pcVar6)(DAT_00d3b5c0,DAT_0070e458 + 2 + param_1,param_2 - DAT_0070e448);
      return;
    }
    if (bVar3 == 3) {
      (*pcVar6)(DAT_00d3b5c0,DAT_0070e454 + 3 + param_1,param_2 - DAT_0070e444);
      return;
    }
    if (bVar3 == 4) {
      (*pcVar6)(DAT_00d3b5c0,DAT_0070e450 + 4 + param_1,param_2 - DAT_0070e440);
      return;
    }
    if (bVar3 == 5) {
      (*pcVar6)(DAT_00d3b5c0,DAT_0070e450 + DAT_0070e45c + 5 + param_1,
                (param_2 - DAT_0070e440) - DAT_0070e44c);
      return;
    }
  }
  return;
}

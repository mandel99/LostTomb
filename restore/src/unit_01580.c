#include "restore_types.h"
#define DAT_005df78c (*(undefined4 *)0x005df78cu)
#define DAT_005df814 (*(undefined4 *)0x005df814u)
#define DAT_005df818 (*(undefined4 *)0x005df818u)
#define DAT_005df81c (*(undefined4 *)0x005df81cu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
extern void FUN_0047c2b0();

void FUN_004c03d0(int param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar1 = param_1;
  iVar6 = param_1 * 0x108;
  iVar2 = (uint)*(byte *)(&DAT_00799a82 + param_1 * 0x84) + DAT_00bca2e0;
  if (7 < iVar2) {
    iVar2 = iVar2 + -8;
  }
  piVar3 = (int *)(&DAT_005df78c + iVar2 * 0x10);
  param_1 = 4;
  do {
    iVar2 = *piVar3;
    if (*(char *)(&DAT_00799a82 + iVar1 * 0x84) == '\0') {
      iVar4 = (int)*(short *)((int)&DAT_007999dc + iVar6 + 2) +
              *(int *)(&DAT_005df81c + iVar2 * 0x10);
      iVar5 = (int)*(short *)(&DAT_007999e0 + iVar6) + *(int *)(&DAT_005df818 + iVar2 * 0x10);
    }
    else if (*(char *)(&DAT_00799a82 + iVar1 * 0x84) == '\x01') {
      iVar4 = (int)*(short *)((int)&DAT_007999dc + iVar6 + 2) +
              *(int *)(&DAT_005df818 + iVar2 * 0x10);
      iVar5 = (int)*(short *)(&DAT_007999e0 + iVar6) + *(int *)(&DAT_005df81c + iVar2 * 0x10);
    }
    else {
      iVar4 = (int)*(short *)((int)&DAT_007999dc + iVar6 + 2);
      iVar5 = (int)*(short *)(&DAT_007999e0 + iVar6);
    }
    FUN_0047c2b0(iVar1,iVar4,iVar5,(&DAT_005df814)[iVar2 * 4],(&DAT_005df814)[iVar2 * 4]);
    piVar3 = piVar3 + 1;
    param_1 = param_1 + -1;
  } while (param_1 != 0);
  return;
}

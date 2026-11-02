#include "restore_types.h"
#define DAT_005df768 (*(undefined4 *)0x005df768u)
#define DAT_005df778 (*(undefined4 *)0x005df778u)
#define DAT_005df810 (*(undefined4 *)0x005df810u)
#define DAT_005df814 (*(undefined4 *)0x005df814u)
#define DAT_005df818 (*(undefined4 *)0x005df818u)
#define DAT_005df81c (*(undefined4 *)0x005df81cu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
extern void FUN_0047afd0();
extern void FUN_0047bf50();
extern int FUN_004c53d0();

void FUN_004bfb30(int *param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined4 uVar11;
  
  iVar2 = (int)param_1;
  iVar3 = (int)*(short *)((int)&DAT_007999dc + (int)param_1 * 0x108 + 2);
  iVar6 = (int)*(short *)(&DAT_007999e0 + (int)param_1 * 0x108);
  iVar4 = FUN_004c53d0(2);
  if ((iVar4 == -1) && (iVar4 = FUN_004c53d0(1), iVar4 == -1)) {
    iVar4 = 0;
  }
  iVar4 = (&DAT_005df778)[iVar4];
  iVar7 = (uint)*(byte *)(&DAT_00799a82 + (int)param_1 * 0x84) + DAT_00bca2e0;
  if (7 < iVar7) {
    iVar7 = iVar7 + -8;
  }
  param_1 = &DAT_005df768;
  do {
    iVar1 = param_1[iVar7 * 4 + 9];
    switch((int)(iVar7)) {
    case 0: ;
    case 3: ;
    case 4: ;
    case 7: ;
      iVar10 = 1;
      break;
default: ;
      iVar10 = 0;
    }
    if (*(char *)(&DAT_00799a82 + iVar2 * 0x84) == '\0') {
      iVar5 = *(int *)(&DAT_005df81c + iVar1 * 0x10);
      iVar8 = *(int *)(&DAT_005df818 + iVar1 * 0x10);
LAB_004bfc2f: ;
      iVar5 = iVar5 + iVar3;
      iVar8 = iVar8 + iVar6;
    }
    else {
      iVar5 = iVar3;
      iVar8 = iVar6;
      if (*(char *)(&DAT_00799a82 + iVar2 * 0x84) == '\x01') {
        iVar5 = *(int *)(&DAT_005df818 + iVar1 * 0x10);
        iVar8 = *(int *)(&DAT_005df81c + iVar1 * 0x10);
        goto LAB_004bfc2f;
      }
    }
    iVar9 = *param_1;
    if (*param_1 == 0x1ae) {
      iVar9 = iVar4;
    }
    if ((&DAT_005df810)[iVar1 * 0x10] == '\0') {
      uVar11 = 0x8008;
    }
    else {
      uVar11 = 0x8048;
    }
    FUN_0047bf50(iVar2,iVar5,iVar8,(&DAT_005df814)[iVar1 * 4],(&DAT_005df814)[iVar1 * 4],
                 (&DAT_00f55bb4)[iVar9 * 2] + iVar10,uVar11);
    param_1 = param_1 + 1;
    if (0x5df777 < (int)param_1) {
      FUN_0047afd0(iVar3,iVar6,5);
      return;
    }
  } while( true );
}

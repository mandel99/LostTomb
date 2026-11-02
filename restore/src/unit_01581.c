#include "restore_types.h"
#define DAT_005ccd14 (*(undefined4 *)0x005ccd14u)
#define DAT_005df78c (*(undefined4 *)0x005df78cu)
#define DAT_005df814 (*(undefined4 *)0x005df814u)
#define DAT_005df818 (*(undefined4 *)0x005df818u)
#define DAT_005df81c (*(undefined4 *)0x005df81cu)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_00799a08 (*(undefined4 *)0x00799a08u)
#define DAT_00799a1b (*(undefined4 *)0x00799a1bu)
#define DAT_00799a1e (*(undefined4 *)0x00799a1eu)
#define DAT_00799a20 (*(undefined4 *)0x00799a20u)
#define DAT_00799a22 (*(undefined4 *)0x00799a22u)
#define DAT_00799a24 (*(undefined4 *)0x00799a24u)
#define DAT_00799a25 (*(undefined4 *)0x00799a25u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f56c5c (*(undefined *)0x00f56c5cu)
extern int FUN_0046a400();
extern void FUN_0046ac50();
extern void FUN_0047bf50();
extern void FUN_004bfee0();

void FUN_004c04a0(int param_1)

{
  int iVar1;
  int iVar2;
  byte bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int *piVar10;
  int local_14;
  int local_8;
  
  iVar6 = param_1 * 0x108;
  DAT_00bc9694 = 0;
  *(undefined2 *)(&DAT_00799a20 + iVar6) = 0;
  (&DAT_00799a1e)[param_1 * 0x84] = 0;
  (&DAT_00799a1b)[iVar6] = 0;
  (&DAT_007999f8)[param_1 * 0x84] = 0;
  FUN_0046ac50(param_1);
  FUN_004bfee0(param_1);
  (&DAT_007999e8)[param_1 * 0x84] = 99;
  *(undefined2 *)(&DAT_00799a08 + iVar6) = 0;
  *(ushort *)(&DAT_00799a22 + iVar6) = ((byte)(&DAT_00799a25)[iVar6] & 0xff07) + 1;
  iVar7 = DAT_00bca2e0;
  (&DAT_00799a24)[iVar6] = 1;
  (&DAT_007999db)[iVar6] = 1;
  iVar7 = (uint)*(byte *)(&DAT_00799a82 + param_1 * 0x84) + iVar7;
  if (7 < iVar7) {
    iVar7 = iVar7 + -8;
  }
  local_8 = 4;
  piVar9 = (int *)(&DAT_005df78c + iVar7 * 0x10);
  do {
    iVar1 = *piVar9;
    if (iVar7 == 0) {
      iVar5 = (int)*(short *)((int)&DAT_007999dc + iVar6 + 2) +
              *(int *)(&DAT_005df81c + iVar1 * 0x10);
      param_1 = (int)*(short *)(&DAT_007999e0 + iVar6) + *(int *)(&DAT_005df818 + iVar1 * 0x10);
    }
    else if (iVar7 == 1) {
      iVar5 = (int)*(short *)((int)&DAT_007999dc + iVar6 + 2) +
              *(int *)(&DAT_005df818 + iVar1 * 0x10);
      param_1 = (int)*(short *)(&DAT_007999e0 + iVar6) + *(int *)(&DAT_005df81c + iVar1 * 0x10);
    }
    else {
      param_1 = (int)*(short *)(&DAT_007999e0 + iVar6);
      iVar5 = (int)*(short *)((int)&DAT_007999dc + iVar6 + 2);
    }
    local_14 = 0;
    if (0 < (int)(&DAT_005df814)[iVar1 * 4]) {
      piVar10 = &DAT_005ccd14;
      do {
        iVar2 = FUN_0046a400(DAT_00eaac28,99,piVar10[-1] + iVar5,param_1 + *piVar10,0,0);
        iVar8 = iVar2 * 0x108;
        bVar3 = (&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar8)] & 3;
        iVar4 = DAT_00f56c5c;
        if (((&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar8)] & 3) != 0) {
          if (bVar3 == 1) {
            iVar4 = DAT_00f56c5c + 9;
          }
          else if (bVar3 == 2) {
            iVar4 = DAT_00f56c5c + 0x12;
          }
          else {
            iVar4 = DAT_00f56c5c + 0x1b;
          }
        }
        FUN_0047bf50(iVar2,(int)*(short *)((int)&DAT_007999dc + iVar8 + 2),
                     (int)*(short *)(&DAT_007999e0 + iVar8),1,1,iVar4,8);
        iVar4 = (&DAT_005df814)[iVar1 * 4];
        *(ushort *)(&DAT_00799a22 + iVar8) = ((byte)(&DAT_00799a25)[iVar8] & 0xff07) + 1;
        local_14 = local_14 + 1;
        piVar10 = piVar10 + 2;
        *(undefined2 *)(&DAT_00799a08 + iVar8) = 0;
        (&DAT_00799a24)[iVar8] = 1;
      } while (local_14 < iVar4);
    }
    piVar9 = piVar9 + 1;
    local_8 = local_8 + -1;
  } while (local_8 != 0);
  return;
}

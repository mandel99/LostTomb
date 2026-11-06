#include "restore_types.h"
#define DAT_005f6bdc (*(undefined4 *)0x005f6bdcu)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2d8 (*(uint32_t *)0x00bca2d8u)
#define DAT_00f56b54 (*(undefined *)0x00f56b54u)
extern void FUN_004ee4a0();
extern void FUN_004ee630();
extern int FUN_004f3360();

int FUN_004f3690(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  char cVar3;
  int *piVar4;
  undefined1 *puVar5;
  int iVar6;
  byte bVar7;
  byte bVar8;
  int iVar9;
  int iVar10;
  
  bVar8 = 0;
  piVar4 = &DAT_005f6bdc;
  param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  bVar7 = 0;
  iVar9 = (int)*(short *)(&DAT_00b4e6fc + param_1 * 2);
  iVar10 = iVar9 * 0x108;
  do {
    if (*(int *)(&DAT_007999e4 + iVar10) + *piVar4 == param_1) {
      bVar8 = (byte)(1 << (bVar7 & 0x1f));
      break;
    }
    piVar4 = piVar4 + 1;
    bVar7 = bVar7 + 1;
  } while ((int)piVar4 < 0x5f6bec);
  bVar7 = *(byte *)((int)&DAT_00799a30 + iVar10 + 3);
  if (bVar7 == 0xf) {
    bVar7 = *(byte *)((int)&DAT_00799a2c + iVar10 + 1);
    if (bVar7 < 10) {
      cVar3 = FUN_004f3360(bVar7);
      *(char *)((int)&DAT_00799a2c + iVar10 + 1) = cVar3;
      if (cVar3 == '\x05') {
        bVar8 = 0xf;
      }
      *(byte *)((int)&DAT_00799a30 + iVar10 + 3) = bVar8;
    }
    else {
      if ((&DAT_007999ea)[iVar9 * 0x84] != 0) {
        return 0;
      }
      if (bVar7 + 1 < 0xc) {
        iVar10 = 4;
        puVar5 = (undefined1 *)((int)&DAT_00799a30 + iVar9 * 0x108 + 3);
        do {
          puVar5[-6] = (char)(bVar7 + 1);
          *puVar5 = 0xf;
          puVar5 = puVar5 + 0x108;
          iVar10 = iVar10 + -1;
        } while (iVar10 != 0);
        FUN_004ee4a0(iVar9);
        FUN_004ee630(iVar9);
        return iVar9;
      }
    }
  }
  else {
    if (*(char *)((int)&DAT_00799a2c + iVar10 + 1) == '\x05') {
      bVar8 = 0xf;
    }
    *(byte *)((int)&DAT_00799a30 + iVar10 + 3) = bVar7 | bVar8;
    iVar2 = DAT_00f56b54;
    if (*(char *)((int)&DAT_00799a2c + iVar10 + 1) == '\x03') {
      *(undefined1 *)((int)&DAT_00799a2c + iVar10 + 1) = 4;
      iVar1 = *(int *)(&DAT_007999e4 + iVar10);
      piVar4 = &DAT_005f6bdc;
      do {
        iVar6 = iVar1 + *piVar4;
        piVar4 = piVar4 + 1;
        bVar7 = (&DAT_00acf85c)[iVar6];
        (&DAT_00af598c)[iVar6] = 0x10000008;
        (&DAT_00b7482c)[iVar6] = (bVar7 & 7) + iVar2;
      } while ((int)piVar4 < 0x5f6bec);
    }
  }
  iVar2 = DAT_00f56b54;
  if (*(byte *)((int)&DAT_00799a2c + iVar10 + 1) < 5) {
    bVar7 = (&DAT_00acf85c)[param_1];
    (&DAT_00af598c)[param_1] = 0x10000008;
    (&DAT_00b7482c)[param_1] = (bVar7 & 7) + iVar2;
  }
  FUN_004ee630(iVar9 - (short)(&DAT_007999ea)[iVar9 * 0x84]);
  FUN_004ee4a0(iVar9 - (short)(&DAT_007999ea)[iVar9 * 0x84]);
  return iVar9;
}

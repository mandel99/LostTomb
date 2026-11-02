#include "restore_types.h"
#define DAT_005df9b0 (*(undefined4 *)0x005df9b0u)
#define DAT_005e00cc (*(undefined4 *)0x005e00ccu)
#define DAT_005e00d0 (*(undefined4 *)0x005e00d0u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00a9010c (*(undefined4 *)0x00a9010cu)
#define DAT_00a9cc1c (*(undefined4 *)0x00a9cc1cu)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00f56b54 (*(undefined *)0x00f56b54u)
extern int FUN_00475b50();
extern void FUN_00475cc0();
extern void FUN_004767d0();
extern void FUN_00477500();
extern void FUN_004776f0();
extern void FUN_004777c0();
extern void FUN_00477890();
extern void FUN_0047a220();
extern void FUN_0047b590();

void FUN_004c0920(int param_1)

{
  uint *puVar1;
  byte bVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int local_34;
  int local_30;
  int local_2c;
  int *local_28;
  int local_24;
  int local_20;
  int local_14;
  
  uVar4 = (uint)*(byte *)(&DAT_00799a82 + param_1 * 0x84);
  iVar5 = ((int)uVar4 >> 1) * 8;
  iVar8 = (int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2) +
          *(int *)(&DAT_005e00cc + iVar5);
  iVar5 = (int)*(short *)(&DAT_007999e0 + param_1 * 0x108) + *(int *)(&DAT_005e00d0 + iVar5);
  local_28 = &DAT_005df9b0;
  local_24 = iVar8;
  local_20 = iVar5;
  do {
    local_14 = 0xd;
    iVar7 = iVar5;
    iVar11 = iVar8;
    do {
      if (uVar4 == 0) {
        local_2c = iVar8 + 0xd;
        local_30 = iVar5 + 7;
        iVar10 = local_20;
        local_34 = iVar11;
      }
      else if (uVar4 == 2) {
        local_2c = iVar8 + -7;
        local_30 = iVar5 + -0xd;
        local_34 = local_24;
        iVar10 = iVar7;
      }
      else if (uVar4 == 4) {
        local_2c = iVar8 + -0xd;
        local_30 = iVar5 + -7;
        iVar10 = local_24 + (iVar5 - iVar8);
        local_34 = (iVar8 - iVar5) + iVar7;
      }
      else {
        local_2c = iVar8 + 7;
        local_30 = iVar5 + 0xd;
        iVar10 = iVar11 + (iVar5 - iVar8);
        local_34 = local_20 + (iVar8 - iVar5);
      }
      iVar9 = DAT_00bca2d8 + iVar10 * 0xe4 + local_34;
      if (*local_28 != 0) {
        if (param_1 != 0) {
          if ((*(short *)(&DAT_00b4e6fc + iVar9 * 2) != param_1) &&
             (iVar6 = FUN_00475b50((int)*(short *)(&DAT_00b4e6fc + iVar9 * 2)), iVar6 != param_1))
          goto LAB_004c0b20;
          if ((&DAT_007999e8)[param_1 * 0x84] != 99) {
            (&DAT_00a9010c)[iVar9] = *(undefined1 *)(&DAT_007999e8 + param_1 * 0x84);
          }
        }
        puVar1 = &DAT_00af598c + iVar9;
        (&DAT_00b285cc)[iVar9] = (&DAT_00b285cc)[iVar9] & 0xe0;
        bVar2 = (&DAT_00b67d1c)[iVar9];
        *(undefined2 *)(&DAT_00b4e6fc + iVar9 * 2) = 0;
        *(undefined2 *)(&DAT_00a9cc1c + iVar9 * 2) = 0;
        (&DAT_00b67d1c)[iVar9] = bVar2 & 0xc0 | 0x40;
        uVar3 = *puVar1;
        (&DAT_00adc36c)[iVar9] = 0;
        *puVar1 = uVar3 & 0xfffffff7;
        (&DAT_00ae8e7c)[iVar9] = 0;
        if (((uVar3 & 4) == 0) || ((uVar3 & 0x10000) != 0)) {
          (&DAT_00b7482c)[iVar9] = ((byte)(&DAT_00acf85c)[iVar9] & 7) + DAT_00f56b54;
          *puVar1 = uVar3 & 0x3052e84;
        }
        else {
          *puVar1 = uVar3 & 4;
          FUN_0047a220(local_34,iVar10,1);
        }
      }
LAB_004c0b20: ;
      local_28 = local_28 + 5;
      iVar7 = iVar7 + -1;
      iVar11 = iVar11 + 1;
      local_14 = local_14 + -1;
    } while (local_14 != 0);
    local_24 = local_24 + -1;
    local_20 = local_20 + 1;
    if (0x5e00cb < (int)local_28) {
      FUN_0047b590(iVar8,iVar5,local_2c,local_30);
      FUN_004767d0(iVar8,iVar5,local_2c,local_30,0,0);
      FUN_00477500(iVar8,iVar5,local_2c,local_30);
      FUN_004777c0(iVar8,iVar5,local_2c,local_30);
      FUN_00475cc0(iVar8,iVar5,local_2c,local_30,1);
      FUN_004776f0(iVar8,iVar5,local_2c,local_30);
      FUN_00477890(iVar8,iVar5,local_2c,local_30);
      DAT_00e38dca = 1;
      return;
    }
  } while( true );
}

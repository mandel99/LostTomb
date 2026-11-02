#include "restore_types.h"
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
#define DAT_00bca2f0 (*(undefined *)0x00bca2f0u)
#define DAT_00bca2f4 (*(undefined *)0x00bca2f4u)
#define DAT_00ee0c50 (*(undefined4 *)0x00ee0c50u)

uint FUN_004ad880(int *param_1,int *param_2)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  short *psVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  byte *pbVar12;
  int local_20;
  short *local_1c;
  int local_10;
  
  iVar11 = *param_2;
  iVar9 = *param_1;
  iVar3 = iVar9 + iVar11 * 0xe4 + DAT_00bca2d8;
  bVar1 = *(byte *)((int)&DAT_00ee0c50 + iVar3);
  if ((((-1 < (char)bVar1) && ((char)bVar1 < '\x05')) && ((bVar1 & 1) == 0)) &&
     (*(short *)(&DAT_00b4e6fc + iVar3 * 2) == 0)) {
    return CONCAT31((int3)((uint)iVar11 >> 8),1);
  }
  local_10 = 1;
  uVar8 = iVar11 + 1 + DAT_00bca2ec;
  iVar3 = iVar9 + 1 + DAT_00bca2e8;
  uVar2 = iVar11 + -1 + DAT_00bca2ec;
  iVar11 = iVar9 + -1 + DAT_00bca2e8;
  do {
    iVar9 = iVar11;
    if (DAT_00bca2e8 > iVar11) {
      iVar9 = DAT_00bca2e8;
    }
    local_20 = iVar3;
    if (iVar3 > DAT_00bca2f0) {
      local_20 = DAT_00bca2f0;
    }
    uVar5 = uVar2;
    if ((int)DAT_00bca2ec > (int)uVar2) {
      uVar5 = DAT_00bca2ec;
    }
    uVar6 = uVar8;
    if ((int)uVar8 > (int)DAT_00bca2f4) {
      uVar6 = DAT_00bca2f4;
    }
    if (((int)DAT_00bca2ec <= (int)uVar2) && (iVar9 <= local_20)) {
      iVar10 = iVar9 + uVar5 * 0xe4;
      pbVar12 = (byte *)((int)&DAT_00ee0c50 + iVar10);
      local_1c = (short *)(&DAT_00b4e6fc + iVar10 * 2);
      iVar10 = iVar9;
      do {
        bVar1 = *pbVar12;
        if (((-1 < (char)bVar1) && ((char)bVar1 < '\x05')) &&
           (((bVar1 & 1) == 0 && (uVar4 = uVar5, *local_1c == 0)))) goto LAB_004adb0c;
        iVar10 = iVar10 + 1;
        local_1c = local_1c + 1;
        pbVar12 = pbVar12 + 1;
      } while (iVar10 <= local_20);
    }
    if (((int)uVar8 <= (int)DAT_00bca2f4) && (iVar9 <= local_20)) {
      iVar10 = iVar9 + uVar6 * 0xe4;
      pbVar12 = (byte *)((int)&DAT_00ee0c50 + iVar10);
      psVar7 = (short *)(&DAT_00b4e6fc + iVar10 * 2);
      iVar10 = iVar9;
      do {
        bVar1 = *pbVar12;
        if ((((-1 < (char)bVar1) && ((char)bVar1 < '\x05')) && ((bVar1 & 1) == 0)) &&
           (uVar4 = uVar6, *psVar7 == 0)) goto LAB_004adb0c;
        iVar10 = iVar10 + 1;
        psVar7 = psVar7 + 1;
        pbVar12 = pbVar12 + 1;
      } while (iVar10 <= local_20);
    }
    if ((DAT_00bca2e8 <= iVar11) && (iVar10 = iVar9 + uVar5 * 0xe4, (int)uVar5 <= (int)uVar6)) {
      pbVar12 = (byte *)((int)&DAT_00ee0c50 + iVar10);
      psVar7 = (short *)(&DAT_00b4e6fc + iVar10 * 2);
      uVar4 = uVar5;
      do {
        bVar1 = *pbVar12;
        if (((-1 < (char)bVar1) && ((char)bVar1 < '\x05')) &&
           (((bVar1 & 1) == 0 && (iVar10 = iVar9, *psVar7 == 0)))) goto LAB_004adb0c;
        uVar4 = uVar4 + 1;
        pbVar12 = pbVar12 + 0xe4;
        psVar7 = psVar7 + 0xe4;
      } while ((int)uVar4 <= (int)uVar6);
    }
    if ((iVar3 <= DAT_00bca2f0) && (iVar9 = local_20 + uVar5 * 0xe4, (int)uVar5 <= (int)uVar6)) {
      pbVar12 = (byte *)((int)&DAT_00ee0c50 + iVar9);
      psVar7 = (short *)(&DAT_00b4e6fc + iVar9 * 2);
      do {
        bVar1 = *pbVar12;
        if ((((-1 < (char)bVar1) && ((char)bVar1 < '\x05')) && ((bVar1 & 1) == 0)) &&
           (uVar4 = uVar5, iVar10 = local_20, *psVar7 == 0)) {
LAB_004adb0c: ;
          *param_1 = iVar10 - DAT_00bca2e8;
          uVar8 = DAT_00bca2ec;
          *param_2 = uVar4 - DAT_00bca2ec;
          return CONCAT31((int3)(uVar8 >> 8),1);
        }
        uVar5 = uVar5 + 1;
        pbVar12 = pbVar12 + 0xe4;
        psVar7 = psVar7 + 0xe4;
      } while ((int)uVar5 <= (int)uVar6);
    }
    local_10 = local_10 + 1;
    iVar3 = iVar3 + 1;
    iVar11 = iVar11 + -1;
    uVar2 = uVar2 - 1;
    uVar8 = uVar8 + 1;
    if (0xe3 < local_10) {
      return uVar2 & 0xffffff00;
    }
  } while( true );
}

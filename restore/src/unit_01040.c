#include "restore_types.h"
#define DAT_005dbcc8 (*(undefined4 *)0x005dbcc8u)
#define DAT_005dbcd8 (*(undefined4 *)0x005dbcd8u)
#define DAT_005dbce8 (*(undefined4 *)0x005dbce8u)
#define DAT_005dbcf8 (*(undefined4 *)0x005dbcf8u)
#define DAT_005dbd08 (*(undefined4 *)0x005dbd08u)
#define DAT_005dbd18 (*(undefined4 *)0x005dbd18u)
#define DAT_005dbd28 (*(undefined4 *)0x005dbd28u)
#define DAT_005dbd38 (*(undefined4 *)0x005dbd38u)
#define DAT_005dbd48 (*(undefined4 *)0x005dbd48u)
#define DAT_005dbd58 (*(undefined4 *)0x005dbd58u)
#define DAT_005dbd68 (*(undefined4 *)0x005dbd68u)
#define DAT_005dbd78 (*(undefined4 *)0x005dbd78u)
#define DAT_005dbd88 (*(undefined4 *)0x005dbd88u)
#define DAT_005dbd98 (*(undefined4 *)0x005dbd98u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bc97c0 (*(undefined *)0x00bc97c0u)
#define DAT_00bc97c4 (*(undefined *)0x00bc97c4u)
#define DAT_00bc97c8 (*(undefined *)0x00bc97c8u)
#define DAT_00bc97cc (*(undefined *)0x00bc97ccu)
#define DAT_00bc97d0 (*(undefined *)0x00bc97d0u)
#define DAT_00bc97d4 (*(undefined *)0x00bc97d4u)
#define DAT_00bc97d8 (*(undefined *)0x00bc97d8u)
#define DAT_00bc97dc (*(undefined *)0x00bc97dcu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
extern void FUN_0047dbc0();

int FUN_00475fd0(int param_1)

{
  short sVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  undefined4 *puVar6;
  int *piVar7;
  int local_60 [24];
  
  piVar3 = local_60 + 8;
  for (iVar4 = 0x10; iVar4 != 0; iVar4 = iVar4 + -1) {
    *piVar3 = 0;
    piVar3 = piVar3 + 1;
  }
  puVar6 = &DAT_00bc97c0;
  for (iVar4 = 0x10; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar6 = 0;
    puVar6 = puVar6 + 1;
  }
  local_60[0] = -0xe4;
  local_60[1] = 0xffffff1d;
  local_60[2] = 1;
  local_60[3] = 0xe5;
  local_60[4] = 0xe4;
  local_60[5] = 0xe3;
  local_60[6] = 0xffffffff;
  local_60[7] = 0xffffff1b;
  DAT_00bc9768 = param_1;
  FUN_0047dbc0(0x20000002);
  piVar3 = &DAT_00bc97c0;
  piVar7 = local_60 + 8;
  for (iVar4 = 8; iVar4 != 0; iVar4 = iVar4 + -1) {
    *piVar7 = *piVar3;
    piVar3 = piVar3 + 1;
    piVar7 = piVar7 + 1;
  }
  FUN_0047dbc0(0x80000);
  iVar5 = 0;
  iVar4 = 0;
  do {
    if (*(int *)((int)&DAT_00bc97c0 + iVar4) == 0) {
      *(undefined4 *)((int)&DAT_00bc97c0 + iVar4) = *(undefined4 *)((int)local_60 + iVar4 + 0x20);
    }
    else {
      *(undefined4 *)((int)&DAT_00bc97c0 + iVar4) = 0;
    }
    iVar2 = DAT_00bc9768;
    iVar4 = iVar4 + 4;
  } while (iVar4 < 0x20);
  do {
    iVar4 = (int)*(short *)(&DAT_00b4e6fc + (*(int *)((int)local_60 + iVar5) + iVar2) * 2);
    if (0 < iVar4) {
      sVar1 = (&DAT_007999e8)[iVar4 * 0x84];
      if ((sVar1 == 0xe6) ||
         (((((sVar1 == 0xe5 || (sVar1 == 0xea)) || (sVar1 == 0xeb)) || (sVar1 == 0xec)) &&
          ((*(int *)(&DAT_00799a40 + iVar4 * 0x84) != 0x12 &&
           ((&DAT_00799a30)[(iVar4 - (short)(&DAT_007999ea)[iVar4 * 0x84]) * 0x42] == 1)))))) {
        *(undefined4 *)((int)&DAT_00bc97c0 + iVar5) = 1;
      }
    }
    iVar5 = iVar5 + 4;
  } while (iVar5 < 0x20);
  iVar4 = 0;
  piVar3 = &DAT_00bc97c0;
  do {
    if (*piVar3 == 1) {
      iVar4 = iVar4 + 1;
    }
    piVar3 = piVar3 + 1;
  } while ((int)piVar3 < 0xbc97e0);
  if (iVar4 == 8) {
    return (uint)(byte)(&DAT_00acf85c)[param_1] % 0x18 + 0x30;
  }
  if (iVar4 == 7) {
    if (DAT_00bc97c4 == 0) {
      return *(int *)(&DAT_005dbcc8 + (DAT_00bca2e0 / 2) * 4) +
             (uint)(byte)(&DAT_00acf85c)[param_1] % 3;
    }
    if (DAT_00bc97dc == 0) {
      return *(int *)(&DAT_005dbcd8 + (DAT_00bca2e0 / 2) * 4) +
             (uint)(byte)(&DAT_00acf85c)[param_1] % 3;
    }
    if (DAT_00bc97d4 == 0) {
      return *(int *)(&DAT_005dbce8 + (DAT_00bca2e0 / 2) * 4) +
             (uint)(byte)(&DAT_00acf85c)[param_1] % 3;
    }
    if (DAT_00bc97cc == 0) {
      return *(int *)(&DAT_005dbcf8 + (DAT_00bca2e0 / 2) * 4) +
             (uint)(byte)(&DAT_00acf85c)[param_1] % 3;
    }
  }
  else if (iVar4 == 6) {
    if ((DAT_00bc97dc == 0) && (DAT_00bc97cc == 0)) {
      return *(int *)(&DAT_005dbd88 + (DAT_00bca2e0 / 2) * 4) +
             (uint)(byte)(&DAT_00acf85c)[param_1] % 3;
    }
    if ((DAT_00bc97c4 == 0) && (DAT_00bc97d4 == 0)) {
      return *(int *)(&DAT_005dbd98 + (DAT_00bca2e0 / 2) * 4) +
             (uint)(byte)(&DAT_00acf85c)[param_1] % 3;
    }
  }
  if (((((DAT_00bc97d8 != 0) && (DAT_00bc97dc != 0)) && (DAT_00bc97c0 != 0)) &&
      ((DAT_00bc97c4 != 0 && (DAT_00bc97c8 != 0)))) && (iVar4 < 8)) {
    return *(int *)(&DAT_005dbd08 + (DAT_00bca2e0 / 2) * 4) +
           (uint)(byte)(&DAT_00acf85c)[param_1] % 6;
  }
  if (((DAT_00bc97d0 != 0) && (DAT_00bc97d4 != 0)) &&
     ((DAT_00bc97d8 != 0 && (((DAT_00bc97dc != 0 && (DAT_00bc97c0 != 0)) && (iVar4 < 8)))))) {
    return *(int *)(&DAT_005dbd18 + (DAT_00bca2e0 / 2) * 4) +
           (uint)(byte)(&DAT_00acf85c)[param_1] % 6;
  }
  if (((DAT_00bc97c8 != 0) && (DAT_00bc97cc != 0)) &&
     (((DAT_00bc97d0 != 0 && ((DAT_00bc97d4 != 0 && (DAT_00bc97d8 != 0)))) && (iVar4 < 8)))) {
    return *(int *)(&DAT_005dbd28 + (DAT_00bca2e0 / 2) * 4) +
           (uint)(byte)(&DAT_00acf85c)[param_1] % 6;
  }
  if ((DAT_00bc97c0 != 0) && (DAT_00bc97c4 != 0)) {
    if ((DAT_00bc97c8 != 0) && (((DAT_00bc97cc != 0 && (DAT_00bc97d0 != 0)) && (iVar4 < 8)))) {
      return *(int *)(&DAT_005dbd38 + (DAT_00bca2e0 / 2) * 4) +
             (uint)(byte)(&DAT_00acf85c)[param_1] % 6;
    }
    if ((DAT_00bc97c4 != 0) && (DAT_00bc97c8 != 0)) {
      return *(int *)(&DAT_005dbd48 + (DAT_00bca2e0 / 2) * 4) +
             (uint)(byte)(&DAT_00acf85c)[param_1] % 3;
    }
  }
  if (((DAT_00bc97d8 != 0) && (DAT_00bc97dc != 0)) && (DAT_00bc97c0 != 0)) {
    return *(int *)(&DAT_005dbd58 + (DAT_00bca2e0 / 2) * 4) +
           (uint)(byte)(&DAT_00acf85c)[param_1] % 3;
  }
  if (((DAT_00bc97d0 != 0) && (DAT_00bc97d4 != 0)) && (DAT_00bc97d8 != 0)) {
    return *(int *)(&DAT_005dbd68 + (DAT_00bca2e0 / 2) * 4) +
           (uint)(byte)(&DAT_00acf85c)[param_1] % 3;
  }
  if (((DAT_00bc97c8 != 0) && (DAT_00bc97cc != 0)) && (DAT_00bc97d0 != 0)) {
    return *(int *)(&DAT_005dbd78 + (DAT_00bca2e0 / 2) * 4) +
           (uint)(byte)(&DAT_00acf85c)[param_1] % 3;
  }
  return -1;
}

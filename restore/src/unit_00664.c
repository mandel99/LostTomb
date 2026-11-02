#include "restore_types.h"
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
#define DAT_00bca2f0 (*(undefined *)0x00bca2f0u)
#define DAT_00bca2f4 (*(undefined *)0x00bca2f4u)
#define DAT_00ed4140 (*(undefined4 *)0x00ed4140u)

uint FUN_004467a0(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  char *pcVar6;
  int iVar7;
  short *psVar8;
  int iVar9;
  int iVar10;
  int local_1c;
  int local_18;
  uint local_8;
  
  iVar7 = *param_2;
  iVar10 = *param_1;
  iVar3 = iVar10 + iVar7 * 0xe4 + DAT_00bca2d8;
  if ((-1 < (char)(&DAT_00ed4140)[iVar3]) && (*(short *)(&DAT_00b350dc + iVar3 * 2) == 0)) {
    return CONCAT31((int3)((uint)iVar7 >> 8),1);
  }
  local_8 = 1;
  local_18 = iVar7 + 1 + DAT_00bca2ec;
  local_1c = iVar7 + -1 + DAT_00bca2ec;
  iVar7 = iVar10 + 1 + DAT_00bca2e8;
  iVar10 = iVar10 + -1 + DAT_00bca2e8;
  do {
    iVar3 = iVar10;
    if (DAT_00bca2e8 > iVar10) {
      iVar3 = DAT_00bca2e8;
    }
    iVar5 = iVar7;
    if (iVar7 > DAT_00bca2f0) {
      iVar5 = DAT_00bca2f0;
    }
    iVar2 = local_1c;
    if (DAT_00bca2ec > local_1c) {
      iVar2 = DAT_00bca2ec;
    }
    iVar9 = local_18;
    if (local_18 > DAT_00bca2f4) {
      iVar9 = DAT_00bca2f4;
    }
    if ((DAT_00bca2ec <= local_1c) && (iVar3 <= iVar5)) {
      iVar4 = iVar3 + iVar2 * 0xe4;
      pcVar6 = &DAT_00ed4140 + iVar4;
      psVar8 = (short *)(&DAT_00b350dc + iVar4 * 2);
      iVar4 = iVar3;
      do {
        if ((-1 < *pcVar6) && (iVar1 = iVar2, *psVar8 == 0)) goto LAB_004469f3;
        iVar4 = iVar4 + 1;
        psVar8 = psVar8 + 1;
        pcVar6 = pcVar6 + 1;
      } while (iVar4 <= iVar5);
    }
    if ((local_18 <= DAT_00bca2f4) && (iVar3 <= iVar5)) {
      iVar4 = iVar3 + iVar9 * 0xe4;
      pcVar6 = &DAT_00ed4140 + iVar4;
      psVar8 = (short *)(&DAT_00b350dc + iVar4 * 2);
      iVar4 = iVar3;
      do {
        if ((-1 < *pcVar6) && (iVar1 = iVar9, *psVar8 == 0)) goto LAB_004469f3;
        iVar4 = iVar4 + 1;
        psVar8 = psVar8 + 1;
        pcVar6 = pcVar6 + 1;
      } while (iVar4 <= iVar5);
    }
    if ((DAT_00bca2e8 <= iVar10) && (iVar4 = iVar3 + iVar2 * 0xe4, iVar2 <= iVar9)) {
      pcVar6 = &DAT_00ed4140 + iVar4;
      psVar8 = (short *)(&DAT_00b350dc + iVar4 * 2);
      iVar1 = iVar2;
      do {
        if ((-1 < *pcVar6) && (iVar4 = iVar3, *psVar8 == 0)) goto LAB_004469f3;
        iVar1 = iVar1 + 1;
        pcVar6 = pcVar6 + 0xe4;
        psVar8 = psVar8 + 0xe4;
      } while (iVar1 <= iVar9);
    }
    if ((iVar7 <= DAT_00bca2f0) && (iVar3 = iVar5 + iVar2 * 0xe4, iVar2 <= iVar9)) {
      pcVar6 = &DAT_00ed4140 + iVar3;
      psVar8 = (short *)(&DAT_00b350dc + iVar3 * 2);
      do {
        if ((-1 < *pcVar6) && (iVar1 = iVar2, iVar4 = iVar5, *psVar8 == 0)) {
LAB_004469f3: ;
          *param_1 = iVar4 - DAT_00bca2e8;
          iVar7 = DAT_00bca2ec;
          *param_2 = iVar1 - DAT_00bca2ec;
          return CONCAT31((int3)((uint)iVar7 >> 8),1);
        }
        iVar2 = iVar2 + 1;
        pcVar6 = pcVar6 + 0xe4;
        psVar8 = psVar8 + 0xe4;
      } while (iVar2 <= iVar9);
    }
    local_8 = local_8 + 1;
    iVar7 = iVar7 + 1;
    iVar10 = iVar10 + -1;
    local_1c = local_1c + -1;
    local_18 = local_18 + 1;
    if (0xe3 < (int)local_8) {
      return local_8 & 0xffffff00;
    }
  } while( true );
}

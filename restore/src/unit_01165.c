#include "restore_types.h"
#define DAT_00adc1a4 (*(undefined4 *)0x00adc1a4u)
#define DAT_00adc288 (*(undefined4 *)0x00adc288u)
#define DAT_00adc36a (*(undefined4 *)0x00adc36au)
#define DAT_00adc36b (*(undefined4 *)0x00adc36bu)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00adc36d (*(undefined4 *)0x00adc36du)
#define DAT_00adc36e (*(undefined4 *)0x00adc36eu)
#define DAT_00adc450 (*(undefined4 *)0x00adc450u)
#define DAT_00adc534 (*(undefined4 *)0x00adc534u)
#define DAT_00af526c (*(undefined4 *)0x00af526cu)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5984 (*(undefined4 *)0x00af5984u)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5994 (*(undefined4 *)0x00af5994u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00af60ac (*(undefined4 *)0x00af60acu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)

uint FUN_0047f9b0(uint param_1)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  byte *pbVar5;
  uint uVar6;
  int iVar7;
  undefined4 *puVar8;
  short *psVar9;
  byte *pbVar10;
  
  if ((*(byte *)(&DAT_00af598c + param_1) & 4) == 0) {
    return 0;
  }
  if ((&DAT_00adc36c)[param_1] != '\0') {
    uVar6 = 0;
    uVar2 = 0;
    if ((((&DAT_00af55fc)[param_1 * 4] & 4) != 0) && ((&DAT_00adc288)[param_1] != '\0')) {
      uVar6 = 1;
    }
    if ((((&DAT_00af526c)[param_1 * 4] & 4) != 0) && ((&DAT_00adc1a4)[param_1] != '\0')) {
      uVar6 = uVar6 + 1;
    }
    if ((((&DAT_00af5d1c)[param_1 * 4] & 4) != 0) && ((&DAT_00adc450)[param_1] != '\0')) {
      uVar6 = uVar6 + 1;
    }
    if ((((&DAT_00af60ac)[param_1 * 4] & 4) != 0) && ((&DAT_00adc534)[param_1] != '\0')) {
      uVar6 = uVar6 + 1;
    }
    if ((((&DAT_00af5990)[param_1 * 4] & 4) != 0) && ((&DAT_00adc36d)[param_1] != '\0')) {
      uVar2 = 1;
    }
    if ((((&DAT_00af5994)[param_1 * 4] & 4) != 0) && ((&DAT_00adc36e)[param_1] != '\0')) {
      uVar2 = uVar2 + 1;
    }
    if ((((&DAT_00af5988)[param_1 * 4] & 4) != 0) && ((&DAT_00adc36b)[param_1] != '\0')) {
      uVar2 = uVar2 + 1;
    }
    if ((((&DAT_00af5984)[param_1 * 4] & 4) != 0) && ((&DAT_00adc36a)[param_1] != '\0')) {
      uVar2 = uVar2 + 1;
    }
    if (uVar6 < uVar2) {
      iVar3 = -1;
      iVar7 = 1;
    }
    else {
      iVar3 = -0xe4;
      iVar7 = 0xe4;
    }
    bVar1 = *(byte *)(&DAT_00af598c + iVar3 + param_1);
    puVar8 = &DAT_00af598c + iVar3 + param_1;
    for (iVar4 = param_1; ((bVar1 & 4) != 0 && ((&DAT_00adc36c)[iVar4 + iVar3] != '\0'));
        iVar4 = iVar4 + iVar3) {
      bVar1 = *(byte *)(puVar8 + iVar3);
      puVar8 = puVar8 + iVar3;
    }
    psVar9 = (short *)(&DAT_00b350dc + iVar4 * 2);
    param_1 = (uint)(*(short *)(&DAT_00b350dc + iVar4 * 2) != 0);
    pbVar5 = &DAT_00b285cc + iVar4;
    *pbVar5 = (&DAT_00b285cc)[iVar4] & 0xbf;
    pbVar10 = (byte *)(&DAT_00af598c + iVar4 + iVar7);
    if ((*(byte *)(&DAT_00af598c + iVar4 + iVar7) & 4) != 0) {
      do {
        if (pbVar5[iVar7 + -0x4c260] == 0) {
          return param_1;
        }
        psVar9 = psVar9 + iVar7;
        pbVar5 = pbVar5 + iVar7;
        pbVar10 = pbVar10 + iVar7 * 4;
        if (*psVar9 != 0) {
          param_1 = param_1 + 1;
        }
        *pbVar5 = *pbVar5 & 0xbf;
      } while ((*pbVar10 & 4) != 0);
    }
    return param_1;
  }
  return 0;
}

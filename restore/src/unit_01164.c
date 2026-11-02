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

void FUN_0047f810(int param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  uint *puVar3;
  uint uVar4;
  byte *pbVar5;
  int iVar6;
  
  if (((*(byte *)(&DAT_00af598c + param_1) & 4) != 0) && ((&DAT_00adc36c)[param_1] != '\0')) {
    uVar4 = 0;
    uVar1 = 0;
    if ((((&DAT_00af55fc)[param_1 * 4] & 4) != 0) && ((&DAT_00adc288)[param_1] != '\0')) {
      uVar4 = 1;
    }
    if ((((&DAT_00af526c)[param_1 * 4] & 4) != 0) && ((&DAT_00adc1a4)[param_1] != '\0')) {
      uVar4 = uVar4 + 1;
    }
    if ((((&DAT_00af5d1c)[param_1 * 4] & 4) != 0) && ((&DAT_00adc450)[param_1] != '\0')) {
      uVar4 = uVar4 + 1;
    }
    if ((((&DAT_00af60ac)[param_1 * 4] & 4) != 0) && ((&DAT_00adc534)[param_1] != '\0')) {
      uVar4 = uVar4 + 1;
    }
    if ((((&DAT_00af5990)[param_1 * 4] & 4) != 0) && ((&DAT_00adc36d)[param_1] != '\0')) {
      uVar1 = 1;
    }
    if ((((&DAT_00af5994)[param_1 * 4] & 4) != 0) && ((&DAT_00adc36e)[param_1] != '\0')) {
      uVar1 = uVar1 + 1;
    }
    if ((((&DAT_00af5988)[param_1 * 4] & 4) != 0) && ((&DAT_00adc36b)[param_1] != '\0')) {
      uVar1 = uVar1 + 1;
    }
    if ((((&DAT_00af5984)[param_1 * 4] & 4) != 0) && ((&DAT_00adc36a)[param_1] != '\0')) {
      uVar1 = uVar1 + 1;
    }
    if (uVar4 < uVar1) {
      iVar2 = -1;
      iVar6 = 1;
    }
    else {
      iVar2 = -0xe4;
      iVar6 = 0xe4;
    }
    pbVar5 = (byte *)(&DAT_00af598c + iVar2 + param_1);
    if ((*(byte *)(&DAT_00af598c + iVar2 + param_1) & 4) != 0) {
      do {
        if ((&DAT_00adc36c)[param_1 + iVar2] == '\0') break;
        pbVar5 = pbVar5 + iVar2 * 4;
        param_1 = param_1 + iVar2;
      } while ((*pbVar5 & 4) != 0);
    }
    if (param_2 == 0) {
      uVar1 = (&DAT_00af598c)[param_1];
      (&DAT_00adc36c)[param_1] = 0;
      (&DAT_00af598c)[param_1] = uVar1 & 0xffffffbf;
    }
    else {
      (&DAT_00b285cc)[param_1] = (&DAT_00b285cc)[param_1] | 0x40;
    }
    pbVar5 = (byte *)(&DAT_00af598c + iVar6 + param_1);
    if ((*(byte *)(&DAT_00af598c + iVar6 + param_1) & 4) != 0) {
      puVar3 = &DAT_00af598c + param_1;
      do {
        if ((&DAT_00adc36c)[param_1 + iVar6] == '\0') {
          return;
        }
        param_1 = param_1 + iVar6;
        puVar3 = puVar3 + iVar6;
        pbVar5 = pbVar5 + iVar6 * 4;
        if (param_2 == 0) {
          *puVar3 = *puVar3 & 0xffffffbf;
          (&DAT_00adc36c)[param_1] = 0;
        }
        else {
          (&DAT_00b285cc)[param_1] = (&DAT_00b285cc)[param_1] | 0x40;
        }
      } while ((*pbVar5 & 4) != 0);
    }
  }
  return;
}

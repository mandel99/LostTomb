#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00eaac44 (*(undefined *)0x00eaac44u)
#define DAT_00eaaf58 (*(undefined *)0x00eaaf58u)

void FUN_004ef450(int param_1)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  byte bVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  byte *pbVar8;
  
  iVar7 = 0;
  DAT_00eaaf58 = 0;
  bVar1 = *(byte *)(&DAT_00799a2c + param_1 * 0x84);
  param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  pbVar8 = (byte *)(&DAT_00799a2e + param_1 * 0x84);
  iVar6 = param_1;
  do {
    if (DAT_005e4ef4 == 5) {
      iVar5 = *(int *)(&DAT_0056f7ec + (uint)bVar1 * 4);
    }
    else {
      iVar5 = *(int *)(&DAT_0056f7d8 + (uint)bVar1 * 4);
    }
    if (iVar5 + param_1 <= iVar6) {
      return;
    }
    bVar2 = pbVar8[-1];
    if ((bVar2 != 0) && (((*pbVar8 == 0 || (DAT_005e4ef4 == 3)) || (DAT_005e4ef4 == 5))))
    goto LAB_004ef53b;
    if (iVar7 == 0) {
LAB_004ef51d: ;
      DAT_00eaaf58 = 0;
      iVar7 = iVar6;
LAB_004ef529: ;
      (&DAT_00eaac44)[DAT_00eaaf58] = iVar6;
      DAT_00eaaf58 = DAT_00eaaf58 + 1;
    }
    else {
      if (bVar2 != *(byte *)((int)&DAT_00799a2c + iVar7 * 0x108 + 1)) {
        if (bVar2 != 0) goto LAB_004ef53b;
        goto LAB_004ef51d;
      }
      bVar3 = *(byte *)((int)&DAT_00799a2e + iVar7 * 0x108 + 1);
      bVar4 = pbVar8[1];
      if (bVar2 != 0) {
        if (bVar3 < bVar4) goto LAB_004ef51d;
        if (bVar4 != bVar3) goto LAB_004ef53b;
        if (*(byte *)(&DAT_00799a2e + iVar7 * 0x84) < *pbVar8) goto LAB_004ef51d;
        if (*pbVar8 != *(byte *)(&DAT_00799a2e + iVar7 * 0x84)) goto LAB_004ef53b;
        goto LAB_004ef529;
      }
      if (bVar4 < bVar3) goto LAB_004ef51d;
      if (bVar4 == bVar3) {
        if (*pbVar8 < *(byte *)(&DAT_00799a2e + iVar7 * 0x84)) goto LAB_004ef51d;
        if (*pbVar8 == *(byte *)(&DAT_00799a2e + iVar7 * 0x84)) goto LAB_004ef529;
      }
    }
LAB_004ef53b: ;
    iVar6 = iVar6 + 1;
    pbVar8 = pbVar8 + 0x108;
  } while( true );
}

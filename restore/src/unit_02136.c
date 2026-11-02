#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005e68d8 (*(undefined4 *)0x005e68d8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
extern void FUN_004f5e70();

void FUN_004f31f0(int param_1)

{
  byte bVar1;
  byte bVar2;
  char cVar3;
  int iVar4;
  char *pcVar5;
  int iVar6;
  byte bVar7;
  int iVar8;
  bool bVar9;
  
  iVar4 = param_1 * 0x108;
  iVar8 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  bVar1 = *(byte *)(&DAT_00799a2c + iVar8 * 0x84);
  *(short *)(&DAT_00799aa0 + iVar8 * 0x42) = *(short *)(&DAT_00799aa0 + iVar8 * 0x42) + -1;
  iVar6 = DAT_005e4ef4;
  if (*(char *)((int)&DAT_00799a2c + iVar4 + 1) == '\0') {
    bVar7 = *(char *)(&DAT_00799a2e + param_1 * 0x84) + 1;
    bVar9 = DAT_005e4ef4 == 1;
    *(byte *)(&DAT_00799a2e + param_1 * 0x84) = bVar7;
    if (bVar9) {
      iVar6 = (-(uint)((&DAT_005e68d8)
                       [(uint)bVar1 * 6 + (uint)*(byte *)((int)&DAT_00799a2e + iVar4 + 1)] != '\0')
              & 0xfffffffd) + 6;
    }
    else if ((iVar6 != 5) || (iVar6 = 1, iVar8 != param_1)) {
      iVar6 = 6;
    }
    if (iVar6 <= (int)(uint)bVar7) {
      bVar2 = *(byte *)((int)&DAT_00799a2e + iVar4 + 1);
      if (bVar2 < *(byte *)(&DAT_00799a30 + param_1 * 0x42)) {
        if (iVar6 < (int)(uint)bVar7) {
          *(byte *)((int)&DAT_00799a2e + iVar4 + 1) = bVar2 + 1;
          *(undefined1 *)(&DAT_00799a2e + param_1 * 0x84) = 1;
        }
      }
      else {
        *(undefined1 *)((int)&DAT_00799a2c + iVar4 + 1) = 1;
        if (iVar6 < 1) {
          iVar6 = 1;
        }
        *(char *)(&DAT_00799a2e + param_1 * 0x84) = (char)iVar6;
      }
    }
  }
  else {
    cVar3 = *(char *)(&DAT_00799a2e + param_1 * 0x84);
    *(byte *)(&DAT_00799a2e + param_1 * 0x84) = cVar3 - 1U;
    if ((byte)(cVar3 - 1U) < *(byte *)((int)&DAT_00799a30 + iVar4 + 1)) {
      *(undefined1 *)((int)&DAT_00799a30 + iVar4 + 1) = 0;
    }
    if (*(byte *)(&DAT_00799a2e + param_1 * 0x84) < *(byte *)((int)&DAT_00799a30 + iVar4 + 2)) {
      *(undefined1 *)((int)&DAT_00799a30 + iVar4 + 2) = 0;
    }
  }
  if (*(short *)(&DAT_00799aa0 + iVar8 * 0x42) == 0) {
    FUN_004f5e70(iVar8,0);
  }
  if ((DAT_005e4ef4 == 3) && (*(short *)(&DAT_00799aa0 + iVar8 * 0x42) == 0)) {
    iVar6 = *(int *)(&DAT_0056f7d8 + (uint)bVar1 * 4);
    pcVar5 = (char *)((int)&DAT_00799a30 + iVar8 * 0x108 + 2);
    for (iVar4 = 0; iVar4 < iVar6; iVar4 = iVar4 + 1) {
      if ((pcVar5[-1] != '\0') || (*pcVar5 != '\0')) {
        pcVar5[-1] = '\0';
        *pcVar5 = '\0';
      }
      pcVar5 = pcVar5 + 0x108;
    }
  }
  return;
}

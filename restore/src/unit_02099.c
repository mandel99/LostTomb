#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ee8 (*(undefined4 *)0x005e4ee8u)
#define DAT_005e4eec (*(undefined4 *)0x005e4eecu)
#define DAT_005e4ef0 (*(undefined4 *)0x005e4ef0u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005e68d8 (*(undefined4 *)0x005e68d8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd646 (*(undefined4 *)0x00bcd646u)
#define DAT_00bcd648 (*(undefined4 *)0x00bcd648u)
#define DAT_00bcd718 (*(undefined4 *)0x00bcd718u)
extern void FUN_004edf50();
extern uint FUN_004eea10();
extern void FUN_004f02f0();
extern void FUN_004f0ef0();
extern int FUN_004f2190();
extern void FUN_004f5e70();

void FUN_004eeb60(int param_1,int param_2)

{
  byte bVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  undefined4 uVar5;
  int iVar6;
  byte bVar7;
  uint uVar8;
  int iVar9;
  int iVar10;
  char *pcVar11;
  int iVar12;
  undefined4 uVar13;
  undefined4 local_8;
  
  iVar12 = param_2 * 0x184;
  if ((&DAT_00bcd602)[iVar12] == 'Q') {
    cVar2 = '\f';
    cVar3 = '\x10';
    local_8 = 0xf;
  }
  else if ((&DAT_00bcd602)[iVar12] == 'P') {
    local_8 = 0xf;
    cVar2 = '\t';
    cVar3 = '\t';
  }
  else {
    local_8 = 0;
    cVar3 = '\0';
    cVar2 = '\0';
  }
  iVar10 = param_1 * 0x108;
  iVar9 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  uVar8 = (uint)*(byte *)(&DAT_00799a2c + iVar9 * 0x84);
  if (*(short *)(&DAT_00799aa0 + iVar9 * 0x42) < 1) {
LAB_004eec00: ;
    (&DAT_00bcd630)[iVar12] = cVar2;
    *(undefined2 *)(&DAT_00bcd718 + iVar12) = 0;
    return;
  }
  if (((short)(&DAT_00799a3c)[param_1 * 0x84] != param_2) &&
     (cVar4 = FUN_004eea10(param_1), cVar4 != '\0')) {
    if (cVar3 == cVar2) {
      (&DAT_00bcd630)[iVar12] = cVar2;
      *(undefined2 *)(&DAT_00bcd718 + iVar12) = 0;
      return;
    }
    if (*(char *)((int)&DAT_00799a2c + iVar10 + 1) != '\x01') {
      FUN_004f0ef0(param_2,1,cVar2,cVar3,local_8);
      return;
    }
    (&DAT_00bcd630)[iVar12] = cVar3;
    uVar5 = FUN_004f2190((int)*(short *)(&DAT_00bcd646 + iVar12));
    uVar13 = 0;
LAB_004eeebd: ;
    FUN_004f02f0(param_2,uVar5,(int)*(short *)(&DAT_00bcd648 + iVar12),uVar13);
    return;
  }
  iVar6 = DAT_005e4ef0;
  if (*(char *)((int)&DAT_00799a2c + iVar10 + 1) == '\0') {
    iVar6 = DAT_005e4ee8 + DAT_005e4eec;
  }
  *(short *)(&DAT_00bcd62e + iVar12) = *(short *)(&DAT_00bcd62e + iVar12) + 1;
  if (iVar6 <= *(short *)(&DAT_00bcd62e + iVar12)) {
    (&DAT_00799a3c)[param_1 * 0x84] = 0;
    *(short *)(&DAT_00799aa0 + iVar9 * 0x42) = *(short *)(&DAT_00799aa0 + iVar9 * 0x42) + -1;
    *(undefined2 *)(&DAT_00bcd62e + iVar12) = 0;
    if (*(char *)((int)&DAT_00799a2c + iVar10 + 1) == '\0') {
      bVar7 = *(char *)(&DAT_00799a2e + param_1 * 0x84) + 1;
      *(byte *)(&DAT_00799a2e + param_1 * 0x84) = bVar7;
      if (DAT_005e4ef4 == 1) {
        iVar6 = (-(uint)((&DAT_005e68d8)
                         [uVar8 * 6 + (uint)*(byte *)((int)&DAT_00799a2e + iVar10 + 1)] != '\0') &
                0xfffffffd) + 6;
      }
      else if ((DAT_005e4ef4 != 5) || (iVar6 = 1, iVar9 != param_1)) {
        iVar6 = 6;
      }
      if (iVar6 <= (int)(uint)bVar7) {
        bVar1 = *(byte *)((int)&DAT_00799a2e + iVar10 + 1);
        if (bVar1 < *(byte *)(&DAT_00799a30 + param_1 * 0x42)) {
          if (iVar6 < (int)(uint)bVar7) {
            *(byte *)((int)&DAT_00799a2e + iVar10 + 1) = bVar1 + 1;
            *(undefined1 *)(&DAT_00799a2e + param_1 * 0x84) = 1;
          }
        }
        else {
          *(undefined1 *)((int)&DAT_00799a2c + iVar10 + 1) = 1;
          if (iVar6 < 1) {
            iVar6 = 1;
          }
          *(char *)(&DAT_00799a2e + param_1 * 0x84) = (char)iVar6;
        }
      }
    }
    else {
      cVar4 = *(char *)(&DAT_00799a2e + param_1 * 0x84);
      *(byte *)(&DAT_00799a2e + param_1 * 0x84) = cVar4 - 1U;
      if ((byte)(cVar4 - 1U) < *(byte *)((int)&DAT_00799a30 + iVar10 + 1)) {
        *(undefined1 *)((int)&DAT_00799a30 + iVar10 + 1) = 0;
      }
      if (*(byte *)(&DAT_00799a2e + param_1 * 0x84) < *(byte *)((int)&DAT_00799a30 + iVar10 + 2)) {
        *(undefined1 *)((int)&DAT_00799a30 + iVar10 + 2) = 0;
      }
    }
    iVar6 = DAT_00bca2e0 / 2;
    FUN_004edf50(param_1,iVar6,uVar8);
    if (*(short *)(&DAT_00799aa0 + iVar9 * 0x42) == 0) {
      FUN_004f5e70(iVar9,0);
    }
    if ((DAT_005e4ef4 == 3) && (*(short *)(&DAT_00799aa0 + iVar9 * 0x42) == 0)) {
      iVar12 = 0;
      pcVar11 = (char *)((int)&DAT_00799a30 + iVar9 * 0x108 + 2);
      while( true ) {
        if (DAT_005e4ef4 == 5) {
          iVar10 = *(int *)(&DAT_0056f7ec + uVar8 * 4);
        }
        else {
          iVar10 = *(int *)(&DAT_0056f7d8 + uVar8 * 4);
        }
        if (iVar10 <= iVar12) break;
        if ((pcVar11[-1] != '\0') || (*pcVar11 != '\0')) {
          pcVar11[-1] = '\0';
          *pcVar11 = '\0';
          FUN_004edf50(iVar12 + iVar9,iVar6,uVar8);
        }
        iVar12 = iVar12 + 1;
        pcVar11 = pcVar11 + 0x108;
      }
    }
    else {
      if ((*(char *)((int)&DAT_00799a2c + iVar10 + 1) == '\x01') &&
         (*(char *)((int)&DAT_00799a2c +
                   (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x108 + 1) != '\0')) {
        if (cVar3 == '\0') goto LAB_004eec00;
        (&DAT_00bcd630)[iVar12] = cVar3;
        uVar5 = FUN_004f2190((int)*(short *)(&DAT_00bcd646 + iVar12));
        uVar13 = 1;
        goto LAB_004eeebd;
      }
      FUN_004f0ef0(param_2,0,cVar2,cVar3,local_8);
    }
  }
  return;
}

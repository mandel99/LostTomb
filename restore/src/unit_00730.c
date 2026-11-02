#include "restore_types.h"
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_00785d74 (*(undefined4 *)0x00785d74u)
#define DAT_00785d78 (*(undefined4 *)0x00785d78u)
#define DAT_00785d80 (*(undefined4 *)0x00785d80u)
#define DAT_00785da2 (*(undefined4 *)0x00785da2u)
#define DAT_00785da4 (*(undefined4 *)0x00785da4u)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5fc8 (*(undefined4 *)0x00ea5fc8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern undefined4 FUN_0044c110();
extern int FUN_0045b4d0();
extern void FUN_004b84b0();

void FUN_0044c1a0(int param_1)

{
  short sVar1;
  undefined2 uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  int local_8;
  
  if ((-1 < param_1) && (param_1 < DAT_00785d70)) {
    iVar4 = param_1 * 0x7c;
    if (&DAT_00785d74 + param_1 * 0x3e != (short *)0x0) {
      iVar6 = (int)*(short *)(&DAT_00785d80 + iVar4);
      *(ushort *)(&DAT_00785da4 + iVar4) = (*(short *)(&DAT_00785da4 + iVar4) != 0) + 2;
      iVar3 = (int)(short)(&DAT_00785d78)[param_1 * 0x3e];
      if (iVar3 == 0x24) {
        (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
        DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
        if (99 < DAT_00d3b1c4) {
          DAT_00d3b1c4 = 0;
        }
        local_8 = 0x1f;
        do {
          uVar5 = DAT_00d3b384 >> 4;
          uVar8 = DAT_00d3b384 & 1;
          uVar9 = DAT_00d3b380 >> 4;
          uVar7 = DAT_00d3b380 & 1;
          DAT_00d3b384 = DAT_00d3b384 >> 1;
          DAT_00d3b380 = DAT_00d3b380 >> 1;
          if ((uVar5 & 1) != uVar8) {
            DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
          }
          if ((uVar9 & 1) != uVar7) {
            DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
          }
          local_8 = local_8 + -1;
        } while (local_8 != 0);
        _DAT_00d3b378 = DAT_00d3b384 & 7;
        _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
        _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
        DAT_00d3b374 = DAT_00d3b384 & 0x7f;
        _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
        _DAT_00d3b36c = DAT_00d3b380 & 7;
        *(ushort *)(&DAT_00785da2 + iVar4) = ((ushort)DAT_00d3b384 & 3) + 1;
        iVar4 = DAT_00eaac28 * 0x49d8;
        *(int *)(&DAT_00ea187c + iVar4) = *(int *)(&DAT_00ea187c + iVar4) - iVar6;
        *(int *)(&DAT_00ea5fc8 + iVar4) = *(int *)(&DAT_00ea5fc8 + iVar4) + iVar6;
        return;
      }
      if (iVar3 == 0x25) {
        *(undefined2 *)(&DAT_00785da4 + iVar4) = 5;
        uVar2 = FUN_0044c110();
        sVar1 = (&DAT_00785d74)[param_1 * 0x3e];
        *(undefined2 *)(&DAT_00785da2 + iVar4) = uVar2;
        FUN_004b84b0((int)sVar1);
        if (*(short *)(&DAT_00785da2 + iVar4) < 1) {
          *(undefined2 *)(&DAT_00785da2 + iVar4) = 0xc;
          return;
        }
      }
      else {
        (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
        DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
        if (99 < DAT_00d3b1c4) {
          DAT_00d3b1c4 = 0;
        }
        local_8 = 0x1f;
        do {
          uVar5 = DAT_00d3b384 >> 4;
          uVar8 = DAT_00d3b384 & 1;
          uVar9 = DAT_00d3b380 >> 4;
          uVar7 = DAT_00d3b380 & 1;
          DAT_00d3b384 = DAT_00d3b384 >> 1;
          DAT_00d3b380 = DAT_00d3b380 >> 1;
          if ((uVar5 & 1) != uVar8) {
            DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
          }
          if ((uVar9 & 1) != uVar7) {
            DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
          }
          local_8 = local_8 + -1;
        } while (local_8 != 0);
        _DAT_00d3b378 = DAT_00d3b384 & 7;
        _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
        _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
        DAT_00d3b374 = DAT_00d3b384 & 0x7f;
        _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
        _DAT_00d3b36c = DAT_00d3b380 & 7;
        *(ushort *)(&DAT_00785da2 + iVar4) = ((ushort)DAT_00d3b384 & 3) + 1;
        FUN_0045b4d0(DAT_00eaac28,iVar3,iVar6 * 100);
      }
      return;
    }
  }
  return;
}

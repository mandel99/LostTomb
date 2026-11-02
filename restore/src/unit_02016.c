#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d798 (*(undefined *)0x0078d798u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_0047fe60();
extern void FUN_004b1df0();
extern int FUN_004b4800();
extern undefined4 FUN_004e7c90();

int FUN_004e7ac0(int param_1)

{
  ulonglong uVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  int local_4;
  
  DAT_0078d798 = param_1;
  iVar7 = 1;
  iVar10 = 0;
  local_4 = 0;
  iVar4 = DAT_0078d790;
  if (0 < DAT_0078d790) {
    do {
      iVar4 = FUN_004e7c90(iVar7);
      iVar2 = DAT_0078d790;
      if ((char)iVar4 != '\0') {
        iVar10 = iVar10 + 1;
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 <= DAT_0078d790);
    if (0 < iVar10) {
      (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
      DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
      if (99 < DAT_00d3b1c4) {
        DAT_00d3b1c4 = 0;
      }
      param_1 = 0x1f;
      do {
        uVar5 = DAT_00d3b384 >> 4;
        uVar8 = DAT_00d3b384 & 1;
        uVar9 = DAT_00d3b380 >> 4;
        uVar6 = DAT_00d3b380 & 1;
        DAT_00d3b384 = DAT_00d3b384 >> 1;
        DAT_00d3b380 = DAT_00d3b380 >> 1;
        if ((uVar5 & 1) != uVar8) {
          DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
        }
        if ((uVar9 & 1) != uVar6) {
          DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
        }
        param_1 = param_1 + -1;
      } while (param_1 != 0);
      _DAT_00d3b378 = DAT_00d3b384 & 7;
      _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
      _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
      _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
      DAT_00d3b374 = DAT_00d3b384 & 0x7f;
      uVar1 = (ulonglong)_DAT_00d3b370;
      iVar4 = (int)((longlong)uVar1 / (longlong)iVar10);
      _DAT_00d3b36c = DAT_00d3b380 & 7;
      iVar7 = 1;
      if (0 < iVar2) {
        while ((cVar3 = FUN_004e7c90(iVar7), cVar3 == '\0' ||
               (local_4 = local_4 + 1, local_4 != (int)((longlong)uVar1 % (longlong)iVar10) + 1))) {
          iVar7 = iVar7 + 1;
          if (DAT_0078d790 < iVar7) {
            return DAT_0078d790;
          }
        }
        FUN_0047fe60((int)(short)(&DAT_007999fe)[iVar7 * 0x84],
                     (int)(short)(&DAT_00799a00)[iVar7 * 0x84],1,6);
        iVar4 = FUN_004b4800(DAT_00eaac28,0x6d,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
        iVar10 = iVar4 * 0x184;
        (&DAT_00bcd652)[iVar4 * 0xc2] = (short)iVar7;
        (&DAT_00bcd630)[iVar10] = 9;
        *(undefined2 *)(&DAT_00bcd62e + iVar10) = 0;
        iVar4 = (FUN_004b1df0(iVar4), restore_unresolved_return(0x004b1df0u));
        *(undefined2 *)(&DAT_00bcd63c + iVar10) = 0;
      }
    }
  }
  return iVar4;
}

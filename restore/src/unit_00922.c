#include "restore_types.h"
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_007999d9 (*(undefined4 *)0x007999d9u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999f0 (*(undefined4 *)0x007999f0u)
#define DAT_00799a0a (*(undefined4 *)0x00799a0au)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a7e (*(undefined4 *)0x00799a7eu)
#define DAT_00799a9a (*(undefined4 *)0x00799a9au)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern void FUN_00467220();
extern undefined4 FUN_00480180();
extern void FUN_004b1df0();
extern void FUN_004b3250();
extern int FUN_004b4800();
extern int FUN_004bc2e0();
extern int FUN_004bc3b0();
extern int FUN_004cde60();

void FUN_00461720(int param_1)

{
  char cVar1;
  short sVar2;
  byte bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar6 = param_1 * 0x108;
  if ((short)(&DAT_007999f0)[param_1 * 0x84] < 1) {
    *(undefined1 *)((int)&DAT_00799a9a + iVar6 + 1) = 2;
  }
  iVar4 = FUN_00480180((int)*(short *)((int)&DAT_007999dc + iVar6 + 2),
                       (int)*(short *)(&DAT_007999e0 + iVar6),(&DAT_007999db)[iVar6]);
  if (iVar4 != 0) {
    if ((short)(&DAT_007999f0)[param_1 * 0x84] < 0x33) {
      FUN_00467220(param_1);
    }
    iVar4 = FUN_004cde60((int)(short)(&DAT_00799a18)[param_1 * 0x84],
                         *(undefined4 *)
                          (&DAT_00705eb8 + (short)(&DAT_007999e8)[param_1 * 0x84] * 0x28));
    if (iVar4 < 100) {
      if (iVar4 < 0x4b) {
        if (iVar4 < 0x32) {
          if (iVar4 < 0x19) {
            return;
          }
          bVar3 = 1;
        }
        else {
          bVar3 = 2;
        }
      }
      else {
        bVar3 = 3;
      }
    }
    else {
      bVar3 = 4;
    }
    if ((*(byte *)(&DAT_00799a7e + param_1 * 0x84) < bVar3) &&
       ((&DAT_00799a0a)[iVar6] = 0, *(byte *)(&DAT_00799a7e + param_1 * 0x84) < bVar3)) {
      iVar6 = FUN_004b4800((&DAT_007999d9)[iVar6],0x6c,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
      iVar4 = iVar6 * 0x184;
      (&DAT_00bcd630)[iVar4] = 8;
      (&DAT_00bcd652)[iVar6 * 0xc2] = (undefined2)param_1;
      *(undefined2 *)(&DAT_00bcd62e + iVar4) = 0;
      FUN_004b1df0(iVar6);
      cVar1 = *(char *)(&DAT_00799a7e + param_1 * 0x84);
      *(undefined2 *)(&DAT_00bcd63c + iVar4) = 0;
      *(char *)(&DAT_00799a7e + param_1 * 0x84) = cVar1 + '\x01';
      (&DAT_00799a2c)[param_1 * 0x84] = 0;
      if ((99 < *(short *)(&DAT_00799aa4 + param_1 * 0x42)) &&
         (iVar5 = FUN_004bc2e0((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],iVar6), iVar5 != 0)) {
        (&DAT_00bcd630)[iVar4] = 9;
        FUN_004b3250(iVar6);
        iVar6 = DAT_00d056f8;
        *(short *)(&DAT_00799aa4 + param_1 * 0x42) =
             *(short *)(&DAT_00799aa4 + param_1 * 0x42) + -100;
        sVar2 = *(short *)(&DAT_00799aa4 + param_1 * 0x42);
        *(undefined2 *)(&DAT_00bcd63c + iVar6 * 0x184) = 0;
        if (-1 < sVar2) {
          return;
        }
        *(undefined2 *)(&DAT_00799aa4 + param_1 * 0x42) = 0;
        return;
      }
      if ((99 < (short)(&DAT_00799a14)[param_1 * 0x84]) &&
         (iVar5 = FUN_004bc3b0((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],iVar6), iVar5 != 0)) {
        (&DAT_00bcd630)[iVar4] = 9;
        FUN_004b3250(iVar6);
        iVar6 = DAT_00d056f8;
        (&DAT_00799a14)[param_1 * 0x84] = (&DAT_00799a14)[param_1 * 0x84] + -100;
        sVar2 = (&DAT_00799a14)[param_1 * 0x84];
        *(undefined2 *)(&DAT_00bcd63c + iVar6 * 0x184) = 0;
        if (-1 < sVar2) {
          return;
        }
        (&DAT_00799a14)[param_1 * 0x84] = 0;
        return;
      }
      (&DAT_00bcd65d)[iVar4] = 1;
      (&DAT_00bcd630)[iVar4] = 4;
    }
  }
  return;
}

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
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern void FUN_00467220();
extern undefined4 FUN_00480180();
extern void FUN_004b1df0();
extern int FUN_004b4800();
extern int FUN_004cde60();

void FUN_004615d0(int param_1)

{
  char cVar1;
  byte bVar2;
  int iVar3;
  int iVar4;
  
  iVar4 = param_1 * 0x108;
  if ((short)(&DAT_007999f0)[param_1 * 0x84] < 1) {
    *(undefined1 *)((int)&DAT_00799a9a + iVar4 + 1) = 2;
  }
  iVar3 = FUN_00480180((int)*(short *)((int)&DAT_007999dc + iVar4 + 2),
                       (int)*(short *)(&DAT_007999e0 + iVar4),(&DAT_007999db)[iVar4]);
  if (iVar3 != 0) {
    if ((short)(&DAT_007999f0)[param_1 * 0x84] < 0x33) {
      FUN_00467220(param_1);
    }
    iVar3 = FUN_004cde60((int)(short)(&DAT_00799a18)[param_1 * 0x84],
                         *(undefined4 *)
                          (&DAT_00705eb8 + (short)(&DAT_007999e8)[param_1 * 0x84] * 0x28));
    if (iVar3 < 100) {
      if (iVar3 < 0x4b) {
        if (iVar3 < 0x32) {
          if (iVar3 < 0x19) {
            return;
          }
          bVar2 = 1;
        }
        else {
          bVar2 = 2;
        }
      }
      else {
        bVar2 = 3;
      }
    }
    else {
      bVar2 = 4;
    }
    if ((*(byte *)(&DAT_00799a7e + param_1 * 0x84) < bVar2) &&
       ((&DAT_00799a0a)[iVar4] = 0, *(byte *)(&DAT_00799a7e + param_1 * 0x84) < bVar2)) {
      iVar4 = FUN_004b4800((&DAT_007999d9)[iVar4],0x50,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
      iVar3 = iVar4 * 0x184;
      (&DAT_00bcd630)[iVar3] = 8;
      (&DAT_00bcd652)[iVar4 * 0xc2] = (short)param_1;
      *(undefined2 *)(&DAT_00bcd62e + iVar3) = 0;
      FUN_004b1df0(iVar4);
      cVar1 = *(char *)(&DAT_00799a7e + param_1 * 0x84);
      *(undefined2 *)(&DAT_00bcd63c + iVar3) = 0;
      *(char *)(&DAT_00799a7e + param_1 * 0x84) = cVar1 + '\x01';
      (&DAT_00799a2c)[param_1 * 0x84] = 0;
      (&DAT_00799a14)[param_1 * 0x84] = 0;
    }
  }
  return;
}

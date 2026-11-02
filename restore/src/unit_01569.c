#include "restore_types.h"
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_007999d9 (*(undefined4 *)0x007999d9u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999f0 (*(undefined4 *)0x007999f0u)
#define DAT_00799a02 (*(undefined4 *)0x00799a02u)
#define DAT_00799a0a (*(undefined4 *)0x00799a0au)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a7e (*(undefined4 *)0x00799a7eu)
#define DAT_00799a9a (*(undefined4 *)0x00799a9au)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern void FUN_00467220();
extern undefined4 FUN_00480180();
extern int FUN_004b4800();
extern int FUN_004cde60();

void FUN_004bed70(int param_1)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  byte bVar5;
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
            if (iVar4 < 1) {
              return;
            }
            bVar5 = 0x2f;
          }
          else {
            bVar5 = 0x1d;
          }
        }
        else {
          bVar5 = 0xf;
        }
      }
      else {
        bVar5 = 7;
      }
    }
    else {
      bVar5 = 3;
    }
    if ((*(byte *)(&DAT_00799a7e + param_1 * 0x84) < 4) &&
       (cVar1 = (&DAT_00799a0a)[iVar6], (&DAT_00799a0a)[iVar6] = cVar1 + 1U, uVar3 = _DAT_00d3b4e4,
       uVar2 = _DAT_00d3b4e0, bVar5 < (byte)(cVar1 + 1U))) {
      (&DAT_00799a0a)[iVar6] = 0;
      iVar6 = FUN_004b4800((&DAT_007999d9)[iVar6],0x55,uVar3,uVar2,0);
      (&DAT_00799a02)[param_1 * 0x84] = (short)iVar6;
      cVar1 = *(char *)(&DAT_00799a7e + param_1 * 0x84);
      (&DAT_00bcd630)[iVar6 * 0x184] = 8;
      *(char *)(&DAT_00799a7e + param_1 * 0x84) = cVar1 + '\x01';
      (&DAT_00bcd652)[iVar6 * 0xc2] = (short)param_1;
    }
  }
  return;
}

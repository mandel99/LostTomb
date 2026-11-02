#include "restore_types.h"
#define DAT_007999d9 (*(undefined4 *)0x007999d9u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a02 (*(undefined4 *)0x00799a02u)
#define DAT_00799a25 (*(undefined4 *)0x00799a25u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_0047fe60();
extern int FUN_004b4800();

void FUN_00469e40(int param_1,char param_2)

{
  byte bVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  
  iVar4 = param_1 * 0x108;
  sVar2 = (&DAT_00799a02)[param_1 * 0x84];
  if (0 < sVar2) {
    iVar3 = sVar2 * 0x184;
    if (((&DAT_00bcd606)[iVar3] != '\0') &&
       ((((&DAT_00bcd602)[iVar3] == '\x17' || ((&DAT_00bcd602)[iVar3] == '\0')) &&
        ((short)(&DAT_00bcd652)[sVar2 * 0xc2] == param_1)))) {
      return;
    }
    (&DAT_00799a02)[param_1 * 0x84] = 0;
  }
  sVar2 = (&DAT_00799a02)[param_1 * 0x84];
  if (0 < sVar2) {
    iVar3 = sVar2 * 0x184;
    if ((((&DAT_00bcd606)[iVar3] != '\0') &&
        (((&DAT_00bcd602)[iVar3] == '\x18' || ((&DAT_00bcd602)[iVar3] == '\0')))) &&
       ((short)(&DAT_00bcd652)[sVar2 * 0xc2] == param_1)) {
      return;
    }
    (&DAT_00799a02)[param_1 * 0x84] = 0;
  }
  iVar3 = FUN_0047fe60((int)*(short *)((int)&DAT_007999dc + iVar4 + 2),
                       (int)*(short *)(&DAT_007999e0 + iVar4),(&DAT_007999db)[iVar4],2);
  if (iVar3 != 0) {
    if (param_2 == '\0') {
      uVar5 = 0x17;
    }
    else {
      uVar5 = 0x18;
    }
    iVar3 = FUN_004b4800((&DAT_007999d9)[iVar4],uVar5,_DAT_00d3b4e4,_DAT_00d3b4e0,4);
    if (iVar3 != 0) {
      (&DAT_00799a02)[param_1 * 0x84] = (short)iVar3;
      bVar1 = (&DAT_00799a25)[iVar4];
      (&DAT_00bcd630)[iVar3 * 0x184] = 8;
      (&DAT_00bcd652)[iVar3 * 0xc2] = (short)param_1;
      *(ushort *)(&DAT_00bcd62e + iVar3 * 0x184) = (bVar1 & 0xff0f) + 10;
    }
  }
  return;
}

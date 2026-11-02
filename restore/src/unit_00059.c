#include "restore_types.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd720 (*(undefined4 *)0x00bcd720u)
#define DAT_00bcd724 (*(undefined4 *)0x00bcd724u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00d05700 (*(undefined *)0x00d05700u)
#define DAT_00d05708 (*(undefined *)0x00d05708u)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00eadcc8 (*(undefined *)0x00eadcc8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_004806c0();
extern bool FUN_004a4300();
extern void FUN_004b3250();
extern int FUN_004cde60();

void FUN_00406e80(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int iVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  
  iVar4 = DAT_00eadcc8;
  DAT_00d05700 = DAT_00eadcc8;
  if (0 < DAT_00eadcc8) {
    iVar7 = DAT_00eadcc8 * 0x184;
    iVar6 = FUN_004cde60(*(undefined4 *)(&DAT_00bcd720 + iVar7),
                         *(undefined4 *)(&DAT_00bcd724 + iVar7));
    if ((((&DAT_00bcd630)[iVar7] != '\v') && ((&DAT_00bcd630)[iVar7] != '\x0f')) || (0xf < iVar6)) {
      if (DAT_00d3b198 == 4) {
        cVar5 = FUN_004a4300(iVar4);
        if (cVar5 != '\0') {
          return;
        }
        if (DAT_00d3b198 == 4) {
          (&DAT_00bcd630)[iVar7] = 9;
          uVar3 = _DAT_00d3b4e4;
          uVar2 = _DAT_00d3b4e0;
          iVar6 = (short)(&DAT_00bcd652)[iVar4 * 0xc2] * 0x108;
          DAT_00d05708 = 0;
          FUN_004806c0((int)*(short *)((int)&DAT_007999dc + iVar6 + 2),
                       (int)*(short *)(&DAT_007999e0 + iVar6),(&DAT_007999db)[iVar6]);
          *(undefined2 *)(&DAT_00bcd624 + iVar7) = (&DAT_00bcd60c)[iVar4 * 0xc2];
          *(undefined2 *)(&DAT_00bcd626 + iVar7) = (&DAT_00bcd60e)[iVar4 * 0xc2];
          uVar1 = DAT_00d056f8;
          *(undefined2 *)(&DAT_00bcd61c + iVar7) = DAT_00d3b4e4;
          *(undefined2 *)(&DAT_00bcd61e + iVar7) = DAT_00d3b4e0;
          _DAT_00d3b4e0 = uVar2;
          _DAT_00d3b4e4 = uVar3;
          FUN_004b3250(uVar1);
        }
      }
      DAT_00e38dda = 1;
      DAT_00e38dca = 1;
    }
  }
  return;
}

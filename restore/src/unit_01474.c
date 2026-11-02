#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007999d9 (*(undefined4 *)0x007999d9u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ec (*(undefined4 *)0x007999ecu)
#define DAT_00799a02 (*(undefined4 *)0x00799a02u)
#define DAT_00799af4 (*(undefined4 *)0x00799af4u)
#define DAT_00bcb9ac (*(undefined *)0x00bcb9acu)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_00480180();
extern int FUN_004b4800();

undefined4 FUN_004b7400(int param_1)

{
  undefined1 uVar1;
  short sVar2;
  short sVar3;
  undefined2 uVar4;
  char *pcVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  if (0 < DAT_00bcb9ac) {
    iVar7 = 1;
    if (1 < DAT_0078d790) {
      pcVar5 = &DAT_00799af4;
      do {
        if ((((pcVar5[-0x14] == '\x01') && (*pcVar5 == (&DAT_007999ec)[param_1 * 0x108])) &&
            (*(short *)(pcVar5 + -4) == 0xad)) &&
           ((0 < *(short *)(pcVar5 + 0x2c) && (*(short *)(pcVar5 + 0x16) == 0)))) {
          iVar6 = FUN_004b4800((&DAT_007999d9)[param_1 * 0x108],0x2a,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
          iVar8 = iVar7 * 0x108;
          sVar2 = *(short *)(&DAT_007999e0 + iVar8);
          sVar3 = *(short *)((int)&DAT_007999dc + iVar8 + 2);
          uVar1 = (&DAT_007999db)[iVar8];
          iVar9 = iVar6 * 0x184;
          (&DAT_00bcd630)[iVar9] = 0xc;
          iVar8 = FUN_00480180((int)sVar3,(int)sVar2,uVar1);
          if (iVar8 == 0) {
            (&DAT_00bcd606)[iVar9] = 2;
          }
          uVar4 = DAT_00d3b4e0;
          *(undefined2 *)(&DAT_00bcd61c + iVar9) = DAT_00d3b4e4;
          *(undefined2 *)(&DAT_00bcd61e + iVar9) = uVar4;
          (&DAT_00bcd652)[iVar6 * 0xc2] = (short)iVar7;
          (&DAT_00799a02)[iVar7 * 0x84] = (short)iVar6;
          return 1;
        }
        iVar7 = iVar7 + 1;
        pcVar5 = pcVar5 + 0x108;
      } while (iVar7 < DAT_0078d790);
    }
  }
  return 0;
}

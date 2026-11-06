#include "restore_types.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00bcbada (*(undefined4 *)0x00bcbadau)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd628 (*(undefined4 *)0x00bcd628u)
#define DAT_00bcd62a (*(undefined4 *)0x00bcd62au)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_0040b5e0();
extern int FUN_0040c180();
extern undefined4 FUN_004806c0();
extern int FUN_004b8460();

void FUN_004b8690(int param_1)

{
  short sVar1;
  short sVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  short *psVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  char *pcVar10;
  short *psVar11;
  undefined2 local_8 [2];
  undefined2 local_4 [2];
  
  iVar7 = 1;
  psVar11 = &DAT_00bcbada;
  do {
    if ((((char)psVar11[-0x41] == '\x01') && ((char)psVar11[-0x20] != '\0')) &&
       (*psVar11 == param_1)) {
      *psVar11 = -1;
      iVar5 = FUN_0040b5e0(iVar7);
      if (iVar5 == 0) {
        uVar9 = (uint)*(byte *)(psVar11 + -0x2d);
        if (uVar9 != 0) {
          psVar6 = psVar11 + -0x3d;
          do {
            if (((0 < *psVar6) && (iVar5 = *psVar6 * 0x184, (&DAT_00bcd606)[iVar5] == '\x01')) &&
               ((&DAT_00bcd630)[iVar5] != '\x04')) {
              (&DAT_00bcd630)[iVar5] = 0x10;
            }
            psVar6 = psVar6 + 1;
            uVar9 = uVar9 - 1;
          } while (uVar9 != 0);
        }
      }
      else {
        iVar5 = FUN_004b8460(iVar7);
        if (iVar5 != 0) {
          iVar8 = iVar5 * 0x184;
          sVar1 = psVar11[-0x2a];
          *(undefined2 *)(&DAT_00bcd624 + iVar8) = (&DAT_00bcd60c)[iVar5 * 0xc2];
          *(undefined2 *)(&DAT_00bcd626 + iVar8) = (&DAT_00bcd60e)[iVar5 * 0xc2];
          FUN_0040c180((int)(short)(&DAT_00bcd60c)[iVar5 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[iVar5 * 0xc2],(int)sVar1,(int)psVar11[-0x29],
                       local_8,local_4);
          sVar1 = psVar11[-0x2a];
          *(undefined2 *)(&DAT_00bcd61c + iVar8) = local_8[0];
          sVar2 = psVar11[-0x29];
          *(undefined2 *)(&DAT_00bcd61e + iVar8) = local_4[0];
          *(short *)(&DAT_00bcd628 + iVar8) = sVar1;
          *(short *)(&DAT_00bcd62a + iVar8) = sVar2;
          (&DAT_00bcd630)[iVar8] = 0x13;
        }
      }
    }
    uVar4 = _DAT_00d3b4e4;
    uVar3 = _DAT_00d3b4e0;
    psVar11 = psVar11 + 0x48;
    iVar7 = iVar7 + 1;
  } while ((int)psVar11 < 0xbcd66a);
  pcVar10 = &DAT_00bcd786;
  do {
    if (((pcVar10[4] != '\0') && (*pcVar10 == 'N')) &&
       ((*(short *)(pcVar10 + 0x128) == param_1 && ((byte)pcVar10[5] == DAT_00eaac28)))) {
      *(undefined2 *)(pcVar10 + 0x22) = *(undefined2 *)(pcVar10 + 10);
      *(undefined2 *)(pcVar10 + 0x24) = *(undefined2 *)(pcVar10 + 0xc);
      iVar7 = *(short *)(pcVar10 + 0x50) * 0x108;
      FUN_004806c0((int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                   (int)*(short *)(&DAT_007999e0 + iVar7),(&DAT_007999db)[iVar7]);
      *(undefined2 *)(pcVar10 + 0x1a) = DAT_00d3b4e4;
      *(undefined2 *)(pcVar10 + 0x1c) = DAT_00d3b4e0;
      pcVar10[0x128] = -1;
      pcVar10[0x129] = -1;
      _DAT_00d3b4e0 = uVar3;
      _DAT_00d3b4e4 = uVar4;
      pcVar10[0x2e] = '\x13';
    }
    pcVar10 = pcVar10 + 0x184;
  } while ((int)pcVar10 < 0xc8ad42);
  return;
}

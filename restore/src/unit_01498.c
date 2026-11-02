#include "restore_types.h"
#define DAT_007770f0 (*(undefined4 *)0x007770f0u)
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_00785d90 (*(undefined4 *)0x00785d90u)
#define DAT_00bcba9a (*(undefined4 *)0x00bcba9au)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00ea3ff0 (*(undefined4 *)0x00ea3ff0u)
#define DAT_00ea3ff2 (*(undefined4 *)0x00ea3ff2u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_0040b5e0();
extern void FUN_004b3250();
extern int FUN_004b8460();

void FUN_004b84b0(int param_1)

{
  char cVar1;
  undefined2 uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  short *psVar8;
  char *pcVar9;
  bool bVar10;
  
  if ((((-1 < param_1) && (param_1 < DAT_00785d70)) && (param_1 * 0x7c != -0x785d74)) &&
     ((iVar4 = (int)*(short *)(&DAT_00785d90 + param_1 * 0x7c), -1 < iVar4 && (iVar4 < 0x32)))) {
    iVar7 = 1;
    pcVar9 = &DAT_00bcba9a;
    cVar1 = (&DAT_007770f0)[iVar4 * 0x144];
    do {
      if (((pcVar9[-0x42] == '\x01') && (*pcVar9 != '\0')) &&
         ((pcVar9[0x25] != '\0' && ((pcVar9[-0x1a] != '\0' && (*(short *)(pcVar9 + 0x40) < 0)))))) {
        iVar4 = FUN_0040b5e0(iVar7);
        bVar10 = iVar4 != 0;
        if (cVar1 == '\x02') {
          if (bVar10) {
LAB_004b8571: ;
            *(short *)(pcVar9 + 0x40) = (short)param_1;
            pcVar9[-0x3f] = '\0';
            if (bVar10) {
              DAT_00d056f8 = FUN_004b8460(iVar7);
              if (DAT_00d056f8 != 0) {
                (&DAT_00bcd630)[DAT_00d056f8 * 0x184] = 0x11;
              }
            }
            else {
              iVar4 = 0;
              uVar2 = *(undefined2 *)(&DAT_00ea3ff2 + DAT_00eaac28 * 0x49d8);
              *(undefined2 *)(pcVar9 + -0x10) =
                   *(undefined2 *)(&DAT_00ea3ff0 + DAT_00eaac28 * 0x49d8);
              *(undefined2 *)(pcVar9 + -0xe) = uVar2;
              if (pcVar9[-0x1a] != '\0') {
                psVar8 = (short *)(pcVar9 + -0x3a);
                do {
                  iVar6 = (int)*psVar8;
                  if (((0 < iVar6) && (iVar5 = iVar6 * 0x184, (&DAT_00bcd606)[iVar5] == '\x01')) &&
                     ((&DAT_00bcd630)[iVar5] != '\x04')) {
                    (&DAT_00bcd630)[iVar5] = 0xf;
                    FUN_004b3250(iVar6);
                  }
                  iVar4 = iVar4 + 1;
                  psVar8 = psVar8 + 1;
                } while (iVar4 < (int)(uint)(byte)pcVar9[-0x1a]);
              }
            }
          }
        }
        else if (!bVar10) goto LAB_004b8571;
      }
      uVar3 = DAT_00eaac28;
      pcVar9 = pcVar9 + 0x90;
      iVar7 = iVar7 + 1;
    } while ((int)pcVar9 < 0xbcd62a);
    if (cVar1 == '\x02') {
      pcVar9 = &DAT_00bcd786;
      DAT_00d056f8 = 2000;
      do {
        if (((pcVar9[4] != '\0') && (*pcVar9 == 'N')) &&
           (((byte)pcVar9[5] == uVar3 && ((pcVar9[-8] != '\0' && (*(short *)(pcVar9 + 0x128) < 0))))
           )) {
          *(short *)(pcVar9 + 0x128) = (short)param_1;
          pcVar9[0x2e] = '\x11';
        }
        pcVar9 = pcVar9 + 0x184;
      } while ((int)pcVar9 < 0xc8ad42);
    }
  }
  return;
}

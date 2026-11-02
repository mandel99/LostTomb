#include "restore_types.h"
#define DAT_00bcb9b4 (*(undefined *)0x00bcb9b4u)
#define DAT_00bcba9a (*(undefined4 *)0x00bcba9au)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00ea5f08 (*(undefined4 *)0x00ea5f08u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00bcb9b8 (*(undefined4 *)0x00bcb9b8u)

void FUN_004b77d0(void)

{
  int *piVar1;
  char cVar2;
  undefined2 uVar3;
  int iVar4;
  char *pcVar5;
  int iVar6;
  short *psVar7;
  int iVar8;
  
  DAT_00bcb9b4 = 0;
  piVar1 = &DAT_00ea5f08 + DAT_00eaac28 * 0x1276;
  _DAT_00bcb9b8 = 0;
  *piVar1 = 0;
  iVar6 = 1;
  pcVar5 = &DAT_00bcba9a;
  do {
    if ((pcVar5[-0x42] == '\x01') && (*pcVar5 != '\0')) {
      DAT_00bcb9b4 = DAT_00bcb9b4 + 1;
      _DAT_00bcb9b8 = iVar6;
      if (*(short *)(pcVar5 + -0x3e) == 0xd) {
        *piVar1 = *piVar1 + 1;
      }
      if (*(short *)(pcVar5 + 0x1c) < 1) {
        iVar8 = 0;
        psVar7 = (short *)(pcVar5 + -0x3a);
        do {
          iVar4 = (int)*psVar7;
          if (iVar4 == 0) break;
          if (((((&DAT_00bcd606)[iVar4 * 0x184] == '\x01') &&
               (cVar2 = (&DAT_00bcd630)[iVar4 * 0x184], cVar2 != '\x06')) && (cVar2 != '\a')) &&
             ((cVar2 != '\x16' && (cVar2 != '\x17')))) {
            uVar3 = (&DAT_00bcd60e)[iVar4 * 0xc2];
            *(undefined2 *)(pcVar5 + -0x14) = (&DAT_00bcd60c)[iVar4 * 0xc2];
            *(undefined2 *)(pcVar5 + -0x12) = uVar3;
            break;
          }
          iVar8 = iVar8 + 1;
          psVar7 = psVar7 + 1;
        } while (iVar8 < 0x10);
      }
    }
    pcVar5 = pcVar5 + 0x90;
    iVar6 = iVar6 + 1;
    if (0xbcd629 < (int)pcVar5) {
      return;
    }
  } while( true );
}

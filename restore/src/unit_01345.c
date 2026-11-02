#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd7d6 (*(undefined4 *)0x00bcd7d6u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00ebab20 (*(undefined4 *)0x00ebab20u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_00480060();
extern void FUN_004b3250();
extern void FUN_004b52f0();
extern void FUN_004b5420();

void FUN_00497b90(void)

{
  int iVar1;
  short sVar2;
  int iVar3;
  short sVar4;
  short *psVar5;
  int iVar6;
  
  iVar6 = 1;
  psVar5 = &DAT_00bcd7d6;
  do {
    if (((char)psVar5[-0x28] == '*') && ((&DAT_00ebab20)[*(int *)(psVar5 + -0x1d)] != '\0')) {
      iVar3 = FUN_00480060((int)psVar5[-0x23],(int)psVar5[-0x22],1,2);
      if (iVar3 == 0) {
        *(undefined1 *)(psVar5 + -0x11) = 8;
        FUN_004b5420(iVar6);
        sVar2 = *(short *)((int)&DAT_007999dc + *psVar5 * 0x108 + 2);
        sVar4 = *(short *)(&DAT_007999e0 + *psVar5 * 0x108);
        psVar5[-0x23] = sVar2;
        psVar5[-0x17] = sVar2;
        psVar5[-0x22] = sVar4;
        psVar5[-0x16] = sVar4;
        *(int *)(psVar5 + -0x1d) = DAT_00bca2d8 + psVar5[-0x23] + psVar5[-0x22] * 0xe4;
        FUN_004b52f0(iVar6);
        *(undefined1 *)(psVar5 + -0x11) = 8;
        FUN_004b3250(iVar6);
      }
      else {
        FUN_004b3250(DAT_00d056f8);
        *(undefined1 *)((int)psVar5 + -0x21) = 0;
        FUN_004b5420(iVar6);
        iVar1 = _DAT_00d3b4e4;
        iVar3 = _DAT_00d3b4e0;
        sVar4 = (short)_DAT_00d3b4e4;
        psVar5[-0x23] = sVar4;
        psVar5[-0x21] = sVar4;
        sVar2 = (short)iVar3;
        psVar5[-0x22] = sVar2;
        psVar5[-0x20] = sVar2;
        psVar5[-8] = sVar4 * 0xf;
        psVar5[-7] = sVar2 * 0xf;
        *(int *)(psVar5 + -0x1d) = iVar1 + iVar3 * 0xe4 + DAT_00bca2d8;
        FUN_004b52f0(iVar6);
        iVar3 = DAT_00d056f8 * 0x184;
        (&DAT_00bcd630)[iVar3] = 0xb;
        *(undefined2 *)(&DAT_00bcd61c + iVar3) = *(undefined2 *)(&DAT_00bcd624 + iVar3);
        *(undefined2 *)(&DAT_00bcd61e + iVar3) = *(undefined2 *)(&DAT_00bcd626 + iVar3);
      }
    }
    psVar5 = psVar5 + 0xc2;
    iVar6 = iVar6 + 1;
  } while ((int)psVar5 < 0xc8ad92);
  return;
}

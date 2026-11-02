#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a84 (*(undefined4 *)0x00799a84u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd673 (*(undefined4 *)0x00bcd673u)

undefined4 FUN_00495a40(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  
  iVar4 = param_1 * 0x184;
  iVar2 = (int)(short)(&DAT_00bcd656)[param_1 * 0xc2];
  if (((&DAT_007999d8)[iVar2 * 0x108] == '\x01') &&
     ((((&DAT_007999e8)[iVar2 * 0x84] == 0x4b || ((&DAT_007999e8)[iVar2 * 0x84] == 0xc0)) &&
      (0 < (short)(&DAT_00799a18)[iVar2 * 0x84])))) {
    iVar3 = 0;
    psVar5 = (short *)((int)&DAT_00799a84 + iVar2 * 0x108 + 2);
    do {
      if (((*psVar5 != 0) && (iVar2 = *psVar5 * 0x184, (&DAT_00bcd606)[iVar2] == '\x01')) &&
         ((&DAT_00bcd630)[iVar2] != '\b')) {
        return 0;
      }
      iVar3 = iVar3 + 1;
      psVar5 = psVar5 + 1;
    } while (iVar3 < 3);
    cVar1 = (&DAT_00bcd673)[iVar4];
    (&DAT_00bcd673)[iVar4] = cVar1 + 1U;
    if ((byte)(cVar1 + 1U) < 10) {
      return 0;
    }
    (&DAT_00bcd673)[iVar4] = 0xb;
  }
  return 1;
}

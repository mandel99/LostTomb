#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a84 (*(undefined4 *)0x00799a84u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)

undefined4 FUN_00495b60(int param_1)

{
  char cVar1;
  undefined4 uVar2;
  int iVar3;
  short *psVar4;
  int iVar5;
  
  uVar2 = 1;
  iVar3 = (short)(&DAT_00bcd656)[param_1 * 0xc2] * 0x108;
  if (((&DAT_007999d8)[iVar3] == '\x01') &&
     (((&DAT_007999e8)[(short)(&DAT_00bcd656)[param_1 * 0xc2] * 0x84] == 0x4b ||
      ((&DAT_007999e8)[(short)(&DAT_00bcd656)[param_1 * 0xc2] * 0x84] == 0xc0)))) {
    iVar5 = 0;
    psVar4 = (short *)((int)&DAT_00799a84 + iVar3 + 2);
    do {
      if ((*psVar4 != 0) && (iVar3 = *psVar4 * 0x184, (&DAT_00bcd606)[iVar3] == '\x01')) {
        cVar1 = (&DAT_00bcd630)[iVar3];
        if (cVar1 == '\t') {
          return 1;
        }
        if (cVar1 == '\v') {
          return 1;
        }
        if (cVar1 == '\x0f') {
          return 1;
        }
        if (cVar1 == '\x0e') {
          return 1;
        }
        if ((((cVar1 == '\n') || (cVar1 == '\f')) || (cVar1 == '\r')) || (cVar1 == '\x10')) {
          return 2;
        }
      }
      iVar5 = iVar5 + 1;
      psVar4 = psVar4 + 1;
    } while (iVar5 < 3);
    uVar2 = 0;
  }
  return uVar2;
}

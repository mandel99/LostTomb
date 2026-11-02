#include "restore_types.h"
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
extern void FUN_004cd680();

void FUN_005219c0(void)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 0;
  iVar1 = 0;
  do {
    FUN_004cd680(iVar1 + 0xb2 + DAT_00f53f0c,DAT_00f53f08 + 0xdd,0x1b,0x1b,
                 (iVar2 != DAT_007056c8 + -2) - 1 & 0xf800);
    iVar1 = iVar1 + 0x20;
    iVar2 = iVar2 + 1;
  } while (iVar1 < 0x120);
  if (iVar2 < 10) {
    iVar2 = 10 - iVar2;
    do {
      FUN_004cd680(DAT_00f53f0c + 0xb4,DAT_00f53f08 + 0x100,0x118,0x19,
                   (-(uint)(DAT_007056c8 != 1) & 0xffff0800) + 0xf800);
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  }
  return;
}

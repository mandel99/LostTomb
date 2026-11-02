#include "restore_types.h"
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
extern void FUN_004d63d0();

void FUN_004f7770(void)

{
  int iVar1;
  int iVar2;
  
  if ((5 - DAT_00f53f48 < 5) && (5 - DAT_00f53f48 < 1)) {
    iVar2 = 1;
    iVar1 = 0x78;
    do {
      FUN_004d63d0(0,iVar1 + -0x28 + DAT_00f53f0c,DAT_00f53f08 + 0x11e,0x78,0x42,
                   DAT_007056c8 == iVar2,DAT_00e38da8,DAT_00e38da4);
      iVar1 = iVar1 + 0x78;
      iVar2 = iVar2 + 1;
    } while (iVar1 < 0x1e1);
  }
  return;
}

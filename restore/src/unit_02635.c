#include "restore_types.h"
#define DAT_00ea60a4 (*(undefined4 *)0x00ea60a4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f56494 (*(undefined *)0x00f56494u)
extern int FUN_004cde40();
extern int FUN_004cde60();
extern int FUN_0053e590();

void FUN_0055a060(void)

{
  undefined4 uVar1;
  int iVar2;
  
  if (0 < DAT_00f53e6c + -0xf) {
    iVar2 = (&DAT_00ea60a4)[DAT_00eaac28 * 0x1276];
    if (iVar2 < 2) {
      uVar1 = 0;
    }
    else if (DAT_00f53e6c < iVar2 + 0xe) {
      uVar1 = 100;
    }
    else {
      uVar1 = FUN_004cde60(iVar2 + -1,DAT_00f53e6c + -0xf);
    }
    iVar2 = FUN_004cde40(0x100,uVar1);
    FUN_0053e590(DAT_00f56494 + 0x27,DAT_00f53f0c + 0x255,DAT_00f53f08 + 0x4e + iVar2);
  }
  return;
}

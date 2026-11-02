#include "restore_types.h"
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00ec7630 (*(undefined4 *)0x00ec7630u)
extern void FUN_0040f8f0();
extern void FUN_005173a0();

void FUN_00517660(void)

{
  FUN_0040f8f0(&DAT_00ec7630,0xcb10,0xffffffff);
  FUN_005173a0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  return;
}

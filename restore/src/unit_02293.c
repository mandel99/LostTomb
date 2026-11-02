#include "restore_types.h"
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00ed4140 (*(undefined4 *)0x00ed4140u)
extern void FUN_0040f8f0();
extern void FUN_00517690();

void FUN_005178b0(void)

{
  FUN_0040f8f0(&DAT_00ed4140,0xcb10,0xffffffff);
  FUN_00517690(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
  return;
}

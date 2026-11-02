#include "restore_types.h"
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00ee0c50 (*(undefined4 *)0x00ee0c50u)
extern void FUN_0040f8f0();
extern void FUN_00516f90();

void FUN_00517370(void)

{
  FUN_0040f8f0(&DAT_00ee0c50,0xcb10,0xffffffff);
  FUN_00516f90(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,1);
  return;
}

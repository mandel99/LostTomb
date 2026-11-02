#include "restore_types.h"
#define DAT_00799ab4 (*(undefined4 *)0x00799ab4u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)

void FUN_0040c630(void)

{
  if ((&DAT_00799ab4)[DAT_00eadd6c * 0x42] == 1) {
    (&DAT_00799ab4)[DAT_00eadd6c * 0x42] = 0x10;
    DAT_00e38dca = 1;
    return;
  }
  if ((&DAT_00799ab4)[DAT_00eadd6c * 0x42] == 0x10) {
    (&DAT_00799ab4)[DAT_00eadd6c * 0x42] = 1;
  }
  DAT_00e38dca = 1;
  return;
}

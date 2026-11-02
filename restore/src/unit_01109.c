#include "restore_types.h"
#define DAT_00ac2d4c (*(undefined4 *)0x00ac2d4cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)

uint FUN_0047b8e0(int param_1)

{
  if (((&DAT_00af598c)[param_1] & 0x10000) != 0) {
    return 0;
  }
  if ('\x04' < (char)(&DAT_00ac2d4c)[param_1]) {
    return 1;
  }
  if ((char)(&DAT_00ac2d4c)[param_1] < '\x01') {
    return 0;
  }
  return (uint)(&DAT_00af598c)[param_1] >> 0xd & 1;
}

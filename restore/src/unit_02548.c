#include "restore_types.h"
#define DAT_00601de0 (*(undefined *)0x00601de0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)

undefined4 FUN_00541ab0(int param_1)

{
  return *(undefined4 *)(&DAT_00601de0 + (short)(&DAT_007999ea)[param_1 * 0x84] * 0x34);
}

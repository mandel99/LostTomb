#include "restore_types.h"
#define DAT_00978d9c (*(undefined4 *)0x00978d9cu)

uint FUN_0047e230(char param_1,int param_2)

{
  return (uint)(&DAT_00978d9c)[param_2] >> (param_1 * '\x03' & 0x1fU) & 7;
}

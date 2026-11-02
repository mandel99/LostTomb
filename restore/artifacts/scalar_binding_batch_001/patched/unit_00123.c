#include "restore_types.h"
#define DAT_00e38d94 (*(uint32_t *)0x00e38d94u)
#define DAT_00e92e18 (*(uint32_t *)0x00e92e18u)

void FUN_0040fa40(int param_1,int param_2,undefined2 param_3)

{
  *(undefined2 *)(DAT_00e92e18 + param_1 * 2 + param_2 * DAT_00e38d94) = param_3;
  return;
}

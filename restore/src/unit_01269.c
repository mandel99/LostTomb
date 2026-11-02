#include "restore_types.h"
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

int FUN_00487d00(int param_1,int param_2)

{
  _DAT_00d3b4e0 = param_2 + 6;
  _DAT_00d3b4e4 = param_1 + 9;
  return DAT_00bca2d8 + (param_2 + 6) * 0xe4 + param_1 + 9;
}

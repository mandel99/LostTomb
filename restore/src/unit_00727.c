#include "restore_types.h"
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_00785d74 (*(undefined4 *)0x00785d74u)

undefined2 * FUN_0044c0d0(int param_1)

{
  if ((-1 < param_1) && (param_1 < DAT_00785d70)) {
    return &DAT_00785d74 + param_1 * 0x3e;
  }
  return (undefined2 *)0x0;
}

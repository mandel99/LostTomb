#include "restore_types.h"
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_004f4a80(undefined4 param_1,int param_2)

{
  *(undefined4 *)(0x00ea5facu + (uint)DAT_00eaac28 * 0x49d8u +
                 (uint)param_2 * 4u) = 0;
  return;
}

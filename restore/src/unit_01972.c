#include "restore_types.h"
extern void FUN_004e5b90();
extern int FUN_004f4aa0();
extern int FUN_004f4f80();

undefined2 FUN_004e57f0(int param_1)

{
  undefined2 uVar1;
  
  FUN_004f4f80(param_1);
  uVar1 = (undefined2)FUN_004f4aa0(param_1);
  *(undefined2 *)(0x007999eau + (uint)param_1 * 0x108u) = uVar1;
  *(undefined1 *)(0x00799a2du + (uint)param_1 * 0x108u) = 0x21;
  FUN_004e5b90(param_1);
  return 1;
}

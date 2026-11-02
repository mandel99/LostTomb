#include "restore_types.h"
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
extern void FUN_004cc110();

undefined1 * FUN_00448f20(int param_1)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  
  puVar1 = DAT_00e20768;
  if ((-1 < param_1) && (param_1 < 0x1e)) {
    FUN_004cc110(0x9c,param_1);
    puVar2 = DAT_00e20768;
    DAT_00e20768 = puVar1;
    return puVar2;
  }
  return &DAT_007027d4;
}

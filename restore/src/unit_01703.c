#include "restore_types.h"
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
extern void FUN_004cc110();
extern void FUN_004cd010();

void FUN_004ccfd0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  FUN_004cc110(param_1,param_2);
  FUN_004cd010(DAT_00e20768,param_3,param_4,param_5,param_6,param_7);
  return;
}

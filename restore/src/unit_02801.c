#include "restore_types.h"
extern undefined4 * FUN_005648c9();
extern undefined4 * FUN_00564a39();

undefined4 * FUN_005619ea(undefined4 param_1,undefined4 param_2,undefined4 param_3)
{
  undefined4 *value = FUN_00564a39();
  if (value == 0) return (undefined4 *)0;
  return FUN_005648c9(param_1,param_2,param_3,value);
}

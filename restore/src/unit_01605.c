#include "restore_types.h"
extern uint FUN_004c5190();

int FUN_004c5240(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_004c5190();
  if (param_1 != 0) {
    if (iVar1 == 1) {
      if (param_1 == 0x19) {
        return 0x28;
      }
      if (param_1 != 0xf) {
        if (0xe < param_1) {
          return param_1;
        }
        return 0xf;
      }
      return 0x19;
    }
    if (1 < iVar1) {
      if (param_1 == 0x19) {
        return 0x28;
      }
      if (param_1 != 0xf) {
        if (0xe < param_1) {
          return param_1;
        }
        return 0x19;
      }
      return 0x28;
    }
    if (iVar1 == -1) {
      if (param_1 == 0x28) {
        return 0x19;
      }
      if (param_1 != 0x19) {
        if (param_1 != 0xf) {
          return param_1;
        }
        return 0;
      }
      return 0xf;
    }
    if (-2 < iVar1) {
      return param_1;
    }
    if (param_1 == 0x28) {
      return 0xf;
    }
    if ((param_1 != 0x19) && (param_1 != 0xf)) {
      return param_1;
    }
  }
  return 0;
}

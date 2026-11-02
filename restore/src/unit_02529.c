#include "restore_types.h"
#define DAT_00e92e1c (*(uint32_t *)0x00e92e1cu)
extern void FUN_00561a2b();

void FUN_0053f790(int *param_1,char param_2)

{
  int iVar1;
  
  iVar1 = *param_1;
  if (iVar1 != 0) {
    if (iVar1 == DAT_00e92e1c) {
      if (param_2 != '\0') {
        *param_1 = 0;
        return;
      }
    }
    else {
      FUN_00561a2b(iVar1);
      *param_1 = 0;
    }
  }
  return;
}

#include "restore_types.h"
#define DAT_00e92dfc (*(uint32_t *)0x00e92dfcu)

void FUN_004cdeb0(ushort *param_1,int param_2)

{
  char cVar1;
  
  if (DAT_00e92dfc == 2) {
    while ((param_2 != 0 && (-1 < param_2))) {
      cVar1 = (char)*param_1;
      if (cVar1 == -1) {
        param_1 = param_1 + 1;
        param_2 = param_2 + -2;
      }
      else {
        param_1 = (ushort *)((int)param_1 + 1);
        param_2 = param_2 + -1;
        for (; '\0' < cVar1; cVar1 = cVar1 + -1) {
          *param_1 = (*param_1 & 0x7fe0) << 1 | *param_1 & 0x1f;
          param_1 = param_1 + 1;
          param_2 = param_2 + -2;
        }
      }
    }
  }
  return;
}

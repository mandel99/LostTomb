#include "restore_types.h"

int FUN_004cbe30(char *param_1,char param_2,int param_3)

{
  int iVar1;
  
  iVar1 = 0;
  if (0 < param_3) {
    do {
      if (*param_1 == param_2) {
        return iVar1 + 1;
      }
      param_1 = param_1 + 1;
      iVar1 = iVar1 + 1;
    } while (iVar1 < param_3);
  }
  return 0;
}

#include "restore_types.h"

int FUN_004cc000(char *param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 10000;
  iVar2 = 0;
  do {
    cVar1 = *param_1;
    param_1 = param_1 + 1;
    if (cVar1 == '\0') {
      return iVar2;
    }
    iVar2 = iVar2 + 1;
    iVar3 = iVar3 + -1;
  } while (0 < iVar3);
  return iVar2;
}

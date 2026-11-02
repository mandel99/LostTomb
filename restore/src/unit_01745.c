#include "restore_types.h"

void FUN_004ce3a0(char *param_1)

{
  char cVar1;
  
  do {
    cVar1 = *param_1;
    if (cVar1 == '.') {
      *param_1 = '\0';
      return;
    }
    param_1 = param_1 + 1;
  } while (cVar1 != '\0');
  return;
}

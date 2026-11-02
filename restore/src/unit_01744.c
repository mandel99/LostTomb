#include "restore_types.h"
#define DAT_00e38aec (*(char *)0x00e38aecu)
#define DAT_00e38aed (*(char *)0x00e38aedu)
#define DAT_00e38aee (*(char *)0x00e38aeeu)

void FUN_004ce370(char *param_1)

{
  char cVar1;
  
  do {
    cVar1 = *param_1;
    if (cVar1 == '.') {
      param_1[1] = DAT_00e38aec;
      param_1[2] = DAT_00e38aed;
      param_1[3] = DAT_00e38aee;
      param_1[4] = '\0';
      return;
    }
    param_1 = param_1 + 1;
  } while (cVar1 != '\0');
  return;
}

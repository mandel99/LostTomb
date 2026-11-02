#include "restore_types.h"
#define DAT_00784de0 (*(undefined *)0x00784de0u)

int FUN_004f4ff0(void)

{
  int iVar1;
  short *psVar2;
  
  iVar1 = 0;
  psVar2 = &DAT_00784de0;
  do {
    if (*psVar2 != 0) {
      iVar1 = iVar1 + 1;
    }
    psVar2 = psVar2 + 1;
  } while ((int)psVar2 < 0x784de6);
  return iVar1;
}

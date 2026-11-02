#include "restore_types.h"
#define DAT_007848e2 (*(undefined *)0x007848e2u)

int FUN_004c53d0(int param_1)

{
  int iVar1;
  short *psVar2;
  
  iVar1 = 0;
  psVar2 = &DAT_007848e2;
  do {
    if (*psVar2 == param_1) {
      return iVar1;
    }
    psVar2 = psVar2 + 1;
    iVar1 = iVar1 + 1;
  } while ((int)psVar2 < 0x7848ec);
  return -1;
}

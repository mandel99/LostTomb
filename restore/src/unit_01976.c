#include "restore_types.h"
extern void FUN_004e5950();

void FUN_004e59a0(void)

{
  int iVar1;
  
  iVar1 = 0;
  do {
    FUN_004e5950(iVar1);
    iVar1 = iVar1 + 1;
  } while (iVar1 < 3);
  return;
}

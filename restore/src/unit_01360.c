#include "restore_types.h"
#define DAT_00799a9c (*(undefined4 *)0x00799a9cu)
extern void FUN_004a3630();

void FUN_004a36b0(int param_1)

{
  int iVar1;
  
  iVar1 = (&DAT_00799a9c)[param_1 * 0x42];
  FUN_004a3630(param_1);
  if (0 < iVar1) {
    FUN_004a3630(iVar1);
  }
  return;
}

#include "restore_types.h"
#define DAT_00ea1888 (*(undefined4 *)0x00ea1888u)
#define DAT_00eaac28 (*(undefined4 *)0x00eaac28u)

void FUN_004e85d0(int param_1)

{
  int *piVar1;
  int iVar2;
  unsigned int city_offset;
  
  city_offset = (unsigned int)DAT_00eaac28 * 0x49d8u;
  piVar1 = (int *)(0x00ea1888u + city_offset);
  iVar2 = *piVar1;
  *piVar1 = iVar2 + param_1;
  if (100 < iVar2 + param_1) {
    *piVar1 = 100;
  }
  if (*piVar1 < 0) {
    *piVar1 = 0;
  }
  return;
}

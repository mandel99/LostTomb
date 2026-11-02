#include "restore_types.h"
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
extern undefined4 FUN_004f68a0();

bool FUN_00460f70(int param_1,int *param_2)

{
  int iVar1;
  
  if (*(char *)((int)&DAT_00799aa0 + param_1 * 0x108 + 3) == '\0') {
    return false;
  }
  iVar1 = FUN_004f68a0();
  *param_2 = iVar1;
  return iVar1 != 0;
}

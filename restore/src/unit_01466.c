#include "restore_types.h"
undefined4 FUN_004b63d0(int param_1)

{
  char cVar1;
  
  cVar1 = *(char *)(0x00bcd630u + (uint)param_1 * 0x184u);
  if ((((cVar1 != '\x06') && (cVar1 != '\a')) && (cVar1 != '\x17')) && (cVar1 != '\x16')) {
    return 0;
  }
  return 1;
}

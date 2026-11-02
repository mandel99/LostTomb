#include "restore_types.h"
#define DAT_00e951d4 (*(undefined *)0x00e951d4u)
#define DAT_00e951d8 (*(undefined *)0x00e951d8u)
#define _DAT_00e951d0 (*(undefined4 *)0x00e951d0u)

void FUN_004e1830(void)

{
  DWORD DVar1;
  
  DAT_00e951d8 = 0;
  DAT_00e951d4 = 0;
  DVar1 = timeGetTime();
  _DAT_00e951d0 = DVar1 - 10000;
  return;
}

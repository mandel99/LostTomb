#include "restore_types.h"
#define DAT_005d9d88 (*(undefined4 *)0x005d9d88u)

undefined4 FUN_004495c0(void)

{
  int in_ECX;
  
  return *(undefined4 *)(&DAT_005d9d88 + (uint)*(byte *)(in_ECX + 2) * 4);
}

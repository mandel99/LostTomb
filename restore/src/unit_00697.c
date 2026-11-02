#include "restore_types.h"
#define DAT_005d9d10 (*(undefined4 *)0x005d9d10u)

undefined4 FUN_004493e0(void)

{
  int in_ECX;
  
  return *(undefined4 *)(&DAT_005d9d10 + (uint)*(byte *)(in_ECX + 2) * 4);
}

#include "restore_types.h"
#define DAT_005e4c3c (*(undefined4 *)0x005e4c3cu)
#define DAT_005e4ccc (*(undefined4 *)0x005e4cccu)
#define DAT_00784b94 (*(undefined *)0x00784b94u)
#define _DAT_00784cb0 (*(undefined4 *)0x00784cb0u)

undefined4 FUN_004eaac0(char param_1)

{
  int iVar1;
  
  if (param_1 != '\0') {
    iVar1 = (_DAT_00784cb0 & 0xff) * 0x30;
    return CONCAT31((int3)((uint)iVar1 >> 8),(&DAT_005e4c3c)[iVar1]);
  }
  iVar1 = ((int)DAT_00784b94 + (_DAT_00784cb0 & 0xff) * 2) * 0x30;
  return CONCAT31((int3)((uint)iVar1 >> 8),(&DAT_005e4ccc)[iVar1]);
}

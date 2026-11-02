#include "restore_types.h"
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

uint FUN_004c5190(void)

{
  uint uVar1;
  int iVar2;
  
  iVar2 = DAT_00eaac28 * 0x49d8;
  uVar1 = (uint)(0 < *(short *)(0x00ea61f4u + (uint)iVar2));
  if (0 < *(short *)(0x00ea61f6u + (uint)iVar2)) {
    uVar1 = uVar1 - 2;
  }
  if (0 < *(short *)(0x00ea61f8u + (uint)iVar2)) {
    uVar1 = uVar1 - 1;
  }
  return uVar1;
}

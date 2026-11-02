#include "restore_types.h"
#define DAT_00604f30 (*(undefined4 *)0x00604f30u)
#define DAT_01266434 (*(undefined *)0x01266434u)
#define _DAT_01266430 (*(undefined4 *)0x01266430u)

void FUN_005672c8(uint param_1)

{
  uint *puVar1;
  int iVar2;
  
  iVar2 = 0;
  DAT_01266434 = param_1;
  puVar1 = &DAT_00604f30;
  do {
    if (param_1 == *puVar1) {
      _DAT_01266430 = *(undefined4 *)(iVar2 * 8 + 0x604f34);
      return;
    }
    puVar1 = puVar1 + 2;
    iVar2 = iVar2 + 1;
  } while ((int)puVar1 < 0x605098);
  if ((0x12 < param_1) && (param_1 < 0x25)) {
    _DAT_01266430 = 0xd;
    return;
  }
  if ((param_1 < 0xbc) || (_DAT_01266430 = 8, 0xca < param_1)) {
    _DAT_01266430 = 0x16;
  }
  return;
}

#include "restore_types.h"
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00f56b84 (*(undefined *)0x00f56b84u)
extern uint FUN_0047b8e0();

char FUN_0051c460(int param_1)

{
  int iVar1;
  
  iVar1 = (&DAT_00b7482c)[param_1] - DAT_00f56b84;
  if (0x2f < iVar1) {
    iVar1 = iVar1 + -0x30;
  }
  switch((int)(iVar1)) {
  case 0: ;
  case 7: ;
  case 9: ;
  case 0xf: ;
  case 0x11: ;
  case 0x13: ;
  case 0x2a: ;
  case 0x2c: ;
  case 0x2e: ;
    iVar1 = FUN_0047b8e0(param_1);
    return (-(iVar1 != 0) & 0x1bU) + 0xf;
  case 1: ;
  case 6: ;
  case 8: ;
  case 0x10: ;
  case 0x12: ;
  case 0x14: ;
  case 0x2b: ;
  case 0x2d: ;
  case 0x2f: ;
    iVar1 = FUN_0047b8e0(param_1);
    return (-(iVar1 != 0) & 0x1bU) + 0x10;
default: ;
    return '\x01';
  case 0x15: ;
  case 0x1c: ;
  case 0x1e: ;
  case 0x24: ;
  case 0x26: ;
  case 0x28: ;
    return '$';
  case 0x16: ;
  case 0x1b: ;
  case 0x1d: ;
  case 0x25: ;
  case 0x27: ;
  case 0x29: ;
    return '\x10';
  }
}

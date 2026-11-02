#include "restore_types.h"

int FUN_00487f80(int param_1)

{
  uint3 uVar1;
  
  uVar1 = (uint3)((uint)(param_1 + -100) >> 8);
  switch((int)(param_1)) {
  case 100: ;
  case 0x65: ;
  case 0x66: ;
  case 0x67: ;
  case 0x68: ;
  case 0x69: ;
  case 0xc4: ;
  case 0xe0: ;
    return CONCAT31(uVar1,1);
default: ;
    return (uint)uVar1 << 8;
  }
}

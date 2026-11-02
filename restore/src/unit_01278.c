#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)

int FUN_00488080(int param_1)

{
  uint3 uVar1;
  
  uVar1 = (uint3)((uint)((short)(&DAT_007999e8)[param_1 * 0x84] + -100) >> 8);
  switch((&DAT_007999e8)[param_1 * 0x84]) {
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

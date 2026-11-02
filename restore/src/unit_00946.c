#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)

int FUN_004696b0(int param_1)

{
  uint3 uVar1;
  
  uVar1 = (uint3)((uint)((short)(&DAT_007999e8)[param_1 * 0x84] + -0xb7) >> 8);
  switch((&DAT_007999e8)[param_1 * 0x84]) {
  case 0xb7: ;
  case 0xcf: ;
  case 0xd2: ;
  case 0xd5: ;
  case 0xd7: ;
  case 0xda: ;
  case 0xdb: ;
  case 0xdc: ;
  case 0xde: ;
  case 0xe1: ;
  case 0xe3: ;
  case 0xe4: ;
  case 0xe5: ;
  case 0xe6: ;
  case 0xea: ;
  case 0xeb: ;
  case 0xec: ;
    return (uint)uVar1 << 8;
default: ;
    return CONCAT31(uVar1,1);
  }
}

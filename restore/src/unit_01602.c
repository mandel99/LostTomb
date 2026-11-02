#include "restore_types.h"
#define DAT_007999e8 (*(int16_t *)0x007999e8u)

undefined4 FUN_004c5120(int param_1)

{
  switch((&DAT_007999e8)[param_1 * 0x84]) {
  case 0x3c: ;
  case 0x41: ;
    return 0;
  case 0x3d: ;
  case 0x42: ;
    return 1;
  case 0x3e: ;
  case 0x43: ;
    return 2;
  case 0x3f: ;
  case 0x44: ;
    return 3;
  case 0x40: ;
  case 0x45: ;
    return 4;
default: ;
    return 0xffffffff;
  }
}

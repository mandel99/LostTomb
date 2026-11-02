#include "restore_types.h"
#define DAT_005e4c24 (*(undefined4 *)0x005e4c24u)
#define DAT_005e4cb4 (*(undefined4 *)0x005e4cb4u)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00784b94 (*(undefined *)0x00784b94u)
#define _DAT_00784cb0 (*(undefined4 *)0x00784cb0u)

uint FUN_004ec410(uint param_1,char param_2)

{
  uint uVar1;
  uint3 uVar2;
  uint uVar3;
  
  if (param_2 == '\0') {
    uVar3 = *(uint *)(&DAT_005e4cb4 + ((int)DAT_00784b94 + (_DAT_00784cb0 & 0xff) * 2) * 0x30);
  }
  else {
    uVar3 = *(uint *)(&DAT_005e4c24 + (_DAT_00784cb0 & 0xff) * 0x30);
  }
  if ((param_1 & 0x3038d022) != 0) {
    return param_1 & 0xffffff00;
  }
  uVar1 = param_1 & 0x26351277;
  uVar2 = (uint3)(uVar1 >> 8);
  if ((uVar3 != 0) || (uVar1 != 0)) {
    if (DAT_00605a3c == 1) {
      return CONCAT31(uVar2,(uVar1 & uVar3) != 0);
    }
    if (((param_1 & 4) != 0) && ((uVar3 & 4) == 0)) {
      return (uint)uVar2 << 8;
    }
    if (((param_1 & 0x40000) != 0) && ((uVar3 & 0x40000) == 0)) {
      return (uint)uVar2 << 8;
    }
  }
  return CONCAT31(uVar2,1);
}

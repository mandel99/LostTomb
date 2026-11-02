#include "restore_types.h"
extern undefined4 FUN_004f56a0();

uint FUN_004f5580(int param_1,undefined4 param_2)

{
  char cVar1;
  uint uVar2;
  undefined4 uVar3;
  
  switch((int)(param_1)) {
  case 0xb1: ;
    uVar2 = FUN_004f56a0(param_2,0x14);
    return uVar2;
  case 0xb2: ;
    uVar2 = FUN_004f56a0(param_2,0xc);
    return uVar2;
  case 0xb3: ;
    cVar1 = FUN_004f56a0(param_2,0x18);
    if (cVar1 != '\0') {
      return 1;
    }
    cVar1 = FUN_004f56a0(param_2,0x19);
    if (cVar1 != '\0') {
      return 1;
    }
    cVar1 = FUN_004f56a0(param_2,0x1a);
    if (cVar1 != '\0') {
      return 1;
    }
    cVar1 = FUN_004f56a0(param_2,0x1e);
    if (cVar1 != '\0') {
      return 1;
    }
    cVar1 = FUN_004f56a0(param_2,0x22);
    if (cVar1 != '\0') {
      return 1;
    }
    uVar3 = 0x23;
    break;
default: ;
    return param_1 - 0xb1U & 0xffffff00;
  case 0xe7: ;
    cVar1 = FUN_004f56a0(param_2,0x21);
    if (cVar1 != '\0') {
      return 1;
    }
    uVar3 = 0xb;
  }
  cVar1 = FUN_004f56a0(param_2,uVar3);
  if (cVar1 != '\0') {
    return 1;
  }
  return 0;
}

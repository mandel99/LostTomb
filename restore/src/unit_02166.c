#include "restore_types.h"
extern uint FUN_004f5050();

uint FUN_004f5470(int param_1)

{
  char cVar1;
  uint uVar2;
  undefined4 uVar3;
  
  switch((int)(param_1)) {
  case 0xb1: ;
    uVar2 = FUN_004f5050(0x14,0);
    return uVar2;
  case 0xb2: ;
    uVar2 = FUN_004f5050(0xc,0);
    return uVar2;
  case 0xb3: ;
    cVar1 = FUN_004f5050(0x18,0);
    if (cVar1 != '\0') {
      return 1;
    }
    cVar1 = FUN_004f5050(0x19,0);
    if (cVar1 != '\0') {
      return 1;
    }
    cVar1 = FUN_004f5050(0x1a,0);
    if (cVar1 != '\0') {
      return 1;
    }
    cVar1 = FUN_004f5050(0x1e,0);
    if (cVar1 != '\0') {
      return 1;
    }
    cVar1 = FUN_004f5050(0x22,0);
    if (cVar1 != '\0') {
      return 1;
    }
    uVar3 = 0x23;
    break;
default: ;
    return param_1 - 0xb1U & 0xffffff00;
  case 0xe7: ;
    cVar1 = FUN_004f5050(0x21,0);
    if (cVar1 != '\0') {
      return 1;
    }
    uVar3 = 0xb;
  }
  cVar1 = FUN_004f5050(uVar3,0);
  if (cVar1 != '\0') {
    return 1;
  }
  return 0;
}

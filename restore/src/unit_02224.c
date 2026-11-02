#include "restore_types.h"
#define DAT_005e4c20 (*(undefined4 *)0x005e4c20u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00f55c7c (*(undefined *)0x00f55c7cu)
#define DAT_00f55c84 (*(undefined *)0x00f55c84u)
#define DAT_00f55c8c (*(undefined *)0x00f55c8cu)
#define DAT_00f571ec (*(undefined *)0x00f571ecu)
#define _DAT_00784cb0 (*(undefined4 *)0x00784cb0u)
extern undefined4 FUN_004c5120();

int FUN_004fa8c0(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  
  iVar4 = param_1 * 0x184;
  uVar3 = (uint)(byte)(&DAT_00bcd602)[iVar4];
  switch((int)(uVar3)) {
  case 0x18: ;
    return DAT_00f571ec + 3;
default: ;
    iVar2 = DAT_00f55c7c + uVar3;
    break;
  case 0x1b: ;
    uVar1 = FUN_004c5120((int)(short)(&DAT_00bcd652)[param_1 * 0xc2]);
    switch((int)(uVar1)) {
    case 1: ;
      return DAT_00f55c8c + 3;
    case 2: ;
      return DAT_00f55c8c + 2;
    case 3: ;
      return DAT_00f55c8c + 4;
    case 4: ;
      return DAT_00f55c8c + 1;
default: ;
      return (uint)(byte)(&DAT_00bcd602)[iVar4] + DAT_00f55c7c;
    }
  case 0x1e: ;
    return DAT_00f55c7c + 0x1d;
  case 0x49: ;
    iVar4 = *(int *)(&DAT_005e4c20 + (_DAT_00784cb0 & 0xff) * 0x30);
    if (iVar4 == 0x44) {
      return DAT_00f55c8c;
    }
    if (iVar4 == 0x45) {
      return DAT_00f55c84;
    }
    iVar2 = DAT_00f55c7c;
    if (iVar4 == 0x46) {
      return DAT_00f55c7c + uVar3;
    }
    break;
  case 0x5b: ;
    if ((&DAT_00bcd603)[iVar4] != 0x1b) {
      return (uint)(byte)(&DAT_00bcd603)[iVar4] + DAT_00f55c7c;
    }
    uVar1 = FUN_004c5120((int)(short)(&DAT_00bcd652)[param_1 * 0xc2]);
    switch((int)(uVar1)) {
    case 1: ;
      return DAT_00f55c8c + 3;
    case 2: ;
      return DAT_00f55c8c + 2;
    case 3: ;
      return DAT_00f55c8c + 4;
    case 4: ;
      return DAT_00f55c8c + 1;
default: ;
      return (uint)(byte)(&DAT_00bcd603)[iVar4] + DAT_00f55c7c;
    }
  case 0x66: ;
    return DAT_00f571ec + 4;
  case 0x67: ;
    return DAT_00f571ec + 5;
  case 0x68: ;
    return DAT_00f571ec + 2;
  case 0x69: ;
    return DAT_00f571ec;
  case 0x6a: ;
    return DAT_00f571ec + 6;
  case 0x6b: ;
    return DAT_00f571ec + 7;
  case 0x6c: ;
    return DAT_00f571ec + 1;
  case 0x6d: ;
    return DAT_00f571ec + 8;
  }
  return iVar2;
}

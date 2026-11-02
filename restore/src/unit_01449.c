#include "restore_types.h"
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
extern void FUN_0053b4a0();
extern uint FUN_00562348();

void FUN_004b5050(int param_1)

{
  int iVar1;
  
  switch((&DAT_00bcd602)[param_1 * 0x184]) {
  case 0xd: ;
  case 0x2d: ;
  case 0x38: ;
    FUN_0053b4a0(param_1,0xd,3);
    return;
default: ;
    iVar1 = FUN_00562348();
    FUN_0053b4a0(param_1,((int)(iVar1 * 8 + (iVar1 * 8 >> 0x1f & 0x7fffU)) >> 0xf) + 0x15,3);
    return;
  case 0x1a: ;
    iVar1 = FUN_00562348();
    FUN_0053b4a0(param_1,((int)(iVar1 * 4 + (iVar1 * 4 >> 0x1f & 0x7fffU)) >> 0xf) + 0x1d,3);
    return;
  case 0x44: ;
    FUN_0053b4a0(param_1,8,3);
    return;
  case 0x45: ;
    FUN_0053b4a0(param_1,4,3);
    return;
  case 0x46: ;
    FUN_0053b4a0(param_1,5,3);
    return;
  case 0x4d: ;
  case 0x4e: ;
  case 0x5c: ;
  case 0x5d: ;
  case 100: ;
  case 0x65: ;
    FUN_0053b4a0(param_1,0x11,3);
    return;
  case 0x52: ;
    FUN_0053b4a0(param_1,7,3);
    return;
  case 0x53: ;
    FUN_0053b4a0(param_1,0,3);
    return;
  case 0x54: ;
    FUN_0053b4a0(param_1,6,3);
    return;
  case 0x66: ;
    FUN_0053b4a0(param_1,3,3);
    return;
  case 0x67: ;
    FUN_0053b4a0(param_1,1,3);
    return;
  case 0x68: ;
    FUN_0053b4a0(param_1,2,3);
    return;
  }
}

#include "restore_types.h"
#define DAT_00bca2bc (*(undefined *)0x00bca2bcu)
#define DAT_00bca2c0 (*(undefined *)0x00bca2c0u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f53f78 (*(undefined *)0x00f53f78u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern void FUN_004304c0();
extern void FUN_00430e60();
extern void FUN_004ddec0();
extern void FUN_004ddf20();
extern void FUN_004ddf70();
extern void FUN_004de080();

void FUN_004fa330(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar2 = DAT_00bca2c0;
  uVar1 = DAT_00bca2bc;
  FUN_004304c0(param_1);
  DAT_00bca2bc = _DAT_00d3b4e4 + -2;
  DAT_00bca2c0 = _DAT_00d3b4e0 + -6;
  FUN_004de080();
  FUN_004ddf70();
  FUN_00430e60(DAT_00bca2bc,DAT_00bca2c0,DAT_00bca2d8,param_2);
  DAT_00bca2bc = uVar1;
  DAT_00bca2c0 = uVar2;
  if (DAT_00f53f78 == 1) {
    FUN_004ddec0();
    return;
  }
  FUN_004ddf20();
  return;
}

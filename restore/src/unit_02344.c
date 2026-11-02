#include "restore_types.h"
#include "city_ui_controllers.h"
#define DAT_00605a3c (*(int32_t *)0x00605a3cu)
#define DAT_00e38d98 (*(int32_t *)0x00e38d98u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dac (*(int32_t *)0x00e38dacu)
#define DAT_00e38db0 (*(int32_t *)0x00e38db0u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00f53f60 (*(int32_t *)0x00f53f60u)
#define DAT_00f53f64 (*(int32_t *)0x00f53f64u)
#define DAT_00f53f78 (*(int32_t *)0x00f53f78u)
#define DAT_00f55c64 (*(int32_t *)0x00f55c64u)
#define DAT_00f567e4 (*(int32_t *)0x00f567e4u)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
#define PTR_DAT_005c7760 (*(unsigned char * *)0x005c7760u)
extern void FUN_00425730();
extern void FUN_004cd9d0();
extern void FUN_004cda40();
extern void FUN_004d62f0();
extern void FUN_004dddc0();
extern int FUN_0053e590();

void FUN_0051e990(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  if (DAT_00e38db0 == 0x400) {
    FUN_00537120();
  }
  else if (DAT_00e38db0 == 800) {
    FUN_00537100();
  }
  if (DAT_00e38db0 == 0x400) {
    uVar3 = 9;
    iVar1 = DAT_00e38dac + -9;
    iVar2 = 0x400;
  }
  else {
    uVar3 = 6;
    iVar1 = DAT_00e38dac + -6;
    iVar2 = DAT_00e38db0;
  }
  FUN_004cd9d0(0,iVar1,iVar2,uVar3,0);
  DAT_00e38dc9 = 1;
  if (DAT_00605a3c == 0) {
    iVar2 = DAT_00f567e4;
    iVar1 = DAT_00f53f64;
    if (DAT_00f53f78 != 0) {
      iVar2 = DAT_00f567e4 + 1;
      iVar1 = DAT_00f53f60;
    }
    FUN_0053e590(iVar2,iVar1,0x1e);
    FUN_0051ec90();
    FUN_0051ed30(1,DAT_00f53f64 + 0xb);
    FUN_0051edd0(0,DAT_00f53f64 + 0xb);
    FUN_0051efb0(1);
    if (DAT_00e38db0 == 0x400) {
      if (DAT_00f53f78 == 0) {
        FUN_0053e590(DAT_00f567e4 + 4,DAT_00f53f64,0x1e0);
        FUN_0051f0a0(1);
        FUN_0051f680(1);
        return;
      }
      if (DAT_00f53f78 == 1) {
        FUN_0053e590(DAT_00f567e4 + 5,DAT_00f53f60,0x1e0);
        FUN_0051f0a0(1);
        FUN_0051f680(1);
        return;
      }
    }
    else if (DAT_00e38db0 == 800) {
      if (DAT_00f53f78 == 0) {
        FUN_0053e590(DAT_00f567e4 + 6,DAT_00f53f64,0x1e0);
        FUN_0051f0a0(1);
        FUN_0051f680(1);
        return;
      }
      if (DAT_00f53f78 == 1) {
        FUN_0053e590(DAT_00f567e4 + 7,DAT_00f53f60,0x1e0);
        FUN_0051f0a0(1);
        FUN_0051f680(1);
        return;
      }
    }
    goto LAB_0051ec70;
  }
  FUN_004cda40(DAT_00f53f64 + 6,0x18,(DAT_00e38db0 - DAT_00f53f64) + -6,DAT_00e38d98 + -0x18,0);
  FUN_004dddc0();
  FUN_0053e590(DAT_00f55c64,DAT_00f53f64,0x18);
  if (DAT_00e38db0 == 0x400) {
    iVar2 = DAT_00f55c64 + 2;
LAB_0051ebe4: ;
    FUN_0053e590(iVar2,DAT_00f53f64,0x1e0);
  }
  else if (DAT_00e38db0 == 800) {
    iVar2 = DAT_00f55c64 + 1;
    goto LAB_0051ebe4;
  }
  FUN_00533e70();
  FUN_00425730(DAT_00f53f64,0x18,&PTR_DAT_005c7760,0xf);
  FUN_0051efb0(1);
  FUN_004d62f0(0x2c,0x84,DAT_00f53f64 + 0xb,0x98,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(2,0,DAT_00f53f64 + 0x5e,0x98,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
LAB_0051ec70: ;
  FUN_0051f0a0(1);
  FUN_0051f680(1);
  return;
}

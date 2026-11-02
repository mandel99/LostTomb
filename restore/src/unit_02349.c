#include "restore_types.h"
#include "city_ui_controllers.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005c79c8 (*(int16_t *)0x005c79c8u)
#define DAT_005c91f0 (*(undefined4 *)0x005c91f0u)
#define DAT_005c9230 (*(undefined4 *)0x005c9230u)
#define DAT_00605a3c (*(int32_t *)0x00605a3cu)
#define DAT_0078d7bc (*(int32_t *)0x0078d7bcu)
#define DAT_0078d7c0 (*(int32_t *)0x0078d7c0u)
#define DAT_00e20778 (*(int32_t *)0x00e20778u)
#define DAT_00e37078 (*(undefined4 *)0x00e37078u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38db0 (*(int32_t *)0x00e38db0u)
#define DAT_00e38dda (*(uint8_t *)0x00e38ddau)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00eaac28 (*(int32_t *)0x00eaac28u)
#define DAT_00f53da4 (*(int32_t *)0x00f53da4u)
#define DAT_00f53da8 (*(int32_t *)0x00f53da8u)
#define DAT_00f53dac (*(int32_t *)0x00f53dacu)
#define DAT_00f53dbc (*(int32_t *)0x00f53dbcu)
#define DAT_00f53dc0 (*(int32_t *)0x00f53dc0u)
#define DAT_00f53dc4 (*(int32_t *)0x00f53dc4u)
#define DAT_00f53f74 (*(int32_t *)0x00f53f74u)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
#define DAT_00f56f9c (*(int32_t *)0x00f56f9cu)
#define DAT_00f56fa4 (*(int32_t *)0x00f56fa4u)
extern void FUN_00425330();
extern void FUN_00425900();
extern void FUN_004cc770();
extern void FUN_004cccb0();
extern void FUN_004d62f0();
extern void FUN_004d6860();
extern void FUN_00536670();

void FUN_0051f0a0(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  undefined4 uVar3;
  
  uVar1 = DAT_00f56f8c;
  if ((DAT_00605a3c == 1) && (DAT_00e38dda == 1)) {
    FUN_004cc770(&DAT_00e37078,0x104,4,DAT_00f56f9c,0);
  }
  switch((int)(DAT_00e38dda)) {
  case 0: ;
  case 4: ;
  case 5: ;
  case 7: ;
  case 0xd: ;
  case 0xe: ;
  case 0xf: ;
  case 0x10: ;
  case 0x11: ;
  case 0x12: ;
  case 0x13: ;
  case 0x14: ;
  case 0x15: ;
  case 0x17: ;
  case 0x26: ;
  case 0x28: ;
  case 0x2a: ;
  case 0x2b: ;
  case 0x2c: ;
  case 0x2d: ;
  case 0x2e: ;
  case 0x2f: ;
  case 0x30: ;
  case 0x3f: ;
  case 0x48: ;
  case 0x4a: ;
  case 0x4c: ;
  case 0x4d: ;
  case 0x52: ;
    goto switchD_0051f0ef_caseD_0;
  }
  if (0x32 < DAT_00e38dda) {
    return;
  }
  iVar2 = 1;
  if (*(int *)((uint8_t *)(uintptr_t)0x00ea1894u + (uint32_t)DAT_00eaac28 * 0x49d8u) == DAT_00f53dac) {
    iVar2 = param_1;
  }
  if (*(int *)((uint8_t *)(uintptr_t)0x00ea187cu + (uint32_t)DAT_00eaac28 * 0x49d8u) != DAT_00f53da8) {
    iVar2 = 1;
  }
  if ((DAT_0078d7c0 == DAT_00f53da4) && (iVar2 == 0)) {
    if (DAT_00f53f74 < 1) {
      return;
    }
    if ((DAT_00f53f74 == 1) || (DAT_00f53f74 == 2)) goto LAB_0051f184;
  }
  else {
    DAT_00f53f74 = 1;
LAB_0051f184: ;
    FUN_00536670(0,DAT_00e38db0);
  }
  DAT_00f53f74 = 0;
  DAT_00f53dac = *(int *)((uint8_t *)(uintptr_t)0x00ea1894u + (uint32_t)DAT_00eaac28 * 0x49d8u);
  DAT_00f53da8 = *(int *)((uint8_t *)(uintptr_t)0x00ea187cu + (uint32_t)DAT_00eaac28 * 0x49d8u);
  if (DAT_00605a3c == 1) {
    FUN_00425330(&DAT_005c9230,4);
    DAT_005c79c8 = (int16_t)(DAT_00e38db0 - DAT_00e38db0 / 5);
    FUN_00425900();
    return;
  }
  FUN_00425330(&DAT_005c91f0,4);
  DAT_00e20778 = 0;
  uVar3 = DAT_00f56f8c;
  if (*(int *)((uint8_t *)(uintptr_t)0x00ea187cu + (uint32_t)DAT_00eaac28 * 0x49d8u) < 0) {
    uVar3 = DAT_00f56fa4;
  }
  if (DAT_00e38db0 == 0x280) {
    DAT_00f53dc4 = 0xf2;
    DAT_00f53dc0 = 0x16f;
    DAT_00f53dbc = 0x1e5;
    FUN_004d62f0(6,0,0xfa,5,uVar3,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(*(undefined4 *)((uint8_t *)(uintptr_t)0x00ea187cu + (uint32_t)DAT_00eaac28 * 0x49d8u),0x40,&DAT_0057c048,
                 DAT_00e20778 + 0xf5,5,uVar3,0,DAT_00e38da8,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d62f0(6,1,0x177,5,uVar1,0xffff,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(*(undefined4 *)((uint8_t *)(uintptr_t)0x00ea1894u + (uint32_t)DAT_00eaac28 * 0x49d8u),0x40,&DAT_0057c048,
                 DAT_00e20778 + 0x16f,5,uVar1,0xffff,DAT_00e38da8,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d62f0(0x19,DAT_0078d7c0,0x203,5,uVar1,0,DAT_00e38da8,DAT_00e38da4);
    iVar2 = DAT_00e20778 + 0x1f8;
    uVar3 = 2;
LAB_0051f5db: ;
    FUN_004cccb0(uVar3,DAT_0078d7bc,iVar2,5,uVar1,0);
  }
  else {
    if (DAT_00e38db0 == 800) {
      DAT_00f53dc4 = 0x152;
      DAT_00f53dc0 = 0x1ca;
      DAT_00f53dbc = 0x28c;
      FUN_004d62f0(6,0,0x157,5,uVar3,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004d6860(*(undefined4 *)((uint8_t *)(uintptr_t)0x00ea187cu + (uint32_t)DAT_00eaac28 * 0x49d8u),0x40,&DAT_0057c048,
                   DAT_00e20778 + 0x153,5,uVar3,0,DAT_00e38da8,DAT_00e38da4);
      DAT_00e20778 = 0;
      FUN_004d62f0(6,1,0x1d6,5,uVar1,0xffff,DAT_00e38da8,DAT_00e38da4);
      FUN_004d6860(*(undefined4 *)((uint8_t *)(uintptr_t)0x00ea1894u + (uint32_t)DAT_00eaac28 * 0x49d8u),0x40,&DAT_0057c048,
                   DAT_00e20778 + 0x1d2,5,uVar1,0xffff,DAT_00e38da8,DAT_00e38da4);
      DAT_00e20778 = 0;
      FUN_004d62f0(0x19,DAT_0078d7c0,0x28f,5,uVar1,0,DAT_00e38da8,DAT_00e38da4);
      iVar2 = DAT_00e20778 + 0x28f;
LAB_0051f5da: ;
      uVar3 = 0;
      goto LAB_0051f5db;
    }
    if (DAT_00e38db0 == 0x400) {
      DAT_00f53dc4 = 0x1ed;
      DAT_00f53dc0 = 0x27d;
      DAT_00f53dbc = 0x375;
      FUN_004d62f0(6,0,0x1ef,5,uVar3,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004d6860(*(undefined4 *)((uint8_t *)(uintptr_t)0x00ea187cu + (uint32_t)DAT_00eaac28 * 0x49d8u),0x40,&DAT_0057c048,
                   DAT_00e20778 + 0x1f5,5,uVar3,0,DAT_00e38da8,DAT_00e38da4);
      DAT_00e20778 = 0;
      FUN_004d62f0(6,1,0x285,5,uVar1,0xffff,DAT_00e38da8,DAT_00e38da4);
      FUN_004d6860(*(undefined4 *)((uint8_t *)(uintptr_t)0x00ea1894u + (uint32_t)DAT_00eaac28 * 0x49d8u),0x40,&DAT_0057c048,
                   DAT_00e20778 + 0x28b,5,uVar1,0xffff,DAT_00e38da8,DAT_00e38da4);
      DAT_00e20778 = 0;
      FUN_004d62f0(0x19,DAT_0078d7c0,0x373,5,uVar1,0,DAT_00e38da8,DAT_00e38da4);
      iVar2 = DAT_00e20778 + 0x373;
      goto LAB_0051f5da;
    }
  }
  if (DAT_00e38db0 == 0x280) {
    DAT_005c79c8 = (short)DAT_00f53dbc + -10;
    FUN_00425900();
    return;
  }
  DAT_005c79c8 = (short)DAT_00f53dbc + -0x2f;
  FUN_00425900();
switchD_0051f0ef_caseD_0: ;
  return;
}

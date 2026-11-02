#include "restore_types.h"
#include "city_ui_controllers.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_00605a30 (*(int32_t *)0x00605a30u)
#define DAT_00605a34 (*(int32_t *)0x00605a34u)
#define DAT_00784b6c (*(int32_t *)0x00784b6cu)
#define DAT_00784b84 (*(int32_t *)0x00784b84u)
#define DAT_00784b94 (*(int16_t *)0x00784b94u)
#define DAT_00784be0 (*(undefined4 *)0x00784be0u)
#define DAT_00784cb0 (*(undefined1 *)0x00784cb0u)
#define DAT_00e20778 (*(int32_t *)0x00e20778u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f53f64 (*(int32_t *)0x00f53f64u)
#define DAT_00f56f84 (*(int32_t *)0x00f56f84u)
#define _DAT_00784c60 (*(int16_t *)0x00784c60u)
#define _DAT_00784c62 (*(int16_t *)0x00784c62u)
#define _DAT_00784c64 (*(int16_t *)0x00784c64u)
#define _DAT_00784c66 (*(int16_t *)0x00784c66u)
#define _DAT_00784c68 (*(int16_t *)0x00784c68u)
#define _DAT_00784c6a (*(int16_t *)0x00784c6au)
#define _DAT_00784c8c (*(int16_t *)0x00784c8cu)
#define _DAT_00784c8e (*(int16_t *)0x00784c8eu)
#define _DAT_00784c90 (*(int16_t *)0x00784c90u)
#define _DAT_00784c92 (*(int16_t *)0x00784c92u)
#define _DAT_00784cb0 (*(undefined4 *)0x00784cb0u)
extern int FUN_0044bd00();
extern void FUN_004cd9d0();
extern void FUN_004d62f0();
extern void FUN_004d6860();
extern void FUN_0051f780();

void FUN_00533e70(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  undefined2 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  
  iVar2 = DAT_00f53f64;
  iVar1 = DAT_00f53f64 + 8;
  FUN_004cd9d0(iVar1,0xaf,0x91,0x6f,0);
  iVar2 = iVar2 + 10;
  FUN_004d62f0(0x31,DAT_00605a30,iVar2,0xb1,DAT_00f56f84,0xfac1,DAT_00e38da8,DAT_00e38da4);
  if (DAT_00605a30 == 0) {
    iVar3 = 0xfac1;
  }
  else if (DAT_00605a30 == 1) {
    iVar3 = 0xfac1;
  }
  else if (DAT_00605a30 == 2) {
    iVar3 = 0xfac1;
  }
  else if (DAT_00605a30 == 4) {
    iVar3 = 0xfac1;
  }
  else if (DAT_00605a30 == 5) {
    iVar3 = 0xfac1;
  }
  else if (DAT_00605a30 == 0x21) {
    iVar3 = 0xfac1;
  }
  else if (DAT_00605a30 == 0x1e) {
    iVar3 = 0xfac1;
  }
  else if (DAT_00605a30 == 6) {
    iVar3 = 0xfac1;
  }
  else if (DAT_00605a30 == 0xb) {
    iVar3 = 0xfac1;
  }
  else if (DAT_00605a30 == 0xc) {
    iVar3 = 0xfac1;
  }
  else {
    iVar3 = (-(uint)(DAT_00605a30 != 0x1a) & 0xffff4747) + 0xfac1;
  }
  FUN_004d62f0(0x30,DAT_00605a34,iVar2,0xc1,DAT_00f56f84,iVar3,DAT_00e38da8,DAT_00e38da4);
  if (DAT_00784b94 == 1) {
    iVar3 = (_DAT_00784cb0 & 0xff) + 0x66;
  }
  else if (DAT_00784cb0 == '\x02') {
    iVar3 = 0x53;
  }
  else if (DAT_00784cb0 == '\x01') {
    iVar3 = 0x52;
  }
  else {
    iVar3 = 0x54;
  }
  FUN_004d62f0(0x40,iVar3,iVar2,0xd1,DAT_00f56f84,0xfac1,DAT_00e38da8,DAT_00e38da4);
  iVar3 = FUN_0044bd00();
  if (iVar3 != 0) {
    if ((_DAT_00784c60 == -1) || (_DAT_00784c62 == -1)) {
      uVar7 = 0xf800;
      uVar6 = 0x39;
    }
    else {
      uVar7 = 0x7e0;
      uVar6 = 0x3a;
    }
    FUN_004d62f0(0x2c,uVar6,iVar2,0xd3,DAT_00f56f84,uVar7,DAT_00e38da8,DAT_00e38da4);
  }
  if ((_DAT_00784c64 == -1) || (_DAT_00784c66 == -1)) {
    uVar7 = 0xf800;
    uVar6 = 0x3b;
  }
  else {
    uVar7 = 0x7e0;
    uVar6 = 0xe0;
  }
  FUN_004d62f0(0x2c,uVar6,iVar2,0xe1,DAT_00f56f84,uVar7,DAT_00e38da8,DAT_00e38da4);
  if ((_DAT_00784c68 == -1) || (_DAT_00784c6a == -1)) {
    uVar7 = 0xf800;
    uVar6 = 0x3d;
  }
  else {
    uVar7 = 0x7e0;
    uVar6 = 0xe1;
  }
  FUN_004d62f0(0x2c,uVar6,iVar2,0xef,DAT_00f56f84,uVar7,DAT_00e38da8,DAT_00e38da4);
  iVar3 = 0;
  psVar4 = &DAT_00784be0;
  do {
    if ((psVar4[-0x10] != -1) && (*psVar4 != -1)) {
      iVar3 = iVar3 + 1;
    }
    psVar4 = psVar4 + 1;
  } while ((int)psVar4 < 0x784c00);
  puVar5 = &DAT_00784b84;
  do {
    puVar5 = puVar5 + 1;
  } while ((int)puVar5 < 0x784b94);
  puVar5 = &DAT_00784b6c;
  do {
    puVar5 = puVar5 + 1;
  } while ((int)puVar5 < 0x784b74);
  if (iVar3 < 1) {
    FUN_004d62f0(0x2c,0x3f,iVar2,0xfd,DAT_00f56f84,0xf800,DAT_00e38da8,DAT_00e38da4);
  }
  else if (iVar3 == 1) {
    FUN_004d62f0(0x2c,0x40,iVar2,0xfd,DAT_00f56f84,0x7e0,DAT_00e38da8,DAT_00e38da4);
  }
  else {
    DAT_00e20778 = 0;
    FUN_004d6860(iVar3,0x40,&DAT_0057c048,iVar2,0xfd,DAT_00f56f84,0x7e0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x2c,0x41,DAT_00e20778 + 2 + iVar1,0xfd,DAT_00f56f84,0x7e0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  if (((_DAT_00784c8c == -1) || (_DAT_00784c8e == -1)) &&
     ((_DAT_00784c90 == -1 || (_DAT_00784c92 == -1)))) {
    uVar7 = 0xf800;
    uVar6 = 0x42;
  }
  else if ((((_DAT_00784c8c == -1) || (_DAT_00784c8e == -1)) || (_DAT_00784c90 == -1)) ||
          (_DAT_00784c92 == -1)) {
    uVar7 = 0xf800;
    uVar6 = 0xe2;
  }
  else {
    uVar7 = 0x7e0;
    uVar6 = 0x43;
  }
  FUN_004d62f0(0x2c,uVar6,iVar2,0x10b,DAT_00f56f84,uVar7,DAT_00e38da8,DAT_00e38da4);
  FUN_0051f780();
  FUN_0051f780();
  return;
}

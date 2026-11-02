#include "restore_types.h"
#include "file_list.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_00605a3c (*(int32_t *)0x00605a3cu)
#define DAT_007009dc (*(undefined4 *)0x007009dcu)
#define DAT_00700a04 (*(undefined4 *)0x00700a04u)
#define DAT_00700a08 (*(undefined4 *)0x00700a08u)
#define DAT_00700a0c (*(undefined4 *)0x00700a0cu)
#define DAT_00700a10 (*(undefined4 *)0x00700a10u)
#define DAT_00700a14 (*(undefined4 *)0x00700a14u)
#define DAT_00700a18 (*(undefined4 *)0x00700a18u)
#define DAT_00700a1c (*(undefined4 *)0x00700a1cu)
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
#define DAT_00704dac (*(int32_t *)0x00704dacu)
#define DAT_00704db0 (*(int32_t *)0x00704db0u)
#define DAT_00704dbc (*(int32_t *)0x00704dbcu)
#define DAT_007848d8 (*(int16_t *)0x007848d8u)
#define DAT_00784904 (*(int32_t *)0x00784904u)
#define DAT_00784914 (*(undefined4 *)0x00784914u)
#define DAT_00784b5e (*(int16_t *)0x00784b5eu)
#define DAT_00784b60 (*(int16_t *)0x00784b60u)
#define DAT_00784c24 (*(int32_t *)0x00784c24u)
#define DAT_00784c28 (*(int32_t *)0x00784c28u)
#define DAT_00784c30 (*(int32_t *)0x00784c30u)
#define DAT_00784c34 (*(int32_t *)0x00784c34u)
#define DAT_00784c38 (*(int32_t *)0x00784c38u)
#define DAT_00784c3c (*(undefined *)0x00784c3cu)
#define DAT_00784c3d (*(char *)0x00784c3du)
#define DAT_00784c3f (*(char *)0x00784c3fu)
#define DAT_00784c48 (*(int32_t *)0x00784c48u)
#define DAT_00784c4c (*(int32_t *)0x00784c4cu)
#define DAT_00784c50 (*(int32_t *)0x00784c50u)
#define DAT_00784c54 (*(int32_t *)0x00784c54u)
#define DAT_00784c58 (*(int32_t *)0x00784c58u)
#define DAT_00784c5c (*(int32_t *)0x00784c5cu)
#define DAT_00784de0 (*(int16_t *)0x00784de0u)
#define DAT_00784f0c (*(int32_t *)0x00784f0cu)
#define DAT_00d3b568 (*(int32_t *)0x00d3b568u)
#define DAT_00d3b56c (*(int32_t *)0x00d3b56cu)
#define DAT_00d3b570 (*(int32_t *)0x00d3b570u)
#define DAT_00d3b574 (*(int32_t *)0x00d3b574u)
#define DAT_00e20764 (*(uint32_t *)0x00e20764u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e20778 (*(int32_t *)0x00e20778u)
#define DAT_00e37078 (*(undefined4 *)0x00e37078u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dac (*(int32_t *)0x00e38dacu)
#define DAT_00e38db0 (*(int32_t *)0x00e38db0u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38df8 (*(undefined1 *)0x00e38df8u)
#define DAT_00e38e04 (*(undefined1 *)0x00e38e04u)
#define DAT_00e38ea8 (*(int8_t *)0x00e38ea8u)
#define DAT_00e39080 (*(int32_t *)0x00e39080u)
#define DAT_00f55c2c (*(int32_t *)0x00f55c2cu)
#define DAT_00f55c94 (*(int32_t *)0x00f55c94u)
#define DAT_00f55cb4 (*(int32_t *)0x00f55cb4u)
#define DAT_00f55cbc (*(int32_t *)0x00f55cbcu)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
#define DAT_00f56f94 (*(int32_t *)0x00f56f94u)
#define DAT_00f56f9c (*(int32_t *)0x00f56f9cu)
#define DAT_00f56fb4 (*(int32_t *)0x00f56fb4u)
#define DAT_00f57314 (*(int32_t *)0x00f57314u)
#define _DAT_00784cb0 (*(undefined4 *)0x00784cb0u)

extern void FUN_0041e1b0();
extern int FUN_0041e3b0();
extern int FUN_0041e470();
extern void FUN_0041f220();
extern void FUN_00448370();
extern void FUN_004cc110();
extern void FUN_004cc160();
extern void FUN_004cc720();
extern void FUN_004ccb30();
extern void FUN_004ccbb0();
extern void FUN_004cccb0();
extern void FUN_004ccd60();
extern void FUN_004cce00();
extern void FUN_004ccf20();
extern void FUN_004ccfd0();
extern void FUN_00536a30();
extern int FUN_0053e590();

void FUN_0041e5c0(void)

{
  int iVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 uVar7;
  int iVar8;
  short *local_d4;
  int local_d0;
  undefined1 local_c8 [200];
  
  uVar7 = DAT_00f56f94;
  iVar4 = DAT_00e38da8;
  iVar1 = DAT_00e38da4;
  DAT_00d3b56c = (0x400 - DAT_00e38db0) / 2;
  DAT_00d3b568 = (0x300 - DAT_00e38dac) / 2;
  DAT_00d3b574 = DAT_00e38db0;
  DAT_00e38df8 = 0;
  DAT_00e38e04 = 0;
  iVar2 = DAT_00e38da8 + 0x140;
  DAT_00d3b570 = DAT_00e38dac;
  if (DAT_00e38dca == '\x01') {
    if (DAT_00605a3c == 1) {
      iVar8 = 0;
      iVar6 = 0;
      uVar5 = DAT_00f55c2c;
    }
    else if (DAT_00704db0 == 0) {
      iVar8 = DAT_00e38da4 + -0x90;
      iVar6 = DAT_00e38da8 + -0xc0;
      uVar5 = DAT_00f55cb4;
    }
    else {
      iVar8 = DAT_00e38da4 + -0x90;
      iVar6 = DAT_00e38da8 + -0xc0;
      uVar5 = DAT_00f55cbc;
    }
    FUN_0053e590(uVar5,iVar6,iVar8);
  }
  DAT_00e38dc9 = 1;
  if (DAT_00704db0 == 1) {
    iVar8 = 0xd;
    iVar6 = 0xfc;
  }
  else {
    iVar8 = 0xf;
    iVar6 = 0xdc;
  }
  FUN_00536a30(DAT_00e38da8 + 0x11e,DAT_00e38da4 + 0x16 + iVar6,2,iVar8 + -3);
  FUN_0041f220();
  FUN_0041e1b0();
  if (DAT_00704db0 == 1) {
    iVar6 = iVar1 + 0x38;
  }
  else {
    iVar6 = iVar1 + 0x24;
  }
  if (DAT_00784b5e < 0x13) {
    iVar8 = DAT_00784b5e + DAT_00f55c94;
  }
  else {
    iVar8 = DAT_00784b5e + -0x13 + DAT_00f57314;
  }
  FUN_0053e590(iVar8,iVar4 + 0x4e,iVar6);
  if (DAT_00704db0 == 1) {
    FUN_004cc110(0x125,6);
    uVar5 = DAT_00784f0c;
    DAT_00784f0c = 0;
    FUN_00448370(DAT_00e20768,local_c8,200);
    DAT_00784f0c = uVar5;
    FUN_004cc720(local_c8,DAT_00e38da8 + 0x14,DAT_00e38da4 + 0x11,600,DAT_00f56fb4,0);
  }
  iVar6 = iVar4 + 0x14f;
  FUN_004cc720(&DAT_00e37078,iVar6,iVar1 + 0x41,0x104,DAT_00f56f94,0);
  FUN_004cc720(&DAT_00784914,iVar6,iVar1 + 0x55,0x104,DAT_00f56f9c,0);
  FUN_004cccb0(1,(int)DAT_007848d8,iVar4 + 0x19a,iVar1 + 0x69,DAT_00f56f94,0);
  if (DAT_00704dac == 3) {
    FUN_004ccb30(0x2c,(_DAT_00784cb0 & 0xff) + 0x4d,iVar6,iVar1 + 0x7d,0x104,uVar7,0);
    switch((int)(DAT_00784904)) {
    case 0x38: ;
      uVar5 = 0x79;
      break;
default: ;
      uVar5 = 0x7e;
      break;
    case 0x54: ;
      uVar5 = 0x7a;
      break;
    case 0x70: ;
      uVar5 = 0x7b;
      break;
    case 0x8c: ;
      uVar5 = 0x7c;
      break;
    case 0xaa: ;
      uVar5 = 0x7d;
    }
    FUN_004ccb30(0x2c,uVar5,iVar6,iVar1 + 0x91,0x104,uVar7,0);
    iVar8 = FUN_0041e3b0();
    if (iVar8 < 1) {
      uVar5 = 0x70;
    }
    else if (iVar8 < 3) {
      uVar5 = 0x71;
    }
    else if (iVar8 < 5) {
      uVar5 = 0x72;
    }
    else if (iVar8 < 0xb) {
      uVar5 = 0x73;
    }
    else {
      uVar5 = 0x74;
    }
    FUN_004ccb30(0x2c,uVar5,iVar6,iVar1 + 0xa5,0x104,uVar7,0);
    iVar8 = FUN_0041e470();
    if (iVar8 < 0x65) {
      cVar3 = '\0';
    }
    else if (iVar8 < 0xc9) {
      cVar3 = '\x01';
    }
    else if (iVar8 < 0x191) {
      cVar3 = '\x02';
    }
    else if (iVar8 < 0x259) {
      cVar3 = '\x03';
    }
    else if (iVar8 < 0x385) {
      cVar3 = '\x04';
    }
    else if (iVar8 < 0x515) {
      cVar3 = '\x05';
    }
    else if (iVar8 < 0x6a5) {
      cVar3 = '\x06';
    }
    else if (iVar8 < 0x835) {
      cVar3 = '\a';
    }
    else {
      cVar3 = (0x9c4 < iVar8) + '\b';
    }
    FUN_004ccb30(0x20,cVar3 + '\x16',iVar6,iVar1 + 0xb9,0x104,uVar7,0);
    if (DAT_00784b60 == 0) {
      FUN_004ccb30(0x2c,0x7f,iVar6,iVar1 + 0xd9,0x104,DAT_00f56f9c,0);
      if (DAT_00784c3c != '\0') {
        DAT_00e20778 = 0;
        FUN_004cce00(DAT_00784c24,0x40,&DAT_0057c048,iVar4 + 0x19a,iVar1 + 0xf5,uVar7,0);
        FUN_004ccbb0(0x2c,0x81,DAT_00e20778 + 0x5a + iVar2,iVar1 + 0xf5,uVar7,0);
      }
      if (DAT_00784c3d != '\0') {
        DAT_00e20778 = 0;
        FUN_004cce00(DAT_00784c28,0x40,&DAT_0057c048,iVar4 + 0x19a,iVar1 + 0x105,uVar7,0);
        FUN_004ccbb0(0x2c,0x82,DAT_00e20778 + 0x5a + iVar2,iVar1 + 0x105,uVar7,0);
      }
      if (DAT_00784c3f != '\0') {
        DAT_00e20778 = 0;
        FUN_004cce00(DAT_00784c30,0x40,&DAT_0057c048,iVar4 + 0x19a,iVar1 + 0x115,uVar7,0);
        FUN_004ccbb0(0x2c,0x84,DAT_00e20778 + 0x5a + iVar2,iVar1 + 0x115,uVar7,0);
      }
      if (DAT_00784c58 != 0) {
        DAT_00e20778 = 0;
        FUN_004cce00(DAT_00784c5c,0x40,&DAT_0057c048,iVar4 + 0x19a,iVar1 + 0x125,uVar7,0);
        FUN_004ccbb0(0x2c,0x85,DAT_00e20778 + 0x5a + iVar2,iVar1 + 0x125,uVar7,0);
      }
      if (DAT_00784c34 != 0) {
        DAT_00e20778 = 0;
        FUN_004cce00(DAT_00784c34,0x40,&DAT_0057c048,iVar4 + 0x19a,iVar1 + 0x135,uVar7,0);
        iVar6 = 0x14;
        if (DAT_00784c34 < 2) {
          iVar6 = 0;
        }
        FUN_004ccbb0(0x1d,DAT_00784c38 + iVar6,DAT_00e20778 + 0x5a + iVar2,iVar1 + 0x135,uVar7,0);
      }
      if (DAT_00784c50 != 0) {
        DAT_00e20778 = 0;
        FUN_004cce00(DAT_00784c54,0x40,&DAT_0057c048,iVar4 + 0x19a,iVar1 + 0x145,uVar7,0);
        FUN_004ccbb0(0x2c,0x87,DAT_00e20778 + 0x5a + iVar2,iVar1 + 0x145,uVar7,0);
      }
      if (DAT_00784c48 != 0) {
        DAT_00e20778 = 0;
        FUN_004cce00(DAT_00784c4c,0x40,&DAT_0057c048,iVar4 + 0x19a,iVar1 + 0x145,uVar7,0);
        FUN_004ccbb0(0x2c,0x86,DAT_00e20778 + 0x5a + iVar2,iVar1 + 0x145,uVar7,0);
      }
      local_d4 = &DAT_00784de0;
      local_d0 = iVar1 + 0x155;
      do {
        DAT_00e20778 = 0;
        if (*local_d4 != 0) {
          FUN_004ccb30(0xc6,(int)*local_d4,iVar4 + 0x15e,local_d0,0x104,uVar7,0);
        }
        local_d4 = local_d4 + 1;
        local_d0 = local_d0 + 0x10;
      } while ((int)local_d4 < 0x784de6);
    }
    else {
      FUN_004ccfd0(0x91,0,iVar4 + 0x159,iVar1 + 0xe1,0x104,uVar7,0);
    }
    if (DAT_00605a3c == 0) {
      DAT_00e20778 = 0;
      FUN_004ccbb0(0x2c,0xd8,iVar4 + 0x19a,iVar1 + 0x188,uVar7,0);
      FUN_004ccbb0(0x99,DAT_00e39080 + 1,DAT_00e20778 + 0x5a + iVar2,iVar1 + 0x188,uVar7,0);
    }
  }
  else if (*(int *)(uintptr_t)(0x007009dcu + (int32_t)DAT_00e38ea8 * 0x4c) == 0) {
    FUN_004ccfd0(0x131,0,iVar4 + 0x159,iVar1 + 0x7d,0x104,uVar7,0);
  }
  else {
    FUN_004ccfd0(0x129,(int)DAT_00e38ea8,iVar4 + 0x159,iVar1 + 0x7d,0x10e,uVar7,0);
    iVar6 = DAT_00e38ea8 * 0x4c;
    if (DAT_00784c3c != '\0') {
      DAT_00e20778 = 0;
      FUN_004cc110(0x12a,0);
      FUN_004ccf20(*(undefined4 *)(uintptr_t)(0x00700a04u + iVar6),DAT_00e20768,&DAT_007027d4,
                   DAT_00e20778 + 0x19 + iVar2,iVar1 + 0x11d,0x10e,uVar7,0);
    }
    if (DAT_00784c3d != '\0') {
      DAT_00e20778 = 0;
      FUN_004cc110(0x12a,1);
      FUN_004ccf20(*(undefined4 *)(uintptr_t)(0x00700a08u + iVar6),DAT_00e20768,&DAT_007027d4,
                   DAT_00e20778 + 0x19 + iVar2,iVar1 + 0x12d,0x10e,uVar7,0);
    }
    if (DAT_00784c3f != '\0') {
      DAT_00e20778 = 0;
      FUN_004cc110(0x12a,3);
      FUN_004ccf20(*(undefined4 *)(uintptr_t)(0x00700a0cu + iVar6),DAT_00e20768,&DAT_007027d4,
                   DAT_00e20778 + 0x19 + iVar2,iVar1 + 0x13d,0x10e,uVar7,0);
    }
    if (DAT_00784c58 != 0) {
      DAT_00e20778 = 0;
      FUN_004cc110(0x12a,4);
      FUN_004ccf20(*(undefined4 *)(uintptr_t)(0x00700a10u + iVar6),DAT_00e20768,&DAT_007027d4,
                   DAT_00e20778 + 0x19 + iVar2,iVar1 + 0x14d,0x10e,uVar7,0);
    }
    DAT_00e20778 = 0;
    FUN_004cc110(0x12a,5);
    FUN_004ccf20(*(undefined4 *)(uintptr_t)(0x00700a14u + iVar6),DAT_00e20768,&DAT_007027d4,
                 DAT_00e20778 + 0x19 + iVar2,iVar1 + 0x15d,0x10e,uVar7,0);
    DAT_00e20778 = 0;
    FUN_004cc110(0x12a,7);
    FUN_004cc160(0x99,*(int *)(uintptr_t)(0x00700a1cu + iVar6) + 1);
    FUN_004ccd60(DAT_00e20768,DAT_00e20764,DAT_00e20778 + 0xf + iVar2,iVar1 + 0x16d,0x10e,uVar7,0);
    DAT_00e20778 = 0;
    FUN_004cc110(0x12a,6);
    FUN_004cc160(0x12a,9);
    FUN_004ccf20(*(int *)(uintptr_t)(0x00700a18u + iVar6) / 0xc,DAT_00e20768,DAT_00e20764,
                 DAT_00e20778 + 0x19 + iVar2,iVar1 + 0x17d,0x10e,uVar7,0);
    DAT_00e20778 = 0;
    FUN_004cc110(0x12a,8);
    FUN_004ccf20(*(undefined4 *)(uintptr_t)(0x007009dcu + iVar6),DAT_00e20768,&DAT_007027d4,
                 DAT_00e20778 + 0x19 + iVar2,iVar1 + 0x18d,0x10e,DAT_00f56f9c,0);
  }
  iVar2 = FUN_0041dfe0(RESTORE_MISSION_FILES,DAT_00704dbc);
  if (iVar2 == 2) {
    iVar4 = iVar4 + 0x160;
    uVar5 = 0x15;
    uVar7 = DAT_00f56f94;
  }
  else {
    uVar7 = DAT_00f56f8c;
    if (DAT_00605a3c == 0) {
      if (DAT_00704db0 == 1) {
        FUN_004ccbb0(0x2c,0xd9,iVar4 + 0x17c,iVar1 + 0x1be,DAT_00f56f8c,0);
        FUN_004ccbb0(0x2c,0xd7,iVar4 + 0x1cc,iVar1 + 0x1be,DAT_00f56f8c,0);
        FUN_004ccb30(0x2c,(DAT_00704dac != 3) + 0xdc,DAT_00e38da8 + 0x162,DAT_00e38da4 + 0x19e,0xfa
                     ,DAT_00f56f94,0);
        return;
      }
      iVar4 = iVar4 + 0x1f9;
      uVar5 = 0x88;
    }
    else {
      iVar4 = iVar4 + 0x1ea;
      uVar5 = 0xd6;
    }
  }
  FUN_004ccbb0(0x2c,uVar5,iVar4,iVar1 + 0x1be,uVar7,0);
  return;
}

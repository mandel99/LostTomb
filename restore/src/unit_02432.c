#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_00784b60 (*(undefined *)0x00784b60u)
#define DAT_00784c24 (*(undefined *)0x00784c24u)
#define DAT_00784c28 (*(undefined *)0x00784c28u)
#define DAT_00784c30 (*(undefined *)0x00784c30u)
#define DAT_00784c34 (*(undefined *)0x00784c34u)
#define DAT_00784c38 (*(undefined *)0x00784c38u)
#define DAT_00784c48 (*(undefined *)0x00784c48u)
#define DAT_00784c4c (*(undefined *)0x00784c4cu)
#define DAT_00784c50 (*(undefined *)0x00784c50u)
#define DAT_00784c54 (*(undefined *)0x00784c54u)
#define DAT_00784c58 (*(undefined *)0x00784c58u)
#define DAT_00784c5c (*(undefined *)0x00784c5cu)
#define DAT_00784cbc (*(byte *)0x00784cbcu)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00784c3c (*(undefined4 *)0x00784c3cu)
extern void FUN_004cccb0();
extern void FUN_004cce90();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d6860();
extern int FUN_004f4ff0();
extern void FUN_00536820();

void FUN_00533470(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  int local_4;
  
  iVar5 = DAT_00e38da8;
  iVar4 = DAT_00e38da4;
  iVar1 = DAT_00e38da8 + 0x10;
  local_4 = 0;
  FUN_00536820(iVar1,DAT_00e38da4 + 0x20,0x26,0x1c);
  FUN_004d62f0(0x2c,0x30,iVar5 + 0x1a,iVar4 + 0x2a,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0xd,3,iVar1,iVar4 + 0x1c8,0x260,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  iVar2 = iVar5 + 0x42;
  FUN_004d62f0(0x2c,0x6b,iVar2,iVar4 + 0x65,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  iVar3 = iVar5 + 0x13c;
  FUN_004d63d0(0,iVar3,iVar4 + 0x5c,0x50,0x1e,DAT_007056c8 == 0xf,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x12,(int)DAT_00784b60,iVar3,iVar4 + 0x65,0x50,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  iVar7 = iVar5 + 0x1a0;
  FUN_004d63d0(0,iVar7,iVar4 + 0x5c,0xb4,0x1e,DAT_007056c8 == 0x10,DAT_00e38da8,DAT_00e38da4);
  FUN_004cce90(RESTORE_SLICE1(DAT_00784cbc,2),0x40,&DAT_0057c048,iVar7,iVar4 + 0x65,0xb4,DAT_00f56f8c,0);
  FUN_004d62f0(0x2c,0xd2,iVar2,iVar4 + 0x8d,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar3,iVar4 + 0x84,0x50,0x1e,DAT_007056c8 == 0x11,DAT_00e38da8,DAT_00e38da4);
  FUN_004cce90(DAT_00784c34,0x40,&DAT_0057c048,iVar3,iVar4 + 0x8d,0x50,DAT_00f56f8c,0);
  if (1 < DAT_00784c34) {
    local_4 = 0x14;
  }
  FUN_004d63d0(0,iVar7,iVar4 + 0x84,0xb4,0x1e,DAT_007056c8 == 0x12,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x1d,DAT_00784c38 + local_4,iVar7,iVar4 + 0x8d,0xb4,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x2c,0x32,iVar2,iVar4 + 0xb5,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar3,iVar4 + 0xac,0x50,0x1e,DAT_007056c8 == 1,DAT_00e38da8,DAT_00e38da4);
  if (DAT_00784b60 == 0) {
    uVar8 = _DAT_00784c3c & 0xff;
  }
  else {
    uVar8 = 0;
  }
  FUN_004d6250(0x12,uVar8,iVar3,iVar4 + 0xb5,0x50,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar7,iVar4 + 0xac,0xb4,0x1e,DAT_007056c8 == 2,DAT_00e38da8,DAT_00e38da4);
  FUN_004cce90(DAT_00784c24,0x40,&DAT_0057c048,iVar7,iVar4 + 0xb5,0xb4,DAT_00f56f8c,0);
  FUN_004d62f0(0x2c,0x33,iVar2,iVar4 + 0xdd,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar3,iVar4 + 0xd4,0x50,0x1e,DAT_007056c8 == 3,DAT_00e38da8,DAT_00e38da4);
  if (DAT_00784b60 == 0) {
    iVar9 = iVar4 + 0xdd;
    uVar8 = _DAT_00784c3c >> 8 & 0xff;
  }
  else {
    iVar9 = iVar4 + 0x71d;
    uVar8 = 0;
  }
  FUN_004d6250(0x12,uVar8,iVar3,iVar9,0x50,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar7,iVar4 + 0xd4,0xb4,0x1e,DAT_007056c8 == 4,DAT_00e38da8,DAT_00e38da4);
  FUN_004cce90(DAT_00784c28,0x40,&DAT_0057c048,iVar7,iVar4 + 0xdd,0xb4,DAT_00f56f8c,0);
  FUN_004d62f0(0x2c,0x34,iVar2,iVar4 + 0x105,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar3,iVar4 + 0xfc,0x50,0x1e,0,DAT_00e38da8,DAT_00e38da4);
  uVar6 = FUN_004f4ff0(0x40,&DAT_0057c048,iVar3,iVar4 + 0x105,0x50,DAT_00f56f8c,0);
  FUN_004cce90(uVar6);
  FUN_004d63d0(0,iVar7,iVar4 + 0xfc,0xb4,0x1e,DAT_007056c8 == 6,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2c,0xc9,iVar7,iVar4 + 0x105,0xb4,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2c,0x35,iVar2,iVar4 + 0x12d,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar3,iVar4 + 0x124,0x50,0x1e,DAT_007056c8 == 7,DAT_00e38da8,DAT_00e38da4);
  if (DAT_00784b60 == 0) {
    uVar8 = _DAT_00784c3c >> 0x18;
  }
  else {
    uVar8 = 0;
  }
  FUN_004d6250(0x12,uVar8,iVar3,iVar4 + 0x12d,0x50,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar7,iVar4 + 0x124,0xb4,0x1e,DAT_007056c8 == 8,DAT_00e38da8,DAT_00e38da4);
  FUN_004cce90(DAT_00784c30,0x40,&DAT_0057c048,iVar7,iVar4 + 0x12d,0xb4,DAT_00f56f8c,0);
  FUN_004d62f0(0x2c,0x36,iVar2,iVar4 + 0x155,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar3,iVar4 + 0x14c,0x50,0x1e,DAT_007056c8 == 9,DAT_00e38da8,DAT_00e38da4);
  uVar6 = DAT_00784c48;
  if (DAT_00784b60 != 0) {
    uVar6 = 0;
  }
  FUN_004d6250(0x12,uVar6,iVar3,iVar4 + 0x155,0x50,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar7,iVar4 + 0x14c,0xb4,0x1e,DAT_007056c8 == 10,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  FUN_004d6860(DAT_00784c4c,0x2b,&DAT_0057c048,iVar5 + 0x1b4,iVar4 + 0x155,DAT_00f56f8c,0,
               DAT_00e38da8,DAT_00e38da4);
  FUN_004cccb0(0,DAT_007848d8 + DAT_00784c4c,DAT_00e20778 + 0x1ae + iVar1,iVar4 + 0x155,DAT_00f56f8c
               ,0);
  FUN_004d62f0(0x2c,0x37,iVar2,iVar4 + 0x17d,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar3,iVar4 + 0x174,0x50,0x1e,DAT_007056c8 == 0xb,DAT_00e38da8,DAT_00e38da4);
  uVar6 = DAT_00784c50;
  if (DAT_00784b60 != 0) {
    uVar6 = 0;
  }
  FUN_004d6250(0x12,uVar6,iVar3,iVar4 + 0x17d,0x50,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar7,iVar4 + 0x174,0xb4,0x1e,DAT_007056c8 == 0xc,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  FUN_004d6860(DAT_00784c54,0x2b,&DAT_0057c048,iVar5 + 0x1b4,iVar4 + 0x17d,DAT_00f56f8c,0,
               DAT_00e38da8,DAT_00e38da4);
  FUN_004cccb0(0,DAT_007848d8 + DAT_00784c54,DAT_00e20778 + 0x1ae + iVar1,iVar4 + 0x17d,DAT_00f56f8c
               ,0);
  iVar1 = iVar4 + 0x1a5;
  FUN_004d62f0(0x2c,0x38,iVar2,iVar1,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar3,iVar4 + 0x19c,0x50,0x1e,DAT_007056c8 == 0xd,DAT_00e38da8,DAT_00e38da4);
  uVar6 = DAT_00784c58;
  if (DAT_00784b60 != 0) {
    uVar6 = 0;
  }
  FUN_004d6250(0x12,uVar6,iVar3,iVar1,0x50,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,iVar7,iVar4 + 0x19c,0xb4,0x1e,DAT_007056c8 == 0xe,DAT_00e38da8,DAT_00e38da4);
  FUN_004cce90(DAT_00784c5c,0x40,&DAT_0057c048,iVar7,iVar1,0xb4,DAT_00f56f8c,0);
  return;
}

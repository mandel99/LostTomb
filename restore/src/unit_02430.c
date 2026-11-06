#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_007056c8 (*(int32_t *)0x007056c8u)
#define DAT_007848d8 (*(int16_t *)0x007848d8u)
#define DAT_007848f8 (*(int32_t *)0x007848f8u)
#define DAT_00784b62 (*(int16_t *)0x00784b62u)
#define DAT_00784c94 (*(int32_t *)0x00784c94u)
#define DAT_00784c98 (*(int32_t *)0x00784c98u)
#define DAT_00784c9c (*(int32_t *)0x00784c9cu)
#define DAT_00784ca0 (*(int32_t *)0x00784ca0u)
#define DAT_00784ddc (*(int32_t *)0x00784ddcu)
#define DAT_00784f08 (*(int32_t *)0x00784f08u)
#define DAT_00784f0c (*(int32_t *)0x00784f0cu)
#define DAT_00e20778 (*(int32_t *)0x00e20778u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f56f8c (*(uint32_t *)0x00f56f8cu)
#define DAT_00f56fb4 (*(uint32_t *)0x00f56fb4u)
extern void FUN_004cccb0();
extern void FUN_004cce90();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d6860();
extern void FUN_00536820();

void FUN_00532920(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = DAT_00e38da8;
  iVar4 = DAT_00e38da4;
  iVar1 = DAT_00e38da8 + 0xc;
  iVar2 = DAT_00e38da4 + 0xc;
  FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x1e,0x22);
  iVar3 = iVar5 + 0x20;
  FUN_004d62f0(0x2c,0x58,iVar3,iVar2,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0xd,3,iVar1,iVar4 + 0x1fb,0x1e0,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2c,0x6c,iVar3,iVar4 + 0x34,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  iVar2 = iVar5 + 0x106;
  FUN_004d63d0(1,iVar2,iVar4 + 0x34,200,0x1e,DAT_007056c8 == 7,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x20,(int)DAT_00784b62,iVar2,iVar4 + 0x3c,200,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x2c,0x59,iVar3,iVar4 + 0x5c,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(1,iVar2,iVar4 + 0x5c,200,0x1e,DAT_007056c8 == 1,DAT_00e38da8,DAT_00e38da4);
  FUN_004cccb0(0,(int)DAT_007848d8,iVar5 + 0x13a,iVar4 + 0x60,DAT_00f56fb4,0);
  FUN_004d62f0(0x2c,0x27,iVar3,iVar4 + 0x84,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(1,iVar2,iVar4 + 0x84,200,0x1e,DAT_007056c8 == 2,DAT_00e38da8,DAT_00e38da4);
  FUN_004cce90(DAT_007848f8,0x40,&DAT_0057c048,iVar2,iVar4 + 0x8c,200,DAT_00f56f8c,0);
  FUN_004d62f0(0x2c,0x44,iVar3,iVar4 + 0xac,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(1,iVar2,iVar4 + 0xac,200,0x1e,DAT_007056c8 == 3,DAT_00e38da8,DAT_00e38da4);
  FUN_004cce90(DAT_00784c94,0x40,&DAT_0057c048,iVar2,iVar4 + 0xb4,200,DAT_00f56f8c,0);
  FUN_004d62f0(0x2c,0x5b,iVar3,iVar4 + 0xd4,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(1,iVar2,iVar4 + 0xd4,200,0x1e,DAT_007056c8 == 4,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  FUN_004d6860(DAT_00784c98,0x2b,&DAT_0057c048,iVar5 + 0x115,iVar4 + 0xdc,DAT_00f56f8c,0,
               DAT_00e38da8,DAT_00e38da4);
  FUN_004cccb0(0,DAT_007848d8 + DAT_00784c98,DAT_00e20778 + 0x113 + iVar1,iVar4 + 0xdc,DAT_00f56f8c,
               0);
  FUN_004d62f0(0x2c,0x5c,iVar3,iVar4 + 0xfc,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(1,iVar2,iVar4 + 0xfc,200,0x1e,DAT_007056c8 == 5,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  FUN_004d6860(DAT_00784c9c,0x2b,&DAT_0057c048,iVar5 + 0x115,iVar4 + 0x104,DAT_00f56f8c,0,
               DAT_00e38da8,DAT_00e38da4);
  FUN_004cccb0(0,DAT_007848d8 + DAT_00784c9c,DAT_00e20778 + 0x113 + iVar1,iVar4 + 0x104,DAT_00f56f8c
               ,0);
  FUN_004d62f0(0x2c,0x5d,iVar3,iVar4 + 0x124,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(1,iVar2,iVar4 + 0x124,200,0x1e,DAT_007056c8 == 6,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  FUN_004d6860(DAT_00784ca0,0x2b,&DAT_0057c048,iVar5 + 0x115,iVar4 + 300,DAT_00f56f8c,0,DAT_00e38da8
               ,DAT_00e38da4);
  FUN_004cccb0(0,DAT_007848d8 + DAT_00784ca0,DAT_00e20778 + 0x113 + iVar1,iVar4 + 300,DAT_00f56f8c,0
              );
  FUN_004d62f0(0x2c,0xb1,iVar3,iVar4 + 0x14c,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(1,iVar2,iVar4 + 0x14c,200,0x1e,DAT_007056c8 == 8,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  FUN_004cce90(DAT_00784ddc,0x40,&DAT_0057c048,iVar2,iVar4 + 0x154,200,DAT_00f56f8c,0);
  FUN_004d62f0(0x2c,0xca,iVar3,iVar4 + 0x174,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(1,iVar2,iVar4 + 0x174,200,0x1e,DAT_007056c8 == 9,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x97,DAT_00784f08,iVar2,iVar4 + 0x17c,200,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2c,0xcb,iVar3,iVar4 + 0x19c,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(1,iVar2,iVar4 + 0x19c,200,0x1e,DAT_007056c8 == 10,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x98,DAT_00784f0c,iVar2,iVar4 + 0x1a4,200,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  return;
}

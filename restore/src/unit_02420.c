#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005f89dc (*(undefined4 *)0x005f89dcu)
#define DAT_005f89e0 (*(undefined4 *)0x005f89e0u)
#define DAT_005f89e4 (*(undefined4 *)0x005f89e4u)
#define DAT_005f89e8 (*(undefined4 *)0x005f89e8u)
#define DAT_005f89ec (*(undefined4 *)0x005f89ecu)
#define DAT_005f89f0 (*(undefined4 *)0x005f89f0u)
#define DAT_005f89f4 (*(undefined4 *)0x005f89f4u)
#define DAT_005f89f8 (*(undefined4 *)0x005f89f8u)
#define DAT_005f89fc (*(undefined4 *)0x005f89fcu)
#define DAT_005f8a00 (*(undefined4 *)0x005f8a00u)
#define DAT_00605978 (*(undefined *)0x00605978u)
#define DAT_0060597c (*(undefined *)0x0060597cu)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004cc720();
extern void FUN_004cce90();
extern void FUN_004cd680();
extern void FUN_004cd9d0();
extern void FUN_004d6250();
extern void FUN_00536820();

void FUN_005309b0(int param_1,int param_2)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  
  FUN_00536820(param_1 + -0x10,param_2 + -0x10,8,0xe);
  FUN_004cd9d0(param_1,param_2,0x60,0x1e,0);
  if (0 < DAT_00605978) {
    FUN_004cce90(DAT_0060597c,0x40,&DAT_0057c048,param_1 + -8,param_2 + 3,0x60,DAT_00f56fb4,0xf800);
  }
  iVar1 = param_2 + 0x23;
  iVar3 = param_1 + 5;
  iVar4 = (-(uint)(DAT_007056c8 != 1) & 0xfffffd41) + 0x2bf;
  FUN_004cd680(iVar3,iVar1,0x19,0x19,iVar4);
  FUN_004cc720(&DAT_005f8a00,iVar3,iVar1,0x19,DAT_00f56fb4,iVar4);
  iVar4 = param_1 + 0x23;
  uVar2 = (DAT_007056c8 != 2) - 1 & 0x2bf;
  FUN_004cd680(iVar4,iVar1,0x19,0x19,uVar2);
  FUN_004cc720(&DAT_005f89fc,iVar4,iVar1,0x19,DAT_00f56fb4,uVar2);
  param_1 = param_1 + 0x41;
  uVar2 = (DAT_007056c8 != 3) - 1 & 0x2bf;
  FUN_004cd680(param_1,iVar1,0x19,0x19,uVar2);
  FUN_004cc720(&DAT_005f89f8,param_1,iVar1,0x19,DAT_00f56fb4,uVar2);
  uVar2 = (DAT_007056c8 != 4) - 1 & 0x2bf;
  iVar1 = param_2 + 0x41;
  FUN_004cd680(iVar3,iVar1,0x19,0x19,uVar2);
  FUN_004cc720(&DAT_005f89f4,iVar3,iVar1,0x19,DAT_00f56fb4,uVar2);
  uVar2 = (DAT_007056c8 != 5) - 1 & 0x2bf;
  FUN_004cd680(iVar4,iVar1,0x19,0x19,uVar2);
  FUN_004cc720(&DAT_005f89f0,iVar4,iVar1,0x19,DAT_00f56fb4,uVar2);
  uVar2 = (DAT_007056c8 != 6) - 1 & 0x2bf;
  FUN_004cd680(param_1,iVar1,0x19,0x19,uVar2);
  FUN_004cc720(&DAT_005f89ec,param_1,iVar1,0x19,DAT_00f56fb4,uVar2);
  iVar1 = param_2 + 0x5f;
  uVar2 = (DAT_007056c8 != 7) - 1 & 0x2bf;
  FUN_004cd680(iVar3,iVar1,0x19,0x19,uVar2);
  FUN_004cc720(&DAT_005f89e8,iVar3,iVar1,0x19,DAT_00f56fb4,uVar2);
  uVar2 = (DAT_007056c8 != 8) - 1 & 0x2bf;
  FUN_004cd680(iVar4,iVar1,0x19,0x19,uVar2);
  FUN_004cc720(&DAT_005f89e4,iVar4,iVar1,0x19,DAT_00f56fb4,uVar2);
  uVar2 = (DAT_007056c8 != 9) - 1 & 0x2bf;
  FUN_004cd680(param_1,iVar1,0x19,0x19,uVar2);
  FUN_004cc720(&DAT_005f89e0,param_1,iVar1,0x19,DAT_00f56fb4,uVar2);
  iVar1 = param_2 + 0x7d;
  uVar2 = (DAT_007056c8 != 10) - 1 & 0x2bf;
  FUN_004cd680(iVar3,iVar1,0x19,0x19,uVar2);
  FUN_004cc720(&DAT_005f89dc,iVar3,iVar1,0x19,DAT_00f56fb4,uVar2);
  uVar2 = (DAT_007056c8 != 0xb) - 1 & 0x2bf;
  FUN_004cd680(iVar4,iVar1,0x37,0x19,uVar2);
  FUN_004d6250(0x2c,0x10,iVar4,param_2 + 0x83,0x37,DAT_00f56f8c,uVar2,DAT_00e38da8,DAT_00e38da4);
  uVar2 = (DAT_007056c8 != 0xc) - 1 & 0x2bf;
  FUN_004cd680(iVar3,param_2 + 0x9b,0x55,0x19,uVar2);
  FUN_004d6250(0x2c,0x11,iVar3,param_2 + 0xa1,0x55,DAT_00f56f8c,uVar2,DAT_00e38da8,DAT_00e38da4);
  return;
}

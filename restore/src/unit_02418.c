#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004cce90();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_00536820();

void FUN_005305e0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = DAT_00e38da8;
  iVar1 = DAT_00e38da4;
  iVar3 = DAT_00e38da8 + 0x80;
  FUN_00536820(iVar3,DAT_00e38da4 + 0x2c,0x14,10);
  DAT_00e20778 = 0;
  FUN_004d6250(0x2c,0xd,iVar2 + 0x8a,iVar1 + 0x38,0x140,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0xd,3,iVar3,iVar1 + 0xb2,0x140,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(1,iVar2 + 0x9e,iVar1 + 100,100,0x1e,DAT_007056c8 == 1,DAT_00e38da8,DAT_00e38da4);
  iVar3 = iVar1 + 0x68;
  FUN_004d62f0(0x14,-1 < DAT_007848d8,iVar2 + 0xa8,iVar3,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
  iVar2 = iVar2 + 0x116;
  FUN_004d63d0(1,iVar2,iVar1 + 100,0x78,0x1e,DAT_007056c8 == 2,DAT_00e38da8,DAT_00e38da4);
  if (DAT_007848d8 < 0) {
    FUN_004cce90(-(int)DAT_007848d8,0x40,&DAT_0057c048,iVar2,iVar3,0x78,DAT_00f56fb4,0);
    return;
  }
  FUN_004cce90((int)DAT_007848d8,0x40,&DAT_0057c048,iVar2,iVar3,0x78,DAT_00f56fb4,0);
  return;
}

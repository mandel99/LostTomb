#include "restore_types.h"
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
extern void FUN_004d6250();
extern void FUN_00536820();

void FUN_005344c0(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = DAT_00e38da8;
  iVar2 = DAT_00e38da4;
  iVar1 = DAT_00e38da8 + 0x10;
  FUN_00536820(iVar1,DAT_00e38da4 + 0x20,0x23,4);
  FUN_004d6250(0x13,param_1 + -2,iVar3 + 0x13,iVar2 + 0x30,0x230,DAT_00f56f84,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0xd,1,iVar1,iVar2 + 0x44,0x230,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  return;
}

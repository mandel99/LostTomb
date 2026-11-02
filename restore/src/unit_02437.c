#include "restore_types.h"
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
extern void FUN_004d6250();
extern void FUN_004d63d0();

void FUN_005345a0(void)

{
  int iVar1;
  int iVar2;
  
  iVar2 = DAT_00e38da4;
  iVar1 = DAT_00e38da8 + 0xf0;
  FUN_004d63d0(0,iVar1,DAT_00e38da4 + 0xb0,0xa0,0x20,DAT_007056c8 == 1,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x12,3,iVar1,iVar2 + 0xbb,0xa0,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  return;
}

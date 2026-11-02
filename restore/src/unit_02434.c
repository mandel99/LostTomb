#include "restore_types.h"
#define DAT_00705650 (*(undefined4 *)0x00705650u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_0077591c (*(undefined *)0x0077591cu)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
extern byte FUN_00444a00();
extern void FUN_004d6250();
extern void FUN_004d63d0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00534320(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  bool bVar8;
  int local_18;
  int *local_14;
  int local_10;
  
  iVar4 = DAT_00e38da4;
  iVar1 = DAT_00e38da4 + 0x20;
  iVar2 = DAT_00e38da8 + 0x10;
  FUN_00536820(iVar2,iVar1,0x1e,0xe);
  FUN_004d6250(0x2c,0x53,iVar2,iVar4 + 0x30,0x1e0,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
  local_18 = 0;
  local_10 = 0;
  local_14 = &DAT_00705650;
  do {
    iVar3 = *local_14;
    if (-1 < iVar3) {
      iVar5 = FUN_00444a00(DAT_0077591c,iVar3);
      bVar8 = local_18 + 1 == DAT_007056c8;
      if ((int)local_14 < 0x705670) {
        iVar7 = 0x3c;
        iVar6 = local_10;
      }
      else {
        iVar6 = local_10 + -400;
        iVar7 = 0x78;
      }
      FUN_0053e590(DAT_00f571fc + iVar3,iVar6 + 0x28 + iVar2,iVar7 + iVar1);
      iVar3 = iVar6 + 0x19 + iVar2;
      FUN_004d63d0(0,iVar3,iVar7 + 0x1e + iVar1,0x32,0x16,bVar8,DAT_00e38da8,DAT_00e38da4);
      if (iVar5 != 0) {
        FUN_004d6250(0x2c,iVar5 + 0x54,iVar3,iVar7 + 0x24 + iVar1,0x32,DAT_00f56f8c,0,DAT_00e38da8,
                     DAT_00e38da4);
      }
    }
    local_14 = local_14 + 1;
    local_18 = local_18 + 1;
    local_10 = local_10 + 0x32;
  } while ((int)local_14 < 0x705690);
  FUN_004d6250(0xd,1,iVar2,iVar4 + 0xe4,0x1e0,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  return;
}

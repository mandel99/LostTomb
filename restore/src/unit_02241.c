#include "restore_types.h"
#define DAT_007056c4 (*(undefined *)0x007056c4u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
#define DAT_00f53e70 (*(undefined4 *)0x00f53e70u)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
extern undefined4 FUN_0047d350();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern undefined4 FUN_005013a0();
extern int FUN_0053e590();

void FUN_005011d0(void)

{
  int iVar1;
  byte bVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  int local_10;
  int *local_c;
  int local_8;
  
  local_8 = 0;
  bVar2 = *(byte *)((int)&DAT_00799a98 + DAT_00eadd6c * 0x108 + 1);
  if (DAT_00f53e6c < 1) {
    DAT_00e38dca = 1;
    return;
  }
  local_10 = 1;
  local_c = &DAT_00f53e70;
  iVar7 = 0x4e;
  do {
    iVar6 = *local_c;
    if (iVar6 != 0) {
      iVar4 = FUN_005013a0(iVar6);
      iVar1 = DAT_00f571fc;
      if (iVar4 != 0) {
        iVar4 = DAT_00eadd90 + 0x20;
        iVar5 = FUN_0047d350(iVar6,3);
        FUN_0053e590(iVar5 + iVar1 + iVar6,iVar4,iVar7);
        iVar4 = DAT_00f571fc;
        iVar1 = DAT_00eadd90 + 0x198;
        iVar5 = FUN_0047d350(iVar6,3);
        FUN_0053e590(iVar5 + iVar4 + iVar6,iVar1,iVar7);
        FUN_004d62f0(0x17,iVar6,DAT_00eadd90 + 0x48,iVar7 + 4,DAT_00f56f9c,0,DAT_00eadd90,
                     DAT_00eadd8c);
        if (DAT_007056c4 == local_10) {
          FUN_004d63d0(0,DAT_00eadd90 + 0xb4,iVar7,0xd2,0x14,1,DAT_00eadd90,DAT_00eadd8c);
        }
        cVar3 = *(char *)((int)&DAT_007900c2 + (uint)bVar2 * 0xc4 + iVar6);
        if (cVar3 == '\0') {
          iVar6 = DAT_00eadd90 + 0x104;
          uVar8 = 8;
          uVar9 = DAT_00f56f9c;
LAB_0050134d: ;
          FUN_004d62f0(0x61,uVar8,iVar6,iVar7 + 5,uVar9,0,DAT_00eadd90,DAT_00eadd8c);
        }
        else if (cVar3 == '\x01') {
          iVar6 = DAT_00eadd90 + 0xf0;
          uVar8 = 9;
          uVar9 = DAT_00f56fa4;
          goto LAB_0050134d;
        }
        local_10 = local_10 + 1;
        iVar7 = iVar7 + 0x14;
      }
    }
    local_8 = local_8 + 1;
    local_c = local_c + 1;
    if (DAT_00f53e6c <= local_8) {
      DAT_00e38dca = 1;
      return;
    }
  } while( true );
}

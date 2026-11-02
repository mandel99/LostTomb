#include "restore_types.h"
#define DAT_005c5b48 (*(undefined4 *)0x005c5b48u)
#define DAT_005c5b60 (*(undefined4 *)0x005c5b60u)
#define DAT_005c81f8 (*(undefined4 *)0x005c81f8u)
#define DAT_007056c4 (*(undefined *)0x007056c4u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007900ea (*(undefined4 *)0x007900eau)
#define DAT_00790134 (*(undefined4 *)0x00790134u)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00799ab8 (*(undefined4 *)0x00799ab8u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
extern void FUN_004256a0();
extern void FUN_00425730();
extern int FUN_00425990();
extern void FUN_00425bc0();
extern int FUN_00425c40();
extern void FUN_00425e80();
extern int FUN_004cde80();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();
extern undefined4 FUN_00502780();
extern void FUN_00502840();
extern int FUN_0053e590();

void FUN_00502990(void)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  char cVar6;
  uint uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  int local_14;
  int *local_10;
  int local_c;
  undefined *local_8;
  
  uVar7 = (uint)*(byte *)((int)&DAT_00799a98 + DAT_00eadd6c * 0x108 + 1);
  iVar2 = FUN_00502780();
  iVar3 = DAT_00f53e6c;
  if (iVar2 < DAT_00f53e6c) {
    iVar3 = FUN_00502780();
  }
  iVar3 = FUN_004cde80(iVar3 * 4,3);
  iVar3 = iVar3 + 7;
  iVar2 = iVar3 * 0x10;
  FUN_004d63d0(0,DAT_00eadd90 + 0x50,iVar2,(DAT_00eadd88 + -10) * 0x10,0x14,DAT_007056c8 == 1,
               DAT_00eadd90,DAT_00eadd8c);
  FUN_004d6250(99,7,DAT_00eadd90 + 0x50,iVar2 + 4,(DAT_00eadd88 + -10) * 0x10,DAT_00f56f8c,0,
               DAT_00eadd90,DAT_00eadd8c);
  local_14 = (&DAT_00799ab8)[DAT_00eadd6c * 0x42];
  if (local_14 == 0) {
    local_14 = 1;
  }
  if (DAT_00f53e6c <= local_14) {
    iVar4 = FUN_00502780();
    if (iVar4 + 1 < DAT_00f53e6c) {
      iVar4 = FUN_00502780();
      local_14 = (DAT_00f53e6c - iVar4) + -1;
    }
    else {
      local_14 = 1;
    }
  }
  iVar4 = FUN_00502780();
  if (iVar4 < DAT_00f53e6c) {
    iVar4 = iVar3 * 0x10 + -0x4e;
    FUN_00425e80(DAT_00eadd90 + 0x19e,iVar4,&DAT_005c5b48,1,10);
    FUN_00425c40(DAT_00eadd90 + 0x19e,iVar4,&DAT_005c5b48,1);
    FUN_004256a0(DAT_00eadd90 + 0x19e,iVar2 + -0x48,&DAT_005c5b48,1);
    FUN_00425e80(DAT_00eadd90 + 0x186,0x52,&DAT_005c5b60,1,10);
    FUN_00425c40(DAT_00eadd90 + 0x186,0x52,&DAT_005c5b60,1);
    FUN_004256a0(DAT_00eadd90 + 0x186,0x52,&DAT_005c5b60,1);
    if (iVar3 == 0x1b) {
      uVar9 = 0x10;
    }
    else {
      uVar9 = 0x18;
    }
    FUN_004d65b0(DAT_00eadd90 + 0x19b,0x6a,2,uVar9,DAT_00eadd90,DAT_00eadd8c);
  }
  local_c = 0;
  iVar3 = FUN_00502780();
  if (0 < iVar3) {
    local_8 = &DAT_005c81f8;
    iVar3 = 0x53;
    local_10 = &DAT_00f53e6c + local_14;
    do {
      iVar2 = *local_10;
      if (iVar2 == 0) goto LAB_00502f22;
      FUN_0053e590(DAT_00f571fc + iVar2,DAT_00eadd90 + 0x19,iVar3 + -5);
      FUN_004d62f0(0x17,iVar2,DAT_00eadd90 + 0x3c,iVar3 + -1,DAT_00f56f9c,0,DAT_00eadd90,
                   DAT_00eadd8c);
      if (DAT_007056c4 == local_c + 1) {
        FUN_004d63d0(0,DAT_00eadd90 + 0xb4,iVar3 + -5,0xe6,0x14,1,DAT_00eadd90,DAT_00eadd8c);
      }
      uVar9 = DAT_00f56fa4;
      switch(*(undefined1 *)((int)&DAT_007900c2 + uVar7 * 0xc4 + iVar2)) {
      case 0: ;
        DAT_00e20778 = 0;
        iVar4 = iVar2 + uVar7 * 0x62;
        if (*(short *)((int)&DAT_007900ea + iVar4 * 2 + 2) < 0x961) {
          uVar9 = 0x20;
        }
        else {
          uVar9 = 0x12;
        }
        FUN_004d62f0(99,uVar9,DAT_00eadd90 + 0xf0,iVar3,DAT_00f56f9c,0,DAT_00eadd90,DAT_00eadd8c);
        sVar1 = *(short *)((int)&DAT_007900ea + iVar4 * 2 + 2);
        if (sVar1 < 0x321) {
          cVar6 = '\x19';
        }
        else if (sVar1 < 0x641) {
          cVar6 = '\x1a';
        }
        else {
          cVar6 = (0x960 < sVar1) + '\x1b';
        }
        FUN_004d62f0(99,cVar6,DAT_00e20778 + 0xf0 + DAT_00eadd90,iVar3,DAT_00f56f9c,0,DAT_00eadd90,
                     DAT_00eadd8c);
        if (*(short *)((int)&DAT_007900ea + iVar4 * 2 + 2) < 0x961) {
          iVar4 = DAT_00e20778 + 0xf0 + DAT_00eadd90;
          uVar8 = 0x1d;
          uVar9 = DAT_00f56f9c;
          goto LAB_00502ea0;
        }
        break;
      case 1: ;
        iVar4 = DAT_00eadd90 + 0xf0;
        uVar8 = 8;
        goto LAB_00502ea0;
      case 2: ;
        DAT_00e20778 = 0;
        FUN_004d62f0(99,0x13,DAT_00eadd90 + 0xf0,iVar3,DAT_00f56f9c,0,DAT_00eadd90,DAT_00eadd8c);
        iVar4 = iVar2 + uVar7 * 0x62;
        sVar1 = *(short *)(&DAT_00790134 + iVar4 * 2);
        if (sVar1 < 0x321) {
          iVar5 = 0x19;
        }
        else if (sVar1 < 0x641) {
          iVar5 = 0x1a;
        }
        else {
          iVar5 = ((0x960 < sVar1) - 1 & 0xfffffffc) + 0x1f;
        }
        FUN_004d62f0(99,iVar5,DAT_00e20778 + 0xf0 + DAT_00eadd90,iVar3,DAT_00f56f9c,0,DAT_00eadd90,
                     DAT_00eadd8c);
        if (*(short *)(&DAT_00790134 + iVar4 * 2) < 0x961) {
          iVar4 = DAT_00e20778 + 0xf0 + DAT_00eadd90;
          uVar8 = 0x1d;
          uVar9 = DAT_00f56f9c;
          goto LAB_00502ea0;
        }
        break;
      case 3: ;
        iVar4 = DAT_00eadd90 + 0xf0;
        uVar8 = 0x15;
LAB_00502ea0: ;
        FUN_004d62f0(99,uVar8,iVar4,iVar3,uVar9,0,DAT_00eadd90,DAT_00eadd8c);
      }
      cVar6 = *(char *)((int)&DAT_007900c2 + uVar7 * 0xc4 + iVar2);
      if ((cVar6 == '\x02') || (cVar6 == '\0')) {
        iVar2 = iVar3 + -3;
        FUN_00425bc0(DAT_00eadd90 + 200,iVar2,local_8,2,10);
        FUN_00425990(DAT_00eadd90 + 200,iVar2,local_8,2);
        FUN_00425730(DAT_00eadd90 + 200,iVar2,local_8,2);
      }
      local_14 = local_14 + 1;
      local_10 = local_10 + 1;
      if (DAT_00f53e6c < local_14) break;
LAB_00502f22: ;
      local_c = local_c + 1;
      local_8 = local_8 + 0x48;
      iVar3 = iVar3 + 0x14;
      iVar2 = FUN_00502780();
    } while (local_c < iVar2);
  }
  DAT_00e38dca = 1;
  FUN_00502840();
  return;
}

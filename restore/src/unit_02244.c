#include "restore_types.h"
#define DAT_005c81f8 (*(undefined4 *)0x005c81f8u)
#define DAT_007056c4 (*(undefined *)0x007056c4u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007900ea (*(undefined4 *)0x007900eau)
#define DAT_00790134 (*(undefined4 *)0x00790134u)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f53dd8 (*(undefined *)0x00f53dd8u)
#define DAT_00f53ddc (*(undefined4 *)0x00f53ddcu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
extern void FUN_00425730();
extern int FUN_00425990();
extern void FUN_00425bc0();
extern int FUN_004cde80();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern int FUN_0053e590();

void FUN_00501b60(void)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  char cVar6;
  uint uVar7;
  int iVar8;
  bool bVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  int *local_14;
  undefined *local_10;
  
  uVar7 = (uint)*(byte *)((int)&DAT_00799a98 + DAT_00eadd6c * 0x108 + 1);
  bVar9 = DAT_007056c8 == 1;
  iVar3 = FUN_004cde80(DAT_00f53dd8 * 4,3);
  iVar3 = (iVar3 + 7) * 0x10;
  FUN_004d63d0(0,DAT_00eadd90 + 0x50,iVar3 + -8,(DAT_00eadd88 + -10) * 0x10,0x14,bVar9,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d6250(99,7,DAT_00eadd90 + 0x50,iVar3 + -4,(DAT_00eadd88 + -10) * 0x10,DAT_00f56f8c,0,
               DAT_00eadd90,DAT_00eadd8c);
  iVar3 = 0;
  if (DAT_00f53dd8 < 1) {
    DAT_00e38dca = 1;
    return;
  }
  local_10 = &DAT_005c81f8;
  local_14 = &DAT_00f53ddc;
  iVar8 = 0x53;
  do {
    iVar2 = *local_14;
    FUN_0053e590(DAT_00f571fc + iVar2,DAT_00eadd90 + 0x20,iVar8 + -5);
    FUN_004d62f0(0x17,iVar2,DAT_00eadd90 + 0x48,iVar8 + -1,DAT_00f56f9c,0,DAT_00eadd90,DAT_00eadd8c)
    ;
    iVar3 = iVar3 + 1;
    if (DAT_007056c4 == iVar3) {
      FUN_004d63d0(0,DAT_00eadd90 + 0xb4,iVar8 + -5,0xfa,0x14,1,DAT_00eadd90,DAT_00eadd8c);
    }
    uVar10 = DAT_00f56fa4;
    switch(*(undefined1 *)((int)&DAT_007900c2 + iVar2 + uVar7 * 0xc4)) {
    case 0: ;
      DAT_00e20778 = 0;
      iVar5 = iVar2 + uVar7 * 0x62;
      if (*(short *)((int)&DAT_007900ea + iVar5 * 2 + 2) < 0x961) {
        uVar10 = 0x20;
      }
      else {
        uVar10 = 0x12;
      }
      FUN_004d62f0(99,uVar10,DAT_00eadd90 + 0xfa,iVar8,DAT_00f56f9c,0,DAT_00eadd90,DAT_00eadd8c);
      sVar1 = *(short *)((int)&DAT_007900ea + iVar5 * 2 + 2);
      if (sVar1 < 0x321) {
        cVar6 = '\x19';
      }
      else if (sVar1 < 0x641) {
        cVar6 = '\x1a';
      }
      else {
        cVar6 = (0x960 < sVar1) + '\x1b';
      }
      FUN_004d62f0(99,cVar6,DAT_00e20778 + 0xfa + DAT_00eadd90,iVar8,DAT_00f56f9c,0,DAT_00eadd90,
                   DAT_00eadd8c);
      if (*(short *)((int)&DAT_007900ea + iVar5 * 2 + 2) < 0x961) {
        iVar5 = DAT_00e20778 + 0xfa + DAT_00eadd90;
        uVar11 = 0x1e;
        uVar10 = DAT_00f56f9c;
        goto LAB_00501f07;
      }
      break;
    case 1: ;
      iVar5 = DAT_00eadd90 + 0xfa;
      uVar11 = 8;
      goto LAB_00501f07;
    case 2: ;
      DAT_00e20778 = 0;
      FUN_004d62f0(99,0x13,DAT_00eadd90 + 0xfa,iVar8,DAT_00f56f9c,0,DAT_00eadd90,DAT_00eadd8c);
      iVar5 = iVar2 + uVar7 * 0x62;
      sVar1 = *(short *)(&DAT_00790134 + iVar5 * 2);
      if (sVar1 < 0x321) {
        iVar4 = 0x19;
      }
      else if (sVar1 < 0x641) {
        iVar4 = 0x1a;
      }
      else {
        iVar4 = ((0x960 < sVar1) - 1 & 0xfffffffc) + 0x1f;
      }
      FUN_004d62f0(99,iVar4,DAT_00e20778 + 0xfa + DAT_00eadd90,iVar8,DAT_00f56f9c,0,DAT_00eadd90,
                   DAT_00eadd8c);
      if (*(short *)(&DAT_00790134 + iVar5 * 2) < 0x961) {
        iVar5 = DAT_00e20778 + 0xfa + DAT_00eadd90;
        uVar11 = 0x1e;
        uVar10 = DAT_00f56f9c;
        goto LAB_00501f07;
      }
      break;
    case 3: ;
      iVar5 = DAT_00eadd90 + 0xfa;
      uVar11 = 0x14;
LAB_00501f07: ;
      FUN_004d62f0(99,uVar11,iVar5,iVar8,uVar10,0,DAT_00eadd90,DAT_00eadd8c);
    }
    cVar6 = *(char *)((int)&DAT_007900c2 + iVar2 + uVar7 * 0xc4);
    if ((cVar6 == '\x02') || (cVar6 == '\0')) {
      iVar2 = iVar8 + -3;
      FUN_00425bc0(DAT_00eadd90 + 200,iVar2,local_10,2,10);
      FUN_00425990(DAT_00eadd90 + 200,iVar2,local_10,2);
      FUN_00425730(DAT_00eadd90 + 200,iVar2,local_10,2);
    }
    local_14 = local_14 + 1;
    local_10 = local_10 + 0x48;
    iVar8 = iVar8 + 0x14;
    if (DAT_00f53dd8 <= iVar3) {
      DAT_00e38dca = 1;
      return;
    }
  } while( true );
}

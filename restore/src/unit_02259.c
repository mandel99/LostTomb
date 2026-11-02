#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005f78dc (*(undefined4 *)0x005f78dcu)
#define DAT_00bcb9ca (*(undefined4 *)0x00bcb9cau)
#define DAT_00bcb9cc (*(undefined4 *)0x00bcb9ccu)
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
#define DAT_00bcb9f4 (*(undefined4 *)0x00bcb9f4u)
#define DAT_00bcba12 (*(undefined4 *)0x00bcba12u)
#define DAT_00bcba14 (*(undefined4 *)0x00bcba14u)
#define DAT_00bcba2c (*(undefined4 *)0x00bcba2cu)
#define DAT_00bcba42 (*(undefined4 *)0x00bcba42u)
#define DAT_00bcd68a (*(undefined4 *)0x00bcd68au)
#define DAT_00bcd720 (*(undefined4 *)0x00bcd720u)
#define DAT_00bcd724 (*(undefined4 *)0x00bcd724u)
#define DAT_00bcd728 (*(undefined4 *)0x00bcd728u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00eadcc8 (*(undefined *)0x00eadcc8u)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f55ce4 (*(undefined *)0x00f55ce4u)
#define DAT_00f5642c (*(undefined *)0x00f5642cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00eadd54 (*(undefined4 *)0x00eadd54u)
extern bool FUN_004a4300();
extern int FUN_004cde60();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d6860();
extern void FUN_0050f890();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_0050f320(void)

{
  int iVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  int iVar8;
  
  iVar2 = DAT_00eadcc8;
  _DAT_00eadd54 = 0x16f;
  FUN_00536820(DAT_00eadd90,DAT_00eadd8c,DAT_00eadd88,DAT_00eadd84);
  FUN_004d6250(0xb8,0,DAT_00eadd90,DAT_00eadd8c + 10,DAT_00eadd88 << 4,DAT_00f56fb4,0,DAT_00eadd90,
               DAT_00eadd8c);
  iVar8 = iVar2 * 0x184;
  iVar4 = FUN_004cde60(*(undefined4 *)(&DAT_00bcd720 + iVar8),*(undefined4 *)(&DAT_00bcd724 + iVar8)
                      );
  FUN_004d62f0(0xb8,2,DAT_00eadd90 + 100,DAT_00eadd8c + 0x28,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  if (iVar4 < 1) {
    cVar3 = '\b';
  }
  else if (iVar4 < 0x1f) {
    cVar3 = '\a';
  }
  else if (iVar4 < 0x33) {
    cVar3 = '\x06';
  }
  else if (iVar4 < 0x47) {
    cVar3 = '\x05';
  }
  else {
    cVar3 = (iVar4 < 0x5b) + '\x03';
  }
  FUN_004d62f0(0xb8,cVar3,DAT_00eadd90 + 300,DAT_00eadd8c + 0x28,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  if ((&DAT_00bcd728)[iVar8] != '\0') {
    iVar4 = (uint)(byte)(&DAT_00bcd68a)[iVar8] * 0x90;
    if (*(short *)(&DAT_00bcb9cc + iVar4) == 0xb) {
      uVar7 = 0x1f;
    }
    else if (*(short *)(&DAT_00bcb9cc + iVar4) == 0xc) {
      uVar7 = 0x21;
    }
    else {
      uVar7 = 0x20;
    }
    DAT_00e20778 = 0;
    FUN_004d62f0(0xb8,uVar7,DAT_00eadd90 + 100,DAT_00eadd8c + 0x3c,DAT_00f56f8c,0,DAT_00eadd90,
                 DAT_00eadd8c);
    FUN_004d62f0(0x8a,(&DAT_00bcb9ca)[iVar4],DAT_00e20778 + 100 + DAT_00eadd90,DAT_00eadd8c + 0x3c,
                 DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
    FUN_004d62f0(0xb8,0x22,DAT_00e20778 + 100 + DAT_00eadd90,DAT_00eadd8c + 0x3c,DAT_00f56f8c,0,
                 DAT_00eadd90,DAT_00eadd8c);
    FUN_004d62f0(0x8a,0x17,DAT_00eadd90 + 100,DAT_00eadd8c + 0x4c,DAT_00f56f8c,0,DAT_00eadd90,
                 DAT_00eadd8c);
    FUN_004d6860((&DAT_00bcb9f0)[iVar4],0x40,&DAT_0057c048,DAT_00eadd90 + 0x126,DAT_00eadd8c + 0x4c,
                 DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
    iVar5 = FUN_004cde60((int)*(short *)(&DAT_00bcba12 + iVar4),
                         (int)*(short *)(&DAT_00bcba14 + iVar4));
    FUN_004d62f0(0x8a,0x18,DAT_00eadd90 + 100,DAT_00eadd8c + 0x5c,DAT_00f56f8c,0,DAT_00eadd90,
                 DAT_00eadd8c);
    if (iVar5 < 1) {
      cVar3 = '\x1a';
    }
    else if (iVar5 < 0x15) {
      cVar3 = '\x1b';
    }
    else if (iVar5 < 0x29) {
      cVar3 = '\x1c';
    }
    else if (iVar5 < 0x38) {
      cVar3 = '\x1d';
    }
    else if (iVar5 < 0x47) {
      cVar3 = '\x1e';
    }
    else {
      cVar3 = (0x5a < iVar5) + '\x1f';
    }
    FUN_004d62f0(0x8a,cVar3,DAT_00eadd90 + 300,DAT_00eadd8c + 0x5c,DAT_00f56f8c,0,DAT_00eadd90,
                 DAT_00eadd8c);
    DAT_00e20778 = 0;
    FUN_004d62f0(0x8a,0x49,DAT_00eadd90 + 100,DAT_00eadd8c + 0x6c,DAT_00f56f8c,0,DAT_00eadd90,
                 DAT_00eadd8c);
    FUN_004d62f0(0x8a,0x19,DAT_00e20778 + 100 + DAT_00eadd90,DAT_00eadd8c + 0x6c,DAT_00f56f8c,0,
                 DAT_00eadd90,DAT_00eadd8c);
    FUN_0053e590((*(short *)(&DAT_00bcba42 + iVar4) + 10) / 0x14 + DAT_00f5642c,DAT_00eadd90 + 0x113
                 ,DAT_00eadd8c + 0x6c);
    FUN_004d62f0(0x8a,(*(short *)(&DAT_00bcba42 + iVar4) + 10) / 0x14 + 0x3c,DAT_00eadd90 + 300,
                 DAT_00eadd8c + 0x6c,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
    if (*(short *)(&DAT_00bcba2c + iVar4) == 0) {
      DAT_00e20778 = 0;
      FUN_004d62f0(0x8a,0x49,DAT_00eadd90 + 100,DAT_00eadd8c + 0x7c,DAT_00f56f8c,0,DAT_00eadd90,
                   DAT_00eadd8c);
      FUN_004d62f0(0x8a,0x24,DAT_00e20778 + 100 + DAT_00eadd90,DAT_00eadd8c + 0x7c,DAT_00f56f8c,0,
                   DAT_00eadd90,DAT_00eadd8c);
      iVar5 = DAT_00eadd90 + 300;
      iVar4 = (int)*(short *)(&DAT_00bcb9f4 + iVar4) / 5 + 0x25;
    }
    else {
      iVar5 = DAT_00eadd90 + 100;
      iVar4 = 0x3b;
    }
    FUN_004d62f0(0x8a,iVar4,iVar5,DAT_00eadd8c + 0x7c,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  }
  iVar4 = 0;
  do {
    iVar1 = iVar4 * 0x55 + 0x15 + DAT_00eadd90;
    iVar5 = DAT_00eadd8c + 0x8d;
    if (iVar4 == 2) {
      if ((&DAT_00bcd728)[iVar8] == '\0') {
LAB_0050f84f: ;
        iVar6 = (&DAT_005f78dc)[iVar4] + DAT_00f55ce4;
      }
      else {
        iVar6 = DAT_00f55ce4 + 3;
      }
    }
    else {
      if ((iVar4 != 4) || (cVar3 = FUN_004a4300(iVar2), cVar3 == '\0')) goto LAB_0050f84f;
      iVar6 = DAT_00f55ce4 + 6;
    }
    FUN_0053e590(iVar6,iVar1,iVar5);
    iVar4 = iVar4 + 1;
    if (4 < iVar4) {
      FUN_0050f890(iVar2);
      return;
    }
  } while( true );
}

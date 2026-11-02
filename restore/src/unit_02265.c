#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_007058b8 (*(undefined *)0x007058b8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bcb9ca (*(undefined4 *)0x00bcb9cau)
#define DAT_00bcb9cb (*(undefined4 *)0x00bcb9cbu)
#define DAT_00bcb9cc (*(undefined4 *)0x00bcb9ccu)
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
#define DAT_00bcb9f4 (*(undefined4 *)0x00bcb9f4u)
#define DAT_00bcba12 (*(undefined4 *)0x00bcba12u)
#define DAT_00bcba14 (*(undefined4 *)0x00bcba14u)
#define DAT_00bcba22 (*(undefined4 *)0x00bcba22u)
#define DAT_00bcba2c (*(undefined4 *)0x00bcba2cu)
#define DAT_00bcba3f (*(undefined4 *)0x00bcba3fu)
#define DAT_00bcba42 (*(undefined4 *)0x00bcba42u)
#define DAT_00bcba46 (*(undefined4 *)0x00bcba46u)
#define DAT_00d3b5dc (*(undefined *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(undefined *)0x00d3b5e0u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00eadd44 (*(undefined *)0x00eadd44u)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f55cc4 (*(undefined *)0x00f55cc4u)
#define DAT_00f55ccc (*(undefined *)0x00f55cccu)
#define DAT_00f55cd4 (*(undefined *)0x00f55cd4u)
#define DAT_00f5642c (*(undefined *)0x00f5642cu)
#define DAT_00f565cc (*(undefined *)0x00f565ccu)
#define DAT_00f5680c (*(undefined *)0x00f5680cu)
#define DAT_00f56814 (*(undefined *)0x00f56814u)
#define DAT_00f56b1c (*(undefined *)0x00f56b1cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define DAT_010d6a0c (*(undefined4 *)0x010d6a0cu)
#define DAT_010d6a0e (*(undefined4 *)0x010d6a0eu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
#define _DAT_00eadd54 (*(undefined4 *)0x00eadd54u)
extern int FUN_004cde60();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d6860();
extern void FUN_00536820();
extern int FUN_0053e590();
extern int FUN_0053fa40();

void FUN_005101b0(int param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  
  _DAT_00eadd54 = 0x57;
  FUN_00536820(DAT_00eadd90,DAT_00eadd8c,DAT_00eadd88,DAT_00eadd84);
  DAT_00eadd44 = 0;
  param_1 = param_1 * 0x90;
  FUN_004d6250(0x8a,(&DAT_00bcb9ca)[param_1],DAT_00eadd90,DAT_00eadd8c + 10,DAT_00eadd88 << 4,
               DAT_00f56fb4,0,DAT_00eadd90,DAT_00eadd8c);
  iVar4 = (uint)(byte)(&DAT_00bcb9ca)[param_1] + DAT_00f56814;
  iVar2 = iVar4;
  if (DAT_0126517c <= iVar4) {
    iVar2 = 0;
  }
  iVar6 = (int)(short)(&DAT_010d6a0e)[iVar2 * 0x20];
  iVar2 = iVar4;
  if (DAT_0126517c <= iVar4) {
    iVar2 = 0;
  }
  FUN_0053e590(iVar4,(0x28 - (short)(&DAT_010d6a0c)[iVar2 * 0x20]) / 2 + 0x10 + DAT_00eadd90,
               DAT_00eadd8c + 0x10);
  if (*(short *)(&DAT_00bcb9cc + param_1) == 0xd) {
    uVar5 = 0x4c;
    iVar2 = DAT_00f5680c;
  }
  else if (*(short *)(&DAT_00bcb9cc + param_1) == 0xc) {
    uVar5 = 0x4a;
    iVar2 = DAT_00f5680c + 0x12;
  }
  else {
    uVar5 = 0x4b;
    iVar2 = DAT_00f5680c + 9;
  }
  if (*(short *)(&DAT_00bcba22 + param_1) != 0) {
    iVar2 = iVar2 + 8;
  }
  iVar4 = iVar2;
  if (DAT_0126517c <= iVar2) {
    iVar4 = 0;
  }
  iVar3 = iVar2;
  if (DAT_0126517c <= iVar2) {
    iVar3 = 0;
  }
  sVar1 = (&DAT_010d6a0e)[iVar3 * 0x20];
  FUN_0053e590(iVar2,(0x28 - (short)(&DAT_010d6a0c)[iVar4 * 0x20]) / 2 + 0x10 + DAT_00eadd90,
               DAT_00eadd8c + 0x10 + iVar6);
  FUN_004d6250(0x8a,uVar5,DAT_00eadd90,DAT_00eadd8c + 0x1e,DAT_00eadd88 << 4,DAT_00f56fb4,0,
               DAT_00eadd90,DAT_00eadd8c);
  iVar4 = (DAT_00f565cc - (int)*(short *)(&DAT_00bcb9f4 + param_1) / 5) + 0x14;
  iVar2 = iVar4;
  if (DAT_0126517c <= iVar4) {
    iVar2 = 0;
  }
  FUN_0053e590(iVar4,(0x28 - (short)(&DAT_010d6a0c)[iVar2 * 0x20]) / 2 + 0x10 + DAT_00eadd90,
               DAT_00eadd8c + sVar1 + 0x10 + iVar6);
  iVar4 = (DAT_00f56b1c - (*(short *)(&DAT_00bcba42 + param_1) + 3) / 5) + 0x14;
  iVar2 = iVar4;
  if (DAT_0126517c <= iVar4) {
    iVar2 = 0;
  }
  FUN_0053e590(iVar4,(0x28 - (short)(&DAT_010d6a0c)[iVar2 * 0x20]) / 2 + 0x10 + DAT_00eadd90,
               DAT_00eadd8c + sVar1 + 0x10 + iVar6);
  FUN_004d62f0(0x8a,0x17,DAT_00eadd90 + 100,DAT_00eadd8c + 0x3c,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d6860((&DAT_00bcb9f0)[param_1],0x40,&DAT_0057c048,DAT_00eadd90 + 0x126,DAT_00eadd8c + 0x3c,
               DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  iVar2 = FUN_004cde60((int)*(short *)(&DAT_00bcba12 + param_1),
                       (int)*(short *)(&DAT_00bcba14 + param_1));
  FUN_004d62f0(0x8a,0x18,DAT_00eadd90 + 100,DAT_00eadd8c + 0x50,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  if (iVar2 < 1) {
    uVar5 = 0x1a;
  }
  else if (iVar2 < 0x15) {
    uVar5 = 0x1b;
  }
  else if (iVar2 < 0x29) {
    uVar5 = 0x1c;
  }
  else if (iVar2 < 0x38) {
    uVar5 = 0x1d;
  }
  else if (iVar2 < 0x47) {
    uVar5 = 0x1e;
  }
  else if (iVar2 < 0x5b) {
    uVar5 = 0x1f;
  }
  else {
    uVar5 = 0x20;
  }
  FUN_004d62f0(0x8a,uVar5,DAT_00eadd90 + 300,DAT_00eadd8c + 0x50,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  DAT_00e20778 = 0;
  FUN_004d62f0(0x8a,0x49,DAT_00eadd90 + 100,DAT_00eadd8c + 100,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d62f0(0x8a,0x19,DAT_00e20778 + 100 + DAT_00eadd90,DAT_00eadd8c + 100,DAT_00f56f8c,0,
               DAT_00eadd90,DAT_00eadd8c);
  FUN_0053e590((*(short *)(&DAT_00bcba42 + param_1) + 10) / 0x14 + DAT_00f5642c,DAT_00eadd90 + 0x113
               ,DAT_00eadd8c + 100);
  FUN_004d62f0(0x8a,(*(short *)(&DAT_00bcba42 + param_1) + 10) / 0x14 + 0x3c,DAT_00eadd90 + 300,
               DAT_00eadd8c + 100,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  if (*(short *)(&DAT_00bcba2c + param_1) == 0) {
    DAT_00e20778 = 0;
    FUN_004d62f0(0x8a,0x49,DAT_00eadd90 + 100,DAT_00eadd8c + 0x78,DAT_00f56f8c,0,DAT_00eadd90,
                 DAT_00eadd8c);
    FUN_004d62f0(0x8a,0x24,DAT_00e20778 + 100 + DAT_00eadd90,DAT_00eadd8c + 0x78,DAT_00f56f8c,0,
                 DAT_00eadd90,DAT_00eadd8c);
    iVar4 = DAT_00eadd90 + 300;
    iVar2 = (int)*(short *)(&DAT_00bcb9f4 + param_1) / 5 + 0x25;
  }
  else {
    iVar4 = DAT_00eadd90 + 100;
    iVar2 = 0x3b;
  }
  FUN_004d62f0(0x8a,iVar2,iVar4,DAT_00eadd8c + 0x78,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  if ((&DAT_00bcb9f0)[param_1] == '\0') {
    if (*(short *)(&DAT_00bcba2c + param_1) != 0) {
      FUN_004d6160(0x59,1,DAT_00eadd90 + 0x20,DAT_00eadd8c + 0xac,(DAT_00eadd88 + -4) * 0x10,
                   DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
      return;
    }
    if (DAT_007058b8 == 0) {
      FUN_004d6160(0x8a,0xb,DAT_00eadd90 + 0x20,DAT_00eadd8c + 0xac,(DAT_00eadd88 + -4) * 0x10,
                   DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
      return;
    }
    FUN_004d6160(0x8a,10,DAT_00eadd90 + 0x20,DAT_00eadd8c + 0xac,(DAT_00eadd88 + -4) * 0x10,
                 DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
    return;
  }
  iVar2 = 0;
  do {
    DAT_00d3b5dc = DAT_00eadd8c + 0x8d;
    DAT_00d3b5e0 = iVar2 * 0x55 + 0x15 + DAT_00eadd90;
    if (*(short *)(&DAT_00bcb9cc + param_1) == 0xd) {
      if (iVar2 == 1) {
        if (*(short *)(&DAT_00bcba46 + param_1) == 0) {
          FUN_0053e590(DAT_00f55cd4 + 1,DAT_00d3b5e0,DAT_00d3b5dc);
        }
        else {
          FUN_0053e590(DAT_00f55cd4 + 6,DAT_00d3b5e0,DAT_00d3b5dc);
        }
      }
      else if (((&DAT_00bcba3f)[param_1] == '\0') || (3 < iVar2)) {
        if ((iVar2 == 4) && ((&DAT_00bcb9cb)[param_1] != '\0')) {
          FUN_0053e590(DAT_00f55cd4 + 5,DAT_00d3b5e0,DAT_00d3b5dc);
        }
        else {
          if ((DAT_00bca2e0 == 0) || (DAT_00bca2e0 == 4)) {
            iVar4 = DAT_00f55cd4 + iVar2;
            goto LAB_00510ae5;
          }
          FUN_0053e590(DAT_00f55cd4 + iVar2,DAT_00d3b5e0,DAT_00d3b5dc);
        }
      }
      else if ((DAT_00bca2e0 == 0) || (DAT_00bca2e0 == 4)) {
        FUN_0053e590(DAT_00f55cd4 + iVar2,DAT_00d3b5e0,DAT_00d3b5dc);
      }
      else {
        iVar4 = DAT_00f55cd4 + iVar2;
LAB_00510ae5: ;
        FUN_0053fa40(iVar4,DAT_00d3b5e0,DAT_00d3b5dc);
      }
    }
    else if (*(short *)(&DAT_00bcb9cc + param_1) == 0xb) {
      if (((&DAT_00bcba3f)[param_1] == '\0') || (3 < iVar2)) {
        if ((iVar2 == 4) && ((&DAT_00bcb9cb)[param_1] != '\0')) {
          FUN_0053e590(DAT_00f55ccc + 5,DAT_00d3b5e0,DAT_00d3b5dc);
        }
        else {
          if ((DAT_00bca2e0 == 0) || (DAT_00bca2e0 == 4)) {
            iVar4 = DAT_00f55ccc + iVar2;
            goto LAB_00510ae5;
          }
          FUN_0053e590(DAT_00f55ccc + iVar2,DAT_00d3b5e0,DAT_00d3b5dc);
        }
      }
      else {
        if ((DAT_00bca2e0 != 0) && (iVar4 = DAT_00f55ccc, DAT_00bca2e0 != 4)) {
LAB_00510ae2: ;
          iVar4 = iVar4 + iVar2;
          goto LAB_00510ae5;
        }
        FUN_0053e590(DAT_00f55ccc + iVar2,DAT_00d3b5e0,DAT_00d3b5dc);
      }
    }
    else if (((&DAT_00bcba3f)[param_1] == '\0') || (3 < iVar2)) {
      if ((iVar2 == 4) && ((&DAT_00bcb9cb)[param_1] != '\0')) {
        FUN_0053e590(DAT_00f55cc4 + 5,DAT_00d3b5e0,DAT_00d3b5dc);
      }
      else {
        iVar4 = DAT_00f55cc4;
        if ((DAT_00bca2e0 == 0) || (DAT_00bca2e0 == 4)) goto LAB_00510ae2;
        FUN_0053e590(DAT_00f55cc4 + iVar2,DAT_00d3b5e0,DAT_00d3b5dc);
      }
    }
    else {
      if ((DAT_00bca2e0 != 0) && (DAT_00bca2e0 != 4)) {
        iVar4 = DAT_00f55cc4 + iVar2;
        goto LAB_00510ae5;
      }
      FUN_0053e590(DAT_00f55cc4 + iVar2,DAT_00d3b5e0,DAT_00d3b5dc);
    }
    iVar2 = iVar2 + 1;
    if (4 < iVar2) {
      return;
    }
  } while( true );
}

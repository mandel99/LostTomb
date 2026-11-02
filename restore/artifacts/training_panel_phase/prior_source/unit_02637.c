#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_0060598c (*(undefined *)0x0060598cu)
#define DAT_007056c0 (*(undefined *)0x007056c0u)
#define DAT_007056c4 (*(undefined *)0x007056c4u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_0078d64c (*(undefined4 *)0x0078d64cu)
#define DAT_0078d6dc (*(undefined4 *)0x0078d6dcu)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea4096 (*(undefined4 *)0x00ea4096u)
#define DAT_00ea40de (*(undefined4 *)0x00ea40deu)
#define DAT_00ea4126 (*(undefined4 *)0x00ea4126u)
#define DAT_00ea42f8 (*(undefined4 *)0x00ea42f8u)
#define DAT_00ea60a8 (*(undefined4 *)0x00ea60a8u)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
extern undefined4 FUN_00443950();
extern undefined4 FUN_004439c0();
extern undefined4 FUN_00443a20();
extern undefined4 FUN_00443b20();
extern undefined4 FUN_00443b90();
extern void FUN_004ccc30();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d6860();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_0055ad30(void)

{
  int *piVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  
  iVar4 = DAT_00f53f08 + 0x80;
  iVar6 = DAT_00f53f0c + 0x10;
  bVar2 = true;
  bVar3 = true;
  (&DAT_00ea60a8)[DAT_00eaac28 * 0x1276] = 1;
  FUN_00536820(iVar6,iVar4,0x26,0xf);
  FUN_0053e590(DAT_00f571fc + DAT_0060598c,DAT_00f53f0c + 0x1a,DAT_00f53f08 + 0x88);
  FUN_004d62f0(0x17,DAT_0060598c,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x89,DAT_00f56fb4,0,DAT_00e38da8
               ,DAT_00e38da4);
  switch(*(undefined2 *)(&DAT_00ea4096 + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2)) {
  case 0: ;
    iVar4 = FUN_00443950(DAT_0060598c);
    if (iVar4 == 0) {
      iVar4 = FUN_004439c0(DAT_0060598c);
      uVar8 = 0x22;
      if (iVar4 == 0) {
        uVar8 = 0x29;
      }
      bVar2 = false;
    }
    else {
      uVar8 = 0x27;
    }
    iVar4 = FUN_00443b20(DAT_0060598c);
    if (iVar4 == 0) {
LAB_0055aeb0: ;
      iVar4 = FUN_00443a20(DAT_0060598c);
      if (iVar4 == 0) {
        uVar7 = 0x2a;
        bVar3 = false;
      }
      else {
        uVar7 = 0x23;
        bVar3 = false;
      }
    }
    else {
      uVar7 = 0x28;
    }
    break;
  case 1: ;
    uVar8 = 0x13;
    iVar4 = FUN_00443b20(DAT_0060598c);
    if (iVar4 == 0) goto LAB_0055aeb0;
    uVar7 = 0x28;
    break;
  case 2: ;
    uVar7 = 0x14;
    iVar4 = FUN_00443950(DAT_0060598c);
    if (iVar4 == 0) {
      iVar4 = FUN_004439c0(DAT_0060598c);
      if (iVar4 == 0) {
LAB_0055af35: ;
        uVar8 = 0x29;
      }
      else {
        uVar8 = 0x22;
      }
LAB_0055af3a: ;
      bVar2 = false;
    }
    else {
      uVar8 = 0x27;
    }
    break;
  case 3: ;
    uVar8 = 0x2b;
    iVar4 = FUN_00443b20(DAT_0060598c);
    if (iVar4 == 0) goto LAB_0055aeb0;
    uVar7 = 0x28;
    break;
default: ;
    uVar7 = 0x2c;
    iVar4 = FUN_00443950(DAT_0060598c);
    if (iVar4 == 0) {
      iVar4 = FUN_004439c0(DAT_0060598c);
      uVar8 = 0x22;
      if (iVar4 == 0) goto LAB_0055af35;
      goto LAB_0055af3a;
    }
    uVar8 = 0x27;
  }
  iVar4 = 1;
  if (DAT_0060598c != 9) {
    iVar4 = DAT_0060598c;
  }
  DAT_00e20778 = 0;
  iVar6 = FUN_00443b90(iVar4);
  if (iVar6 == 0) {
    iVar5 = DAT_00f53f08 + 0xac;
    iVar6 = DAT_00f53f0c + 0x62;
    uVar9 = 0x19;
  }
  else {
    piVar1 = &DAT_0078d6dc + iVar4;
    iVar6 = *piVar1;
    if (iVar6 < 1) {
      iVar5 = DAT_00f53f08 + 0xac;
      iVar6 = DAT_00f53f0c + 0x62;
      uVar9 = 7;
    }
    else if (*(short *)(&DAT_00ea4126 + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2) == 1) {
      FUN_004d6860(iVar6,0x40,&DAT_0057c048,DAT_00f53f0c + 0x62,DAT_00f53f08 + 0xac,DAT_00f56f8c,0,
                   DAT_00e38da8,DAT_00e38da4);
      if (*piVar1 == 1) {
        iVar5 = DAT_00f53f08 + 0xac;
        iVar6 = DAT_00e20778 + 0x62 + DAT_00f53f0c;
        uVar9 = 10;
      }
      else {
        iVar5 = DAT_00f53f08 + 0xac;
        iVar6 = DAT_00e20778 + 0x62 + DAT_00f53f0c;
        uVar9 = 0xb;
      }
    }
    else if (iVar6 == (&DAT_0078d64c)[iVar4]) {
      FUN_004d6860(iVar6,0x40,&DAT_0057c048,DAT_00f53f0c + 0x62,DAT_00f53f08 + 0xac,DAT_00f56f8c,0,
                   DAT_00e38da8,DAT_00e38da4);
      if (*piVar1 == 1) {
        iVar5 = DAT_00f53f08 + 0xac;
        iVar6 = DAT_00e20778 + 0x62 + DAT_00f53f0c;
        uVar9 = 8;
      }
      else {
        iVar5 = DAT_00f53f08 + 0xac;
        iVar6 = DAT_00e20778 + 0x62 + DAT_00f53f0c;
        uVar9 = 9;
      }
    }
    else {
      FUN_004d6860((&DAT_0078d64c)[iVar4],0x40,&DAT_0057c048,DAT_00f53f0c + 0x62,DAT_00f53f08 + 0xac
                   ,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004d62f0(0x36,0xc,DAT_00e20778 + 0x62 + DAT_00f53f0c,DAT_00f53f08 + 0xac,DAT_00f56f8c,0,
                   DAT_00e38da8,DAT_00e38da4);
      FUN_004d6860(*piVar1 - (&DAT_0078d64c)[iVar4],0x40,&DAT_0057c048,
                   DAT_00e20778 + 0x62 + DAT_00f53f0c,DAT_00f53f08 + 0xac,DAT_00f56f8c,0,
                   DAT_00e38da8,DAT_00e38da4);
      if ((&DAT_0078d64c)[iVar4] == 1) {
        iVar5 = DAT_00f53f08 + 0xac;
        iVar6 = DAT_00e20778 + 0x62 + DAT_00f53f0c;
        uVar9 = 0xd;
      }
      else {
        iVar5 = DAT_00f53f08 + 0xac;
        iVar6 = DAT_00e20778 + 0x62 + DAT_00f53f0c;
        uVar9 = 0xe;
      }
    }
  }
  FUN_004d62f0(0x36,uVar9,iVar6,iVar5,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  iVar6 = (&DAT_00ea6160)[DAT_00eaac28 * 0x1276 + DAT_0060598c];
  DAT_00e20778 = 0;
  switch((int)(DAT_0060598c)) {
  case 0x18: ;
  case 0x19: ;
  case 0x1a: ;
  case 0x1e: ;
  case 0x23: ;
    FUN_004ccc30(8,0x38,iVar6 / 100,DAT_00f53f0c + 0x62,DAT_00f53f08 + 0xc0,DAT_00f56f8c,0);
    break;
default: ;
    switch((int)(DAT_0060598c)) {
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1e: ;
    case 0x23: ;
switchD_0055b343_caseD_18: ;
      iVar6 = iVar6 / 100;
      break;
default: ;
      if (((DAT_0060598c == 0x1c) || (DAT_0060598c == 10)) || (DAT_0060598c == 0x24))
      goto switchD_0055b343_caseD_18;
    }
    FUN_004d6860(iVar6,0x40,&DAT_0057c048,DAT_00f53f0c + 0x62,DAT_00f53f08 + 0xc0,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
  }
  FUN_004d62f0(0x36,0xf,DAT_00e20778 + 0x62 + DAT_00f53f0c,DAT_00f53f08 + 0xc0,DAT_00f56f8c,0,
               DAT_00e38da8,DAT_00e38da4);
  if (DAT_007056c4 == 1) {
    if (bVar2) {
      uVar9 = 0xf800;
LAB_0055b45f: ;
      FUN_004d63d0(0,DAT_00f53f0c + 0x1a,DAT_00f53f08 + 0xd4,0x123,0x1e,uVar9,DAT_00e38da8,
                   DAT_00e38da4);
      goto LAB_0055b468;
    }
LAB_0055b4a9: ;
    if (bVar3) {
      uVar9 = 0;
LAB_0055b4dd: ;
      FUN_004d63d0(0,DAT_00f53f0c + 0x141,DAT_00f53f08 + 0xd4,0x123,0x1e,uVar9,DAT_00e38da8,
                   DAT_00e38da4);
    }
  }
  else {
    if (bVar2) {
      uVar9 = 0;
      goto LAB_0055b45f;
    }
LAB_0055b468: ;
    if (DAT_007056c4 != 2) goto LAB_0055b4a9;
    if (bVar3) {
      uVar9 = 0xf800;
      goto LAB_0055b4dd;
    }
  }
  if (*(short *)(&DAT_00ea4096 + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2) == 1) {
    DAT_00e20778 = 0;
    FUN_004d62f0(0x36,uVar8,DAT_00f53f0c + 0x1f,DAT_00f53f08 + 0xdd,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    switch((int)(DAT_0060598c)) {
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1e: ;
    case 0x23: ;
      break;
default: ;
      if (((DAT_0060598c != 0x1c) && (DAT_0060598c != 10)) && (DAT_0060598c != 0x24)) {
        FUN_004d6860(*(short *)(&DAT_00ea40de + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2) * 100,
                     0x40,&DAT_0057c048,DAT_00e20778 + 0x1a + DAT_00f53f0c,DAT_00f53f08 + 0xdd,
                     DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
        goto LAB_0055b6ce;
      }
    }
    switch((int)(DAT_0060598c)) {
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1e: ;
    case 0x23: ;
      FUN_004ccc30(8,0x38,(int)*(short *)(&DAT_00ea40de + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2
                                         ),DAT_00e20778 + 0x18 + DAT_00f53f0c,DAT_00f53f08 + 0xdd,
                   DAT_00f56f8c,0);
      break;
default: ;
      FUN_004d6860((int)*(short *)(&DAT_00ea40de + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2),0x40,
                   &DAT_0057c048,DAT_00e20778 + 0x1a + DAT_00f53f0c,DAT_00f53f08 + 0xdd,DAT_00f56f8c
                   ,0,DAT_00e38da8,DAT_00e38da4);
    }
  }
  else {
    FUN_004d6250(0x36,uVar8,DAT_00f53f0c + 0x26,DAT_00f53f08 + 0xdd,200,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
LAB_0055b6ce: ;
  if (*(short *)(&DAT_00ea4096 + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2) == 2) {
    DAT_00e20778 = 0;
    FUN_004d62f0(0x36,uVar7,DAT_00f53f0c + 0x14b,DAT_00f53f08 + 0xdd,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    switch((int)(DAT_0060598c)) {
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1e: ;
    case 0x23: ;
      break;
default: ;
      if (((DAT_0060598c != 0x1c) && (DAT_0060598c != 10)) && (DAT_0060598c != 0x24)) {
        FUN_004d6860(*(short *)(&DAT_00ea40de + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2) * 100,
                     0x40,&DAT_0057c048,DAT_00e20778 + 0x146 + DAT_00f53f0c,DAT_00f53f08 + 0xdd,
                     DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
        goto LAB_0055b8c6;
      }
    }
    switch((int)(DAT_0060598c)) {
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1e: ;
    case 0x23: ;
      FUN_004ccc30(8,0x38,(int)*(short *)(&DAT_00ea40de + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2
                                         ),DAT_00e20778 + 0x144 + DAT_00f53f0c,DAT_00f53f08 + 0xdd,
                   DAT_00f56f8c,0);
      break;
default: ;
      FUN_004d6860((int)*(short *)(&DAT_00ea40de + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2),0x40,
                   &DAT_0057c048,DAT_00e20778 + 0x146 + DAT_00f53f0c,DAT_00f53f08 + 0xdd,
                   DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    }
  }
  else {
    FUN_004d6250(0x36,uVar7,DAT_00f53f0c + 0x14e,DAT_00f53f08 + 0xdd,200,DAT_00f56f8c,0,DAT_00e38da8
                 ,DAT_00e38da4);
  }
LAB_0055b8c6: ;
  if (0 < (int)(&DAT_0078d6dc)[iVar4]) {
    FUN_004d63d0(0,DAT_00f53f0c + 0x62,DAT_00f53f08 + 0xfa,0x1b0,0x1e,DAT_007056c8 != 0,DAT_00e38da8
                 ,DAT_00e38da4);
    if (*(short *)(&DAT_00ea4126 + (DAT_00eaac28 * 0x24ec + DAT_0060598c) * 2) == 0) {
      uVar7 = 0x10;
    }
    else {
      uVar7 = 0x11;
    }
    FUN_004d6250(0x36,uVar7,DAT_00f53f0c + 0x72,DAT_00f53f08 + 0x103,400,DAT_00f56f8c,0,DAT_00e38da8
                 ,DAT_00e38da4);
  }
  FUN_004d63d0(0,DAT_00f53f0c + 0x62,DAT_00f53f08 + 0x120,0x1b0,0x32,DAT_007056c0 != 0,DAT_00e38da8,
               DAT_00e38da4);
  if (*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + DAT_0060598c) * 4) == 0) {
    FUN_004d6250(0x36,0x1c,DAT_00f53f0c + 0x72,DAT_00f53f08 + 0x128,400,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6250(0x36,0x1d,DAT_00f53f0c + 0x72,DAT_00f53f08 + 0x13c,400,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    return;
  }
  FUN_004d6250(0x36,0x1a,DAT_00f53f0c + 0x72,DAT_00f53f08 + 0x128,400,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x36,0x1b,DAT_00f53f0c + 0x72,DAT_00f53f08 + 0x13c,400,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  return;
}

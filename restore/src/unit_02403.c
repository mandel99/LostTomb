#include "restore_types.h"
#define BYTE_0052c4c4 (*(char *)0x0052c4c4u)
#define DAT_00784de0 (*(undefined *)0x00784de0u)
#define UNK_0052c4d4 (*(undefined *)0x0052c4d4u)
#define UNK_0052c4d8 (*(undefined *)0x0052c4d8u)
#define UNK_0052c4e8 (*(undefined *)0x0052c4e8u)
#define UNK_0052c4ec (*(undefined *)0x0052c4ecu)
#define UNK_0052c4fc (*(undefined *)0x0052c4fcu)
#define UNK_0052c500 (*(undefined *)0x0052c500u)
#define UNK_0052c510 (*(undefined *)0x0052c510u)
#define _DAT_007056c8 (*(undefined4 *)0x007056c8u)
#define _DAT_00e20774 (*(undefined4 *)0x00e20774u)
#define _DAT_00e20778 (*(undefined4 *)0x00e20778u)
#define _DAT_00e38da4 (*(undefined4 *)0x00e38da4u)
#define _DAT_00e38da8 (*(undefined4 *)0x00e38da8u)
#define _DAT_00eaac28 (*(undefined4 *)0x00eaac28u)
#define _DAT_00f53f08 (*(undefined4 *)0x00f53f08u)
#define _DAT_00f53f0c (*(undefined4 *)0x00f53f0cu)
#define _DAT_00f56f9c (*(undefined4 *)0x00f56f9cu)
#define _DAT_00f571fc (*(undefined4 *)0x00f571fcu)
extern void FUN_00401540();
extern int FUN_0040cc40();
extern int FUN_0040d940();
extern int FUN_00417050();
extern void FUN_004cd1a0();
extern void FUN_004cd290();
extern void FUN_004d6200();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern int FUN_004df040();
extern int FUN_004e6710();
extern void FUN_004f1310();
extern int FUN_004f52b0();
extern int FUN_0053e590();
extern int FUN_00542ad0();
extern undefined4 FUN_00545740();
extern undefined4 FUN_0054f470();
extern undefined4 FUN_00555e80();
extern undefined4 FUN_00558be0();

void FUN_0052b6d0(void)

{
  byte bVar1;
  undefined2 uVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 unaff_EBX;
  int *piVar7;
  int unaff_EBP;
  uint uVar8;
  undefined4 uVar9;
  int iStack_84;
  int iStack_80;
  int iStack_7c;
  uint uStack_78;
  int iStack_74;
  undefined1 auStack_70 [52];
  uint uStack_3c;
  
  iVar4 = FUN_0040cc40(0);
  if (iVar4 != 0) {
    iVar4 = 0;
    iStack_80 = 0;
    piVar7 = (int *)0x784de8;
    do {
      uVar8 = iStack_80 - 0x18;
      switch((int)(iStack_80)) {
      case 1: ;
      case 8: ;
      case 10: ;
      case 0xd: ;
      case 0xf: ;
      case 0x11: ;
      case 0x12: ;
      case 0x13: ;
      case 0x14: ;
      case 0x17: ;
      case 0x18: ;
      case 0x19: ;
      case 0x1a: ;
      case 0x1c: ;
      case 0x1e: ;
        if (0 < *piVar7) {
          _DAT_00e20778 = 0;
          FUN_0053e590(_DAT_00f571fc + iStack_80,_DAT_00f53f0c + 0x28,iVar4 + 0x42 + _DAT_00f53f08);
          iVar5 = *piVar7 - piVar7[0x24];
          if (uVar8 < 0xc) {
                    // WARNING: Could not recover jumptable at 0x0052b789. Too many branches
                    // WARNING: Treating indirect jump as call
            (**(code **)(&UNK_0052c4d4 + (uint)(&BYTE_0052c4c4)[unaff_EBP] * 4))();
            return;
          }
          if (((piVar7 != (int *)0x784e58) && (piVar7 != (int *)0x784e10)) &&
             (piVar7 != (int *)0x784e78)) {
            iVar5 = iVar5 * 100;
          }
          FUN_004cd1a0(0,unaff_EBX,_DAT_007056c8 == iStack_80,0,0);
          _DAT_00e20778 = 0;
          if (iVar5 == 0) {
            FUN_004d62f0(199,2,_DAT_00f53f0c + 0x3c,iVar4 + 0x42 + _DAT_00f53f08,_DAT_00f56f9c,0,
                         _DAT_00e38da8,_DAT_00e38da4);
          }
          else {
            FUN_004d6860(iVar5,0x40,0x57c048,_DAT_00f53f0c + 0x3c);
          }
          if (uVar8 < 0xc) {
                    // WARNING: Could not recover jumptable at 0x0052b847. Too many branches
                    // WARNING: Treating indirect jump as call
            (**(code **)(&UNK_0052c4e8 + (uint)(byte)(&UNK_0052c4d8)[unaff_EBP] * 4))();
            return;
          }
          iVar6 = unaff_EBP;
          if (((piVar7 == (int *)0x784e58) || (piVar7 == (int *)0x784e10)) && (1 < iVar5)) {
            iVar6 = iStack_84 + 0x37;
          }
          FUN_004d62f0(0x17,iVar6,_DAT_00f53f0c + 0x3c + _DAT_00e20778,iVar4 + 0x42 + _DAT_00f53f08,
                       _DAT_00f56f9c,0,_DAT_00e38da8,_DAT_00e38da4);
          _DAT_00e20778 = 0;
          iVar5 = *(int *)((_DAT_00eaac28 * 0x1276 + unaff_EBP) * 4 + 0xea6160);
          if (uVar8 < 0xc) {
                    // WARNING: Could not recover jumptable at 0x0052b90e. Too many branches
                    // WARNING: Treating indirect jump as call
            (**(code **)(&UNK_0052c4fc + (uint)(byte)(&UNK_0052c4ec)[unaff_EBP] * 4))();
            return;
          }
          if (((piVar7 == (int *)0x784e58) || (piVar7 == (int *)0x784e10)) ||
             (piVar7 == (int *)0x784e78)) {
            iVar5 = iVar5 / 100;
          }
          FUN_004d6860(iVar5,0x40,0x57c048,_DAT_00f53f0c + 300,iVar4 + 0x42 + _DAT_00f53f08,
                       _DAT_00f56f9c,0,_DAT_00e38da8,_DAT_00e38da4);
          if (uVar8 < 0xc) {
                    // WARNING: Could not recover jumptable at 0x0052b994. Too many branches
                    // WARNING: Treating indirect jump as call
            (**(code **)(&UNK_0052c510 + (uint)(byte)(&UNK_0052c500)[unaff_EBP] * 4))();
            return;
          }
          FUN_004d62f0(199,1,_DAT_00f53f0c + 300 + _DAT_00e20778,iVar4 + 0x42 + _DAT_00f53f08,
                       _DAT_00f56f9c,0,_DAT_00e38da8,_DAT_00e38da4);
          iVar4 = iVar4 + 0x2a;
        }
      }
      piVar7 = piVar7 + 1;
      iStack_80 = iStack_80 + 1;
    } while ((int)piVar7 < 0x784e78);
  }
  iVar4 = 5;
  iStack_74 = 5;
  FUN_004d65b0(_DAT_00f53f0c + 0x26,_DAT_00f53f08 + 0x11a,0x23,8,_DAT_00e38da8,_DAT_00e38da4);
  iStack_80 = 0;
  do {
    iStack_84 = 0;
    iVar5 = *(short *)(&DAT_00784de0 + iStack_80 * 2) * 0x10;
    iStack_7c = 0;
    uStack_78 = 0;
    if (0 < *(short *)(&DAT_00784de0 + iStack_80 * 2)) {
      uVar2 = *(undefined2 *)(iVar5 + 0x5d19ca);
      if (*(short *)(iVar5 + 0x5d19c8) == 1) {
        switch((int)(uVar2)) {
        case 1: ;
          iVar5 = 0xf;
          break;
        case 2: ;
          iVar5 = 5;
          break;
        case 3: ;
          iVar5 = 10;
          break;
        case 4: ;
          iVar5 = 0;
          break;
default: ;
          iVar5 = 0x14;
        }
        iVar6 = (_DAT_00eaac28 * 0x1276 + iStack_80) * 4;
        if (*(int *)(iVar6 + 0xea5fa0) == 0) {
          iVar6 = iVar5 + 0xe;
          iStack_84 = iVar5 + 0xf;
          uVar8 = 100;
        }
        else {
          FUN_004f1310(*(undefined4 *)(iVar6 + 0xea5fac),auStack_70);
          uVar8 = uStack_3c;
          if ((int)uStack_3c < 100) {
            iVar6 = iVar5 + 0x10;
            iStack_84 = 0xd;
            if (_DAT_007056c8 == iVar4 + 1) {
              FUN_004cd1a0(0,iVar4 * 0x1c + 0x5c5798,0xf800,0,0);
            }
          }
          else {
            uVar8 = 100;
            if ((*(int *)(_DAT_00eaac28 * 0x49d8 + 0xea6090) == 0) &&
               (0xb < *(int *)(_DAT_00eaac28 * 0x49d8 + 0xea6098))) {
              iVar6 = iVar5 + 0x11;
            }
            else {
              iVar6 = iVar5 + 0x12;
            }
          }
        }
      }
      else {
        switch((int)(uVar2)) {
        case 1: ;
          if (*(int *)((_DAT_00eaac28 * 0x1276 + iStack_80) * 4 + 0xea5fa0) == 0) {
            uVar8 = 100;
            iVar6 = 0x27;
            iStack_84 = 0x28;
          }
          else {
            iVar5 = *(short *)(_DAT_00eaac28 * 0x49d8 + 0xea5baa) * 100;
            uVar8 = (int)(iVar5 + (iVar5 >> 0x1f & 0xfU)) >> 4;
            if ((int)uVar8 < 100) {
              iVar6 = 0x29;
LAB_0052c0e0: ;
              uStack_78 = 0;
              iStack_7c = 0;
              iStack_84 = 0xd;
              if (_DAT_007056c8 == iVar4 + 1) {
                FUN_004cd1a0(0,iVar4 * 0x1c + 0x5c5798,0xf800,0,0);
              }
            }
            else {
              uVar8 = 100;
              iVar6 = 0x2a;
            }
          }
          break;
        case 2: ;
          iVar5 = (_DAT_00eaac28 * 0x1276 + iStack_80) * 4;
          if (*(int *)(iVar5 + 0xea5fa0) == 0) {
            uVar8 = 100;
            iVar6 = 0x2b;
            iStack_84 = 0x2c;
            iVar4 = FUN_004f52b0(iStack_80);
            uStack_78 = (-(uint)(iVar4 != 0xcf) & 10000) + 10000;
            iStack_7c = *(int *)(_DAT_00eaac28 * 0x49d8 + 0xea61c8);
          }
          else {
            iStack_7c = 0;
            uStack_78 = 0;
            bVar1 = *(byte *)(*(int *)(iVar5 + 0xea5fac) * 0x108 + 0x799a2d);
            uVar8 = (uint)bVar1;
            if (bVar1 < 100) {
              iVar6 = 0x2d;
              iStack_84 = 0xd;
              if (_DAT_007056c8 == iVar4 + 1) {
                FUN_004cd1a0(0,iVar4 * 0x1c + 0x5c5798,0xf800,0,0);
              }
            }
            else {
              uVar8 = 100;
              iVar6 = 0x2e;
            }
          }
          break;
        case 3: ;
          iVar5 = (_DAT_00eaac28 * 0x1276 + iStack_80) * 4;
          if (*(int *)(iVar5 + 0xea5fa0) == 0) {
            uVar8 = 100;
            iVar6 = 0x2f;
            iStack_7c = *(int *)(_DAT_00eaac28 * 0x49d8 + 0xea61d8);
            iStack_84 = 0x30;
            uStack_78 = 22000;
          }
          else {
            iStack_7c = 0;
            uStack_78 = 0;
            uVar8 = FUN_00542ad0(*(undefined4 *)(iVar5 + 0xea5fac),0);
            if ((int)uVar8 < 100) {
              iVar6 = 0x31;
              goto LAB_0052c0e0;
            }
            uVar8 = 100;
            iVar6 = 0x32;
          }
          break;
default: ;
          iVar4 = (_DAT_00eaac28 * 0x1276 + iStack_80) * 4;
          if (*(int *)(iVar4 + 0xea5fa0) == 0) {
            uVar8 = 100;
            iVar6 = 0x33;
            iStack_7c = *(int *)(_DAT_00eaac28 * 0x49d8 + 0xea61d8);
            iStack_84 = 0x34;
            uStack_78 = 24000;
          }
          else {
            iStack_7c = 0;
            uStack_78 = 0;
            uVar8 = FUN_004df040(*(undefined4 *)(iVar4 + 0xea5fac),0);
            if ((int)uVar8 < 100) {
              iVar6 = 0x35;
              iStack_84 = 0xd;
            }
            else {
              uVar8 = 100;
              if ((*(int *)(_DAT_00eaac28 * 0x49d8 + 0xea6090) != 0) ||
                 (iVar6 = 0x36, *(int *)(_DAT_00eaac28 * 0x49d8 + 0xea6098) < 0xc)) {
                iVar6 = 0x37;
              }
            }
          }
          break;
        case 5: ;
          iVar5 = (_DAT_00eaac28 * 0x1276 + iStack_80) * 4;
          if (*(int *)(iVar5 + 0xea5fa0) == 0) {
            uVar8 = 100;
            iVar6 = 0x43;
            iStack_84 = 0x44;
          }
          else {
            uVar8 = FUN_004e6710(*(undefined4 *)(iVar5 + 0xea5fac),0);
            if ((int)uVar8 < 100) {
              iVar6 = 0x45;
              iStack_84 = 0xd;
              if (_DAT_007056c8 == iVar4 + 1) {
                FUN_004cd1a0(0,iVar4 * 0x1c + 0x5c5798,0xf800,0,0);
              }
            }
            else {
              uVar8 = 100;
              iVar6 = 0x46;
            }
          }
          break;
        case 6: ;
          iVar5 = (_DAT_00eaac28 * 0x1276 + iStack_80) * 4;
          if (*(int *)(iVar5 + 0xea5fa0) == 0) {
            uVar8 = 100;
            iVar6 = 0x3b;
            iStack_84 = 0x3c;
          }
          else {
            uVar8 = FUN_0040d940(*(undefined4 *)(iVar5 + 0xea5fac),0);
            if ((int)uVar8 < 100) {
              iVar6 = 0x3d;
              iStack_84 = 0xd;
              if (_DAT_007056c8 == iVar4 + 1) {
                FUN_004cd1a0(0,iVar4 * 0x1c + 0x5c5798,0xf800,0,0);
              }
            }
            else {
              uVar8 = 100;
              iVar6 = 0x3e;
            }
          }
          break;
        case 7: ;
          iVar5 = (_DAT_00eaac28 * 0x1276 + iStack_80) * 4;
          if (*(int *)(iVar5 + 0xea5fa0) == 0) {
            uVar8 = 100;
            iVar6 = 0x3f;
            iStack_84 = 0x40;
          }
          else {
            uVar8 = FUN_00417050(*(undefined4 *)(iVar5 + 0xea5fac),0);
            if ((int)uVar8 < 100) {
              iVar6 = 0x41;
              iStack_84 = 0xd;
              if (_DAT_007056c8 == iVar4 + 1) {
                FUN_004cd1a0(0,iVar4 * 0x1c + 0x5c5798,0xf800,0,0);
              }
            }
            else {
              uVar8 = 100;
              iVar6 = 0x42;
            }
          }
          break;
        case 10: ;
          iVar5 = (_DAT_00eaac28 * 0x1276 + iStack_80) * 4;
          if (*(int *)(iVar5 + 0xea5fa0) == 0) {
            uVar8 = 100;
            iVar6 = 0x4b;
            iStack_84 = 0x4c;
          }
          else {
            uVar8 = FUN_00545740(*(undefined4 *)(iVar5 + 0xea5fac),0);
            if ((int)uVar8 < 100) {
              iVar6 = 0x4d;
              iStack_84 = 0xd;
              if (_DAT_007056c8 == iVar4 + 1) {
                FUN_004cd1a0(0,iVar4 * 0x1c + 0x5c5798,0xf800,0,0);
              }
            }
            else {
              uVar8 = 100;
              iVar6 = 0x4e;
            }
          }
          break;
        case 0xb: ;
          iVar5 = (_DAT_00eaac28 * 0x1276 + iStack_80) * 4;
          if (*(int *)(iVar5 + 0xea5fa0) == 0) {
            uVar8 = 100;
            iVar6 = 0x4f;
            iStack_84 = 0x50;
          }
          else {
            uVar8 = FUN_00558be0(*(undefined4 *)(iVar5 + 0xea5fac),0);
            if ((int)uVar8 < 100) {
              iVar6 = 0x51;
              iStack_84 = 0xd;
              if (_DAT_007056c8 == iVar4 + 1) {
                FUN_004cd1a0(0,iVar4 * 0x1c + 0x5c5798,0xf800,0,0);
              }
            }
            else {
              uVar8 = 100;
              iVar6 = 0x52;
            }
          }
          break;
        case 0xc: ;
          iVar5 = (_DAT_00eaac28 * 0x1276 + iStack_80) * 4;
          if (*(int *)(iVar5 + 0xea5fa0) == 0) {
            uVar8 = 100;
            iVar6 = 0x53;
            iStack_84 = 0x54;
          }
          else {
            uVar8 = FUN_00555e80(*(undefined4 *)(iVar5 + 0xea5fac),0);
            if ((int)uVar8 < 100) {
              iVar6 = 0x55;
              goto LAB_0052c0e0;
            }
            uVar8 = 100;
            iVar6 = 0x56;
          }
          break;
        case 0xd: ;
          iVar5 = (_DAT_00eaac28 * 0x1276 + iStack_80) * 4;
          if (*(int *)(iVar5 + 0xea5fa0) == 0) {
            uVar8 = 100;
            iVar6 = 0x57;
            iStack_84 = 0x58;
          }
          else {
            uVar8 = FUN_0054f470(*(undefined4 *)(iVar5 + 0xea5fac),0);
            if ((int)uVar8 < 100) {
              iVar6 = 0x59;
              iStack_84 = 0xd;
              if (_DAT_007056c8 == iVar4 + 1) {
                FUN_004cd1a0(0,iVar4 * 0x1c + 0x5c5798,0xf800,0,0);
              }
            }
            else {
              uVar8 = 100;
              iVar6 = 0x5a;
            }
          }
          break;
        case 0xe: ;
          iVar5 = (_DAT_00eaac28 * 0x1276 + iStack_80) * 4;
          if (*(int *)(iVar5 + 0xea5fa0) == 0) {
            uVar8 = 100;
            iVar6 = 0x47;
            iStack_84 = 0x48;
          }
          else {
            uVar8 = (FUN_00401540(*(undefined4 *)(iVar5 + 0xea5fac),0), restore_unresolved_return(0x00401540u));
            if ((int)uVar8 < 100) {
              iVar6 = 0x49;
              goto LAB_0052c0e0;
            }
            uVar8 = 100;
            iVar6 = 0x4a;
          }
        }
      }
      uVar3 = _DAT_00f56f9c;
      _DAT_00e20778 = 0;
      _DAT_00e20774 = 0;
      iVar4 = iStack_74 * 0x1c + 0x5c5798;
      FUN_004d6200(199,iVar6,iVar4,_DAT_00f56f9c,0,8,8,_DAT_00e38da8,_DAT_00e38da4);
      if ((int)uVar8 < 100) {
        FUN_004cd290(uVar8,0x40,0x5e03fc,iVar4,uVar3,0,_DAT_00e20778 + 8,8);
        FUN_004d6200(199,2,iVar4,uVar3,0,_DAT_00e20778 + 8,8,_DAT_00e38da8,_DAT_00e38da4);
      }
      if (uStack_78 != 0) {
        FUN_004cd290(uStack_78 / 100,0x40,0x57c048,iVar4,uVar3,0,_DAT_00e20778 + 8,8);
      }
      _DAT_00e20778 = 0;
      if (iStack_84 != 0) {
        FUN_004d6200(199,iStack_84,iVar4,uVar3,0,8,0x18,_DAT_00e38da8,_DAT_00e38da4);
      }
      if (uStack_78 != 0) {
        FUN_004cd290(iStack_7c / 100,0x40,0x57c048,iVar4,uVar3,0,_DAT_00e20778 + 8,0x18);
        if (iStack_7c == 100) {
          uVar9 = 0x39;
        }
        else {
          uVar9 = 0x3a;
        }
        FUN_004d6200(199,uVar9,iVar4,uVar3,0,_DAT_00e20778 + 8,0x18,_DAT_00e38da8,_DAT_00e38da4);
      }
      iVar4 = iStack_74 + 1;
      iStack_74 = iVar4;
    }
    iStack_80 = iStack_80 + 1;
    if (2 < iStack_80) {
      return;
    }
  } while( true );
}

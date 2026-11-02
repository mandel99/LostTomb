#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005c5b78 (*(undefined4 *)0x005c5b78u)
#define DAT_005c5b90 (*(undefined4 *)0x005c5b90u)
#define DAT_005d2c48 (*(undefined4 *)0x005d2c48u)
#define DAT_005d2c4c (*(undefined4 *)0x005d2c4cu)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea4096 (*(undefined4 *)0x00ea4096u)
#define DAT_00ea40de (*(undefined4 *)0x00ea40deu)
#define DAT_00ea4104 (*(undefined4 *)0x00ea4104u)
#define DAT_00ea4126 (*(undefined4 *)0x00ea4126u)
#define DAT_00ea42f8 (*(undefined4 *)0x00ea42f8u)
#define DAT_00ea60a4 (*(undefined4 *)0x00ea60a4u)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00ea623c (*(undefined4 *)0x00ea623cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
#define _DAT_00603322 (*(undefined4 *)0x00603322u)
#define _DAT_00603326 (*(undefined4 *)0x00603326u)
extern void FUN_004256a0();
extern int FUN_00425c40();
extern void FUN_00425e80();
extern undefined4 FUN_00443950();
extern undefined4 FUN_004439c0();
extern undefined4 FUN_00443a20();
extern undefined4 FUN_00443b20();
extern void FUN_004ccc30();
extern void FUN_004cce90();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern int FUN_0053e590();
extern int FUN_00559fc0();
extern void FUN_0055a060();

void FUN_0055a0e0(void)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  int local_4c;
  int *local_48;
  int local_44;
  int local_40 [16];
  
  piVar5 = &DAT_00ea60a4 + DAT_00eaac28 * 0x1276;
  local_40[0] = 0;
  local_40[1] = 0;
  local_40[2] = 0;
  local_40[3] = 0;
  local_40[4] = 0;
  local_40[5] = 0;
  local_40[6] = 0;
  local_40[7] = 0;
  local_40[8] = 0;
  local_40[9] = 0;
  local_40[10] = 0;
  local_40[0xb] = 0;
  local_40[0xc] = 0;
  local_40[0xd] = 0;
  local_40[0xe] = 0;
  local_40[0xf] = 0;
  if ((DAT_00f53e6c - (&DAT_00ea60a4)[DAT_00eaac28 * 0x1276]) + 1 < 0xf) {
    *piVar5 = DAT_00f53e6c + -0xe;
  }
  if (*piVar5 < 1) {
    *piVar5 = 1;
  }
  local_44 = *piVar5;
  if (5 < DAT_00f53f48) {
    FUN_004d65b0(DAT_00f53f0c + 0x10,DAT_00f53f08 + 0x34,0x24,0x15,DAT_00e38da8,DAT_00e38da4);
    if (0xe < DAT_00f53e6c) {
      FUN_00425e80(DAT_00f53f0c + 0x23d,DAT_00f53f08 + 0x34,&DAT_005c5b90,1,10);
      FUN_00425c40(DAT_00f53f0c + 0x23d,DAT_00f53f08 + 0x34,&DAT_005c5b90,1);
      FUN_004256a0(DAT_00f53f0c + 0x23d,DAT_00f53f08 + 0x34,&DAT_005c5b90,1);
      FUN_00425e80(DAT_00f53f0c + 0x255,DAT_00f53f08 + 0x16c,&DAT_005c5b78,1,10);
      FUN_00425c40(DAT_00f53f0c + 0x255,DAT_00f53f08 + 0x16c,&DAT_005c5b78,1);
      FUN_004256a0(DAT_00f53f0c + 0x255,DAT_00f53f08 + 0x16c,&DAT_005c5b78,1);
      FUN_004d65b0(DAT_00f53f0c + 0x251,DAT_00f53f08 + 0x4c,2,0x12,DAT_00e38da8,DAT_00e38da4);
    }
    local_48 = &DAT_00f53e6c + local_44;
    _DAT_00603322 = 0;
    _DAT_00603326 = 0;
    local_4c = 0;
    iVar6 = 0;
    do {
      iVar3 = *local_48;
      FUN_0053e590(DAT_00f571fc + iVar3,DAT_00f53f0c + 0x16,iVar6 + 0x39 + DAT_00f53f08);
      if (local_4c == DAT_007056c8 + -4) {
        FUN_004d63d0(0,DAT_00f53f0c + 0x14,iVar6 + 0x36 + DAT_00f53f08,0x23a,0x18,1,DAT_00e38da8,
                     DAT_00e38da4);
      }
      uVar8 = DAT_00f56fa4;
      if (*(short *)(&DAT_00ea4126 + (DAT_00eaac28 * 0x24ec + iVar3) * 2) != 1) {
        uVar8 = DAT_00f56f9c;
      }
      FUN_004d62f0(0x17,iVar3,DAT_00f53f0c + 0x2c,iVar6 + 0x3d + DAT_00f53f08,uVar8,0,DAT_00e38da8,
                   DAT_00e38da4);
      if ((&DAT_00ea623c)[DAT_00eaac28 * 0x49d8] == '\0') {
        iVar2 = (&DAT_00ea6160)[DAT_00eaac28 * 0x1276 + iVar3];
        switch((int)(iVar3)) {
        case 0x18: ;
        case 0x19: ;
        case 0x1a: ;
        case 0x1e: ;
        case 0x23: ;
switchD_0055a3db_caseD_18: ;
          iVar2 = iVar2 / 100;
          break;
default: ;
          if (((iVar3 == 0x1c) || (iVar3 == 10)) || (iVar3 == 0x24)) goto switchD_0055a3db_caseD_18;
        }
        FUN_004cce90(iVar2,0x40,&DAT_0057c048,DAT_00f53f0c + 0x8c,iVar6 + 0x3d + DAT_00f53f08,100,
                     DAT_00f56f9c,0);
        if (*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + iVar3) * 4) != 0) {
          FUN_004d62f0(0x36,3,DAT_00f53f0c + 0xe8,iVar6 + 0x3d + DAT_00f53f08,DAT_00f56f9c,0,
                       DAT_00e38da8,DAT_00e38da4);
        }
        uVar8 = DAT_00f56f9c;
        sVar1 = *(short *)(&DAT_00ea4096 + (DAT_00eaac28 * 0x24ec + iVar3) * 2);
        if (sVar1 == 0) {
          iVar2 = FUN_00443950(iVar3);
          if ((iVar2 == 0) || (iVar2 = FUN_00443b20(iVar3), iVar2 == 0)) {
            iVar2 = FUN_00443950(iVar3);
            if (iVar2 == 0) {
              iVar2 = FUN_00443b20(iVar3);
              if (iVar2 == 0) {
                iVar2 = FUN_00443a20(iVar3);
                if ((iVar2 == 0) || (iVar2 = FUN_004439c0(iVar3), iVar2 == 0)) {
                  iVar2 = FUN_004439c0(iVar3);
                  if (iVar2 == 0) {
                    iVar3 = FUN_00443a20(iVar3);
                    if (iVar3 == 0) goto LAB_0055ab2f;
                    iVar3 = 0x23;
                    uVar8 = DAT_00f56f94;
                  }
                  else {
                    iVar3 = 0x22;
                    uVar8 = DAT_00f56f94;
                  }
                }
                else {
                  iVar3 = 0x24;
                  uVar8 = DAT_00f56f94;
                }
              }
              else {
                iVar3 = 0x20;
              }
            }
            else {
              iVar3 = 0x1f;
            }
          }
          else {
            iVar3 = 0x21;
          }
          iVar2 = iVar6 + 0x3d + DAT_00f53f08;
          iVar4 = DAT_00f53f0c + 0x144;
          goto LAB_0055ab25;
        }
        if (sVar1 == 1) {
          DAT_00e20778 = 0;
          if (iVar3 == 0x13) {
            DAT_00e20778 = 0;
            iVar3 = FUN_00559fc0(local_40);
            FUN_004d62f0(0x36,5,DAT_00f53f0c + 0x144,iVar6 + 0x3d + DAT_00f53f08,DAT_00f56f9c,0,
                         DAT_00e38da8,DAT_00e38da4);
            FUN_004d6860(*(short *)(&DAT_00ea4104 + DAT_00eaac28 * 0x49d8) * 100,0x40,&DAT_0057c048,
                         DAT_00e20778 + 0x144 + DAT_00f53f0c,iVar6 + 0x3d + DAT_00f53f08,
                         DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
            uVar8 = DAT_00f56f9c;
            if (iVar3 == 1) {
              iVar3 = 0;
              piVar5 = local_40;
              do {
                if (*piVar5 != 0) {
                  iVar2 = iVar6 + 0x3d + DAT_00f53f08;
                  iVar3 = iVar3 + 0x27;
                  iVar4 = DAT_00e20778 + 0x144 + DAT_00f53f0c;
                  uVar7 = 0x17;
                  goto LAB_0055ab27;
                }
                iVar3 = iVar3 + 1;
                piVar5 = piVar5 + 1;
              } while (iVar3 < 0x10);
            }
            else if (iVar3 != 0) {
              _DAT_00603326 = (short)local_4c * 0x16;
              _DAT_00603322 = _DAT_00603326 + 0x38;
              _DAT_00603326 = _DAT_00603326 + 0x4c;
              iVar2 = iVar6 + 0x3d + DAT_00f53f08;
              iVar4 = DAT_00e20778 + 0x144 + DAT_00f53f0c;
              iVar3 = 0x2d;
              goto LAB_0055ab25;
            }
            goto LAB_0055ab2f;
          }
          FUN_004d62f0(0x36,5,DAT_00f53f0c + 0x144,iVar6 + 0x3d + DAT_00f53f08,DAT_00f56f9c,0,
                       DAT_00e38da8,DAT_00e38da4);
          iVar2 = *(short *)(&DAT_00ea40de + (DAT_00eaac28 * 0x24ec + iVar3) * 2) * 100;
          switch((int)(iVar3)) {
          case 0x18: ;
          case 0x19: ;
          case 0x1a: ;
          case 0x1e: ;
          case 0x23: ;
switchD_0055a751_caseD_18: ;
            iVar2 = iVar2 / 100;
            break;
default: ;
            if (((iVar3 == 0x1c) || (iVar3 == 10)) || (iVar3 == 0x24))
            goto switchD_0055a751_caseD_18;
          }
          switch((int)(iVar3)) {
          case 0x18: ;
          case 0x19: ;
          case 0x1a: ;
          case 0x1e: ;
          case 0x23: ;
            FUN_004ccc30(8,0x38,iVar2,DAT_00e20778 + 0x144 + DAT_00f53f0c,
                         iVar6 + 0x3d + DAT_00f53f08,DAT_00f56f9c,0);
            break;
default: ;
            iVar3 = iVar6 + 0x3d + DAT_00f53f08;
            iVar4 = DAT_00e20778 + 0x144 + DAT_00f53f0c;
LAB_0055a8c7: ;
            FUN_004d6860(iVar2,0x40,&DAT_0057c048,iVar4,iVar3,DAT_00f56f9c,0,DAT_00e38da8,
                         DAT_00e38da4);
          }
        }
        else {
          if (sVar1 == 2) {
            DAT_00e20778 = 0;
            FUN_004d62f0(0x36,6,DAT_00f53f0c + 0x144,iVar6 + 0x3d + DAT_00f53f08,DAT_00f56f9c,0,
                         DAT_00e38da8,DAT_00e38da4);
            iVar2 = *(short *)(&DAT_00ea40de + (DAT_00eaac28 * 0x24ec + iVar3) * 2) * 100;
            switch((int)(iVar3)) {
            case 0x18: ;
            case 0x19: ;
            case 0x1a: ;
            case 0x1e: ;
            case 0x23: ;
switchD_0055a86b_caseD_18: ;
              iVar2 = iVar2 / 100;
              break;
default: ;
              if (((iVar3 == 0x1c) || (iVar3 == 10)) || (iVar3 == 0x24))
              goto switchD_0055a86b_caseD_18;
            }
            iVar3 = iVar6 + 0x3d + DAT_00f53f08;
            iVar4 = DAT_00e20778 + 0x144 + DAT_00f53f0c;
            goto LAB_0055a8c7;
          }
          if (sVar1 != 3) {
            iVar4 = DAT_00f53f0c + 0x144;
            iVar2 = iVar6 + 0x3d + DAT_00f53f08;
            iVar3 = 0x26;
            goto LAB_0055ab25;
          }
          if (iVar3 != 0x13) {
            iVar4 = DAT_00f53f0c + 0x144;
            iVar2 = iVar6 + 0x3d + DAT_00f53f08;
            iVar3 = 0x25;
            goto LAB_0055ab25;
          }
          DAT_00e20778 = 0;
          iVar3 = FUN_00559fc0(local_40);
          FUN_004d62f0(0x36,0x25,DAT_00f53f0c + 0x144,iVar6 + 0x3d + DAT_00f53f08,DAT_00f56f9c,0,
                       DAT_00e38da8,DAT_00e38da4);
          uVar8 = DAT_00f56f9c;
          if (iVar3 == 1) {
            iVar3 = 0;
            piVar5 = local_40;
            do {
              if (*piVar5 != 0) {
                iVar2 = iVar6 + 0x3d + DAT_00f53f08;
                iVar3 = iVar3 + 0x27;
                iVar4 = DAT_00e20778 + 0x144 + DAT_00f53f0c;
                uVar7 = 0x17;
                goto LAB_0055ab27;
              }
              iVar3 = iVar3 + 1;
              piVar5 = piVar5 + 1;
            } while (iVar3 < 0x10);
          }
          else if (iVar3 != 0) {
            _DAT_00603326 = (short)local_4c * 0x16;
            _DAT_00603322 = _DAT_00603326 + 0x38;
            _DAT_00603326 = _DAT_00603326 + 0x4c;
            iVar2 = iVar6 + 0x3d + DAT_00f53f08;
            iVar4 = DAT_00e20778 + 0x144 + DAT_00f53f0c;
            iVar3 = 0x2d;
            goto LAB_0055ab25;
          }
        }
      }
      else {
        FUN_004cce90(*(undefined4 *)(&DAT_005d2c48 + iVar3 * 8),0x40,&DAT_0057c048,
                     DAT_00f53f0c + 0xaf,iVar6 + 0x3d + DAT_00f53f08,100,DAT_00f56f9c,0);
        FUN_004cce90(*(undefined4 *)(&DAT_005d2c4c + iVar3 * 8),0x40,&DAT_0057c048,
                     DAT_00f53f0c + 0x113,iVar6 + 0x3d + DAT_00f53f08,100,DAT_00f56f9c,0);
        switch((int)(iVar3)) {
        case 0x18: ;
        case 0x19: ;
        case 0x1a: ;
        case 0x1e: ;
        case 0x23: ;
          iVar3 = 0x31;
          break;
default: ;
          if (iVar3 == 10) {
            iVar3 = 0x2f;
          }
          else {
            iVar3 = (-(uint)(iVar3 != 0x1c) & 2) + 0x30;
          }
        }
        iVar2 = iVar6 + 0x3d + DAT_00f53f08;
        iVar4 = DAT_00f53f0c + 0x1a9;
        uVar8 = DAT_00f56f9c;
LAB_0055ab25: ;
        uVar7 = 0x36;
LAB_0055ab27: ;
        FUN_004d62f0(uVar7,iVar3,iVar4,iVar2,uVar8,0,DAT_00e38da8,DAT_00e38da4);
      }
LAB_0055ab2f: ;
      local_44 = local_44 + 1;
      local_48 = local_48 + 1;
      if (DAT_00f53e6c < local_44) break;
      iVar6 = iVar6 + 0x16;
      local_4c = local_4c + 1;
    } while (iVar6 < 0x14a);
    FUN_004d63d0(0,DAT_00f53f0c + 0x18e,DAT_00f53f08 + 0x18c,200,0x18,DAT_007056c8 == 1,DAT_00e38da8
                 ,DAT_00e38da4);
    if ((&DAT_00ea623c)[DAT_00eaac28 * 0x49d8] == '\0') {
      uVar8 = 2;
    }
    else {
      uVar8 = 0x2e;
    }
    FUN_004d6250(0x36,uVar8,DAT_00f53f0c + 400,DAT_00f53f08 + 0x192,200,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d63d0(0,DAT_00f53f0c + 0x62,DAT_00f53f08 + 0x18c,200,0x18,DAT_007056c8 == 2,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6250(0x36,0x1e,DAT_00f53f0c + 100,DAT_00f53f08 + 0x192,200,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_0055a060();
  }
  return;
}

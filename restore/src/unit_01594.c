#include "restore_types.h"
#define DAT_00705af0 (*(undefined4 *)0x00705af0u)
#define DAT_0078d64c (*(undefined4 *)0x0078d64cu)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
extern int FUN_0045fe20();
extern uint FUN_00562348();

undefined4 FUN_004c3ac0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int *piVar5;
  int iVar6;
  short *psVar7;
  int local_18 [6];
  
  iVar6 = 0;
  local_18[0] = 0x4a;
  local_18[1] = 0x6f;
  local_18[2] = 0x71;
  local_18[3] = 0xffffffff;
  local_18[4] = 0x11;
  local_18[5] = 0x13;
  iVar1 = 0;
  do {
    if ((int)(&DAT_00705af0)[*(int *)((int)local_18 + iVar1)] < 1) {
      iVar2 = *(int *)((int)local_18 + iVar1 + 0xc);
      if ((iVar2 < 1) || ((int)(&DAT_0078d64c)[iVar2] < 1)) {
        *(undefined4 *)((int)local_18 + iVar1) = 0;
      }
      else {
        iVar6 = iVar6 + 1;
      }
    }
    else {
      iVar6 = iVar6 + 1;
    }
    iVar1 = iVar1 + 4;
  } while (iVar1 < 0xc);
  iVar1 = 0;
  if (iVar6 == 0) {
    return 0;
  }
  iVar2 = FUN_00562348();
  iVar3 = 0;
  piVar5 = local_18;
  do {
    if (*piVar5 != 0) {
      if (iVar1 == iVar2 % iVar6) {
        iVar1 = local_18[iVar3];
        if (iVar1 == -1) {
          return 0;
        }
        iVar6 = 1;
        psVar7 = &DAT_00799af0;
        do {
          if (((char)psVar7[-8] == '\x01') && (iVar2 = (int)*psVar7, iVar2 == iVar1)) {
            switch((int)(iVar2)) {
            case 0x20: ;
            case 0x33: ;
              iVar3 = 300;
              break;
default: ;
              iVar3 = 200;
              break;
            case 0x2f: ;
            case 0x35: ;
              iVar3 = 500;
              break;
            case 0x54: ;
            case 0x55: ;
            case 0x56: ;
            case 0x57: ;
            case 0xb8: ;
            case 0xbb: ;
            case 0xbc: ;
            case 0xbd: ;
              iVar3 = 100000;
            }
            if (iVar3 - psVar7[0x16] < 0) {
              iVar2 = 0;
            }
            else {
              switch((int)(iVar2)) {
              case 0x20: ;
              case 0x33: ;
                iVar2 = 300;
                break;
default: ;
                iVar2 = 200;
                break;
              case 0x2f: ;
              case 0x35: ;
                iVar2 = 500;
                break;
              case 0x54: ;
              case 0x55: ;
              case 0x56: ;
              case 0x57: ;
              case 0xb8: ;
              case 0xbb: ;
              case 0xbc: ;
              case 0xbd: ;
                iVar2 = 100000;
              }
              iVar2 = iVar2 - psVar7[0x16];
            }
            switch(psVar7[1]) {
            case 1: ;
              uVar4 = 0x10;
              break;
default: ;
              uVar4 = 0xffffffff;
              break;
            case 3: ;
              uVar4 = 0x18;
              break;
            case 4: ;
              uVar4 = 0x14;
              break;
            case 5: ;
              uVar4 = 0xb;
              break;
            case 6: ;
              uVar4 = 0x19;
              break;
            case 7: ;
              uVar4 = 0xc;
              break;
            case 8: ;
              uVar4 = 10;
              break;
            case 9: ;
              uVar4 = 0xe;
              break;
            case 10: ;
              uVar4 = 0x12;
              break;
            case 0xb: ;
              uVar4 = 0x16;
              break;
            case 0xc: ;
              uVar4 = 0x17;
              break;
            case 0xd: ;
              uVar4 = 9;
              break;
            case 0xe: ;
              uVar4 = 0x1c;
              break;
            case 0xf: ;
              uVar4 = 0x15;
              break;
            case 0x10: ;
              uVar4 = 0x11;
              break;
            case 0x11: ;
              uVar4 = 0x1d;
              break;
            case 0x12: ;
              uVar4 = 0x1e;
              break;
            case 0x14: ;
              uVar4 = 0x20;
              break;
            case 0x15: ;
              uVar4 = 2;
              break;
            case 0x16: ;
              uVar4 = 8;
              break;
            case 0x17: ;
              uVar4 = 0x1f;
              break;
            case 0x18: ;
              uVar4 = 0xd;
              break;
            case 0x19: ;
              uVar4 = 0x21;
              break;
            case 0x1a: ;
              uVar4 = 0x22;
            }
            FUN_0045fe20(iVar6,iVar2,uVar4);
          }
          psVar7 = psVar7 + 0x84;
          iVar6 = iVar6 + 1;
        } while ((int)psVar7 < 0x89b6e8);
        return 1;
      }
      iVar1 = iVar1 + 1;
    }
    iVar3 = iVar3 + 1;
    piVar5 = piVar5 + 1;
  } while (iVar3 < 3);
  return 0;
}

#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799ae3 (*(undefined4 *)0x00799ae3u)
#define DAT_00ea54d4 (*(undefined4 *)0x00ea54d4u)
#define DAT_00ea5574 (*(undefined4 *)0x00ea5574u)
extern undefined4 FUN_00480180();

void FUN_0045e6e0(int param_1)

{
  short sVar1;
  undefined2 uVar2;
  short sVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined1 *puVar8;
  
  iVar7 = 0x1b;
  puVar4 = &DAT_00ea5574 + param_1 * 0x1276;
  do {
    puVar4[-0x28] = 0;
    *puVar4 = 0;
    puVar4 = puVar4 + 1;
    iVar7 = iVar7 + -1;
  } while (iVar7 != 0);
  iVar7 = 1;
  if (0 < DAT_0078d790) {
    puVar8 = &DAT_00799ae3;
    do {
      if (puVar8[-3] == '\x01') {
        sVar1 = *(short *)(puVar8 + 0xd);
        if ((sVar1 < 0x6e) || (0x72 < sVar1)) {
          switch((int)(sVar1)) {
          case 0x20: ;
          case 0x2f: ;
          case 0x33: ;
          case 0x35: ;
          case 0x4a: ;
          case 0x71: ;
          case 0xb1: ;
          case 0xb2: ;
          case 0xbb: ;
          case 0xbc: ;
          case 0xbd: ;
          case 0xc2: ;
          case 0xcb: ;
          case 0xcc: ;
          case 0xcd: ;
          case 0xe2: ;
          case 0xe7: ;
          case 0xe8: ;
          case 0xe9: ;
            break;
default: ;
            goto switchD_0045e758_caseD_21;
          }
        }
        puVar8[0x41] = 0;
        iVar5 = FUN_00480180((int)*(short *)(puVar8 + 3),(int)*(short *)(puVar8 + 5),*puVar8);
        if (iVar5 != 0) {
          puVar8[0x41] = 1;
          uVar2 = *(undefined2 *)(puVar8 + 0xd);
          switch((int)(uVar2)) {
          case 0x20: ;
          case 0x33: ;
            iVar5 = 300;
            break;
default: ;
            iVar5 = 200;
            break;
          case 0x2f: ;
          case 0x35: ;
            iVar5 = 500;
            break;
          case 0x54: ;
          case 0x55: ;
          case 0x56: ;
          case 0x57: ;
          case 0xb8: ;
          case 0xbb: ;
          case 0xbc: ;
          case 0xbd: ;
            iVar5 = 100000;
          }
          sVar1 = *(short *)(puVar8 + 0x39);
          iVar5 = iVar5 - sVar1;
          if (iVar5 < 0) {
            iVar5 = 0;
          }
          iVar6 = (int)*(short *)(puVar8 + 0xf) + param_1 * 0x1276;
          (&DAT_00ea5574)[iVar6] = (&DAT_00ea5574)[iVar6] + iVar5;
          sVar3 = *(short *)(puVar8 + 0xcb);
          (&DAT_00ea54d4)[iVar6] = (&DAT_00ea54d4)[iVar6] + (int)sVar1;
          if (sVar3 != 0) {
            switch((int)(uVar2)) {
            case 0x20: ;
            case 0x33: ;
              iVar5 = 300;
              break;
default: ;
              iVar5 = 200;
              break;
            case 0x2f: ;
            case 0x35: ;
              iVar5 = 500;
              break;
            case 0x54: ;
            case 0x55: ;
            case 0x56: ;
            case 0x57: ;
            case 0xb8: ;
            case 0xbb: ;
            case 0xbc: ;
            case 0xbd: ;
              iVar5 = 100000;
            }
            sVar1 = *(short *)(puVar8 + 0xc9);
            iVar5 = iVar5 - sVar1;
            if (iVar5 < 0) {
              iVar5 = 0;
            }
            iVar6 = (int)sVar3 + param_1 * 0x1276;
            (&DAT_00ea5574)[iVar6] = (&DAT_00ea5574)[iVar6] + iVar5;
            (&DAT_00ea54d4)[iVar6] = (&DAT_00ea54d4)[iVar6] + (int)sVar1;
          }
        }
      }
switchD_0045e758_caseD_21: ;
      iVar7 = iVar7 + 1;
      puVar8 = puVar8 + 0x108;
    } while (iVar7 <= DAT_0078d790);
  }
  return;
}

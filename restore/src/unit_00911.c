#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)
#define DAT_00799adc (*(undefined4 *)0x00799adcu)
#define DAT_00799ade (*(undefined4 *)0x00799adeu)
#define DAT_00799b24 (*(undefined4 *)0x00799b24u)
#define DAT_0096e6b0 (*(undefined4 *)0x0096e6b0u)
#define DAT_0096e6b4 (*(undefined4 *)0x0096e6b4u)
#define DAT_00972530 (*(undefined4 *)0x00972530u)
#define DAT_00972534 (*(undefined4 *)0x00972534u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ea42f8 (*(undefined4 *)0x00ea42f8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_00459240();
extern int FUN_004b28f0();

int FUN_0045eab0(undefined4 param_1,int param_2,uint param_3)

{
  short sVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  short sVar6;
  int iVar7;
  short sVar8;
  char *pcVar9;
  int local_4;
  
  if (*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + param_2) * 4) == 0) {
    switch((int)(param_2)) {
    case 2: ;
      sVar2 = 0x15;
      break;
default: ;
      goto switchD_0045eae4_caseD_3;
    case 8: ;
      sVar2 = 0x16;
      break;
    case 9: ;
      sVar2 = 0xd;
      break;
    case 10: ;
      sVar2 = 8;
      break;
    case 0xb: ;
      sVar2 = 5;
      break;
    case 0xc: ;
      sVar2 = 7;
      break;
    case 0xd: ;
      sVar2 = 0x18;
      break;
    case 0xe: ;
      sVar2 = 9;
      break;
    case 0xf: ;
      sVar2 = 0x13;
      break;
    case 0x10: ;
      sVar2 = 1;
      break;
    case 0x11: ;
      sVar2 = 0x10;
      break;
    case 0x12: ;
      sVar2 = 10;
      break;
    case 0x14: ;
      sVar2 = 4;
      break;
    case 0x15: ;
      sVar2 = 0xf;
      break;
    case 0x16: ;
      sVar2 = 0xb;
      break;
    case 0x17: ;
      sVar2 = 0xc;
      break;
    case 0x18: ;
      sVar2 = 3;
      break;
    case 0x19: ;
      sVar2 = 6;
      break;
    case 0x1c: ;
      sVar2 = 0xe;
      break;
    case 0x1d: ;
      sVar2 = 0x11;
      break;
    case 0x1e: ;
      sVar2 = 0x12;
      break;
    case 0x1f: ;
      sVar2 = 0x17;
      break;
    case 0x20: ;
      sVar2 = 0x14;
      break;
    case 0x21: ;
      sVar2 = 0x19;
      break;
    case 0x22: ;
      sVar2 = 0x1a;
    }
    iVar3 = 0;
    sVar6 = 10000;
    DAT_0096e6b0 = 0;
    iVar7 = 10000;
    local_4 = 1;
    if (0 < DAT_0078d790) {
      pcVar9 = &DAT_00799b24;
      do {
        if (pcVar9[-0x44] == '\x01') {
          iVar3 = FUN_00459240(local_4);
          iVar4 = DAT_00bca2d8;
          if (iVar3 == 0) {
            sVar1 = *(short *)(pcVar9 + -0x34);
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
                goto switchD_0045ec70_caseD_21;
              }
            }
            if (((((*pcVar9 != '\0') && (0 < *(short *)(pcVar9 + -0x24))) &&
                 ((*(short *)(pcVar9 + -0x32) == sVar2 || (*(short *)(pcVar9 + 0x8a) == sVar2)))) &&
                ((byte)pcVar9[-0x30] == param_3)) &&
               ((((sVar1 != 0xbb && (sVar1 != 0xbc)) && (sVar1 != 0xbd)) ||
                (*(short *)(pcVar9 + -4) != 0)))) {
              if ((*(short *)(pcVar9 + 0x8a) == 0) || (*(short *)(pcVar9 + 0x8a) != sVar2)) {
                switch((int)(sVar1)) {
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
                iVar5 = (int)*(short *)(pcVar9 + -8);
                if (iVar5 + 100 <= iVar3) {
                  sVar8 = *(short *)(pcVar9 + 0xc0);
                  if (0 < sVar8) {
                    switch((int)(sVar1)) {
                    case 0x20: ;
                    case 0x33: ;
                      goto switchD_0045ed5c_caseD_20;
default: ;
                      goto switchD_0045ed5c_caseD_21;
                    case 0x2f: ;
                    case 0x35: ;
                      goto switchD_0045ed5c_caseD_2f;
                    case 0x54: ;
                    case 0x55: ;
                    case 0x56: ;
                    case 0x57: ;
                    case 0xb8: ;
                    case 0xbb: ;
                    case 0xbc: ;
                    case 0xbd: ;
                      goto switchD_0045ed5c_caseD_54;
                    }
                  }
                  goto LAB_0045edfc;
                }
              }
              else {
                switch((int)(sVar1)) {
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
                iVar5 = (int)*(short *)(pcVar9 + 0x88);
                if (iVar5 + 100 <= iVar3) {
                  sVar8 = *(short *)(pcVar9 + 0xc2);
                  if (0 < sVar8) {
                    switch((int)(sVar1)) {
                    case 0x20: ;
                    case 0x33: ;
switchD_0045ed5c_caseD_20: ;
                      iVar3 = 300;
                      break;
default: ;
switchD_0045ed5c_caseD_21: ;
                      iVar3 = 200;
                      break;
                    case 0x2f: ;
                    case 0x35: ;
switchD_0045ed5c_caseD_2f: ;
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
switchD_0045ed5c_caseD_54: ;
                      iVar3 = 100000;
                    }
                    if ((iVar3 + sVar8 * -100) - iVar5 < 1) goto switchD_0045ec70_caseD_21;
                  }
LAB_0045edfc: ;
                  if ((sVar8 <= sVar6) && (iVar5 <= iVar7)) {
                    if (iVar5 < iVar7) {
                      DAT_0096e6b0 = 0;
                      iVar7 = iVar5;
                      sVar6 = sVar8;
                    }
                    sVar1 = *(short *)(pcVar9 + -0x1c);
                    (&DAT_00972534)[DAT_0096e6b0] = local_4;
                    iVar3 = DAT_0096e6b0 + 1;
                    DAT_0096e6b0 = iVar3;
                    (&DAT_0096e6b0)[iVar3] = iVar4 + *(short *)(pcVar9 + -0x1e) + sVar1 * 0xe4;
                    goto LAB_0045ee51;
                  }
                }
              }
            }
          }
switchD_0045ec70_caseD_21: ;
          iVar3 = DAT_0096e6b0;
        }
LAB_0045ee51: ;
        local_4 = local_4 + 1;
        pcVar9 = pcVar9 + 0x108;
      } while (local_4 <= DAT_0078d790);
    }
    iVar3 = FUN_004b28f0(param_1,&DAT_0096e6b4,iVar3);
    if (iVar3 == 0) {
      iVar3 = 0;
    }
    else {
      iVar3 = (&DAT_00972530)[iVar3];
    }
    iVar7 = iVar3 * 0x108;
    _DAT_00d3b4e4 = (int)(short)(&DAT_007999fe)[iVar3 * 0x84];
    _DAT_00d3b4e0 = (int)(short)(&DAT_00799a00)[iVar3 * 0x84];
    if (iVar3 != 0) {
      if ((&DAT_007999e8)[iVar3 * 0x84] == 0xcc) {
        switch(*(undefined2 *)((int)&DAT_00799aa4 + iVar7 + 2)) {
        case 1: ;
          iVar4 = 0x10;
          break;
default: ;
          iVar4 = -1;
          break;
        case 3: ;
          iVar4 = 0x18;
          break;
        case 4: ;
          iVar4 = 0x14;
          break;
        case 5: ;
          iVar4 = 0xb;
          break;
        case 6: ;
          iVar4 = 0x19;
          break;
        case 7: ;
          iVar4 = 0xc;
          break;
        case 8: ;
          iVar4 = 10;
          break;
        case 9: ;
          iVar4 = 0xe;
          break;
        case 10: ;
          iVar4 = 0x12;
          break;
        case 0xb: ;
          iVar4 = 0x16;
          break;
        case 0xc: ;
          iVar4 = 0x17;
          break;
        case 0xd: ;
          iVar4 = 9;
          break;
        case 0xe: ;
          iVar4 = 0x1c;
          break;
        case 0xf: ;
          iVar4 = 0x15;
          break;
        case 0x10: ;
          iVar4 = 0x11;
          break;
        case 0x11: ;
          iVar4 = 0x1d;
          break;
        case 0x12: ;
          iVar4 = 0x1e;
          break;
        case 0x14: ;
          iVar4 = 0x20;
          break;
        case 0x15: ;
          iVar4 = 2;
          break;
        case 0x16: ;
          iVar4 = 8;
          break;
        case 0x17: ;
          iVar4 = 0x1f;
          break;
        case 0x18: ;
          iVar4 = 0xd;
          break;
        case 0x19: ;
          iVar4 = 0x21;
          break;
        case 0x1a: ;
          iVar4 = 0x22;
        }
        if (param_2 == iVar4) {
          *(short *)(&DAT_00799ade + iVar7) = *(short *)(&DAT_00799ade + iVar7) + 1;
          return iVar3;
        }
      }
      *(short *)(&DAT_00799adc + iVar7) = *(short *)(&DAT_00799adc + iVar7) + 1;
    }
    return iVar3;
  }
switchD_0045eae4_caseD_3: ;
  return 0;
}

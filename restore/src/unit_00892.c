#include "restore_types.h"
#define DAT_007058b8 (*(undefined *)0x007058b8u)
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_00790134 (*(undefined4 *)0x00790134u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999ec (*(undefined4 *)0x007999ecu)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)
#define DAT_00799adc (*(undefined4 *)0x00799adcu)
#define DAT_00799ade (*(undefined4 *)0x00799adeu)
#define DAT_00799b20 (*(undefined4 *)0x00799b20u)
#define DAT_00a76aec (*(undefined4 *)0x00a76aecu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcb9b4 (*(undefined *)0x00bcb9b4u)
#define DAT_00ea42f8 (*(undefined4 *)0x00ea42f8u)
#define DAT_00ea5574 (*(undefined4 *)0x00ea5574u)
#define DAT_00ea5d1c (*(undefined4 *)0x00ea5d1cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_0078d58c (*(undefined4 *)0x0078d58cu)
extern undefined4 FUN_00459240();
extern int FUN_0045d510();
extern int FUN_004cde60();

int FUN_0045bf00(int param_1,int param_2)

{
  char cVar1;
  short sVar2;
  short sVar3;
  undefined2 uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  short *psVar11;
  int local_c;
  
  uVar5 = (uint)*(byte *)((int)&DAT_00799a98 + param_1 * 0x108 + 1);
  cVar1 = (&DAT_00a76aec)
          [(short)(&DAT_00799a00)[param_1 * 0x84] * 0xe4 +
           DAT_00bca2d8 + (short)(&DAT_007999fe)[param_1 * 0x84]];
  iVar6 = FUN_004cde60((int)(short)(&DAT_00799a18)[param_1 * 0x84],
                       *(undefined4 *)
                        (&DAT_00705eb8 + (short)(&DAT_007999e8)[param_1 * 0x84] * 0x28));
  if (iVar6 < 0x32) {
    return -1;
  }
  iVar6 = DAT_00eaac28;
  if (param_2 != 0) {
    iVar10 = 1;
    do {
      if ((*(char *)((int)&DAT_007900c2 + iVar10 + uVar5 * 0xc4) == '\x02') &&
         (*(int *)(&DAT_00ea42f8 + (iVar6 * 0x1276 + iVar10) * 4) == 0)) {
        iVar6 = 8;
        do {
          iVar6 = iVar6 + -1;
        } while (iVar6 != 0);
        iVar6 = 8;
        do {
          iVar6 = iVar6 + -1;
        } while (iVar6 != 0);
        iVar7 = FUN_0045d510(param_1,iVar10);
        iVar6 = DAT_00eaac28;
        if (iVar7 <= *(short *)(&DAT_00790134 + (iVar10 + uVar5 * 0x62) * 2)) {
          _DAT_0078d58c = iVar10;
          return 0;
        }
      }
      iVar10 = iVar10 + 1;
    } while (iVar10 < 0x24);
  }
  if (((0 < DAT_007058b8) && (DAT_00bcb9b4 != 0)) &&
     (iVar10 = (&DAT_00ea5d1c)[iVar6 * 0x1276],
     (&DAT_007999ec)[param_1 * 0x108] == (&DAT_007999ec)[iVar10 * 0x108])) {
    iVar7 = iVar10 * 0x108;
    sVar2 = (&DAT_00799a14)[iVar10 * 0x84];
    sVar3 = *(short *)(&DAT_00799aa4 + iVar10 * 0x42);
    uVar4 = (&DAT_007999e8)[iVar10 * 0x84];
    if (sVar3 < sVar2) {
      switch((int)(uVar4)) {
      case 0x20: ;
      case 0x33: ;
        iVar10 = 300;
        break;
default: ;
        iVar10 = 200;
        break;
      case 0x2f: ;
      case 0x35: ;
        iVar10 = 500;
        break;
      case 0x54: ;
      case 0x55: ;
      case 0x56: ;
      case 0x57: ;
      case 0xb8: ;
      case 0xbb: ;
      case 0xbc: ;
      case 0xbd: ;
        iVar10 = 100000;
      }
      if ((int)sVar3 + *(short *)(&DAT_00799ade + iVar7) * 100 < iVar10) {
        iVar9 = 0;
        iVar10 = param_1;
        do {
          iVar10 = (int)*(short *)(&DAT_00799a12 + iVar10 * 0x108);
          if (((0 < iVar10) && (0 < (short)(&DAT_00799a14)[iVar10 * 0x84])) &&
             ((iVar8 = iVar6 * 0x1276, iVar6 = DAT_00eaac28,
              *(int *)(&DAT_00ea42f8 + (iVar8 + (short)(&DAT_007999ea)[iVar10 * 0x84]) * 4) == 0 &&
              ((&DAT_007999ea)[iVar10 * 0x84] == 0x1c)))) {
            return 0x1c;
          }
          iVar9 = iVar9 + 1;
        } while (iVar9 < 8);
      }
      switch((int)(uVar4)) {
      case 0x20: ;
      case 0x33: ;
        iVar10 = 300;
        break;
default: ;
        iVar10 = 200;
        break;
      case 0x2f: ;
      case 0x35: ;
        iVar10 = 500;
        break;
      case 0x54: ;
      case 0x55: ;
      case 0x56: ;
      case 0x57: ;
      case 0xb8: ;
      case 0xbb: ;
      case 0xbc: ;
      case 0xbd: ;
        iVar10 = 100000;
      }
      if ((int)sVar2 + *(short *)(&DAT_00799adc + iVar7) * 100 < iVar10) {
        iVar7 = 0;
        iVar10 = param_1;
        do {
          iVar10 = (int)*(short *)(&DAT_00799a12 + iVar10 * 0x108);
          if (((0 < iVar10) && (0 < (short)(&DAT_00799a14)[iVar10 * 0x84])) &&
             ((*(int *)(&DAT_00ea42f8 +
                       (iVar6 * 0x1276 + (int)(short)(&DAT_007999ea)[iVar10 * 0x84]) * 4) == 0 &&
              ((&DAT_007999ea)[iVar10 * 0x84] == 10)))) {
            return 10;
          }
          iVar7 = iVar7 + 1;
        } while (iVar7 < 8);
      }
    }
    else {
      switch((int)(uVar4)) {
      case 0x20: ;
      case 0x33: ;
        iVar10 = 300;
        break;
default: ;
        iVar10 = 200;
        break;
      case 0x2f: ;
      case 0x35: ;
        iVar10 = 500;
        break;
      case 0x54: ;
      case 0x55: ;
      case 0x56: ;
      case 0x57: ;
      case 0xb8: ;
      case 0xbb: ;
      case 0xbc: ;
      case 0xbd: ;
        iVar10 = 100000;
      }
      if ((int)sVar2 + *(short *)(&DAT_00799adc + iVar7) * 100 < iVar10) {
        iVar9 = 0;
        iVar10 = param_1;
        do {
          iVar10 = (int)*(short *)(&DAT_00799a12 + iVar10 * 0x108);
          if ((((0 < iVar10) && (0 < (short)(&DAT_00799a14)[iVar10 * 0x84])) &&
              (iVar8 = iVar6 * 0x1276, iVar6 = DAT_00eaac28,
              *(int *)(&DAT_00ea42f8 + (iVar8 + (short)(&DAT_007999ea)[iVar10 * 0x84]) * 4) == 0))
             && ((&DAT_007999ea)[iVar10 * 0x84] == 10)) {
            return 10;
          }
          iVar9 = iVar9 + 1;
        } while (iVar9 < 8);
      }
      switch((int)(uVar4)) {
      case 0x20: ;
      case 0x33: ;
        iVar10 = 300;
        break;
default: ;
        iVar10 = 200;
        break;
      case 0x2f: ;
      case 0x35: ;
        iVar10 = 500;
        break;
      case 0x54: ;
      case 0x55: ;
      case 0x56: ;
      case 0x57: ;
      case 0xb8: ;
      case 0xbb: ;
      case 0xbc: ;
      case 0xbd: ;
        iVar10 = 100000;
      }
      if ((int)sVar3 + *(short *)(&DAT_00799ade + iVar7) * 100 < iVar10) {
        iVar7 = 0;
        iVar10 = param_1;
        do {
          iVar10 = (int)*(short *)(&DAT_00799a12 + iVar10 * 0x108);
          if (((0 < iVar10) && (0 < (short)(&DAT_00799a14)[iVar10 * 0x84])) &&
             ((*(int *)(&DAT_00ea42f8 +
                       (iVar6 * 0x1276 + (int)(short)(&DAT_007999ea)[iVar10 * 0x84]) * 4) == 0 &&
              ((&DAT_007999ea)[iVar10 * 0x84] == 0x1c)))) {
            return 0x1c;
          }
          iVar7 = iVar7 + 1;
        } while (iVar7 < 8);
      }
    }
  }
  param_2 = 1;
  if (0 < DAT_0078d790) {
    psVar11 = &DAT_00799b20;
    do {
      if (((char)psVar11[-0x20] != '\0') && (*psVar11 != 0)) {
        sVar2 = psVar11[-0x18];
        if ((sVar2 < 0x6e) || (0x72 < sVar2)) {
          switch((int)(sVar2)) {
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
            goto switchD_0045c2dc_caseD_21;
          }
        }
        iVar6 = FUN_00459240(param_2);
        if (iVar6 == 0) {
          local_c = 0;
          iVar6 = param_1;
          do {
            iVar6 = (int)*(short *)(&DAT_00799a12 + iVar6 * 0x108);
            if ((0 < iVar6) && (0 < (short)(&DAT_00799a14)[iVar6 * 0x84])) {
              if ((*(int *)(&DAT_00ea42f8 +
                           ((int)(short)(&DAT_007999ea)[iVar6 * 0x84] + DAT_00eaac28 * 0x1276) * 4)
                   == 0) && ((char)psVar11[-0x16] == cVar1)) {
                switch((int)(short)(&DAT_007999ea)[iVar6 * 0x84]) {
                case 2: ;
                  iVar10 = 0x15;
                  break;
default: ;
                  goto switchD_0045c370_caseD_3;
                case 8: ;
                  iVar10 = 0x16;
                  break;
                case 9: ;
                  iVar10 = 0xd;
                  break;
                case 10: ;
                  iVar10 = 8;
                  break;
                case 0xb: ;
                  iVar10 = 5;
                  break;
                case 0xc: ;
                  iVar10 = 7;
                  break;
                case 0xd: ;
                  iVar10 = 0x18;
                  break;
                case 0xe: ;
                  iVar10 = 9;
                  break;
                case 0xf: ;
                  iVar10 = 0x13;
                  break;
                case 0x10: ;
                  iVar10 = 1;
                  break;
                case 0x11: ;
                  iVar10 = 0x10;
                  break;
                case 0x12: ;
                  iVar10 = 10;
                  break;
                case 0x14: ;
                  iVar10 = 4;
                  break;
                case 0x15: ;
                  iVar10 = 0xf;
                  break;
                case 0x16: ;
                  iVar10 = 0xb;
                  break;
                case 0x17: ;
                  iVar10 = 0xc;
                  break;
                case 0x18: ;
                  iVar10 = 3;
                  break;
                case 0x19: ;
                  iVar10 = 6;
                  break;
                case 0x1c: ;
                  iVar10 = 0xe;
                  break;
                case 0x1d: ;
                  iVar10 = 0x11;
                  break;
                case 0x1e: ;
                  iVar10 = 0x12;
                  break;
                case 0x1f: ;
                  iVar10 = 0x17;
                  break;
                case 0x20: ;
                  iVar10 = 0x14;
                  break;
                case 0x21: ;
                  iVar10 = 0x19;
                  break;
                case 0x22: ;
                  iVar10 = 0x1a;
                }
                if (0 < (int)(&DAT_00ea5574)[DAT_00eaac28 * 0x1276 + iVar10]) {
                  if (psVar11[-0x17] == iVar10) {
                    switch(psVar11[-0x18]) {
                    case 0x20: ;
                    case 0x33: ;
                      iVar10 = 300;
                      break;
default: ;
                      iVar10 = 200;
                      break;
                    case 0x2f: ;
                    case 0x35: ;
                      iVar10 = 500;
                      break;
                    case 0x54: ;
                    case 0x55: ;
                    case 0x56: ;
                    case 0x57: ;
                    case 0xb8: ;
                    case 0xbb: ;
                    case 0xbc: ;
                    case 0xbd: ;
                      iVar10 = 100000;
                    }
                    if (psVar11[-2] < iVar10) {
                      switch(psVar11[-0x18]) {
                      case 0x20: ;
                      case 0x33: ;
                        iVar10 = 300;
                        break;
default: ;
                        iVar10 = 200;
                        break;
                      case 0x2f: ;
                      case 0x35: ;
                        iVar10 = 500;
                        break;
                      case 0x54: ;
                      case 0x55: ;
                      case 0x56: ;
                      case 0x57: ;
                      case 0xb8: ;
                      case 0xbb: ;
                      case 0xbc: ;
                      case 0xbd: ;
                        iVar10 = 100000;
                      }
                      if (psVar11[-2] + 100 <= iVar10) {
                        return (int)(short)(&DAT_007999ea)[iVar6 * 0x84];
                      }
                    }
                  }
                  else if ((psVar11[0x47] != 0) && (psVar11[0x47] == iVar10)) {
                    switch(psVar11[-0x18]) {
                    case 0x20: ;
                    case 0x33: ;
                      iVar10 = 300;
                      break;
default: ;
                      iVar10 = 200;
                      break;
                    case 0x2f: ;
                    case 0x35: ;
                      iVar10 = 500;
                      break;
                    case 0x54: ;
                    case 0x55: ;
                    case 0x56: ;
                    case 0x57: ;
                    case 0xb8: ;
                    case 0xbb: ;
                    case 0xbc: ;
                    case 0xbd: ;
                      iVar10 = 100000;
                    }
                    if (psVar11[0x46] < iVar10) {
                      switch(psVar11[-0x18]) {
                      case 0x20: ;
                      case 0x33: ;
                        iVar10 = 300;
                        break;
default: ;
                        iVar10 = 200;
                        break;
                      case 0x2f: ;
                      case 0x35: ;
                        iVar10 = 500;
                        break;
                      case 0x54: ;
                      case 0x55: ;
                      case 0x56: ;
                      case 0x57: ;
                      case 0xb8: ;
                      case 0xbb: ;
                      case 0xbc: ;
                      case 0xbd: ;
                        iVar10 = 100000;
                      }
                      if (psVar11[0x46] + 100 <= iVar10) {
                        return (int)(short)(&DAT_007999ea)[iVar6 * 0x84];
                      }
                    }
                  }
                }
              }
            }
switchD_0045c370_caseD_3: ;
            local_c = local_c + 1;
          } while (local_c < 8);
        }
      }
switchD_0045c2dc_caseD_21: ;
      param_2 = param_2 + 1;
      psVar11 = psVar11 + 0x84;
    } while (param_2 <= DAT_0078d790);
  }
  iVar6 = 0;
  while (((param_1 = (int)*(short *)(&DAT_00799a12 + param_1 * 0x108), param_1 < 1 ||
          ((short)(&DAT_00799a14)[param_1 * 0x84] < 1)) ||
         (*(char *)((int)&DAT_007900c2 + uVar5 * 0xc4 + (int)(short)(&DAT_007999ea)[param_1 * 0x84])
          != '\x03'))) {
    iVar6 = iVar6 + 1;
    if (7 < iVar6) {
      return -1;
    }
  }
  return (int)(short)(&DAT_007999ea)[param_1 * 0x84];
}

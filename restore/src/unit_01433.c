#include "restore_types.h"
#define DAT_005f80fc (*(undefined4 *)0x005f80fcu)
#define DAT_0078d7cc (*(undefined *)0x0078d7ccu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a02 (*(undefined4 *)0x00799a02u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a9c (*(undefined4 *)0x00799a9cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd5e8 (*(undefined4 *)0x00bcd5e8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd608 (*(undefined4 *)0x00bcd608u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60a (*(undefined4 *)0x00bcd60au)
#define DAT_00bcd60b (*(undefined4 *)0x00bcd60bu)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd631 (*(undefined4 *)0x00bcd631u)
#define DAT_00bcd634 (*(undefined4 *)0x00bcd634u)
#define DAT_00bcd66a (*(undefined4 *)0x00bcd66au)
#define DAT_00bcd66e (*(undefined4 *)0x00bcd66eu)
#define DAT_00ebab20 (*(undefined4 *)0x00ebab20u)
#define DAT_00ec7630 (*(undefined4 *)0x00ec7630u)
#define DAT_00ed4140 (*(undefined4 *)0x00ed4140u)
#define DAT_00ee0c50 (*(undefined4 *)0x00ee0c50u)
extern void FUN_004690f0();
extern uint FUN_004ad880();
extern void FUN_004b3250();
extern void FUN_004b33d0();
extern undefined4 FUN_004b3470();
extern void FUN_004b3c60();
extern void FUN_004b52f0();
extern void FUN_004b5420();
extern undefined4 FUN_004f65b0();
extern bool FUN_004f6650();

void FUN_004b3530(int param_1)

{
  char cVar1;
  char cVar2;
  byte bVar3;
  short sVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int local_c;
  int local_8;
  int local_4;
  
  iVar7 = param_1 * 0x184;
  cVar1 = (&DAT_00bcd609)[iVar7];
  if ('\a' < cVar1) {
    return;
  }
  if ((&DAT_00bcd66a)[iVar7] == '\v') {
    return;
  }
  iVar8 = (&DAT_005f80fc)[cVar1] + *(int *)(&DAT_00bcd618 + iVar7);
  local_4 = (int)*(short *)(&DAT_00b4e6fc + iVar8 * 2);
  uVar5 = (&DAT_00af598c)[iVar8];
  if ((&DAT_00bcd66e)[iVar7] == '\0') {
    cVar2 = (&DAT_00bcd66a)[iVar7];
    if (cVar2 == '\x02') {
      cVar2 = (&DAT_00ec7630)[iVar8];
      if (-1 < cVar2) {
        if (cVar2 == '\0') {
          return;
        }
        if (cVar2 == '\x05') {
          return;
        }
        if (cVar2 == '\x01') {
          (&DAT_00bcd60b)[iVar7] = cVar1;
          (&DAT_00bcd609)[iVar7] = 0xb;
          if (((byte)DAT_0078d7cc & 3) != 0) {
            return;
          }
          FUN_004690f0(iVar8,10);
          return;
        }
        if (cVar2 == '\x02') {
          if ((uVar5 & 0x400) != 0) {
            return;
          }
          (&DAT_00bcd60b)[iVar7] = cVar1;
          (&DAT_00bcd609)[iVar7] = 0xb;
          if (((byte)DAT_0078d7cc & 3) != 0) {
            return;
          }
          FUN_004690f0(iVar8,10);
          return;
        }
        (&DAT_00bcd60b)[iVar7] = cVar1;
        (&DAT_00bcd609)[iVar7] = 0xb;
        if (cVar2 != '\x03') {
          if (((byte)DAT_0078d7cc & 3) != 0) {
            return;
          }
          FUN_004690f0(iVar8,400);
          return;
        }
        if (((byte)DAT_0078d7cc & 3) != 0) {
          return;
        }
        FUN_004690f0(iVar8,200);
        return;
      }
    }
    else {
      if (cVar2 == '\x04') {
        if ((byte)(&DAT_00ebab20)[iVar8] < 0x80) {
          return;
        }
        (&DAT_00bcd609)[iVar7] = 9;
        return;
      }
      cVar1 = *(char *)((int)&DAT_00ee0c50 + iVar8);
      if (cVar2 == '\x06') {
        if (cVar1 != -1) {
          return;
        }
        if ((uVar5 & 0x10004) != 0) {
          return;
        }
        (&DAT_00bcd609)[iVar7] = 9;
        return;
      }
      if (((cVar1 == '\x01') || (cVar1 == '\x03')) &&
         ((bVar3 = (&DAT_00bcd602)[iVar7], bVar3 == 0 || ((3 < bVar3 && (bVar3 != 0x5e)))))) {
        local_c = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
        local_8 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
        FUN_004ad880(&local_c,&local_8);
        FUN_004b5420(param_1);
        (&DAT_00bcd60c)[param_1 * 0xc2] = (short)local_c;
        (&DAT_00bcd60e)[param_1 * 0xc2] = (short)local_8;
        *(int *)(&DAT_00bcd618 + param_1 * 0x184) = DAT_00bca2d8 + local_8 * 0xe4 + local_c;
        (&DAT_00bcd631)[param_1 * 0x184] = 0xf;
        FUN_004b52f0(param_1);
        FUN_004b3250(param_1);
        iVar7 = param_1 * 0x184;
        (&DAT_00bcd609)[iVar7] = 9;
      }
      iVar6 = local_4;
      if (((uVar5 & 0x40) == 0) && ((uVar5 & 0x40000400) == 0)) {
        if ((uVar5 & 0xc000) == 0) {
          if ((uVar5 & 8) == 0) {
            if ((uVar5 & 4) == 0) {
              return;
            }
          }
          else {
            if (((&DAT_00af598c)[iVar8] & 0x10000000) != 0) {
              return;
            }
            sVar4 = (&DAT_007999e8)[local_4 * 0x84];
            if (sVar4 == 0x38) {
              return;
            }
            if ((sVar4 == 0x36) && ((&DAT_00bcd66a)[iVar7] != '\0')) {
              return;
            }
            if ((sVar4 == 0xb7) && ((&DAT_00bcd66a)[iVar7] == '\a')) {
              return;
            }
            if (sVar4 == 0xd1) {
              return;
            }
            if ((sVar4 == 0x88) && (DAT_00bcd5e8 == 0)) {
              switch((&DAT_00bcd602)[iVar7]) {
              case 1: ;
              case 2: ;
              case 3: ;
              case 0x5e: ;
                return;
              case 4: ;
              case 9: ;
              case 0xb: ;
              case 0xc: ;
              case 0xd: ;
              case 0xf: ;
              case 0x10: ;
              case 0x11: ;
              case 0x12: ;
              case 0x13: ;
              case 0x26: ;
              case 0x27: ;
              case 0x2a: ;
              case 0x49: ;
              case 0x4b: ;
              case 0x4f: ;
              case 0x50: ;
              case 0x51: ;
              case 0x55: ;
              case 0x5a: ;
              case 0x69: ;
              case 0x6c: ;
                iVar7 = (&DAT_00799a9c)[local_4 * 0x42];
                iVar8 = FUN_004b3470(param_1,local_4);
                if (iVar8 != 1) {
                  if (iVar8 != 2) {
                    (&DAT_00bcd609)[param_1 * 0x184] = 9;
                    return;
                  }
                  iVar7 = 0;
                  do {
                    FUN_004b33d0(param_1);
                    iVar8 = param_1 * 0x184;
                    (&DAT_00bcd60a)[iVar8] = (&DAT_00bcd609)[iVar8];
                    *(short *)(&DAT_00bcd634 + iVar8) = *(short *)(&DAT_00bcd634 + iVar8) + 1;
                    FUN_004b3c60(param_1);
                    if ('\a' < (char)(&DAT_00bcd609)[param_1 * 0x184]) {
                      return;
                    }
                    iVar7 = iVar7 + 1;
                  } while (iVar7 < 3);
                  return;
                }
                iVar8 = FUN_004f6650();
                if (((iVar8 == 0) && (0 < (short)(&DAT_00799a18)[iVar6 * 0x84])) &&
                   (0 < (short)(&DAT_00799a18)[iVar7 * 0x84])) {
                  if ((&DAT_00bcd630)[param_1 * 0x184] == '\a') {
                    return;
                  }
                  FUN_004f65b0(&param_1);
                  iVar7 = param_1 * 0x184;
                  (&DAT_00bcd608)[iVar7] = (&DAT_00bcd630)[iVar7];
                  (&DAT_00bcd630)[iVar7] = 7;
                  FUN_004b3250(param_1);
                  *(undefined2 *)(&DAT_00bcd62e + (short)(&DAT_00799a02)[iVar6 * 0x84] * 0x184) = 0;
                  return;
                }
                FUN_004b3250(param_1);
                local_c = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
                local_8 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
                FUN_004ad880(&local_c,&local_8);
                FUN_004b5420(param_1);
                (&DAT_00bcd60c)[param_1 * 0xc2] = (short)local_c;
                (&DAT_00bcd60e)[param_1 * 0xc2] = (short)local_8;
                *(int *)(&DAT_00bcd618 + param_1 * 0x184) = DAT_00bca2d8 + local_8 * 0xe4 + local_c;
                (&DAT_00bcd631)[param_1 * 0x184] = 0xf;
                FUN_004b52f0(param_1);
                param_1 = param_1 * 0x184;
                switch((&DAT_00bcd602)[param_1]) {
                case 4: ;
                case 9: ;
                case 0xf: ;
                case 0x10: ;
                case 0x11: ;
                case 0x12: ;
                case 0x13: ;
                case 0x26: ;
                case 0x4b: ;
                case 0x4f: ;
                case 0x50: ;
                case 0x51: ;
                case 0x55: ;
                case 0x5a: ;
                case 0x69: ;
                  (&DAT_00bcd630)[param_1] = 8;
                  return;
default: ;
                  return;
                case 0xb: ;
                case 0xc: ;
                case 0xd: ;
                case 0x2a: ;
                  (&DAT_00bcd606)[param_1] = 2;
                  return;
                case 0x27: ;
                  (&DAT_00bcd630)[param_1] = 9;
                  return;
                case 0x49: ;
                  (&DAT_00bcd630)[param_1] = 0xe;
                  return;
                }
              }
            }
          }
        }
      }
      else {
        if (DAT_00bcd5e8 == 0) {
          return;
        }
        if ((uVar5 & 8) == 0) {
          return;
        }
        sVar4 = (&DAT_007999e8)[local_4 * 0x84];
        if ((((sVar4 != 0x3a) && (sVar4 != 0xaa)) && (sVar4 != 0xab)) &&
           (((sVar4 != 200 && (sVar4 != 0xc9)) && ((sVar4 != 0xca && (sVar4 != 0x8a)))))) {
          return;
        }
      }
    }
  }
  else if ((uVar5 & 4) != 0) {
    if (-1 < (char)(&DAT_00ed4140)[iVar8]) {
      return;
    }
    (&DAT_00bcd609)[iVar7] = 9;
    return;
  }
  (&DAT_00bcd609)[iVar7] = 9;
  return;
}

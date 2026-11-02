#include "restore_types.h"
#define DAT_007090cc (*(undefined4 *)0x007090ccu)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd608 (*(undefined4 *)0x00bcd608u)
#define DAT_00bcd60b (*(undefined4 *)0x00bcd60bu)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd610 (*(undefined4 *)0x00bcd610u)
#define DAT_00bcd612 (*(undefined4 *)0x00bcd612u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd662 (*(undefined4 *)0x00bcd662u)
#define DAT_00bcd683 (*(undefined4 *)0x00bcd683u)
#define DAT_00bcd684 (*(undefined4 *)0x00bcd684u)
#define DAT_00bcd686 (*(undefined4 *)0x00bcd686u)
#define DAT_00bcd688 (*(undefined4 *)0x00bcd688u)
#define DAT_00ea5c88 (*(undefined4 *)0x00ea5c88u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004bc4a0();
extern undefined4 FUN_004c5120();
extern bool FUN_004c52d0();
extern undefined4 FUN_0051cba0();

void FUN_004b1210(int param_1,int param_2)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  int iVar5;
  bool bVar6;
  char cVar7;
  char cVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  
  iVar13 = param_2;
  bVar6 = false;
  iVar10 = param_1 * 0x184;
  iVar12 = (&DAT_007090cc)[(uint)(byte)(&DAT_00bcd602)[iVar10] * 0xe];
  if ((0 < iVar12) && (iVar12 < 8)) {
    if ((&DAT_00bcd630)[iVar10] == '\x13') {
      FUN_004bc4a0(param_1,param_2);
      return;
    }
    if (((&DAT_00bcd630)[param_2 * 0x184] != '\x13') && ((&DAT_00bcd630)[iVar10] != '\x05')) {
      param_2 = 1;
      while (iVar9 = DAT_00eaac28, 0 < iVar13) {
        if (iVar13 == param_1) {
          sVar1 = *(short *)(&DAT_00bcd600 + iVar13 * 0x184);
        }
        else {
          iVar11 = iVar13 * 0x184;
          switch((uint)(byte)(&DAT_00bcd602)[iVar11]) {
          case 0x19: ;
          case 0x4d: ;
          case 0x4e: ;
          case 0x5c: ;
          case 0x5d: ;
          case 100: ;
          case 0x65: ;
            break;
default: ;
            iVar5 = (&DAT_007090cc)[(uint)(byte)(&DAT_00bcd602)[iVar11] * 0xe];
            if ((&DAT_00bcd606)[iVar11] == '\x01') {
              if ((&DAT_00bcd630)[iVar11] == '\x04') {
                bVar6 = false;
              }
              else {
                if (iVar12 == 2) {
                  if (iVar5 == 5) {
                    if ((&DAT_00bcd630)[iVar11] != '\v') goto LAB_004b13a0;
                  }
                  else if (iVar5 != 4) {
joined_r0x004b1361: ;
                    if (iVar5 != 3) goto LAB_004b1376;
                  }
                }
                else if (iVar12 == 3) {
                  if (((iVar5 != 1) && (iVar5 != 2)) && (iVar5 != 4)) {
LAB_004b1376: ;
                    if (iVar5 != 7) goto LAB_004b13a0;
                  }
                }
                else {
                  if (iVar12 != 4) {
                    if (iVar12 == 1) {
                      if (iVar5 == 7) goto LAB_004b139b;
                    }
                    else {
                      if (iVar12 == 6) goto LAB_004b1376;
                      if ((iVar12 == 7) &&
                         ((((iVar5 == 1 || (iVar5 == 2)) || (iVar5 == 3)) ||
                          ((iVar5 == 4 || (iVar5 == 6)))))) goto LAB_004b139b;
                    }
                    goto LAB_004b13a0;
                  }
                  if (iVar5 != 2) goto joined_r0x004b1361;
                }
LAB_004b139b: ;
                bVar6 = true;
              }
            }
            else {
              bVar6 = false;
            }
LAB_004b13a0: ;
            cVar7 = (&DAT_00bcd602)[iVar10];
            if (((cVar7 == '\x17') && ((&DAT_00bcd602)[iVar11] == '\x04')) &&
               ((&DAT_00bcd603)[iVar11] == '\x15')) {
              (&DAT_00bcd630)[iVar10] = 10;
              *(undefined2 *)(&DAT_00bcd62e + iVar10) = 0;
              (&DAT_00bcd606)[iVar13 * 0x184] = 2;
              (&DAT_00ea5c88)[iVar9 * 0x1276] = (&DAT_00ea5c88)[iVar9 * 0x1276] + 1;
              return;
            }
            if (((cVar7 == '\x04') && ((&DAT_00bcd603)[iVar10] == '\x15')) &&
               ((&DAT_00bcd602)[iVar11] == '\x17')) {
              (&DAT_00bcd630)[iVar13 * 0x184] = 10;
              *(undefined2 *)(&DAT_00bcd62e + iVar13 * 0x184) = 0;
LAB_004b153a: ;
              (&DAT_00bcd606)[iVar10] = 2;
              return;
            }
            if ((cVar7 == '!') && ((&DAT_00bcd602)[iVar11] == 'b')) {
              (&DAT_00bcd606)[iVar13 * 0x184] = 2;
              return;
            }
            if ((cVar7 == 'b') && ((&DAT_00bcd602)[iVar11] == '!')) goto LAB_004b153a;
            if (((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] != 0) &&
               ((iVar9 = FUN_004c5120((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276]), iVar9 == 4 &&
                (cVar7 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],1), cVar7 != '\0')))) {
              if (((&DAT_00bcd602)[iVar10] == '\x1b') &&
                 ((iVar9 = FUN_004c5120((int)(short)(&DAT_00bcd652)[param_1 * 0xc2]), iVar9 == 4 &&
                  ((&DAT_00bcd602)[iVar11] == 'b')))) {
                (&DAT_00bcd606)[iVar13 * 0x184] = 2;
                return;
              }
              if ((((&DAT_00bcd602)[iVar10] == 'b') && ((&DAT_00bcd602)[iVar11] == '\x1b')) &&
                 (iVar9 = FUN_004c5120((int)(short)(&DAT_00bcd652)[iVar13 * 0xc2]), iVar9 == 4))
              goto LAB_004b153a;
            }
            if (bVar6) {
              if (((&DAT_00bcd630)[iVar11] != '\x05') || ((byte)(&DAT_00bcd683)[iVar11] < 2)) {
                (&DAT_00bcd608)[iVar10] = (&DAT_00bcd630)[iVar10];
                (&DAT_00bcd630)[iVar10] = 5;
                *(short *)(&DAT_00bcd688 + iVar10) = (short)iVar13;
                *(short *)(&DAT_00bcd684 + iVar10) = (short)iVar13;
                (&DAT_00bcd683)[iVar10] = 1;
                iVar12 = iVar13 * 0x184;
                (&DAT_00bcd662)[iVar10] = 0xc;
                sVar1 = (&DAT_00bcd60c)[iVar13 * 0xc2];
                if ((sVar1 == *(short *)(&DAT_00bcd61c + iVar12)) &&
                   (sVar2 = (&DAT_00bcd60e)[iVar13 * 0xc2],
                   sVar2 == *(short *)(&DAT_00bcd61e + iVar12))) {
                  sVar3 = *(short *)(&DAT_00bcd612 + iVar10);
                  sVar4 = *(short *)(&DAT_00bcd610 + iVar10);
                }
                else {
                  sVar2 = *(short *)(&DAT_00bcd612 + iVar12);
                  sVar1 = *(short *)(&DAT_00bcd610 + iVar12);
                  sVar3 = *(short *)(&DAT_00bcd612 + iVar10);
                  sVar4 = *(short *)(&DAT_00bcd610 + iVar10);
                }
                cVar7 = FUN_0051cba0((int)sVar4,(int)sVar3,(int)sVar1,(int)sVar2);
                (&DAT_00bcd60b)[iVar10] = cVar7;
                if ('\a' < cVar7) {
                  (&DAT_00bcd60b)[iVar10] = 0;
                }
                if ((&DAT_00bcd630)[iVar12] != '\x05') {
                  (&DAT_00bcd608)[iVar12] = (&DAT_00bcd630)[iVar12];
                  (&DAT_00bcd630)[iVar12] = 5;
                  (&DAT_00bcd662)[iVar12] = 0;
                  cVar7 = (&DAT_00bcd60b)[iVar10];
                  cVar8 = cVar7 + '\x04';
                  (&DAT_00bcd60b)[iVar12] = cVar8;
                  if ('\a' < cVar8) {
                    (&DAT_00bcd60b)[iVar12] = cVar7 + -4;
                  }
                }
                if ((&DAT_00bcd683)[iVar12] != '\0') {
                  if ((&DAT_00bcd683)[iVar12] != '\x01') {
                    return;
                  }
                  *(undefined2 *)(&DAT_00bcd686 + iVar12) = (undefined2)param_1;
                  (&DAT_00bcd683)[iVar12] = 2;
                  return;
                }
                *(undefined2 *)(&DAT_00bcd684 + iVar12) = (undefined2)param_1;
                *(undefined2 *)(&DAT_00bcd688 + iVar12) = (undefined2)param_1;
                (&DAT_00bcd683)[iVar12] = 1;
                return;
              }
              bVar6 = false;
            }
          }
          sVar1 = *(short *)(&DAT_00bcd600 + iVar11);
        }
        iVar13 = (int)sVar1;
        param_2 = param_2 + 1;
        if (999 < param_2) {
          return;
        }
      }
    }
  }
  return;
}

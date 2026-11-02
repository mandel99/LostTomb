#include "restore_types.h"
#define DAT_0056f720 (*(undefined4 *)0x0056f720u)
#define DAT_0056f734 (*(undefined4 *)0x0056f734u)
#define DAT_0056f748 (*(undefined4 *)0x0056f748u)
#define DAT_0056f75c (*(undefined4 *)0x0056f75cu)
#define DAT_0056f784 (*(undefined4 *)0x0056f784u)
#define DAT_0056f798 (*(undefined4 *)0x0056f798u)
#define DAT_0056f7ac (*(undefined4 *)0x0056f7acu)
#define DAT_0056f7c0 (*(undefined4 *)0x0056f7c0u)
#define DAT_0057953c (*(undefined *)0x0057953cu)
#define DAT_007027bc (*(char *)0x007027bcu)
#define DAT_007848e2 (*(undefined *)0x007848e2u)
#define DAT_0078d620 (*(undefined4 *)0x0078d620u)
#define DAT_0078d7c4 (*(undefined *)0x0078d7c4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ea9 (*(undefined1 *)0x00e38ea9u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00e39084 (*(undefined *)0x00e39084u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea5c00 (*(undefined4 *)0x00ea5c00u)
#define DAT_00ea5c0a (*(undefined4 *)0x00ea5c0au)
#define DAT_00ea5c0e (*(undefined4 *)0x00ea5c0eu)
#define DAT_00ea5c14 (*(undefined4 *)0x00ea5c14u)
#define DAT_00ea5c50 (*(undefined4 *)0x00ea5c50u)
#define DAT_00ea5c78 (*(undefined4 *)0x00ea5c78u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5e48 (*(undefined4 *)0x00ea5e48u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00ea6086 (*(undefined4 *)0x00ea6086u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00417ae0();
extern undefined4 FUN_004c06e0();
extern void FUN_004c47a0();
extern void FUN_004c51d0();
extern bool FUN_004e9f50();
extern uint FUN_00562348();

void FUN_004e9890(int param_1)

{
  char *pcVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  int iVar5;
  char cVar6;
  int iVar7;
  short *psVar8;
  int iVar9;
  undefined1 uVar10;
  int iVar11;
  undefined4 *puVar12;
  int iVar13;
  
  iVar5 = DAT_00eaac28;
  if ((DAT_00e38ebc == 0) && ((char)DAT_007027bc == '\0')) {
    iVar5 = 0;
    iVar7 = DAT_00eaac28 * 0x49d8;
    do {
      *(undefined1 *)((int)&DAT_00ea5c00 + iVar5 + iVar7) = 0x1e;
      *(undefined1 *)((int)&DAT_00ea5c0a + iVar5 + iVar7) = 0x1e;
      iVar5 = iVar5 + 1;
    } while (iVar5 < 5);
    return;
  }
  iVar7 = 0;
  puVar12 = &DAT_0078d620;
  psVar8 = &DAT_007848e2;
  do {
    if (*psVar8 != 0) {
      *(undefined1 *)((int)&DAT_00ea5c00 + iVar7 + iVar5 * 0x49d8) = *(undefined1 *)puVar12;
    }
    psVar8 = psVar8 + 1;
    iVar7 = iVar7 + 1;
    puVar12 = puVar12 + 1;
  } while ((int)psVar8 < 0x7848ec);
  iVar7 = 0;
  psVar8 = &DAT_007848e2;
  do {
    if (*psVar8 != 0) {
      iVar11 = (&DAT_00ea5c50)[iVar5 * 0x1276 + iVar7];
      if (0x28 < iVar11) {
        iVar11 = 0x28;
      }
      pcVar1 = (char *)((int)&DAT_00ea5c00 + iVar7 + iVar5 * 0x49d8);
      *pcVar1 = *pcVar1 + ('\f' - (char)iVar11);
    }
    psVar8 = psVar8 + 1;
    iVar7 = iVar7 + 1;
  } while ((int)psVar8 < 0x7848ec);
  cVar6 = '2';
  iVar5 = iVar5 * 0x49d8;
  iVar7 = *(int *)(&DAT_00ea1894 + iVar5);
  if (iVar7 < 400) {
    cVar3 = '2';
  }
  else if (iVar7 < 0x1c2) {
    cVar3 = '(';
    cVar6 = '<';
  }
  else if (iVar7 < 500) {
    cVar3 = '\x1e';
    cVar6 = 'F';
  }
  else if (iVar7 < 0x226) {
    cVar3 = '\x14';
    cVar6 = 'P';
  }
  else if (iVar7 < 600) {
    cVar3 = '\n';
    cVar6 = 'Z';
  }
  else {
    cVar3 = '\0';
    cVar6 = 'd';
  }
  iVar7 = 0;
  psVar8 = &DAT_007848e2;
  do {
    if (*psVar8 != 0) {
      if ('d' < *(char *)((int)&DAT_00ea5c00 + iVar7 + iVar5)) {
        *(undefined1 *)((int)&DAT_00ea5c00 + iVar7 + iVar5) = 100;
      }
      if (*(char *)((int)&DAT_00ea5c00 + iVar7 + iVar5) < cVar3) {
        *(char *)((int)&DAT_00ea5c00 + iVar7 + iVar5) = cVar3;
      }
      if (cVar6 < *(char *)((int)&DAT_00ea5c00 + iVar7 + iVar5)) {
        *(char *)((int)&DAT_00ea5c00 + iVar7 + iVar5) = cVar6;
      }
    }
    iVar11 = DAT_00e38ebc;
    cVar4 = DAT_00e38ea9;
    psVar8 = psVar8 + 1;
    iVar7 = iVar7 + 1;
  } while ((int)psVar8 < 0x7848ec);
  if (param_1 != 0) {
    iVar7 = 0;
    psVar8 = &DAT_007848e2;
    do {
      if (*psVar8 != 0) {
        cVar6 = *(char *)((int)&DAT_00ea5c0a + iVar7 + iVar5);
        cVar2 = *(char *)((int)&DAT_00ea5c00 + iVar7 + iVar5);
        if (cVar2 < cVar6) {
          cVar6 = cVar6 + -1;
LAB_004e9a70: ;
          *(char *)((int)&DAT_00ea5c0a + iVar7 + iVar5) = cVar6;
        }
        else if (cVar6 < cVar2) {
          cVar6 = cVar6 + '\x01';
          goto LAB_004e9a70;
        }
        if ((((DAT_00e38ea8 == '\0') && (iVar11 == 0)) && (cVar4 == '\0')) &&
           (*(char *)((int)&DAT_00ea5c0a + iVar7 + iVar5) < '2')) {
          *(undefined1 *)((int)&DAT_00ea5c0a + iVar7 + iVar5) = 0x32;
        }
      }
      psVar8 = psVar8 + 1;
      iVar7 = iVar7 + 1;
    } while ((int)psVar8 < 0x7848ec);
    iVar11 = FUN_00562348();
    iVar7 = DAT_00eaac28;
    iVar5 = DAT_00e39080;
    iVar11 = iVar11 % 5;
    if ((iVar11 < 5) && ((&DAT_007848e2)[iVar11] != 0)) {
      iVar9 = DAT_00eaac28 * 0x49d8 + iVar11;
      cVar6 = *(char *)((int)&DAT_00ea5c0a + iVar9);
      if (cVar6 < 'Z') {
        if (cVar6 < 'P') {
          if (cVar6 < 'F') {
            if (cVar6 < '3') {
              if (cVar6 == '2') {
                *(undefined1 *)((int)&DAT_00ea5c14 + iVar9) = 0;
              }
              else {
                if (cVar6 < '\x1f') {
                  if (cVar6 < '\x15') {
                    *(undefined1 *)((int)&DAT_00ea6086 + iVar9) = 0;
                    if (cVar6 < '\v') {
                      cVar6 = (&DAT_0056f7c0)[iVar5 * 4];
                    }
                    else {
                      cVar6 = (&DAT_0056f7ac)[iVar5 * 4];
                    }
                  }
                  else {
                    *(undefined1 *)((int)&DAT_00ea6086 + iVar9) = 0;
                    cVar6 = (&DAT_0056f798)[iVar5 * 4];
                  }
                }
                else {
                  *(undefined1 *)((int)&DAT_00ea6086 + iVar9) = 0;
                  cVar6 = (&DAT_0056f784)[iVar5 * 4];
                }
                *(char *)((int)&DAT_00ea5c14 + iVar9) =
                     *(char *)((int)&DAT_00ea5c14 + iVar9) + cVar6;
              }
            }
            else {
              *(char *)((int)&DAT_00ea6086 + iVar9) =
                   *(char *)((int)&DAT_00ea6086 + iVar9) + (&DAT_0056f75c)[DAT_00e39080 * 4];
              *(undefined1 *)((int)&DAT_00ea5c14 + iVar9) = 0;
            }
          }
          else {
            *(char *)((int)&DAT_00ea6086 + iVar9) =
                 *(char *)((int)&DAT_00ea6086 + iVar9) + (&DAT_0056f748)[DAT_00e39080 * 4];
            *(undefined1 *)((int)&DAT_00ea5c14 + iVar9) = 0;
          }
        }
        else {
          *(char *)((int)&DAT_00ea6086 + iVar9) =
               *(char *)((int)&DAT_00ea6086 + iVar9) + (&DAT_0056f734)[DAT_00e39080 * 4];
          *(undefined1 *)((int)&DAT_00ea5c14 + iVar9) = 0;
        }
      }
      else {
        *(char *)((int)&DAT_00ea6086 + iVar9) =
             *(char *)((int)&DAT_00ea6086 + iVar9) + (&DAT_0056f720)[DAT_00e39080 * 4];
        *(undefined1 *)((int)&DAT_00ea5c14 + iVar9) = 0;
      }
      if ('1' < *(char *)((int)&DAT_00ea5c14 + iVar9)) {
        *(undefined1 *)((int)&DAT_00ea5c14 + iVar9) = 0x32;
      }
      if ('1' < *(char *)((int)&DAT_00ea6086 + iVar9)) {
        *(undefined1 *)((int)&DAT_00ea6086 + iVar9) = 0x32;
      }
    }
    iVar5 = 0;
    iVar9 = iVar7 * 0x49d8;
    do {
      cVar6 = *(char *)((int)&DAT_00ea5c0a + iVar5 + iVar9);
      if (cVar6 < '2') {
        *(undefined1 *)((int)&DAT_00ea6086 + iVar5 + iVar9) = 0;
        cVar6 = *(char *)((int)&DAT_00ea5c0a + iVar5 + iVar9);
      }
      if ('2' < cVar6) {
        *(undefined1 *)((int)&DAT_00ea5c14 + iVar5 + iVar9) = 0;
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < 5);
    if (((DAT_00e38ebc == 0) && (DAT_00e38ea8 == DAT_0057953c)) &&
       ((RESTORE_SLICE1(DAT_007027bc,1) == '\0' &&
        (((char)DAT_007027bc != '\0' && ('\'' < *(char *)(&DAT_00ea5c0e + iVar7 * 0x24ec))))))) {
      FUN_00417ae0(5);
    }
    if (DAT_0078d7c4 == 0) {
      FUN_004c51d0();
      iVar5 = DAT_00eaac28 * 0x49d8;
      if ((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] == 0) {
        iVar7 = 0;
        psVar8 = &DAT_007848e2;
        do {
          if ((*psVar8 != 0) && (*psVar8 == 2)) {
            *(char *)((int)&DAT_00ea5c00 + iVar7 + iVar5) =
                 (char)((*(char *)((int)&DAT_00ea5c00 + iVar7 + iVar5) * 0x62 + 0x32) / 100);
          }
          psVar8 = psVar8 + 1;
          iVar7 = iVar7 + 1;
        } while ((int)psVar8 < 0x7848ec);
      }
      else {
        iVar5 = 0;
        psVar8 = &DAT_007848e2;
        do {
          iVar7 = DAT_00eaac28;
          if (*psVar8 != 0) {
            iVar13 = DAT_00eaac28 * 0x49d8;
            iVar9 = FUN_004c06e0(iVar5,1);
            if ((short)(&DAT_007999e8)[(&DAT_00ea5f9c)[iVar7 * 0x1276] * 0x84] == iVar9) {
              iVar7 = *(char *)((int)&DAT_00ea5c00 + iVar5 + iVar13) * 0x73;
LAB_004e9d55: ;
              uVar10 = (undefined1)((iVar7 + 0x32) / 100);
            }
            else {
              if (*psVar8 == 2) {
                iVar7 = *(char *)((int)&DAT_00ea5c00 + iVar5 + iVar13) * 0x5a;
                goto LAB_004e9d55;
              }
              if (*psVar8 != 1) goto LAB_004e9d99;
              uVar10 = (undefined1)
                       ((*(char *)((int)&DAT_00ea5c00 + iVar5 + iVar13) * 0x5f + 0x32) / 100);
            }
            *(undefined1 *)((int)&DAT_00ea5c00 + iVar5 + iVar13) = uVar10;
          }
LAB_004e9d99: ;
          psVar8 = psVar8 + 1;
          iVar5 = iVar5 + 1;
        } while ((int)psVar8 < 0x7848ec);
      }
      iVar5 = 0;
      psVar8 = &DAT_007848e2;
      do {
        if (*psVar8 != 0) {
          pcVar1 = (char *)((int)&DAT_00ea5c00 + iVar5 + DAT_00eaac28 * 0x49d8);
          if ('d' < *(char *)((int)&DAT_00ea5c00 + iVar5 + DAT_00eaac28 * 0x49d8)) {
            *pcVar1 = 'd';
          }
          if (*pcVar1 < cVar3) {
            *pcVar1 = cVar3;
          }
        }
        iVar7 = DAT_00eaac28;
        psVar8 = psVar8 + 1;
        iVar5 = iVar5 + 1;
      } while ((int)psVar8 < 0x7848ec);
      iVar5 = 0;
      psVar8 = &DAT_007848e2;
      do {
        if (*psVar8 != 0) {
          iVar9 = iVar7 * 0x1276 + iVar5;
          (&DAT_00ea5c50)[iVar9] = (&DAT_00ea5c50)[iVar9] + 1;
        }
        psVar8 = psVar8 + 1;
        iVar5 = iVar5 + 1;
      } while ((int)psVar8 < 0x7848ec);
      if ((4 < iVar11) && (iVar5 = FUN_004e9f50(), iVar5 != 0)) {
        iVar11 = (&DAT_00ea5c78)[DAT_00eaac28 * 0x1276] + -1;
      }
      if (DAT_00e39084 != 0) {
        iVar5 = FUN_00562348(0xffffffff);
        FUN_004c47a0(iVar11,iVar5 % 3);
        cVar6 = 'd';
        iVar5 = 0;
        psVar8 = &DAT_007848e2;
        do {
          if ((*psVar8 != 0) &&
             (cVar3 = *(char *)((int)&DAT_00ea5c0a + iVar5 + DAT_00eaac28 * 0x49d8), cVar3 < cVar6))
          {
            cVar6 = cVar3;
          }
          psVar8 = psVar8 + 1;
          iVar5 = iVar5 + 1;
        } while ((int)psVar8 < 0x7848ec);
        if ((&DAT_00ea5e48)[DAT_00eaac28 * 0x1276] != 0) {
          (&DAT_00ea5e48)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5e48)[DAT_00eaac28 * 0x1276] + -1;
          return;
        }
        if (cVar6 < '\x1e') {
          (&DAT_00ea5e48)[DAT_00eaac28 * 0x1276] = 0x14;
        }
      }
    }
  }
  return;
}

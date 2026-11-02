#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd632 (*(undefined4 *)0x00bcd632u)
#define DAT_00bcd634 (*(undefined4 *)0x00bcd634u)
#define DAT_00bcd636 (*(undefined4 *)0x00bcd636u)
#define DAT_00bcd66a (*(undefined4 *)0x00bcd66au)
#define DAT_00bcd66e (*(undefined4 *)0x00bcd66eu)
#define DAT_00bcd738 (*(undefined4 *)0x00bcd738u)
#define DAT_00bcd754 (*(undefined4 *)0x00bcd754u)
#define DAT_00d04e58 (*(undefined4 *)0x00d04e58u)
#define _DAT_00784c8c (*(undefined4 *)0x00784c8cu)
#define _DAT_00784c8e (*(undefined4 *)0x00784c8eu)
extern undefined4 FUN_00489970();
extern bool FUN_00517f20();
extern bool FUN_005181c0();
extern bool FUN_00518400();
extern bool FUN_005186d0();
extern bool FUN_00518880();
extern bool FUN_00519390();
extern bool FUN_005195e0();
extern bool FUN_00519840();
extern bool FUN_00519be0();
extern bool FUN_0051a030();
extern bool FUN_0051a420();
extern bool FUN_0051b040();
extern int FUN_0051b400();
extern undefined4 FUN_0051ce90();
extern undefined4 FUN_0051d160();
extern bool FUN_0051d420();
extern int FUN_0051d980();
extern int FUN_0051dc10();

void FUN_004b2d20(int param_1)

{
  short sVar1;
  short sVar2;
  short sVar3;
  short sVar4;
  bool bVar5;
  int iVar6;
  char cVar7;
  int iVar8;
  int iVar9;
  
  iVar6 = param_1;
  bVar5 = true;
  iVar9 = param_1 * 0x184;
  *(undefined2 *)(&DAT_00bcd632 + iVar9) = 0;
  *(undefined2 *)(&DAT_00bcd634 + iVar9) = 0;
  *(undefined2 *)(&DAT_00bcd636 + iVar9) = 0;
  cVar7 = FUN_00489970(param_1,&param_1);
  if (cVar7 == '\0') {
    return;
  }
  if ((&DAT_00bcd66e)[iVar9] == '\0') {
    cVar7 = (&DAT_00bcd66a)[iVar9];
    if (cVar7 == '\x02') {
      iVar8 = FUN_0051a420((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                           (int)*(short *)(&DAT_00bcd61c + iVar9),
                           (int)*(short *)(&DAT_00bcd61e + iVar9),0,100000);
      if (iVar8 == 0) {
        iVar8 = FUN_0051b040((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                             (int)*(short *)(&DAT_00bcd61c + iVar9),
                             (int)*(short *)(&DAT_00bcd61e + iVar9));
        goto LAB_004b3187;
      }
LAB_004b31f0: ;
      sVar1 = *(short *)(&DAT_00bcd61e + iVar9);
      sVar2 = *(short *)(&DAT_00bcd61c + iVar9);
      sVar3 = (&DAT_00bcd60e)[iVar6 * 0xc2];
      sVar4 = (&DAT_00bcd60c)[iVar6 * 0xc2];
    }
    else {
      if (cVar7 == '\x04') {
        bVar5 = false;
        iVar8 = FUN_0051a030((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                             (int)*(short *)(&DAT_00bcd61c + iVar9),
                             (int)*(short *)(&DAT_00bcd61e + iVar9));
LAB_004b3187: ;
        if (iVar8 == 0) {
          return;
        }
        if (bVar5) goto LAB_004b31f0;
      }
      else {
        if (cVar7 == '\x05') {
          iVar8 = FUN_005181c0((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                               (int)*(short *)(&DAT_00bcd61c + iVar9),
                               (int)*(short *)(&DAT_00bcd61e + iVar9));
          goto LAB_004b3187;
        }
        if (cVar7 != '\x03') {
          if (cVar7 == '\x06') {
            iVar8 = FUN_00519be0((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                                 (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                                 (int)*(short *)(&DAT_00bcd61c + iVar9),
                                 (int)*(short *)(&DAT_00bcd61e + iVar9));
            if (iVar8 != 0) goto LAB_004b31f0;
            iVar8 = FUN_00517f20((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                                 (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                                 (int)*(short *)(&DAT_00bcd61c + iVar9),
                                 (int)*(short *)(&DAT_00bcd61e + iVar9));
          }
          else if (cVar7 == '\x01') {
            bVar5 = false;
            iVar8 = FUN_00519840((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                                 (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                                 (int)*(short *)(&DAT_00bcd61c + iVar9),
                                 (int)*(short *)(&DAT_00bcd61e + iVar9));
          }
          else if (cVar7 == '\0') {
            iVar8 = FUN_00517f20((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                                 (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                                 (int)*(short *)(&DAT_00bcd61c + iVar9),
                                 (int)*(short *)(&DAT_00bcd61e + iVar9));
          }
          else if (cVar7 == '\t') {
            iVar8 = FUN_00518400((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                                 (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                                 (int)*(short *)(&DAT_00bcd61c + iVar9),
                                 (int)*(short *)(&DAT_00bcd61e + iVar9));
          }
          else if (cVar7 == '\n') {
            iVar8 = FUN_0051d420((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                                 (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                                 (int)*(short *)(&DAT_00bcd61c + iVar9),
                                 (int)*(short *)(&DAT_00bcd61e + iVar9));
          }
          else if (cVar7 == '\a') {
            iVar8 = FUN_005195e0((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                                 (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                                 (int)*(short *)(&DAT_00bcd61c + iVar9),
                                 (int)*(short *)(&DAT_00bcd61e + iVar9));
            if (iVar8 != 0) goto LAB_004b3196;
            iVar8 = FUN_00519390((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                                 (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                                 (int)*(short *)(&DAT_00bcd61c + iVar9),
                                 (int)*(short *)(&DAT_00bcd61e + iVar9));
          }
          else {
            if (cVar7 != '\v') {
              return;
            }
            iVar8 = FUN_005186d0((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                                 (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                                 (int)*(short *)(&DAT_00bcd61c + iVar9),
                                 (int)*(short *)(&DAT_00bcd61e + iVar9));
          }
          goto LAB_004b3187;
        }
        iVar8 = FUN_00519840((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                             (int)*(short *)(&DAT_00bcd61c + iVar9),
                             (int)*(short *)(&DAT_00bcd61e + iVar9));
        if (iVar8 == 0) {
          iVar8 = FUN_00518880((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                               (int)*(short *)(&DAT_00bcd61c + iVar9),
                               (int)*(short *)(&DAT_00bcd61e + iVar9));
          goto LAB_004b3187;
        }
      }
LAB_004b3196: ;
      iVar8 = FUN_0051b400(4,param_1,(int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                           (int)*(short *)(&DAT_00bcd61c + iVar9),
                           (int)*(short *)(&DAT_00bcd61e + iVar9));
      if (0 < iVar8) goto LAB_004b3223;
      sVar1 = *(short *)(&DAT_00bcd61e + iVar9);
      sVar2 = *(short *)(&DAT_00bcd61c + iVar9);
      sVar3 = (&DAT_00bcd60e)[iVar6 * 0xc2];
      sVar4 = (&DAT_00bcd60c)[iVar6 * 0xc2];
    }
    iVar8 = FUN_0051b400(8,param_1,(int)sVar4,(int)sVar3,(int)sVar2,(int)sVar1);
  }
  else {
    if (DAT_00605a3c == 1) {
      FUN_0051d160((int)_DAT_00784c8c,(int)_DAT_00784c8e);
    }
    else if ((&DAT_00bcd66e)[iVar9] == '\x02') {
      FUN_0051d160((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                   (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2]);
    }
    else {
      FUN_0051ce90((int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                   (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2]);
    }
    if ((&DAT_00bcd66e)[iVar9] == '\x02') {
      iVar8 = FUN_0051dc10(param_1,(int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                           (int)*(short *)(&DAT_00bcd61c + iVar9),
                           (int)*(short *)(&DAT_00bcd61e + iVar9),1);
    }
    else if (((&DAT_00bcd602)[iVar9] == 'N') &&
            (((&DAT_00bcd630)[iVar9] == '\t' || (*(int *)(&DAT_00bcd754 + iVar9) == 9)))) {
      iVar8 = FUN_0051d980(param_1,(int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                           (int)*(short *)(&DAT_00bcd61c + iVar9),
                           (int)*(short *)(&DAT_00bcd61e + iVar9),
                           *(undefined4 *)(&DAT_00bcd738 + iVar9));
      if (iVar8 != 0) goto LAB_004b3223;
      iVar8 = FUN_0051dc10(param_1,(int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                           (int)*(short *)(&DAT_00bcd61c + iVar9),
                           (int)*(short *)(&DAT_00bcd61e + iVar9),0);
    }
    else {
      iVar8 = FUN_0051dc10(param_1,(int)(short)(&DAT_00bcd60c)[iVar6 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[iVar6 * 0xc2],
                           (int)*(short *)(&DAT_00bcd61c + iVar9),
                           (int)*(short *)(&DAT_00bcd61e + iVar9),0);
    }
  }
  if (iVar8 == 0) {
    return;
  }
LAB_004b3223: ;
  *(short *)(&DAT_00bcd632 + iVar9) = (short)param_1;
  *(short *)(&DAT_00bcd636 + iVar9) = (short)iVar8;
  *(short *)((int)&DAT_00d04e58 + param_1 * 2) = (short)iVar6;
  return;
}

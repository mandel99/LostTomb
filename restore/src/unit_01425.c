#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
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
extern void FUN_004b3250();
extern int FUN_00518b30();
extern int FUN_00518dd0();
extern int FUN_005190a0();
extern int FUN_00519e30();
extern int FUN_0051a220();
extern int FUN_0051a9d0();
extern int FUN_0051b220();
extern int FUN_0051b400();
extern undefined4 FUN_0051ce90();
extern undefined4 FUN_0051d160();
extern int FUN_0051d6d0();
extern int FUN_0051d980();
extern int FUN_0051dc10();

int FUN_004b28f0(int param_1,int param_2,undefined4 param_3)

{
  short sVar1;
  short sVar2;
  short sVar3;
  bool bVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  short sVar8;
  int iVar9;
  int local_4;
  
  iVar9 = param_1 * 0x184;
  *(undefined2 *)(&DAT_00bcd632 + iVar9) = 0;
  *(undefined2 *)(&DAT_00bcd634 + iVar9) = 0;
  *(undefined2 *)(&DAT_00bcd636 + iVar9) = 0;
  cVar5 = FUN_00489970(param_1,&local_4);
  if (cVar5 == '\0') {
    return 0;
  }
  bVar4 = true;
  if ((&DAT_00bcd66e)[iVar9] == '\0') {
    cVar5 = (&DAT_00bcd66a)[iVar9];
    if (cVar5 == '\x02') {
      iVar7 = FUN_0051a9d0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],param_2,param_3,0,100000);
      if (iVar7 == 0) {
        iVar7 = FUN_0051b220((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],param_2,param_3);
        goto LAB_004b2c1e;
      }
    }
    else {
      if (cVar5 == '\x04') {
        bVar4 = false;
        iVar7 = FUN_0051a220((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],param_2,param_3);
      }
      else if (cVar5 == '\x05') {
LAB_004b2b2e: ;
        iVar7 = FUN_00518b30((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],param_2,param_3);
      }
      else if (cVar5 == '\x03') {
        iVar7 = FUN_00519e30((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],param_2,param_3);
        bVar4 = false;
        if (iVar7 != 0) goto LAB_004b2c2b;
        bVar4 = true;
        iVar7 = FUN_005190a0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],param_2,param_3);
      }
      else if (cVar5 == '\x01') {
        bVar4 = false;
        iVar7 = FUN_00519e30((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],param_2,param_3);
      }
      else {
        if (cVar5 == '\0') goto LAB_004b2b2e;
        if (cVar5 == '\t') {
          iVar7 = FUN_00518dd0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],param_2,param_3);
        }
        else {
          if (cVar5 != '\n') {
            return 0;
          }
          iVar7 = FUN_0051d6d0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],param_2,param_3);
        }
      }
LAB_004b2c1e: ;
      if (iVar7 == 0) {
        return 0;
      }
    }
LAB_004b2c2b: ;
    iVar6 = *(int *)(param_2 + -4 + iVar7 * 4) - DAT_00bca2d8;
    *(short *)(&DAT_00bcd61c + iVar9) = (short)(iVar6 % 0xe4);
    sVar8 = ((short)(iVar6 / 0xe4) + (short)(iVar6 >> 0x1f)) -
            (short)((longlong)iVar6 * 0x8fb823ef >> 0x3f);
    *(short *)(&DAT_00bcd61e + iVar9) = sVar8;
    if (bVar4) {
      sVar1 = *(short *)(&DAT_00bcd61c + iVar9);
      sVar2 = (&DAT_00bcd60e)[param_1 * 0xc2];
      sVar3 = (&DAT_00bcd60c)[param_1 * 0xc2];
    }
    else {
      iVar6 = FUN_0051b400(4,local_4,(int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                           (int)*(short *)(&DAT_00bcd61c + iVar9),(int)sVar8);
      if (0 < iVar6) goto LAB_004b2cef;
      sVar8 = *(short *)(&DAT_00bcd61e + iVar9);
      sVar1 = *(short *)(&DAT_00bcd61c + iVar9);
      sVar2 = (&DAT_00bcd60e)[param_1 * 0xc2];
      sVar3 = (&DAT_00bcd60c)[param_1 * 0xc2];
    }
    iVar6 = FUN_0051b400(8,local_4,(int)sVar3,(int)sVar2,(int)sVar1,(int)sVar8);
  }
  else {
    if (DAT_00605a3c == 1) {
      FUN_0051d160((int)_DAT_00784c8c,(int)_DAT_00784c8e);
    }
    else if ((&DAT_00bcd66e)[iVar9] == '\x02') {
      FUN_0051d160((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                   (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    }
    else {
      FUN_0051ce90((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                   (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    }
    if ((&DAT_00bcd66e)[iVar9] == '\x02') {
      iVar6 = FUN_0051dc10(local_4,(int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                           (int)*(short *)(&DAT_00bcd61c + iVar9),
                           (int)*(short *)(&DAT_00bcd61e + iVar9),1);
      iVar7 = iVar6;
    }
    else if (((&DAT_00bcd602)[iVar9] == 'N') &&
            (((&DAT_00bcd630)[iVar9] == '\t' || (*(int *)(&DAT_00bcd754 + iVar9) == 9)))) {
      iVar6 = FUN_0051d980(local_4,(int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                           (int)*(short *)(&DAT_00bcd61c + iVar9),
                           (int)*(short *)(&DAT_00bcd61e + iVar9),
                           *(undefined4 *)(&DAT_00bcd738 + iVar9));
      iVar7 = iVar6;
      if (iVar6 == 0) {
        iVar6 = FUN_0051dc10(local_4,(int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                             (int)*(short *)(&DAT_00bcd61c + iVar9),
                             (int)*(short *)(&DAT_00bcd61e + iVar9),0);
        iVar7 = iVar6;
      }
    }
    else {
      iVar6 = FUN_0051dc10(local_4,(int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                           (int)*(short *)(&DAT_00bcd61c + iVar9),
                           (int)*(short *)(&DAT_00bcd61e + iVar9),0);
      iVar7 = iVar6;
    }
  }
  if (iVar6 == 0) {
    return iVar7;
  }
LAB_004b2cef: ;
  FUN_004b3250(param_1);
  *(short *)(&DAT_00bcd632 + iVar9) = (short)local_4;
  *(short *)(&DAT_00bcd636 + iVar9) = (short)iVar6;
  *(short *)((int)&DAT_00d04e58 + local_4 * 2) = (short)param_1;
  return iVar7;
}

#include "restore_types.h"
#define DAT_005dbf70 (*(undefined4 *)0x005dbf70u)
#define DAT_007065f8 (*(undefined4 *)0x007065f8u)
#define DAT_00706760 (*(undefined4 *)0x00706760u)
#define DAT_00706918 (*(undefined4 *)0x00706918u)
#define DAT_00706968 (*(undefined4 *)0x00706968u)
#define DAT_00707b88 (*(undefined4 *)0x00707b88u)
#define DAT_00707ef8 (*(undefined4 *)0x00707ef8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999f0 (*(undefined4 *)0x007999f0u)
#define DAT_00799a80 (*(undefined4 *)0x00799a80u)
#define DAT_009ab9dc (*(undefined4 *)0x009ab9dcu)
#define DAT_009ab9e0 (*(undefined4 *)0x009ab9e0u)
#define DAT_009ab9e8 (*(undefined4 *)0x009ab9e8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd65e (*(undefined4 *)0x00bcd65eu)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_0042ff20();
extern void FUN_0047e250();
extern int FUN_00482c10();
extern undefined4 FUN_00482dd0();
extern void FUN_00482ef0();
extern int FUN_00483000();
extern undefined4 FUN_00483160();
extern void FUN_00483260();
extern int FUN_004833c0();
extern int FUN_004834d0();
extern int FUN_004835e0();
extern int FUN_004836f0();
extern int FUN_00483800();
extern int FUN_00483910();
extern int FUN_00483e90();
extern int FUN_00483fb0();
extern int FUN_00484120();
extern int FUN_00484230();
extern int FUN_00484340();
extern int FUN_00484450();
extern int FUN_00484560();
extern void FUN_00484670();
extern int FUN_00484780();
extern undefined4 FUN_00484890();
extern int FUN_00484990();
extern int FUN_00484a90();
extern undefined4 FUN_004c5120();
extern bool FUN_004c52d0();

undefined4 FUN_004baa10(int param_1)

{
  byte bVar1;
  short sVar2;
  short sVar3;
  char cVar4;
  short sVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  int iVar9;
  undefined4 uVar10;
  
  sVar5 = 0;
  iVar9 = param_1 * 0x184;
  cVar4 = (&DAT_00bcd602)[iVar9];
  if (cVar4 == '(') {
    return 0;
  }
  if (cVar4 == '\x05') {
    sVar5 = FUN_00484990((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    goto LAB_004bb020;
  }
  if (cVar4 == '\a') {
    sVar5 = FUN_00484a90((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    (&DAT_00bcd65e)[iVar9] = (undefined1)DAT_009ab9e8;
    goto LAB_004bb020;
  }
  if (cVar4 == '\x1a') {
    sVar5 = FUN_00483910((int)(short)(&DAT_00bcd652)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    goto LAB_004bb020;
  }
  if (cVar4 == 'W') {
    sVar5 = FUN_00483e90((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    goto LAB_004bb020;
  }
  if (cVar4 == ' ') {
    sVar5 = FUN_00484450((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    FUN_00482ef0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                 (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],DAT_00707ef8);
    goto LAB_004bb020;
  }
  if (cVar4 == '\x1d') {
    sVar5 = FUN_00484120((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    goto LAB_004bb020;
  }
  if (cVar4 == '\x1e') {
    sVar5 = FUN_00484230((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    goto LAB_004bb020;
  }
  if (cVar4 == '\x1f') {
    sVar5 = FUN_00484340((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    goto LAB_004bb020;
  }
  if (cVar4 == '!') {
    sVar5 = FUN_00484560((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    iVar7 = FUN_00482dd0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],DAT_007065f8);
    bVar1 = (&DAT_00bcd65e)[iVar9];
    if ((int)(uint)bVar1 < iVar7) {
      (&DAT_00bcd65e)[iVar9] = (char)iVar7;
      goto LAB_004bb020;
    }
    if (10 < bVar1) {
      (&DAT_00bcd65e)[iVar9] = bVar1 - 10;
      goto LAB_004bb020;
    }
LAB_004babc9: ;
    (&DAT_00bcd65e)[iVar9] = 0;
    goto LAB_004bb020;
  }
  if (cVar4 == 'b') {
    FUN_0042ff20((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                 (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    goto LAB_004bb020;
  }
  if (cVar4 == '\"') {
    sVar5 = FUN_00484780((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    goto LAB_004bb020;
  }
  if (cVar4 == '@') {
    sVar5 = FUN_00484890((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
    goto LAB_004bb020;
  }
  if (cVar4 == '\x1b') {
    sVar2 = (&DAT_00bcd652)[param_1 * 0xc2];
    uVar8 = (&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276];
    sVar3 = (&DAT_007999e8)[sVar2 * 0x84];
    if (sVar3 == 0x3c) {
      uVar10 = 0;
LAB_004bace3: ;
      iVar7 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
      iVar6 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
LAB_004bacf3: ;
      sVar5 = FUN_00483fb0(iVar6,iVar7,uVar10);
    }
    else {
      if (sVar3 == 0x41) {
        uVar10 = 0;
LAB_004bacaf: ;
        iVar7 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
        iVar6 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
        goto LAB_004bacf3;
      }
      if (sVar3 == 0x3d) {
        uVar10 = 1;
LAB_004bacc9: ;
        iVar7 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
        iVar6 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
        goto LAB_004bacf3;
      }
      if (sVar3 == 0x42) {
        uVar10 = 1;
        goto LAB_004bace3;
      }
      if (sVar3 == 0x3e) {
        uVar10 = 2;
        goto LAB_004bacaf;
      }
      if (sVar3 == 0x43) {
        uVar10 = 2;
        goto LAB_004bacc9;
      }
      if (sVar3 == 0x3f) {
        uVar10 = 3;
        goto LAB_004bace3;
      }
      if (sVar3 == 0x44) {
        uVar10 = 3;
        goto LAB_004bacaf;
      }
      if (sVar3 == 0x40) {
        uVar10 = 4;
        goto LAB_004bacc9;
      }
      sVar5 = 0;
      if (sVar3 == 0x45) {
        uVar10 = 4;
        goto LAB_004bace3;
      }
    }
    if ((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] == 0) goto LAB_004bb020;
    iVar7 = FUN_004c5120(uVar8);
    iVar6 = FUN_004c5120((int)sVar2);
    if (iVar6 != iVar7) goto LAB_004bb020;
    iVar7 = FUN_004c5120(uVar8);
    if (iVar7 == 1) {
      cVar4 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],1);
      uVar8 = DAT_00706918;
    }
    else {
      if (iVar7 != 3) {
        if ((iVar7 == 4) &&
           (cVar4 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],10), cVar4 != '\0')) {
          FUN_00483160((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
        }
        goto LAB_004bb020;
      }
      cVar4 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],2);
      uVar8 = DAT_00706968;
    }
    sVar2 = sVar5;
    if (cVar4 == '\0') goto LAB_004bb020;
  }
  else {
    if (cVar4 == '\b') {
      sVar5 = FUN_00482c10((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
      bVar1 = (&DAT_00bcd65e)[iVar9];
      if ((int)(uint)bVar1 < DAT_009ab9e0) {
        (&DAT_00bcd65e)[iVar9] = (char)DAT_009ab9e0;
        goto LAB_004bb020;
      }
      if (10 < bVar1) {
        (&DAT_00bcd65e)[iVar9] = bVar1 - 10;
        goto LAB_004bb020;
      }
      goto LAB_004babc9;
    }
    if (cVar4 == '\n') {
      sVar5 = FUN_00483000((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
      (&DAT_00bcd65e)[iVar9] = (char)sVar5;
      goto LAB_004bb020;
    }
    uVar8 = DAT_00706760;
    sVar2 = 0;
    if (cVar4 != 'X') {
      if (cVar4 == 'Y') {
        FUN_00483260((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                     (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],DAT_00707b88);
        FUN_00484670((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                     (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
        (&DAT_00bcd65e)[iVar9] = DAT_009ab9dc;
      }
      else if (cVar4 == '\x0f') {
        sVar5 = FUN_004833c0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
      }
      else if (cVar4 == '\x10') {
        if (((&DAT_00bcd630)[iVar9] == '\f') || ((&DAT_00bcd630)[iVar9] == '\r')) {
          sVar5 = (&DAT_00bcd652)[param_1 * 0xc2];
        }
        else {
          sVar5 = (&DAT_00bcd656)[param_1 * 0xc2];
        }
        if (*(char *)(&DAT_00799a80 + sVar5 * 0x84) == '\0') {
          sVar5 = FUN_004834d0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],1);
        }
        else {
          sVar5 = FUN_004834d0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],2);
        }
      }
      else if (cVar4 == '\x11') {
        if (((&DAT_00bcd630)[iVar9] == '\f') || ((&DAT_00bcd630)[iVar9] == '\r')) {
          sVar5 = (&DAT_00bcd652)[param_1 * 0xc2];
        }
        else {
          sVar5 = (&DAT_00bcd656)[param_1 * 0xc2];
        }
        if (*(char *)(&DAT_00799a80 + sVar5 * 0x84) == '\0') {
          sVar5 = FUN_004835e0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],1);
        }
        else {
          sVar5 = FUN_004835e0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],2);
        }
      }
      else if (cVar4 == '\x12') {
        sVar5 = FUN_004836f0((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
      }
      else if (cVar4 == 'i') {
        sVar5 = FUN_00483800((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
      }
      goto LAB_004bb020;
    }
  }
  sVar5 = sVar2;
  FUN_00483260((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
               (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],uVar8);
  (&DAT_00bcd65e)[iVar9] = DAT_009ab9dc;
LAB_004bb020: ;
  iVar7 = (int)(short)(&DAT_00bcd652)[param_1 * 0xc2];
  bVar1 = (&DAT_00bcd602)[iVar9];
  if (iVar7 != 0) {
    (&DAT_007999f0)[iVar7 * 0x84] = (&DAT_007999f0)[iVar7 * 0x84] + sVar5;
    if (300 < (short)(&DAT_007999f0)[iVar7 * 0x84]) {
      (&DAT_007999f0)[iVar7 * 0x84] = 300;
    }
  }
  FUN_0047e250(*(undefined4 *)(&DAT_005dbf70 + (uint)bVar1 * 4),
               *(undefined4 *)(&DAT_00bcd618 + iVar9));
  return 0;
}

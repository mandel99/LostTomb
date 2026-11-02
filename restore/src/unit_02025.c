#include "restore_types.h"
#define DAT_005e4b8c (*(undefined4 *)0x005e4b8cu)
#define DAT_00799ae5 (*(undefined4 *)0x00799ae5u)
#define DAT_00799bd0 (*(undefined4 *)0x00799bd0u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00ea1884 (*(undefined4 *)0x00ea1884u)
#define DAT_00ea1888 (*(undefined4 *)0x00ea1888u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00ea5fc0 (*(undefined4 *)0x00ea5fc0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eaac2c (*(undefined1 *)0x00eaac2cu)
extern undefined4 FUN_0042fbb0();
extern undefined4 FUN_0042fcf0();
extern void FUN_00430040();
extern uint FUN_00430060();
extern void FUN_00430250();
extern void FUN_004302b0();
extern undefined4 FUN_004c5120();
extern bool FUN_004c52d0();
extern int FUN_004cde40();
extern int FUN_004cde60();
extern void FUN_004e2970();
extern void FUN_004e7f00();
extern longlong FUN_0056133c();

void FUN_004e81d0(void)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int *piVar6;
  undefined4 in_ECX;
  undefined3 uVar7;
  undefined3 extraout_var;
  int iVar8;
  int iVar9;
  char *pcVar10;
  int iVar11;
  
  cVar2 = DAT_00e38ea8;
  uVar7 = (undefined3)((uint)in_ECX >> 8);
  iVar3 = DAT_00eaac28 * 0x49d8;
  iVar9 = 0;
  iVar8 = 0;
  *(undefined4 *)(&DAT_00ea5fc0 + iVar3) = 0;
  bVar1 = false;
  if ((cVar2 < '\x02') && (DAT_00e38ebc == 0)) {
    bVar1 = true;
    *(undefined4 *)(&DAT_00ea1888 + iVar3) = 0x4b;
    *(undefined4 *)(&DAT_00ea1884 + iVar3) = 0x4b;
    goto LAB_004e8579;
  }
  iVar3 = 1;
  pcVar10 = &DAT_00799ae5;
  do {
    if (((pcVar10[-5] == '\x01') && (*pcVar10 != '\0')) && (*(short *)(pcVar10 + 0x17) != 0)) {
      iVar8 = iVar8 + *(short *)(pcVar10 + 0x17);
      FUN_004e7f00(iVar3,CONCAT31(uVar7,DAT_00eaac2c));
      if (*(int *)(pcVar10 + 0xe3) == 1000) {
        FUN_00430060(iVar3);
        *(int *)(&DAT_00ea5fc0 + DAT_00eaac28 * 0x49d8) =
             *(int *)(&DAT_00ea5fc0 + DAT_00eaac28 * 0x49d8) + 1;
      }
      else if (*(int *)(pcVar10 + 0xf7) == 1000) {
        FUN_00430040(iVar3);
      }
      iVar4 = FUN_0056133c();
      iVar9 = iVar9 + iVar4;
      uVar7 = extraout_var;
    }
    pcVar10 = pcVar10 + 0x108;
    iVar3 = iVar3 + 1;
  } while ((int)pcVar10 < 0x89b6dd);
  DAT_00eaac2c = 0;
  iVar9 = FUN_004cde60(iVar9,iVar8);
  iVar8 = DAT_00eaac28;
  iVar4 = DAT_00eaac28 * 0x49d8;
  iVar3 = 0;
  iVar11 = 0;
  *(int *)(&DAT_00ea1884 + iVar4) = iVar9;
  piVar6 = &DAT_005e4b8c;
  do {
    if (*(int *)(&DAT_00ea1894 + iVar4) < *piVar6) {
      iVar3 = *(int *)(iVar11 * 8 + 0x5e4b90);
      break;
    }
    piVar6 = piVar6 + 2;
    iVar11 = iVar11 + 1;
  } while ((int)piVar6 < 0x5e4bdc);
  *(int *)(&DAT_00ea1884 + iVar4) = iVar9 + iVar3;
  if ((((&DAT_00ea5ce8)[iVar8 * 0x24ec] != 0) &&
      (iVar3 = FUN_004c5120((&DAT_00ea5f9c)[iVar8 * 0x1276]), iVar3 == 4)) &&
     (cVar2 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],1), cVar2 != '\0')) {
    uVar5 = FUN_004cde40(*(undefined4 *)(&DAT_00ea1884 + DAT_00eaac28 * 0x49d8),0x69);
    *(undefined4 *)(&DAT_00ea1884 + DAT_00eaac28 * 0x49d8) = uVar5;
  }
  cVar2 = FUN_0042fbb0();
  if (cVar2 != '\0') {
    FUN_0042fcf0(0);
  }
  iVar8 = DAT_00eaac28;
  iVar9 = DAT_00eaac28 * 0x49d8;
  iVar3 = *(int *)(&DAT_00ea1888 + iVar9);
  if (iVar3 < *(int *)(&DAT_00ea1884 + iVar9)) {
    *(int *)(&DAT_00ea1888 + iVar9) = iVar3 + 2;
    if (*(int *)(&DAT_00ea1884 + iVar9) < iVar3 + 2) {
      *(int *)(&DAT_00ea1888 + iVar9) = *(int *)(&DAT_00ea1884 + iVar9);
    }
    iVar4 = *(int *)(&DAT_00ea1888 + iVar9);
    if (((iVar3 / 10 != iVar4 / 10) && (300 < *(int *)(&DAT_00ea1894 + iVar9))) && (9 < iVar4)) {
      if (iVar4 < 0x14) {
        uVar5 = 99;
      }
      else if (iVar4 < 0x1e) {
        uVar5 = 100;
      }
      else {
        if (0x27 < iVar4) goto LAB_004e854f;
        uVar5 = 0x65;
      }
LAB_004e853c: ;
      FUN_004e2970(uVar5,0,0);
      iVar8 = DAT_00eaac28;
    }
  }
  else if (*(int *)(&DAT_00ea1884 + iVar9) < iVar3) {
    *(int *)(&DAT_00ea1888 + iVar9) = iVar3 + -2;
    if (iVar3 + -2 < *(int *)(&DAT_00ea1884 + iVar9)) {
      *(int *)(&DAT_00ea1888 + iVar9) = *(int *)(&DAT_00ea1884 + iVar9);
    }
    iVar4 = *(int *)(&DAT_00ea1888 + iVar9);
    if (((iVar3 / 10 != iVar4 / 10) && (300 < *(int *)(&DAT_00ea1894 + iVar9))) || (iVar4 < 0x14)) {
      if (iVar4 < 10) {
        uVar5 = 0x5f;
      }
      else if (iVar4 < 0x14) {
        uVar5 = 0x60;
      }
      else if (iVar4 < 0x1e) {
        uVar5 = 0x61;
      }
      else {
        if (0x27 < iVar4) goto LAB_004e854f;
        uVar5 = 0x62;
      }
      goto LAB_004e853c;
    }
  }
LAB_004e854f: ;
  piVar6 = (int *)(&DAT_00ea1888 + iVar8 * 0x49d8);
  if (100 < *(int *)(&DAT_00ea1888 + iVar8 * 0x49d8)) {
    *piVar6 = 100;
  }
  if (*piVar6 < 0) {
    *piVar6 = 0;
  }
LAB_004e8579: ;
  piVar6 = &DAT_00799bd0;
  do {
    if ((((char)piVar6[-0x3c] == '\x01') && (*(char *)((int)piVar6 + -0xeb) != '\0')) &&
       ((*(char *)((int)piVar6 + -0x69) != '\0' &&
        ((*piVar6 != 0 && (iVar3 = *piVar6 + -1, *piVar6 = iVar3, iVar3 == 0)))))) {
      *(undefined1 *)((int)piVar6 + -0x69) = 0;
    }
    piVar6 = piVar6 + 0x42;
  } while ((int)piVar6 < 0x89b7c8);
  if (!bVar1) {
    FUN_00430250();
    FUN_004302b0();
  }
  return;
}

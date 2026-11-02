#include "restore_types.h"
#define DAT_00bcb9b4 (*(undefined *)0x00bcb9b4u)
#define DAT_00ea1888 (*(undefined4 *)0x00ea1888u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea4388 (*(undefined4 *)0x00ea4388u)
#define DAT_00ea439c (*(undefined4 *)0x00ea439cu)
#define DAT_00ea43a0 (*(undefined4 *)0x00ea43a0u)
#define DAT_00ea4468 (*(undefined4 *)0x00ea4468u)
#define DAT_00ea5b48 (*(undefined4 *)0x00ea5b48u)
#define DAT_00ea5bf4 (*(undefined4 *)0x00ea5bf4u)
#define DAT_00ea5c0a (*(undefined4 *)0x00ea5c0au)
#define DAT_00ea5ed8 (*(undefined4 *)0x00ea5ed8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int * FUN_0044bb90();
extern uint FUN_0044d700();

int FUN_0053d3a0(undefined4 *param_1,undefined1 *param_2)

{
  char cVar1;
  int iVar2;
  bool bVar3;
  bool bVar4;
  char cVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int local_4;
  
  *param_1 = 0;
  iVar9 = 0;
  param_1[1] = 0;
  bVar4 = false;
  bVar3 = false;
  iVar8 = 0;
  local_4 = 0;
  for (iVar6 = FUN_0044bb90(0,0); iVar6 != 0; iVar6 = FUN_0044bb90(iVar6,0)) {
    if ((*(uint *)(iVar6 + 0x28) >> 4 & 1) != 0) {
      if ((iVar6 != 0) && ((*(uint *)(iVar6 + 0x28) >> 2 & 1) != 0)) {
        bVar4 = true;
      }
      break;
    }
  }
  for (iVar6 = FUN_0044bb90(0,0); iVar6 != 0; iVar6 = FUN_0044bb90(iVar6,0)) {
    cVar5 = FUN_0044d700();
    if (cVar5 != '\0') {
      bVar3 = true;
      break;
    }
  }
  do {
    iVar10 = 0;
    iVar6 = 0;
    switch((int)(iVar9)) {
    case 0: ;
      iVar6 = *(int *)(&DAT_00ea1888 + DAT_00eaac28 * 0x49d8);
      if (iVar6 < 0x14) {
        iVar8 = 8;
        *param_2 = 0;
      }
      else if (iVar6 < 0x1e) {
        iVar8 = 6;
        *param_2 = 0;
      }
      else {
LAB_0053d622: ;
        if (0x27 < iVar6) goto LAB_0053d6ba;
        iVar8 = 4;
        *param_2 = 0;
      }
      break;
    case 1: ;
      if ((int)(&DAT_00ea4388)[DAT_00eaac28 * 0x1276] < 1) {
        iVar8 = 6;
        *param_2 = 0;
      }
      else {
        if (1 < (int)(&DAT_00ea4388)[DAT_00eaac28 * 0x1276]) goto LAB_0053d6ba;
        iVar8 = 4;
        *param_2 = 0;
      }
      break;
    case 2: ;
      if (bVar3) {
        if ((DAT_00bcb9b4 == 0) && ((&DAT_00ea5ed8)[DAT_00eaac28 * 0x49d8] == '\0')) {
          iVar8 = 8;
          *param_2 = 0;
        }
      }
      else {
LAB_0053d6ba: ;
        iVar8 = 1;
      }
      break;
    case 3: ;
      iVar8 = 10;
      piVar7 = (int *)(&DAT_00ea43a0 + DAT_00eaac28 * 0x49d8);
      do {
        iVar6 = iVar6 + *piVar7;
        piVar7 = piVar7 + 5;
        iVar8 = iVar8 + -1;
      } while (iVar8 != 0);
      piVar7 = &DAT_00ea439c + DAT_00eaac28 * 0x1276;
      iVar8 = 10;
      do {
        iVar2 = *piVar7;
        piVar7 = piVar7 + 5;
        iVar10 = iVar10 + iVar2;
        iVar8 = iVar8 + -1;
      } while (iVar8 != 0);
      if (iVar10 == 0) goto LAB_0053d6ba;
      iVar6 = 100 - (iVar6 * 100) / iVar10;
      if (iVar6 < 0x15) {
        if (iVar6 < 0xb) {
          if (iVar6 < 6) goto LAB_0053d6ba;
          iVar8 = 4;
          *param_2 = 0;
        }
        else {
          iVar8 = 6;
          *param_2 = 0;
        }
      }
      else {
        iVar8 = 8;
        *param_2 = 0;
      }
      break;
    case 4: ;
      iVar6 = 0;
      cVar5 = 'd';
      do {
        cVar1 = *(char *)((int)&DAT_00ea5c0a + iVar6 + DAT_00eaac28 * 0x49d8);
        if (cVar1 < cVar5) {
          cVar5 = cVar1;
        }
        iVar6 = iVar6 + 1;
      } while (iVar6 < 5);
      if (cVar5 < '\x15') {
        iVar8 = 6;
        *param_2 = 0;
      }
      else {
        if ('\x1e' < cVar5) goto LAB_0053d6ba;
        iVar8 = 4;
        *param_2 = 0;
      }
      break;
    case 5: ;
      if (bVar4) {
        iVar8 = 0;
      }
      else {
        iVar6 = (&DAT_00ea5b48)[DAT_00eaac28 * 0x1276];
        if (iVar6 < 1) {
          iVar8 = 8;
          *param_2 = 0;
        }
        else {
          if (0x13 < iVar6) goto LAB_0053d622;
          iVar8 = 6;
          *param_2 = 0;
        }
      }
      break;
    case 6: ;
      if (*(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8) == 0) goto LAB_0053d6ba;
      iVar6 = ((&DAT_00ea4468)[DAT_00eaac28 * 0x1276] * 100) /
              *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8);
      if (iVar6 < 0x12) {
        if (iVar6 < 0xb) {
          if (iVar6 < 6) goto LAB_0053d6ba;
          iVar8 = 4;
          *param_2 = 0;
        }
        else {
          iVar8 = 6;
          *param_2 = 0;
        }
      }
      else {
        iVar8 = 8;
        *param_2 = 0;
      }
      break;
    case 7: ;
      if ((int)(&DAT_00ea5bf4)[DAT_00eaac28 * 0x1276] < 1) {
        iVar8 = 6;
        *param_2 = 0;
      }
      else {
        if (0x13 < (int)(&DAT_00ea5bf4)[DAT_00eaac28 * 0x1276]) goto LAB_0053d6ba;
        iVar8 = 4;
        *param_2 = 0;
      }
    }
    local_4 = local_4 + iVar8;
    *(char *)((int)param_1 + iVar9) = (char)iVar8;
    iVar9 = iVar9 + 1;
    if (7 < iVar9) {
      if ((*(byte *)((int)param_1 + 3) == 1) && (*(char *)((int)param_1 + 6) != '\0')) {
        *(undefined1 *)((int)param_1 + 3) = 0;
        return local_4 + -1;
      }
      if ((1 < *(byte *)((int)param_1 + 3)) && (*(char *)((int)param_1 + 6) == '\x01')) {
        *(undefined1 *)((int)param_1 + 6) = 0;
        local_4 = local_4 + -1;
      }
      return local_4;
    }
  } while( true );
}

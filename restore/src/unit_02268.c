#include "restore_types.h"
#include "town_trade.h"
#define DAT_005d19d6 (*(undefined4 *)0x005d19d6u)
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_00784de0 (*(undefined *)0x00784de0u)
#define DAT_00784de8 (*(undefined4 *)0x00784de8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a1b (*(undefined4 *)0x00799a1bu)
#define DAT_00799a5d (*(undefined4 *)0x00799a5du)
#define DAT_00ea5fa0 (*(undefined4 *)0x00ea5fa0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
extern int * FUN_0044baf0();

extern int FUN_00488030();
extern uint FUN_004f5050();
extern undefined4 FUN_005150a0();

void FUN_00515130(undefined4 *param_1)

{
  byte bVar1;
  short sVar2;
  char cVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int *piVar7;
  short *psVar8;
  undefined4 *puVar9;
  bool bVar10;
  
  puVar9 = param_1;
  for (iVar6 = 0xf; iVar6 != 0; iVar6 = iVar6 + -1) {
    *puVar9 = 0;
    puVar9 = puVar9 + 1;
  }
  iVar6 = (int)(short)(&DAT_007999e8)[DAT_00eadd6c * 0x84];
  switch((int)(iVar6)) {
  case 0x1e: ;
  case 0x1f: ;
  case 0x20: ;
  case 0x21: ;
  case 0x22: ;
  case 0x23: ;
  case 0x24: ;
  case 0x25: ;
  case 0xe2: ;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < *(int *)(&DAT_00705eb8 + iVar6 * 0x28)) {
      param_1[1] = 1;
    }
    param_1[10] = 1;
    break;
  case 0x26: ;
  case 0x27: ;
  case 0x29: ;
  case 0x2a: ;
  case 0x2b: ;
  case 0x52: ;
  case 0x53: ;
  case 0x88: ;
  case 0x8a: ;
    param_1[0xd] = 1;
    break;
  case 0x28: ;
  case 0x2c: ;
  case 0x2d: ;
  case 0xa9: ;
    param_1[2] = 1;
    break;
  case 0x2e: ;
  case 0x2f: ;
  case 0x31: ;
  case 0xb4: ;
  case 0xce: ;
    param_1[8] = 1;
    goto LAB_0051572b;
  case 0x33: ;
  case 0x35: ;
    param_1[9] = 1;
    goto LAB_0051572b;
  case 0x37: ;
  case 0x51: ;
  case 0xa7: ;
  case 0xb8: ;
    param_1[0xd] = 1;
    goto LAB_0051572b;
  case 0x46: ;
  case 0x4c: ;
  case 0xbe: ;
  case 0xc2: ;
    param_1[7] = 1;
    if (*(int *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] * 0x28) <=
        (int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84]) break;
    goto LAB_00515784;
  case 0x47: ;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < *(int *)(&DAT_00705eb8 + iVar6 * 0x28)) {
      param_1[1] = 1;
    }
    iVar6 = 0;
    param_1[7] = 1;
    for (iVar4 = FUN_0044baf0(0); iVar4 != 0; iVar4 = FUN_0044baf0(iVar4)) {
      cVar3 = FUN_0044d4c0((const struct RestoreTownRecord *)(uintptr_t)iVar4);
      if (cVar3 != '\0') {
        if (4 < iVar6) break;
        if ((0 < *(short *)(iVar4 + 4)) && (*(short *)(iVar4 + 4) < 9)) goto LAB_00515603;
        iVar6 = iVar6 + 1;
      }
    }
    break;
  case 0x48: ;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < *(int *)(&DAT_00705eb8 + iVar6 * 0x28)) {
      param_1[1] = 1;
    }
    iVar6 = 0;
    param_1[6] = 1;
    for (iVar4 = FUN_0044baf0(0); iVar4 != 0; iVar4 = FUN_0044baf0(iVar4)) {
      cVar3 = FUN_0044d4c0((const struct RestoreTownRecord *)(uintptr_t)iVar4);
      if (cVar3 != '\0') {
        if (4 < iVar6) break;
        if (*(short *)(iVar4 + 4) < 0x24) goto LAB_00515603;
        iVar6 = iVar6 + 1;
      }
    }
    break;
  case 0x4a: ;
  case 0xb5: ;
  case 0xb6: ;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < *(int *)(&DAT_00705eb8 + iVar6 * 0x28)) {
      param_1[1] = 1;
    }
    param_1[3] = 1;
    break;
  case 0x4b: ;
  case 0x6d: ;
  case 0x6e: ;
  case 0x6f: ;
  case 0x71: ;
  case 0x72: ;
  case 0xa2: ;
  case 0xc0: ;
  case 0xc3: ;
    param_1[6] = 1;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] <
        *(int *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] * 0x28)) {
      param_1[1] = 1;
    }
    uVar5 = (uint)(byte)(&DAT_00799a1b)[DAT_00eadd6c * 0x108];
    switch((int)(uVar5)) {
    case 1: ;
    case 8: ;
    case 10: ;
    case 0xd: ;
    case 0xf: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1c: ;
    case 0x1e: ;
switchD_00515406_caseD_1: ;
      bVar10 = (&DAT_00784de8)[uVar5] == 0;
      goto LAB_00515721;
    }
    break;
  case 0x4d: ;
  case 0x4e: ;
  case 0x4f: ;
    param_1[4] = 1;
    goto LAB_0051572b;
  case 0x56: ;
  case 0x57: ;
  case 0xa1: ;
  case 0xbb: ;
  case 0xbc: ;
  case 0xbd: ;
    param_1[0xc] = 1;
    param_1[5] = 1;
LAB_0051572b: ;
    iVar6 = (int)(short)(&DAT_007999e8)[DAT_00eadd6c * 0x84];
switchD_00515176_caseD_5a: ;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < *(int *)(&DAT_00705eb8 + iVar6 * 0x28)) {
LAB_00515784: ;
      param_1[1] = 1;
    }
    break;
  case 0x5a: ;
    goto switchD_00515176_caseD_5a;
  case 0x5c: ;
    param_1[8] = 1;
    break;
  case 0x5e: ;
  case 0x5f: ;
  case 0xab: ;
  case 0xad: ;
  case 0xca: ;
    param_1[2] = 1;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] <
        *(int *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] * 0x28)) {
      param_1[1] = 1;
    }
    uVar5 = (uint)(byte)(&DAT_00799a1b)[DAT_00eadd6c * 0x108];
    switch((int)(uVar5)) {
    case 1: ;
    case 8: ;
    case 10: ;
    case 0xd: ;
    case 0xf: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1c: ;
    case 0x1e: ;
      goto switchD_00515406_caseD_1;
    }
    break;
  case 0x6a: ;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < *(int *)(&DAT_00705eb8 + iVar6 * 0x28)) {
      param_1[1] = 1;
    }
    cVar3 = FUN_004f5050(0x18,1);
    if (cVar3 != '\0') {
      param_1[0xe] = 1;
    }
    uVar5 = (uint)(byte)(&DAT_00799a1b)[DAT_00eadd6c * 0x108];
    switch((int)(uVar5)) {
    case 1: ;
    case 8: ;
    case 10: ;
    case 0xd: ;
    case 0xf: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1c: ;
    case 0x1e: ;
      goto switchD_00515406_caseD_1;
    }
    break;
  case 0x6b: ;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < *(int *)(&DAT_00705eb8 + iVar6 * 0x28)) {
      param_1[1] = 1;
    }
    cVar3 = FUN_004f5050(0x19,1);
    if (cVar3 != '\0') {
      param_1[0xe] = 1;
    }
    uVar5 = (uint)(byte)(&DAT_00799a1b)[DAT_00eadd6c * 0x108];
    switch((int)(uVar5)) {
    case 1: ;
    case 8: ;
    case 10: ;
    case 0xd: ;
    case 0xf: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1c: ;
    case 0x1e: ;
      goto switchD_00515406_caseD_1;
    }
    break;
  case 0x6c: ;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < *(int *)(&DAT_00705eb8 + iVar6 * 0x28)) {
      param_1[1] = 1;
    }
    cVar3 = FUN_004f5050(0x14,1);
    if (cVar3 != '\0') {
      param_1[0xe] = 1;
    }
    uVar5 = (uint)(byte)(&DAT_00799a1b)[DAT_00eadd6c * 0x108];
    switch((int)(uVar5)) {
    case 1: ;
    case 8: ;
    case 10: ;
    case 0xd: ;
    case 0xf: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1c: ;
    case 0x1e: ;
      goto switchD_00515406_caseD_1;
    }
    break;
  case 0x70: ;
  case 0xcd: ;
    param_1[2] = 1;
    param_1[6] = 1;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] <
        *(int *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] * 0x28)) {
      param_1[1] = 1;
    }
    uVar5 = (uint)(byte)(&DAT_00799a1b)[DAT_00eadd6c * 0x108];
    switch((int)(uVar5)) {
    case 1: ;
    case 8: ;
    case 10: ;
    case 0xd: ;
    case 0xf: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1c: ;
    case 0x1e: ;
      goto switchD_00515406_caseD_1;
    }
    break;
  case 0x73: ;
    param_1[7] = 1;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] <
        *(int *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] * 0x28)) {
      param_1[1] = 1;
    }
    uVar5 = (uint)(byte)(&DAT_00799a1b)[DAT_00eadd6c * 0x108];
    switch((int)(uVar5)) {
    case 1: ;
    case 8: ;
    case 10: ;
    case 0xd: ;
    case 0xf: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1c: ;
    case 0x1e: ;
      goto switchD_00515406_caseD_1;
    }
    break;
  case 0xb1: ;
  case 0xb2: ;
  case 0xb3: ;
    if (*(int *)(&DAT_00705eb8 + iVar6 * 0x28) <= (int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84])
    goto switchD_00515176_caseD_b7;
    param_1[1] = 1;
    param_1[0xe] = 1;
    break;
  case 0xb7: ;
  case 0xcf: ;
  case 0xd0: ;
  case 0xd2: ;
  case 0xd5: ;
  case 0xde: ;
switchD_00515176_caseD_b7: ;
    param_1[0xe] = 1;
    break;
  case 199: ;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < *(int *)(&DAT_00705eb8 + iVar6 * 0x28)) {
      param_1[1] = 1;
    }
    psVar8 = &DAT_00784de0;
    piVar7 = &DAT_00ea5fa0 + DAT_00eaac28 * 0x1276;
    do {
      if (*piVar7 == 0) {
        cVar3 = (&DAT_005d19d6)[*psVar8 * 0x10];
      }
      else {
        cVar3 = (&DAT_005d19d6)[*piVar7 * 0x10];
      }
      if (cVar3 != '\0') goto switchD_00515176_caseD_b7;
      psVar8 = psVar8 + 1;
      piVar7 = piVar7 + 1;
    } while ((int)psVar8 < 0x784de6);
    break;
  case 0xcb: ;
    param_1[6] = 1;
    param_1[9] = 1;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] <
        *(int *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] * 0x28)) {
      param_1[1] = 1;
    }
    uVar5 = (uint)(byte)(&DAT_00799a1b)[DAT_00eadd6c * 0x108];
    switch((int)(uVar5)) {
    case 1: ;
    case 8: ;
    case 10: ;
    case 0xd: ;
    case 0xf: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1c: ;
    case 0x1e: ;
      goto switchD_00515406_caseD_1;
    }
    break;
  case 0xcc: ;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < *(int *)(&DAT_00705eb8 + iVar6 * 0x28)) {
      param_1[1] = 1;
    }
    cVar3 = FUN_004f5050(0xc,1);
    bVar10 = cVar3 == '\0';
LAB_00515721: ;
    if (!bVar10) goto switchD_00515176_caseD_b7;
    break;
  case 0xd8: ;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < *(int *)(&DAT_00705eb8 + iVar6 * 0x28)) {
      param_1[1] = 1;
    }
    cVar3 = FUN_004f5050(0x1a,1);
    if (cVar3 != '\0') {
      param_1[0xe] = 1;
    }
    uVar5 = (uint)(byte)(&DAT_00799a1b)[DAT_00eadd6c * 0x108];
    switch((int)(uVar5)) {
    case 1: ;
    case 8: ;
    case 10: ;
    case 0xd: ;
    case 0xf: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1c: ;
    case 0x1e: ;
      goto switchD_00515406_caseD_1;
    }
    break;
  case 0xdd: ;
    if ((int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] < *(int *)(&DAT_00705eb8 + iVar6 * 0x28)) {
      param_1[1] = 1;
    }
    cVar3 = FUN_004f5050(0x1e,1);
    if (cVar3 != '\0') {
      param_1[0xe] = 1;
    }
    uVar5 = (uint)(byte)(&DAT_00799a1b)[DAT_00eadd6c * 0x108];
    switch((int)(uVar5)) {
    case 1: ;
    case 8: ;
    case 10: ;
    case 0xd: ;
    case 0xf: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1c: ;
    case 0x1e: ;
      goto switchD_00515406_caseD_1;
    }
  }
switchD_00515176_caseD_30: ;
  if (((short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] < 10) ||
     (0x1d < (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84])) goto LAB_0051581c;
  switch((uint)(byte)(&DAT_00799a1b)[DAT_00eadd6c * 0x108]) {
  case 1: ;
  case 8: ;
  case 10: ;
  case 0xd: ;
  case 0xf: ;
  case 0x11: ;
  case 0x12: ;
  case 0x13: ;
  case 0x14: ;
  case 0x17: ;
  case 0x18: ;
  case 0x19: ;
  case 0x1a: ;
  case 0x1c: ;
  case 0x1e: ;
    if ((&DAT_00784de8)[(byte)(&DAT_00799a1b)[DAT_00eadd6c * 0x108]] != 0) {
      param_1[0xe] = 1;
    }
  }
  bVar1 = (&DAT_00799a5d)[DAT_00eadd6c * 0x108];
  if (bVar1 < 0x1e) {
    uVar5 = bVar1 + 0x1e;
  }
  else {
    uVar5 = (uint)bVar1;
  }
  if (uVar5 < 0x44) {
    iVar6 = FUN_005150a0(uVar5);
    if (iVar6 != 0) goto LAB_00515816;
  }
  else {
    iVar6 = 6;
LAB_00515816: ;
    param_1[iVar6] = 1;
  }
  param_1[7] = 1;
LAB_0051581c: ;
  cVar3 = FUN_00488030(DAT_00eadd6c);
  if ((cVar3 != '\0') &&
     (param_1[7] = 1,
     (int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] <
     *(int *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] * 0x28))) {
    param_1[1] = 1;
  }
  if ((0x8b < (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84]) &&
     ((short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] < 0x96)) {
    param_1[0xb] = 1;
  }
  sVar2 = (&DAT_007999e8)[DAT_00eadd6c * 0x84];
  if (((((0x40 < sVar2) && (sVar2 < 0x46)) || ((0x3b < sVar2 && (sVar2 < 0x41)))) || (sVar2 == 0xd1)
      ) && (param_1[0xb] = 1,
           (int)(short)(&DAT_00799a18)[DAT_00eadd6c * 0x84] <
           *(int *)(&DAT_00705eb8 + (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] * 0x28))) {
    param_1[1] = 1;
  }
  return;
LAB_00515603: ;
  param_1[4] = 1;
  goto switchD_00515176_caseD_30;
}

#include "restore_types.h"
#define DAT_0070cdcc (*(undefined4 *)0x0070cdccu)
#define DAT_0070cdd0 (*(undefined4 *)0x0070cdd0u)
#define DAT_0070cdd4 (*(undefined4 *)0x0070cdd4u)
#define DAT_0070cdd8 (*(undefined4 *)0x0070cdd8u)
#define DAT_0070cddc (*(undefined4 *)0x0070cddcu)
#define DAT_0070cde0 (*(undefined4 *)0x0070cde0u)
#define DAT_0070cde8 (*(undefined4 *)0x0070cde8u)
#define DAT_0070cdec (*(undefined4 *)0x0070cdecu)
#define DAT_0070cdf0 (*(undefined4 *)0x0070cdf0u)
#define DAT_0070cdf4 (*(undefined4 *)0x0070cdf4u)
#define DAT_0070cdf8 (*(undefined4 *)0x0070cdf8u)
#define DAT_0070cdfc (*(undefined4 *)0x0070cdfcu)
#define DAT_0070ce00 (*(undefined4 *)0x0070ce00u)
#define DAT_0070ce04 (*(undefined4 *)0x0070ce04u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a0d (*(undefined4 *)0x00799a0du)
#define DAT_00799a17 (*(undefined4 *)0x00799a17u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799a3e (*(undefined4 *)0x00799a3eu)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a42 (*(undefined4 *)0x00799a42u)
#define DAT_00799a46 (*(undefined4 *)0x00799a46u)
#define DAT_00799a48 (*(undefined4 *)0x00799a48u)
#define DAT_00799a4a (*(undefined4 *)0x00799a4au)
#define DAT_00799a4b (*(undefined4 *)0x00799a4bu)
#define DAT_00799a4d (*(undefined4 *)0x00799a4du)
#define DAT_00799a56 (*(undefined4 *)0x00799a56u)
#define DAT_00799a58 (*(undefined4 *)0x00799a58u)
#define DAT_00799a59 (*(undefined4 *)0x00799a59u)
#define DAT_00799a5a (*(undefined4 *)0x00799a5au)
#define DAT_00799a5b (*(undefined4 *)0x00799a5bu)
#define DAT_00799a5d (*(undefined4 *)0x00799a5du)
#define DAT_00799a96 (*(undefined4 *)0x00799a96u)
#define DAT_00ea5f14 (*(undefined4 *)0x00ea5f14u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eadcd4 (*(undefined *)0x00eadcd4u)
#define DAT_00f53dd8 (*(undefined *)0x00f53dd8u)
#define DAT_00f53ddc (*(undefined4 *)0x00f53ddcu)

void FUN_00454190(int param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int local_38;
  int local_34;
  int local_30;
  int local_2c;
  uint local_28;
  int local_c;
  
  iVar4 = param_1;
  iVar12 = param_1 * 0x108;
  iVar11 = (int)(short)(&DAT_007999ea)[param_1 * 0x84];
  iVar7 = iVar11 * 0x58;
  if ((int)(&DAT_0070cdcc)[iVar11 * 0x16] < (int)(char)(&DAT_00799a96)[iVar12]) {
    local_28 = (uint)(*(int *)(&DAT_0070cdd0 + iVar7) <= (int)(char)(&DAT_00799a96)[iVar12]);
  }
  else {
    local_28 = 0xffffffff;
  }
  iVar2 = *(int *)(&DAT_0070cdd4 + iVar7);
  iVar3 = *(int *)(&DAT_0070cde0 + iVar7);
  cVar1 = (&DAT_007999db)[iVar12];
  iVar6 = *(int *)(&DAT_0070cddc + iVar7);
  iVar8 = *(int *)(&DAT_0070cdf4 + iVar7);
  local_34 = *(int *)(&DAT_0070cdf8 + iVar7);
  if (((cVar1 == '\x01') && (0 < local_34)) &&
     (local_34 = (int)(local_34 + 2 + (local_34 + 2 >> 0x1f & 3U)) >> 2, local_34 < 1)) {
    local_34 = 1;
  }
  local_30 = *(int *)(&DAT_0070cdfc + iVar7);
  if (((cVar1 == '\x01') && (0 < local_30)) &&
     (local_30 = (int)(local_30 + 2 + (local_30 + 2 >> 0x1f & 3U)) >> 2, local_30 < 1)) {
    local_30 = 1;
  }
  local_2c = *(int *)(&DAT_0070ce00 + iVar7);
  if (((cVar1 == '\x01') && (0 < local_2c)) &&
     (local_2c = (int)(local_2c + 2 + (local_2c + 2 >> 0x1f & 3U)) >> 2, local_2c < 1)) {
    local_2c = 1;
  }
  local_38 = *(int *)(&DAT_0070ce04 + iVar7);
  if (((cVar1 == '\x01') && (0 < local_38)) &&
     (local_38 = (int)(local_38 + 2 + (local_38 + 2 >> 0x1f & 3U)) >> 2, local_38 < 1)) {
    local_38 = 1;
  }
  iVar10 = 0;
  local_c = 0;
  if (0 < DAT_00f53dd8) {
    piVar9 = &DAT_00f53ddc;
    iVar13 = DAT_00f53dd8;
    do {
      if (*piVar9 != 0) {
        switch(*piVar9) {
        case 1: ;
          iVar5 = 0;
          break;
        case 2: ;
          iVar5 = 1;
          break;
        case 3: ;
          iVar5 = 2;
          break;
        case 4: ;
          iVar5 = 3;
          break;
        case 5: ;
          iVar5 = 4;
          break;
        case 6: ;
          iVar5 = 5;
          break;
        case 7: ;
          iVar5 = 6;
          break;
        case 8: ;
          iVar5 = 7;
          break;
default: ;
          iVar5 = -1;
          break;
        case 0xd: ;
          iVar5 = 9;
          break;
        case 0xf: ;
          iVar5 = 8;
          break;
        case 0x11: ;
          iVar5 = 10;
          break;
        case 0x13: ;
          iVar5 = 0xb;
        }
        if ((&DAT_00799a2c)[param_1 * 0x84 + iVar5] != 0) {
          iVar10 = iVar10 + 1;
        }
      }
      piVar9 = piVar9 + 1;
      iVar13 = iVar13 + -1;
      local_c = iVar10;
    } while (iVar13 != 0);
  }
  if (local_28 == 0xffffffff) {
    (&DAT_00799a5d)[iVar12] = 0;
    return;
  }
  if (*(int *)(&DAT_0070cdd8 + iVar7) == 1) {
    if (((&DAT_00799a0d)[iVar12] == '\0') && ((&DAT_00799a17)[iVar12] == '\0')) {
      (&DAT_00799a5d)[iVar12] = 1;
      return;
    }
  }
  else if ((*(int *)(&DAT_0070cdd8 + iVar7) == 2) && ((&DAT_00799a0d)[iVar12] == '\0')) {
    (&DAT_00799a5d)[iVar12] = 2;
    return;
  }
  cVar1 = (&DAT_00799a58)[iVar12];
  if (cVar1 < iVar2) {
    if (cVar1 == '\0') {
      (&DAT_00799a5d)[iVar12] = 3;
      return;
    }
    if (iVar2 < 10) {
      (&DAT_00799a5d)[iVar12] = 4;
      return;
    }
    if (iVar2 < 0x19) {
      (&DAT_00799a5d)[iVar12] = 5;
      return;
    }
    if (iVar2 < 0x32) {
      (&DAT_00799a5d)[iVar12] = 6;
      return;
    }
    if (iVar2 < 0x50) {
      (&DAT_00799a5d)[iVar12] = 7;
      return;
    }
    (&DAT_00799a5d)[iVar12] = 8;
    return;
  }
  if (local_c < iVar8) {
    if (iVar8 == 1) {
      (&DAT_00799a5d)[iVar12] = 9;
      return;
    }
    if (iVar8 == 2) {
      (&DAT_00799a5d)[iVar12] = 10;
      return;
    }
    if (iVar8 == 3) {
      (&DAT_00799a5d)[iVar12] = 0xb;
      return;
    }
  }
  if ((char)(&DAT_00799a59)[iVar12] < iVar3) {
    if (iVar3 == 1) {
      (&DAT_00799a5d)[iVar12] = 0xe;
      return;
    }
    if (iVar3 == 2) {
      if ((&DAT_00799a4a)[iVar12] != '\0') {
        (&DAT_00799a5d)[iVar12] = 0xf;
        return;
      }
      if ((&DAT_00799a4b)[iVar12] != '\0') {
        (&DAT_00799a5d)[iVar12] = 0x10;
        return;
      }
    }
    else if (iVar3 == 3) {
      (&DAT_00799a5d)[iVar12] = 0x11;
      return;
    }
  }
  if ((int)(uint)(byte)(&DAT_00799a48)[iVar12] < *(int *)(&DAT_0070cdec + iVar7)) {
    (&DAT_00799a5d)[iVar12] = 0x12;
    return;
  }
  if ((short)(&DAT_00799a3e)[param_1 * 0x84] < local_34) {
    (&DAT_00799a5d)[iVar12] = 0x13;
    return;
  }
  if ((char)(&DAT_00799a5b)[iVar12] < iVar6) {
    if (iVar6 == 1) {
      (&DAT_00799a5d)[iVar12] = 0x14;
      return;
    }
    if (iVar6 == 2) {
      (&DAT_00799a5d)[iVar12] = 0x15;
      return;
    }
    if (iVar6 == 3) {
      (&DAT_00799a5d)[iVar12] = 0x16;
      return;
    }
  }
  if ((int)(uint)(byte)(&DAT_00799a4d)[iVar12] < *(int *)(&DAT_0070cde8 + iVar7)) {
    (&DAT_00799a5d)[iVar12] = 0x17;
    return;
  }
  if ((int)(char)(&DAT_00799a5a)[iVar12] < *(int *)(&DAT_0070cdf0 + iVar7)) {
    if (*(int *)(&DAT_0070cdf0 + iVar7) == 1) {
      (&DAT_00799a5d)[iVar12] = 0x18;
      return;
    }
    if ((&DAT_00799a46)[iVar12] != '\0') {
      (&DAT_00799a5d)[iVar12] = 0x19;
      return;
    }
    (&DAT_00799a5d)[iVar12] = 0x1a;
    return;
  }
  if ((short)(&DAT_00799a3c)[param_1 * 0x84] < local_38) {
    (&DAT_00799a5d)[iVar12] = 0x1d;
    return;
  }
  if ((short)(&DAT_00799a40)[param_1 * 0x84] < local_30) {
    (&DAT_00799a5d)[iVar12] = 0x1b;
    return;
  }
  if ((short)(&DAT_00799a42)[param_1 * 0x84] < local_2c) {
    (&DAT_00799a5d)[iVar12] = 0x48;
    return;
  }
  if ((1 < local_2c) && ((int)(&DAT_00ea5f14)[DAT_00eaac28 * 0x1276] < 2)) {
    (&DAT_00799a5d)[iVar12] = 0x46;
    return;
  }
  if (0x12 < iVar11) {
    (&DAT_00799a5d)[iVar12] = 0x3c;
    return;
  }
  iVar8 = (iVar11 + 1) * 0x58;
  iVar7 = *(int *)(&DAT_0070cdd4 + iVar8);
  iVar11 = *(int *)(&DAT_0070cde0 + iVar8);
  iVar2 = *(int *)(&DAT_0070cddc + iVar8);
  iVar3 = *(int *)(&DAT_0070cdf4 + iVar8);
  iVar6 = *(int *)(&DAT_0070cdf8 + iVar8);
  if ((((&DAT_007999db)[iVar12] == '\x01') && (0 < iVar6)) &&
     (iVar6 = (int)(iVar6 + 2 + (iVar6 + 2 >> 0x1f & 3U)) >> 2, iVar6 < 1)) {
    iVar6 = 1;
  }
  param_1 = *(int *)(&DAT_0070cdfc + iVar8);
  if ((((&DAT_007999db)[iVar12] == '\x01') && (0 < param_1)) &&
     (param_1 = (int)(param_1 + 2 + (param_1 + 2 >> 0x1f & 3U)) >> 2, param_1 < 1)) {
    param_1 = 1;
  }
  local_38 = *(int *)(&DAT_0070ce00 + iVar8);
  if ((((&DAT_007999db)[iVar12] == '\x01') && (0 < local_38)) &&
     (local_38 = (int)(local_38 + 2 + (local_38 + 2 >> 0x1f & 3U)) >> 2, local_38 < 1)) {
    local_38 = 1;
  }
  iVar10 = *(int *)(&DAT_0070ce04 + iVar8);
  if ((((&DAT_007999db)[iVar12] == '\x01') && (0 < iVar10)) &&
     (iVar10 = (int)(iVar10 + 2 + (iVar10 + 2 >> 0x1f & 3U)) >> 2, iVar10 < 1)) {
    iVar10 = 1;
  }
  if (local_28 == 0) {
    if (DAT_00eadcd4 != 0) {
      (&DAT_00799a5d)[iVar12] = 0x3e;
      return;
    }
    (&DAT_00799a5d)[iVar12] = 0x1e;
    return;
  }
  if (*(int *)(&DAT_0070cdd8 + iVar8) == 1) {
    if (((&DAT_00799a0d)[iVar12] == '\0') && ((&DAT_00799a17)[iVar12] == '\0')) {
      (&DAT_00799a5d)[iVar12] = 0x1f;
      return;
    }
  }
  else if ((*(int *)(&DAT_0070cdd8 + iVar8) == 2) && ((&DAT_00799a0d)[iVar12] == '\0')) {
    (&DAT_00799a5d)[iVar12] = 0x20;
    return;
  }
  if (cVar1 < iVar7) {
    if ((&DAT_00799a58)[iVar12] == '\0') {
      (&DAT_00799a5d)[iVar12] = 0x21;
      return;
    }
    if (iVar7 < 10) {
      (&DAT_00799a5d)[iVar12] = 0x22;
      return;
    }
    if (iVar7 < 0x19) {
      (&DAT_00799a5d)[iVar12] = 0x23;
      return;
    }
    if (iVar7 < 0x32) {
      (&DAT_00799a5d)[iVar12] = 0x24;
      return;
    }
    if (iVar7 < 0x50) {
      (&DAT_00799a5d)[iVar12] = 0x25;
      return;
    }
    (&DAT_00799a5d)[iVar12] = 0x26;
    return;
  }
  if (local_c < iVar3) {
    if (iVar3 == 1) {
      (&DAT_00799a5d)[iVar12] = 0x27;
      return;
    }
    if (iVar3 == 2) {
      (&DAT_00799a5d)[iVar12] = 0x28;
      return;
    }
    if (iVar3 == 3) {
      (&DAT_00799a5d)[iVar12] = 0x29;
      return;
    }
  }
  if ((char)(&DAT_00799a59)[iVar12] < iVar11) {
    if (iVar11 == 1) {
      (&DAT_00799a5d)[iVar12] = 0x2c;
      return;
    }
    if (iVar11 == 2) {
      if ((&DAT_00799a4a)[iVar12] != '\0') {
        (&DAT_00799a5d)[iVar12] = 0x2d;
        return;
      }
      if ((&DAT_00799a4b)[iVar12] != '\0') {
        (&DAT_00799a5d)[iVar12] = 0x2e;
        return;
      }
    }
  }
  if ((int)(uint)(byte)(&DAT_00799a48)[iVar12] < *(int *)(&DAT_0070cdec + iVar8)) {
    (&DAT_00799a5d)[iVar12] = 0x30;
    return;
  }
  if ((short)(&DAT_00799a3e)[iVar4 * 0x84] < iVar6) {
    (&DAT_00799a5d)[iVar12] = 0x31;
    return;
  }
  if ((char)(&DAT_00799a5b)[iVar12] < iVar2) {
    if (iVar2 == 1) {
      (&DAT_00799a5d)[iVar12] = 0x32;
      return;
    }
    if (iVar2 == 2) {
      (&DAT_00799a5d)[iVar12] = 0x33;
      return;
    }
    if (iVar2 == 3) {
      (&DAT_00799a5d)[iVar12] = 0x34;
      return;
    }
  }
  if ((int)(uint)(byte)(&DAT_00799a4d)[iVar12] < *(int *)(&DAT_0070cde8 + iVar8)) {
    (&DAT_00799a5d)[iVar12] = 0x35;
    return;
  }
  if ((int)(char)(&DAT_00799a5a)[iVar12] < *(int *)(&DAT_0070cdf0 + iVar8)) {
    if (*(int *)(&DAT_0070cdf0 + iVar8) == 1) {
      (&DAT_00799a5d)[iVar12] = 0x36;
      return;
    }
    if ((&DAT_00799a46)[iVar12] != '\0') {
      (&DAT_00799a5d)[iVar12] = 0x37;
      return;
    }
    (&DAT_00799a5d)[iVar12] = 0x38;
    return;
  }
  if ((short)(&DAT_00799a3c)[iVar4 * 0x84] < iVar10) {
    (&DAT_00799a5d)[iVar12] = 0x3b;
    return;
  }
  if ((short)(&DAT_00799a40)[iVar4 * 0x84] < param_1) {
    (&DAT_00799a5d)[iVar12] = 0x39;
    return;
  }
  if ((short)(&DAT_00799a42)[iVar4 * 0x84] < local_38) {
    (&DAT_00799a5d)[iVar12] = 0x44;
    return;
  }
  if ((1 < local_38) && ((int)(&DAT_00ea5f14)[DAT_00eaac28 * 0x1276] < 2)) {
    (&DAT_00799a5d)[iVar12] = 0x4a;
    return;
  }
  (&DAT_00799a5d)[iVar12] = 0x3d;
  if ((&DAT_00799a56)[iVar12] == '\x01') {
    (&DAT_00799a5d)[iVar12] = 0x40;
  }
  return;
}

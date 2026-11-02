#include "restore_types.h"
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
#define DAT_00799a48 (*(undefined4 *)0x00799a48u)
#define DAT_00799a4d (*(undefined4 *)0x00799a4du)
#define DAT_00799a58 (*(undefined4 *)0x00799a58u)
#define DAT_00799a59 (*(undefined4 *)0x00799a59u)
#define DAT_00799a5a (*(undefined4 *)0x00799a5au)
#define DAT_00799a5b (*(undefined4 *)0x00799a5bu)
#define DAT_00ea5b74 (*(undefined4 *)0x00ea5b74u)
#define DAT_00ea5b78 (*(undefined4 *)0x00ea5b78u)
#define DAT_00ea5b7c (*(undefined4 *)0x00ea5b7cu)
#define DAT_00ea5b80 (*(undefined4 *)0x00ea5b80u)
#define DAT_00ea5b84 (*(undefined4 *)0x00ea5b84u)
#define DAT_00ea5b88 (*(undefined4 *)0x00ea5b88u)
#define DAT_00ea5b8c (*(undefined4 *)0x00ea5b8cu)
#define DAT_00ea5b94 (*(undefined4 *)0x00ea5b94u)
#define DAT_00ea5b98 (*(undefined4 *)0x00ea5b98u)
#define DAT_00ea5b9c (*(undefined4 *)0x00ea5b9cu)
#define DAT_00ea5bac (*(undefined4 *)0x00ea5bacu)
#define DAT_00ea5bb0 (*(undefined4 *)0x00ea5bb0u)
#define DAT_00ea5bb4 (*(undefined4 *)0x00ea5bb4u)
#define DAT_00ea5bb8 (*(undefined4 *)0x00ea5bb8u)
#define DAT_00ea5bbc (*(undefined4 *)0x00ea5bbcu)
#define DAT_00ea5bc0 (*(undefined4 *)0x00ea5bc0u)
#define DAT_00ea5bc4 (*(undefined4 *)0x00ea5bc4u)
#define DAT_00ea5bc8 (*(undefined4 *)0x00ea5bc8u)
#define DAT_00ea5bcc (*(undefined4 *)0x00ea5bccu)
#define DAT_00ea5bf8 (*(undefined4 *)0x00ea5bf8u)
#define DAT_00ea5f14 (*(undefined4 *)0x00ea5f14u)
#define DAT_00ea5f6c (*(undefined4 *)0x00ea5f6cu)
#define DAT_00ea5fb8 (*(undefined4 *)0x00ea5fb8u)
#define DAT_00ea5fbc (*(undefined4 *)0x00ea5fbcu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53dd8 (*(undefined *)0x00f53dd8u)
#define DAT_00f53ddc (*(undefined4 *)0x00f53ddcu)

undefined4 FUN_00453b40(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int *piVar11;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  
  iVar8 = DAT_00eaac28;
  iVar10 = param_1 * 0x108;
  iVar9 = (int)(short)(&DAT_007999ea)[param_1 * 0x84];
  if (param_2 != 0) {
    iVar9 = iVar9 + 1;
  }
  cVar1 = (&DAT_007999db)[iVar10];
  iVar9 = iVar9 * 0x58;
  iVar2 = *(int *)(&DAT_0070cddc + iVar9);
  iVar3 = *(int *)(&DAT_0070cde8 + iVar9);
  iVar4 = *(int *)(&DAT_0070cdec + iVar9);
  iVar5 = *(int *)(&DAT_0070cdf0 + iVar9);
  iVar6 = *(int *)(&DAT_0070cde0 + iVar9);
  iVar7 = *(int *)(&DAT_0070cdf4 + iVar9);
  local_24 = *(int *)(&DAT_0070cdf8 + iVar9);
  if (((cVar1 == '\x01') && (0 < local_24)) &&
     (local_24 = (int)(local_24 + 2 + (local_24 + 2 >> 0x1f & 3U)) >> 2, local_24 < 1)) {
    local_24 = 1;
  }
  local_20 = *(int *)(&DAT_0070cdfc + iVar9);
  if (((cVar1 == '\x01') && (0 < local_20)) &&
     (local_20 = (int)(local_20 + 2 + (local_20 + 2 >> 0x1f & 3U)) >> 2, local_20 < 1)) {
    local_20 = 1;
  }
  local_1c = *(int *)(&DAT_0070ce00 + iVar9);
  if (((cVar1 == '\x01') && (0 < local_1c)) &&
     (local_1c = (int)(local_1c + 2 + (local_1c + 2 >> 0x1f & 3U)) >> 2, local_1c < 1)) {
    local_1c = 1;
  }
  param_2 = *(int *)(&DAT_0070ce04 + iVar9);
  if (((cVar1 == '\x01') && (0 < param_2)) &&
     (param_2 = (int)(param_2 + 2 + (param_2 + 2 >> 0x1f & 3U)) >> 2, param_2 < 1)) {
    param_2 = 1;
  }
  if ((&DAT_00799a0d)[iVar10] == '\0') {
    if (1 < *(int *)(&DAT_0070cdd8 + iVar9)) {
      (&DAT_00ea5b74)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5b74)[DAT_00eaac28 * 0x1276] + 1;
      return 0;
    }
    if ((*(int *)(&DAT_0070cdd8 + iVar9) == 1) && ((&DAT_00799a17)[iVar10] == '\0')) {
      (&DAT_00ea5b78)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5b78)[DAT_00eaac28 * 0x1276] + 1;
      return 0;
    }
  }
  if ((int)(char)(&DAT_00799a58)[iVar10] < *(int *)(&DAT_0070cdd4 + iVar9)) {
    if ((&DAT_00799a58)[iVar10] == '\0') {
      (&DAT_00ea5bf8)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5bf8)[DAT_00eaac28 * 0x1276] + 1;
      return 0;
    }
    (&DAT_00ea5b7c)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5b7c)[DAT_00eaac28 * 0x1276] + 1;
    return 0;
  }
  if ((char)(&DAT_00799a59)[iVar10] < iVar6) {
    if ((&DAT_00799a59)[iVar10] != '\0') {
      (&DAT_00ea5b80)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5b80)[DAT_00eaac28 * 0x1276] + 1;
      return 0;
    }
    (&DAT_00ea5b84)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5b84)[DAT_00eaac28 * 0x1276] + 1;
    return 0;
  }
  if (iVar6 == 2) {
    (&DAT_00ea5b88)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5b88)[DAT_00eaac28 * 0x1276] + 1;
    (&DAT_00ea5b8c)[iVar8 * 0x1276] = (&DAT_00ea5b8c)[iVar8 * 0x1276] + 1;
  }
  else if (iVar6 == 1) {
    (&DAT_00ea5b88)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5b88)[DAT_00eaac28 * 0x1276] + 1;
  }
  if ((char)(&DAT_00799a5b)[iVar10] < iVar2) {
    if (iVar2 == 1) {
      (&DAT_00ea5bc0)[iVar8 * 0x1276] = (&DAT_00ea5bc0)[iVar8 * 0x1276] + 1;
      return 0;
    }
    if (iVar2 == 2) {
      (&DAT_00ea5bc4)[iVar8 * 0x1276] = (&DAT_00ea5bc4)[iVar8 * 0x1276] + 1;
      return 0;
    }
    if (iVar2 == 3) {
      (&DAT_00ea5bc8)[iVar8 * 0x1276] = (&DAT_00ea5bc8)[iVar8 * 0x1276] + 1;
      return 0;
    }
  }
  else if (0 < iVar2) {
    (&DAT_00ea5bcc)[iVar8 * 0x1276] = (&DAT_00ea5bcc)[iVar8 * 0x1276] + 1;
  }
  if ((int)(uint)(byte)(&DAT_00799a4d)[iVar10] < iVar3) {
    (&DAT_00ea5b94)[iVar8 * 0x1276] = (&DAT_00ea5b94)[iVar8 * 0x1276] + 1;
    return 0;
  }
  if (iVar3 == 1) {
    (&DAT_00ea5bb4)[iVar8 * 0x1276] = (&DAT_00ea5bb4)[iVar8 * 0x1276] + 1;
  }
  if (iVar4 <= (int)(uint)(byte)(&DAT_00799a48)[iVar10]) {
    if (iVar4 == 2) {
      (&DAT_00ea5bb8)[iVar8 * 0x1276] = (&DAT_00ea5bb8)[iVar8 * 0x1276] + 1;
      *(int *)(&DAT_00ea5fb8 + iVar8 * 0x49d8) = *(int *)(&DAT_00ea5fb8 + iVar8 * 0x49d8) + 1;
    }
    else if (iVar4 == 1) {
      (&DAT_00ea5bb8)[iVar8 * 0x1276] = (&DAT_00ea5bb8)[iVar8 * 0x1276] + 1;
    }
    if ((char)(&DAT_00799a5a)[iVar10] < iVar5) {
      if (1 < iVar5) {
        (&DAT_00ea5bb0)[iVar8 * 0x1276] = (&DAT_00ea5bb0)[iVar8 * 0x1276] + 1;
        return 0;
      }
      (&DAT_00ea5bac)[iVar8 * 0x1276] = (&DAT_00ea5bac)[iVar8 * 0x1276] + 1;
      return 0;
    }
    if (0 < iVar5) {
      (&DAT_00ea5bbc)[iVar8 * 0x1276] = (&DAT_00ea5bbc)[iVar8 * 0x1276] + 1;
    }
    local_18 = 0;
    if (0 < DAT_00f53dd8) {
      piVar11 = &DAT_00f53ddc;
      iVar9 = DAT_00f53dd8;
      do {
        if (*piVar11 != 0) {
          switch(*piVar11) {
          case 1: ;
            iVar10 = 0;
            break;
          case 2: ;
            iVar10 = 1;
            break;
          case 3: ;
            iVar10 = 2;
            break;
          case 4: ;
            iVar10 = 3;
            break;
          case 5: ;
            iVar10 = 4;
            break;
          case 6: ;
            iVar10 = 5;
            break;
          case 7: ;
            iVar10 = 6;
            break;
          case 8: ;
            iVar10 = 7;
            break;
default: ;
            iVar10 = -1;
            break;
          case 0xd: ;
            iVar10 = 9;
            break;
          case 0xf: ;
            iVar10 = 8;
            break;
          case 0x11: ;
            iVar10 = 10;
            break;
          case 0x13: ;
            iVar10 = 0xb;
          }
          if ((&DAT_00799a2c)[iVar10 + param_1 * 0x84] != 0) {
            local_18 = local_18 + 1;
          }
        }
        piVar11 = piVar11 + 1;
        iVar9 = iVar9 + -1;
      } while (iVar9 != 0);
    }
    if (local_18 < iVar7) {
      (&DAT_00ea5b9c)[iVar8 * 0x1276] = (&DAT_00ea5b9c)[iVar8 * 0x1276] + 1;
      return 0;
    }
    if ((short)(&DAT_00799a3e)[param_1 * 0x84] < local_24) {
      return 0;
    }
    if ((param_2 != 0) && ((short)(&DAT_00799a3c)[param_1 * 0x84] < 1)) {
      return 0;
    }
    if ((short)(&DAT_00799a40)[param_1 * 0x84] < local_20) {
      return 0;
    }
    if ((short)(&DAT_00799a42)[param_1 * 0x84] < local_1c) {
      return 0;
    }
    if ((1 < local_1c) && ((int)(&DAT_00ea5f14)[iVar8 * 0x1276] < 2)) {
      (&DAT_00ea5f6c)[iVar8 * 0x1276] = (&DAT_00ea5f6c)[iVar8 * 0x1276] + 1;
      return 0;
    }
    return 1;
  }
  if ((&DAT_00799a48)[iVar10] != 0) {
    (&DAT_00ea5b98)[iVar8 * 0x1276] = (&DAT_00ea5b98)[iVar8 * 0x1276] + 1;
    return 0;
  }
  *(int *)(&DAT_00ea5fbc + iVar8 * 0x49d8) = *(int *)(&DAT_00ea5fbc + iVar8 * 0x49d8) + 1;
  return 0;
}

#include "restore_types.h"
#define DAT_005d199c (*(undefined4 *)0x005d199cu)
#define DAT_0070cdf4 (*(undefined4 *)0x0070cdf4u)
#define DAT_0070ce08 (*(undefined4 *)0x0070ce08u)
#define DAT_00799ae5 (*(undefined4 *)0x00799ae5u)
#define DAT_00799bc4 (*(undefined4 *)0x00799bc4u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00ea1384 (*(undefined4 *)0x00ea1384u)
#define DAT_00ea1878 (*(undefined4 *)0x00ea1878u)
#define DAT_00ea1880 (*(undefined4 *)0x00ea1880u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea446c (*(undefined4 *)0x00ea446cu)
#define DAT_00ea4478 (*(undefined4 *)0x00ea4478u)
#define DAT_00ea447c (*(undefined4 *)0x00ea447cu)
#define DAT_00ea44a8 (*(undefined4 *)0x00ea44a8u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5da4 (*(undefined4 *)0x00ea5da4u)
#define DAT_00ea5da8 (*(undefined4 *)0x00ea5da8u)
#define DAT_00ea5db0 (*(undefined4 *)0x00ea5db0u)
#define DAT_00ea5ea8 (*(undefined4 *)0x00ea5ea8u)
#define DAT_00ea5ef8 (*(undefined4 *)0x00ea5ef8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_00458f80();
extern undefined4 FUN_004c38f0();
extern undefined4 FUN_004c5120();
extern bool FUN_004c52d0();
extern void FUN_004e2970();
extern int FUN_004e8ec0();
extern void FUN_004e9350();

void FUN_004e8840(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  char *pcVar6;
  int iVar7;
  byte bVar8;
  uint uVar9;
  int iVar10;
  uint local_20;
  uint local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  uint local_8;
  int local_4;
  
  iVar2 = FUN_004c38f0();
  iVar3 = FUN_00458f80(DAT_00eaac28);
  if (iVar3 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8)) {
    FUN_004e9350(DAT_00eaac28,*(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8) - iVar3);
  }
  iVar3 = DAT_00eaac28;
  iVar10 = DAT_00eaac28 * 0x49d8;
  uVar9 = 0;
  local_18 = 0;
  local_20 = 0;
  if ((&DAT_00ea44a8)[DAT_00eaac28 * 0x1276] == 0) {
    iVar7 = 0;
  }
  else {
    iVar7 = ((&DAT_00ea44a8)[DAT_00eaac28 * 0x1276] + 10) / 10;
    if (10 < iVar7) {
      iVar7 = 10;
    }
  }
  local_8 = (&DAT_00ea1384)[iVar7 + *(int *)(&DAT_00ea1878 + iVar10) * 0xb];
  iVar2 = ((&DAT_00ea4478)[DAT_00eaac28 * 0x1276] - (&DAT_00ea447c)[DAT_00eaac28 * 0x1276]) - iVar2;
  if (iVar2 < 0) {
    local_20 = iVar2 / 2;
    if (local_20 == 0) {
      local_20 = 0xffffffff;
    }
  }
  else if (iVar2 < 8) {
    if (iVar2 < 5) {
      if (iVar2 < 2) {
        if (0 < iVar2) {
          local_20 = 1;
        }
      }
      else {
        local_20 = 2;
      }
    }
    else {
      local_20 = 3;
    }
  }
  else {
    local_20 = 4;
  }
  (&DAT_00ea5ea8)[DAT_00eaac28 * 0x1276] = (&DAT_00ea4478)[DAT_00eaac28 * 0x1276];
  iVar2 = (&DAT_00ea446c)[iVar3 * 0x1276];
  if (iVar2 < 0x1a) {
    if (iVar2 < 0x12) {
      if (iVar2 < 0xb) {
        local_1c = (uint)(iVar2 < 5);
      }
      else {
        local_1c = 0xffffffff;
      }
    }
    else {
      local_1c = 0xfffffffe;
    }
  }
  else {
    local_1c = 0xfffffffd;
  }
  (&DAT_00ea5da4)[iVar3 * 0x1276] = iVar2;
  if (*(char *)((int)&DAT_00ea5ef8 + iVar10 + 3) == '\0') {
    local_4 = 0;
    *(undefined1 *)((int)&DAT_00ea5ef8 + iVar10 + 3) = 1;
  }
  else {
    local_4 = FUN_004e8ec0();
    iVar10 = DAT_00eaac28 * 0x49d8;
    *(undefined1 *)((int)&DAT_00ea5ef8 + iVar10 + 3) = 0;
  }
  local_14 = 0;
  local_10 = 0;
  iVar2 = 0;
  local_c = 0;
  piVar5 = &DAT_00799bc4;
  iVar3 = DAT_00e39080;
  do {
    if (((char)piVar5[-0x39] == '\x01') && (*(char *)((int)piVar5 + -0xdf) != '\0')) {
      if ((short)piVar5[-0x32] == 0) {
        *(char *)((int)piVar5 + -0x22) = (&DAT_005d199c)[iVar3 * 4] + '\n';
      }
      else if (*(int *)(&DAT_00ea1894 + iVar10) < 300) {
        uVar9 = 0;
        local_1c = 0;
        local_8 = 0;
        local_20 = 0;
        local_18 = 0;
        if (*(int *)(&DAT_00ea1894 + iVar10) < 200) {
          *(char *)((int)piVar5 + -0x22) = (&DAT_005d199c)[iVar3 * 4] + '\n';
          *piVar5 = 0;
        }
        else {
          *(undefined *)((int)piVar5 + -0x22) = (&DAT_005d199c)[iVar3 * 4];
          *piVar5 = 0;
        }
      }
      else {
        *piVar5 = *piVar5 + *(int *)(&DAT_0070ce08 + *(short *)((int)piVar5 + -0xd2) * 0x58);
        *(char *)((int)piVar5 + -0x22) =
             *(char *)((int)piVar5 + -0x22) + (char)local_1c + (char)local_20 + (char)local_8;
        local_10 = local_10 + 1;
        uVar9 = 0;
        local_18 = 0;
        if (*(int *)(&DAT_0070cdf4 + *(short *)((int)piVar5 + -0xd2) * 0x58) == 0) {
          local_c = local_c + local_4;
          *(undefined1 *)(piVar5 + -10) = 0;
          local_18 = local_4;
        }
        else {
          local_14 = local_14 + 1;
          if (*(byte *)((int)piVar5 + -0x65) < 2) {
            if (*(byte *)((int)piVar5 + -0x65) == 0) {
              bVar8 = (char)piVar5[-10] + 1;
              *(byte *)(piVar5 + -10) = bVar8;
              if (3 < bVar8) {
                *(undefined1 *)(piVar5 + -10) = 3;
              }
              if (*(byte *)(piVar5 + -10) < 3) {
                if (*(byte *)(piVar5 + -10) < 2) {
                  uVar9 = 0xffffffff;
                  iVar2 = iVar2 + -1;
                }
                else {
                  uVar9 = 0xfffffffe;
                  iVar2 = iVar2 + -2;
                }
              }
              else {
                uVar9 = 0xfffffffd;
                iVar2 = iVar2 + -3;
              }
            }
            else {
              uVar9 = 1;
              iVar2 = iVar2 + 1;
              *(undefined1 *)(piVar5 + -10) = 0;
            }
          }
          else {
            uVar9 = 2;
            *(undefined1 *)(piVar5 + -10) = 0;
            iVar2 = iVar2 + 2;
          }
        }
        cVar1 = *(char *)((int)piVar5 + -0x22) + (char)local_18 + (char)uVar9;
        *(char *)((int)piVar5 + -0x22) = cVar1;
        if (cVar1 < '\0') {
          *(undefined1 *)((int)piVar5 + -0x22) = 0;
        }
        iVar3 = DAT_00e39080;
        if ('d' < *(char *)((int)piVar5 + -0x22)) {
          *(undefined1 *)((int)piVar5 + -0x22) = 100;
          iVar3 = DAT_00e39080;
        }
      }
    }
    piVar5 = piVar5 + 0x42;
  } while ((int)piVar5 < 0x89b7bc);
  if (local_14 != 0) {
    uVar9 = iVar2 / local_14;
  }
  if (local_10 != 0) {
    local_18 = local_c / local_10;
  }
  iVar3 = 0;
  iVar2 = 0;
  pcVar6 = &DAT_00799ae5;
  do {
    if (((pcVar6[-5] == '\x01') && (*pcVar6 != '\0')) && (*(short *)(pcVar6 + 0x17) != 0)) {
      iVar2 = iVar2 + 1;
      iVar3 = iVar3 + pcVar6[0xbd];
    }
    pcVar6 = pcVar6 + 0x108;
  } while ((int)pcVar6 < 0x89b6dd);
  if (iVar2 == 0) {
    *(undefined4 *)(&DAT_00ea1880 + iVar10) = 0x3c;
    iVar3 = DAT_00eaac28;
  }
  else {
    *(int *)(&DAT_00ea1880 + iVar10) = iVar3 / iVar2;
    iVar3 = DAT_00eaac28;
    if (((*(short *)((int)&DAT_00ea5ce8 + iVar10) != 0) &&
        (iVar2 = FUN_004c5120(*(undefined4 *)((int)&DAT_00ea5f9c + iVar10)), iVar3 = DAT_00eaac28,
        iVar2 == 4)) &&
       (cVar1 = FUN_004c52d0((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276],2), iVar3 = DAT_00eaac28,
       cVar1 != '\0')) {
      iVar2 = *(int *)(&DAT_00ea1880 + DAT_00eaac28 * 0x49d8) + 10;
      if (99 < iVar2) {
        iVar2 = 100;
      }
      *(int *)(&DAT_00ea1880 + DAT_00eaac28 * 0x49d8) = iVar2;
    }
  }
  uVar4 = 0;
  if ((int)uVar9 < 0) {
    (&DAT_00ea5db0)[iVar3 * 0x1276] = 1;
    uVar4 = uVar9;
  }
  if ((int)local_1c < (int)uVar4) {
    (&DAT_00ea5db0)[iVar3 * 0x1276] = 2;
    uVar4 = local_1c;
  }
  if ((int)local_8 < (int)uVar4) {
    (&DAT_00ea5db0)[iVar3 * 0x1276] = 3;
    uVar4 = local_8;
  }
  if ((int)local_20 < (int)uVar4) {
    (&DAT_00ea5db0)[iVar3 * 0x1276] = 4;
    uVar4 = local_20;
  }
  if (local_18 < (int)uVar4) {
    (&DAT_00ea5db0)[iVar3 * 0x1276] = 5;
  }
  iVar2 = 0;
  if ((DAT_00e38ea8 < '\x02') && (DAT_00e38ebc == 0)) goto LAB_004e8d8f;
  iVar10 = *(int *)(&DAT_00ea1880 + iVar3 * 0x49d8);
  if (((int)(&DAT_00ea5da8)[iVar3 * 0x1276] / 10 == iVar10 / 10) ||
     (*(int *)(&DAT_00ea1894 + iVar3 * 0x49d8) < 0x12d)) goto LAB_004e8d8f;
  if (iVar10 < 1) {
    iVar7 = 0x69;
    if ((&DAT_00ea5db0)[iVar3 * 0x1276] != 0) {
      iVar2 = (&DAT_00ea5db0)[iVar3 * 0x1276] + 0x74;
    }
LAB_004e8d65: ;
    FUN_004e2970(iVar7,0,0);
    iVar3 = DAT_00eaac28;
  }
  else {
    if (99 < iVar10) {
      iVar7 = 0x74;
      goto LAB_004e8d65;
    }
    iVar7 = iVar10 / 10 + 0x6a;
    if ((iVar10 < 0x32) && ((&DAT_00ea5db0)[iVar3 * 0x1276] != 0)) {
      iVar2 = (&DAT_00ea5db0)[iVar3 * 0x1276] + 0x74;
    }
    if (iVar7 != 0) goto LAB_004e8d65;
  }
  if (iVar2 != 0) {
    FUN_004e2970(iVar2,0,0);
    iVar3 = DAT_00eaac28;
  }
LAB_004e8d8f: ;
  (&DAT_00ea5da8)[iVar3 * 0x1276] = *(undefined4 *)(&DAT_00ea1880 + iVar3 * 0x49d8);
  return;
}

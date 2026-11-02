#include "restore_types.h"
#define DAT_00799ae5 (*(undefined4 *)0x00799ae5u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea5bd0 (*(undefined4 *)0x00ea5bd0u)
#define DAT_00ea5bd4 (*(undefined4 *)0x00ea5bd4u)
#define DAT_00ea5bd8 (*(undefined4 *)0x00ea5bd8u)
#define DAT_00ea5bdc (*(undefined4 *)0x00ea5bdcu)
#define DAT_00ea5be0 (*(undefined4 *)0x00ea5be0u)
#define DAT_00ea5be4 (*(undefined4 *)0x00ea5be4u)
#define DAT_00ea5be8 (*(undefined4 *)0x00ea5be8u)
#define DAT_00ea5bec (*(undefined4 *)0x00ea5becu)
#define DAT_00ea5bf0 (*(undefined4 *)0x00ea5bf0u)
#define DAT_00ea5bf4 (*(undefined4 *)0x00ea5bf4u)
#define DAT_00ea5d38 (*(undefined4 *)0x00ea5d38u)
#define DAT_00ea5d3c (*(undefined4 *)0x00ea5d3cu)
#define DAT_00ea5d64 (*(undefined4 *)0x00ea5d64u)
#define DAT_00ea5d74 (*(undefined4 *)0x00ea5d74u)
#define DAT_00ea5d78 (*(undefined4 *)0x00ea5d78u)
#define DAT_00ea5d7c (*(undefined4 *)0x00ea5d7cu)
#define DAT_00ea5d80 (*(undefined4 *)0x00ea5d80u)
#define DAT_00ea5d84 (*(undefined4 *)0x00ea5d84u)
#define DAT_00ea5d88 (*(undefined4 *)0x00ea5d88u)
#define DAT_00ea5d8c (*(undefined4 *)0x00ea5d8cu)
#define DAT_00ea5d90 (*(undefined4 *)0x00ea5d90u)
#define DAT_00ea619c (*(undefined4 *)0x00ea619cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

void FUN_004550e0(void)

{
  char cVar1;
  char cVar2;
  short sVar3;
  int iVar4;
  char *pcVar5;
  int iVar6;
  short *psVar7;
  int iVar8;
  
  iVar4 = DAT_00eaac28;
  iVar6 = DAT_00eaac28 * 0x49d8;
  (&DAT_00ea5bf4)[DAT_00eaac28 * 0x1276] = 0;
  (&DAT_00ea5bd0)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5bd4)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5bd8)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5bdc)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5be0)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5be4)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5be8)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5bec)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5d38)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5d3c)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5bf0)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5d88)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5d8c)[iVar4 * 0x1276] = 0;
  (&DAT_00ea5d90)[iVar4 * 0x1276] = 0;
  iVar8 = 0;
  pcVar5 = &DAT_00799ae5;
  do {
    if ((pcVar5[-5] == '\x01') && (*pcVar5 != '\0')) {
      iVar8 = iVar8 + 1;
      cVar1 = pcVar5[0x7e];
      (&DAT_00ea5bf4)[iVar4 * 0x1276] = (&DAT_00ea5bf4)[iVar4 * 0x1276] + (int)pcVar5[0x7b];
      cVar2 = pcVar5[0x7c];
      (&DAT_00ea5d88)[iVar4 * 0x1276] = (&DAT_00ea5d88)[iVar4 * 0x1276] + (int)cVar1;
      cVar1 = pcVar5[0x7d];
      (&DAT_00ea5d8c)[iVar4 * 0x1276] = (&DAT_00ea5d8c)[iVar4 * 0x1276] + (int)cVar2;
      (&DAT_00ea5d90)[iVar4 * 0x1276] = (&DAT_00ea5d90)[iVar4 * 0x1276] + (int)cVar1;
    }
    pcVar5 = pcVar5 + 0x108;
  } while ((int)pcVar5 < 0x89b6dd);
  if (iVar8 != 0) {
    (&DAT_00ea5bf4)[iVar4 * 0x1276] = (int)(&DAT_00ea5bf4)[iVar4 * 0x1276] / iVar8;
    (&DAT_00ea5d88)[iVar4 * 0x1276] = (int)(&DAT_00ea5d88)[iVar4 * 0x1276] / iVar8;
    (&DAT_00ea5d8c)[iVar4 * 0x1276] = (int)(&DAT_00ea5d8c)[iVar4 * 0x1276] / iVar8;
    (&DAT_00ea5d90)[iVar4 * 0x1276] = (int)(&DAT_00ea5d90)[iVar4 * 0x1276] / iVar8;
  }
  psVar7 = &DAT_00799af0;
  do {
    if ((char)psVar7[-8] == '\x01') {
      sVar3 = *psVar7;
      if (sVar3 == 0x1f) {
        if (*(char *)((int)psVar7 + 0x97) == '\0') {
          (&DAT_00ea5bd4)[iVar4 * 0x1276] = (&DAT_00ea5bd4)[iVar4 * 0x1276] + 1;
        }
        else {
          (&DAT_00ea5bd0)[iVar4 * 0x1276] = (&DAT_00ea5bd0)[iVar4 * 0x1276] + 1;
        }
      }
      else if (sVar3 == 0x1e) {
        if (*(char *)((int)psVar7 + 0x97) == '\0') {
          (&DAT_00ea5bd4)[iVar4 * 0x1276] = (&DAT_00ea5bd4)[iVar4 * 0x1276] + 2;
        }
        else {
          (&DAT_00ea5bd0)[iVar4 * 0x1276] = (&DAT_00ea5bd0)[iVar4 * 0x1276] + 1;
        }
        if ((char)psVar7[0x4c] == '\0') {
          (&DAT_00ea5bdc)[iVar4 * 0x1276] = (&DAT_00ea5bdc)[iVar4 * 0x1276] + 2;
        }
        else {
          (&DAT_00ea5bd8)[iVar4 * 0x1276] = (&DAT_00ea5bd8)[iVar4 * 0x1276] + 1;
        }
      }
      else if (sVar3 == 0x21) {
        if (*(char *)((int)psVar7 + 0x97) == '\0') {
          (&DAT_00ea5bd4)[iVar4 * 0x1276] = (&DAT_00ea5bd4)[iVar4 * 0x1276] + 3;
        }
        else {
          (&DAT_00ea5bd0)[iVar4 * 0x1276] = (&DAT_00ea5bd0)[iVar4 * 0x1276] + 1;
        }
        if ((char)psVar7[0x4c] == '\0') {
          (&DAT_00ea5bdc)[iVar4 * 0x1276] = (&DAT_00ea5bdc)[iVar4 * 0x1276] + 3;
        }
        else {
          (&DAT_00ea5bd8)[iVar4 * 0x1276] = (&DAT_00ea5bd8)[iVar4 * 0x1276] + 1;
        }
        if (*(char *)((int)psVar7 + 0x99) == '\0') {
          (&DAT_00ea5be4)[iVar4 * 0x1276] = (&DAT_00ea5be4)[iVar4 * 0x1276] + 3;
        }
        else {
          (&DAT_00ea5be0)[iVar4 * 0x1276] = (&DAT_00ea5be0)[iVar4 * 0x1276] + 1;
        }
      }
      else if (sVar3 == 0x20) {
        if (*(char *)((int)psVar7 + 0x97) == '\0') {
          (&DAT_00ea5bec)[iVar4 * 0x1276] = (&DAT_00ea5bec)[iVar4 * 0x1276] + 100;
        }
        else {
          (&DAT_00ea5be8)[iVar4 * 0x1276] = (&DAT_00ea5be8)[iVar4 * 0x1276] + 1;
        }
      }
      else if (sVar3 == 0xe2) {
        if (*(char *)((int)psVar7 + 0x97) == '\0') {
          (&DAT_00ea5d3c)[iVar4 * 0x1276] = (&DAT_00ea5d3c)[iVar4 * 0x1276] + 100;
        }
        else {
          (&DAT_00ea5d38)[iVar4 * 0x1276] = (&DAT_00ea5d38)[iVar4 * 0x1276] + 1;
        }
      }
    }
    psVar7 = psVar7 + 0x84;
  } while ((int)psVar7 < 0x89b6e8);
  iVar8 = 0;
  if (0 < (int)(&DAT_00ea5bd4)[iVar4 * 0x1276]) {
    (&DAT_00ea5bf0)[iVar4 * 0x1276] = 1;
    iVar8 = (&DAT_00ea5bd4)[iVar4 * 0x1276];
  }
  if (iVar8 < (int)(&DAT_00ea5bdc)[iVar4 * 0x1276]) {
    (&DAT_00ea5bf0)[iVar4 * 0x1276] = 2;
    iVar8 = (&DAT_00ea5bdc)[iVar4 * 0x1276];
  }
  if (iVar8 < (int)(&DAT_00ea5be4)[iVar4 * 0x1276]) {
    (&DAT_00ea5bf0)[iVar4 * 0x1276] = 3;
    iVar8 = (&DAT_00ea5be4)[iVar4 * 0x1276];
  }
  if (iVar8 < (int)(&DAT_00ea5bec)[iVar4 * 0x1276]) {
    (&DAT_00ea5bf0)[iVar4 * 0x1276] = 4;
    iVar8 = (&DAT_00ea5bec)[iVar4 * 0x1276];
  }
  if (iVar8 < (int)(&DAT_00ea5d3c)[iVar4 * 0x1276]) {
    (&DAT_00ea5bf0)[iVar4 * 0x1276] = 5;
  }
  (&DAT_00ea5d74)[iVar4 * 0x1276] = *(int *)(&DAT_00ea1894 + iVar6) / 0x14 + 10;
  (&DAT_00ea5d78)[iVar4 * 0x1276] = *(int *)(&DAT_00ea1894 + iVar6) / 10 + 0x14;
  (&DAT_00ea5d7c)[iVar4 * 0x1276] = *(int *)(&DAT_00ea1894 + iVar6) / 5 + 0x28;
  (&DAT_00ea5d80)[iVar4 * 0x1276] = (*(int *)(&DAT_00ea1894 + iVar6) / 500 + 1) * 100;
  (&DAT_00ea5d84)[iVar4 * 0x1276] = 0;
  if ((*(int *)(&DAT_00ea619c + iVar6) < (int)(&DAT_00ea5d80)[iVar4 * 0x1276]) &&
     ((&DAT_00ea5d84)[iVar4 * 0x1276] = 1, (&DAT_00ea5d64)[iVar4 * 0x1276] == 3)) {
    (&DAT_00ea5d64)[iVar4 * 0x1276] = 2;
  }
  return;
}

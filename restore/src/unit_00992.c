#include "restore_types.h"
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea3f78 (*(undefined4 *)0x00ea3f78u)
#define DAT_00ea3f7c (*(undefined4 *)0x00ea3f7cu)
#define DAT_00ea41e4 (*(undefined4 *)0x00ea41e4u)
#define DAT_00ea446c (*(undefined4 *)0x00ea446cu)
#define DAT_00ea4474 (*(undefined4 *)0x00ea4474u)
#define DAT_00ea447c (*(undefined4 *)0x00ea447cu)
#define DAT_00ea448c (*(undefined4 *)0x00ea448cu)
#define DAT_00ea44c0 (*(undefined4 *)0x00ea44c0u)
#define DAT_00ea44d8 (*(undefined4 *)0x00ea44d8u)
#define DAT_00ea44e0 (*(undefined4 *)0x00ea44e0u)
#define DAT_00ea44e8 (*(undefined4 *)0x00ea44e8u)
#define DAT_00ea44f8 (*(undefined4 *)0x00ea44f8u)
#define DAT_00ea4500 (*(undefined4 *)0x00ea4500u)
#define DAT_00ea5b40 (*(undefined4 *)0x00ea5b40u)
#define DAT_00ea5be8 (*(undefined4 *)0x00ea5be8u)
#define DAT_00ea5cc4 (*(undefined4 *)0x00ea5cc4u)
#define DAT_00ea5d58 (*(undefined4 *)0x00ea5d58u)
#define DAT_00ea5e18 (*(undefined4 *)0x00ea5e18u)
#define DAT_00ea5f18 (*(undefined4 *)0x00ea5f18u)
#define DAT_00ea601c (*(undefined4 *)0x00ea601cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern uint FUN_00451b10();
extern void FUN_0046e410();
extern undefined4 FUN_004c38f0();
extern int FUN_004cde60();

void FUN_0046e1e0(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int local_8;
  
  iVar2 = FUN_004c38f0();
  iVar4 = DAT_00eaac28 * 0x49d8;
  local_8 = 0;
  iVar3 = (&DAT_00ea44f8)[DAT_00eaac28 * 0x1276];
  iVar6 = *(int *)(&DAT_00ea601c + iVar4) + (&DAT_00ea5cc4)[DAT_00eaac28 * 0x1276] +
          (&DAT_00ea44c0)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44d8)[DAT_00eaac28 * 0x1276];
  iVar5 = (&DAT_00ea4500)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44e8)[DAT_00eaac28 * 0x1276] +
          (&DAT_00ea448c)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44e0)[DAT_00eaac28 * 0x1276] + iVar3;
  if (iVar6 - iVar5 < 1) {
    if ((iVar3 / 2 - iVar5) + iVar6 < 1) {
      if (iVar6 - iVar5 < 0) {
        local_8 = -8;
      }
      else if ((iVar3 - iVar5) + iVar6 < 0) {
        local_8 = -5;
      }
    }
    else {
      local_8 = 3;
    }
  }
  else {
    local_8 = 5;
  }
  if ((int)(&DAT_00ea44e0)[DAT_00eaac28 * 0x1276] < (int)(&DAT_00ea44d8)[DAT_00eaac28 * 0x1276]) {
    local_8 = local_8 + 1;
  }
  else if ((int)(&DAT_00ea44d8)[DAT_00eaac28 * 0x1276] < (int)(&DAT_00ea44e0)[DAT_00eaac28 * 0x1276]
          ) {
    local_8 = local_8 + -1;
  }
  if ((int)(&DAT_00ea4474)[DAT_00eaac28 * 0x1276] < 1) {
    iVar3 = (&DAT_00ea446c)[DAT_00eaac28 * 0x1276];
    if (iVar3 < 5) {
      local_8 = local_8 + 2;
    }
    else if (9 < iVar3) {
      if (iVar3 < 0xf) {
        local_8 = local_8 + -1;
      }
      else if (iVar3 < 0x14) {
        local_8 = local_8 + -2;
      }
      else {
        local_8 = local_8 + -3;
      }
    }
  }
  else {
    local_8 = local_8 + -1;
  }
  if ((-1 < *(int *)(&DAT_00ea41e4 + iVar4)) && (*(int *)(&DAT_00ea41e4 + iVar4) < 2)) {
    local_8 = local_8 + -1;
  }
  iVar2 = ((int)(&DAT_00ea5e18)[DAT_00eaac28 * 0x1276] / 0xc -
          (&DAT_00ea447c)[DAT_00eaac28 * 0x1276]) - iVar2;
  if (iVar2 < 6) {
    if (iVar2 < 1) {
      if (iVar2 < -10) {
        local_8 = local_8 + -3;
      }
      else if (iVar2 < -5) {
        local_8 = local_8 + -2;
      }
      else if (iVar2 < 0) {
        local_8 = local_8 + -1;
      }
    }
    else {
      local_8 = local_8 + 1;
    }
  }
  else {
    local_8 = local_8 + 2;
  }
  iVar3 = FUN_004cde60(*(undefined4 *)(&DAT_00ea3f78 + iVar4),*(undefined4 *)(&DAT_00ea1894 + iVar4)
                      );
  iVar2 = FUN_004cde60(*(undefined4 *)(&DAT_00ea3f7c + DAT_00eaac28 * 0x49d8),
                       *(undefined4 *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8));
  if (0x5f < iVar3) {
    local_8 = local_8 + -1;
  }
  if (10 < iVar2) {
    local_8 = local_8 + 2;
  }
  if ((&DAT_00ea5d58)[DAT_00eaac28 * 0x1276] != 0) {
    local_8 = local_8 + -1;
  }
  if (0 < (int)(&DAT_00ea5be8)[DAT_00eaac28 * 0x1276]) {
    local_8 = local_8 + 1;
  }
  cVar1 = FUN_00451b10();
  iVar3 = DAT_00eaac28;
  if (cVar1 != '\0') {
    local_8 = local_8 + 1;
  }
  iVar2 = (&DAT_00ea5b40)[DAT_00eaac28 * 0x1276];
  (&DAT_00ea5b40)[DAT_00eaac28 * 0x1276] = iVar2 + local_8;
  if ((int)(&DAT_00ea5f18)[iVar3 * 0x1276] < iVar2 + local_8) {
    (&DAT_00ea5b40)[iVar3 * 0x1276] = (&DAT_00ea5f18)[iVar3 * 0x1276];
  }
  if ((int)(&DAT_00ea5b40)[iVar3 * 0x1276] < 0) {
    (&DAT_00ea5b40)[iVar3 * 0x1276] = 0;
  }
  if (100 < (int)(&DAT_00ea5b40)[iVar3 * 0x1276]) {
    (&DAT_00ea5b40)[iVar3 * 0x1276] = 100;
  }
  FUN_0046e410();
  return;
}

#include "restore_types.h"
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
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
#define DAT_00ea44fc (*(undefined4 *)0x00ea44fcu)
#define DAT_00ea4500 (*(undefined4 *)0x00ea4500u)
#define DAT_00ea5b40 (*(undefined4 *)0x00ea5b40u)
#define DAT_00ea5b54 (*(undefined4 *)0x00ea5b54u)
#define DAT_00ea5be8 (*(undefined4 *)0x00ea5be8u)
#define DAT_00ea5ca8 (*(undefined4 *)0x00ea5ca8u)
#define DAT_00ea5cc4 (*(undefined4 *)0x00ea5cc4u)
#define DAT_00ea5d58 (*(undefined4 *)0x00ea5d58u)
#define DAT_00ea5e18 (*(undefined4 *)0x00ea5e18u)
#define DAT_00ea5f18 (*(undefined4 *)0x00ea5f18u)
#define DAT_00ea601c (*(undefined4 *)0x00ea601cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern uint FUN_00451b10();
extern undefined4 FUN_004c38f0();
extern int FUN_004cde60();

void FUN_0046e410(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  undefined4 local_10;
  int local_c;
  
  iVar2 = FUN_004c38f0();
  iVar3 = DAT_00eaac28 * 0x49d8;
  uVar4 = 0;
  iVar6 = *(int *)(&DAT_00ea601c + iVar3) + (&DAT_00ea5cc4)[DAT_00eaac28 * 0x1276] +
          (&DAT_00ea44c0)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44d8)[DAT_00eaac28 * 0x1276];
  iVar5 = (&DAT_00ea44f8)[DAT_00eaac28 * 0x1276] + (&DAT_00ea4500)[DAT_00eaac28 * 0x1276] +
          (&DAT_00ea44e8)[DAT_00eaac28 * 0x1276] + (&DAT_00ea448c)[DAT_00eaac28 * 0x1276] +
          (&DAT_00ea44e0)[DAT_00eaac28 * 0x1276];
  local_c = 0;
  if (iVar6 - iVar5 < 1) {
    if (((int)(&DAT_00ea44fc)[DAT_00eaac28 * 0x1276] / 2 - iVar5) + iVar6 < 1) {
      if (iVar6 - iVar5 < 0) {
        uVar4 = 8;
        local_c = -8;
      }
      else if (((&DAT_00ea44fc)[DAT_00eaac28 * 0x1276] - iVar5) + iVar6 < 0) {
        uVar4 = 5;
        local_c = -5;
      }
    }
    else {
      local_c = 3;
    }
  }
  else {
    local_c = 5;
  }
  local_10 = 4;
  if ((-1 < *(int *)(&DAT_00ea41e4 + iVar3)) && (*(int *)(&DAT_00ea41e4 + iVar3) < 2)) {
    local_c = local_c + -1;
  }
  if ((int)(&DAT_00ea44e0)[DAT_00eaac28 * 0x1276] < (int)(&DAT_00ea44d8)[DAT_00eaac28 * 0x1276]) {
    local_c = local_c + 1;
  }
  else if ((int)(&DAT_00ea44d8)[DAT_00eaac28 * 0x1276] < (int)(&DAT_00ea44e0)[DAT_00eaac28 * 0x1276]
          ) {
    if (uVar4 == 0) {
      uVar4 = 1;
      local_10 = 0xb;
    }
    local_c = local_c + -1;
  }
  else if (uVar4 == 0) {
    uVar4 = 1;
    local_10 = 0xb;
  }
  if ((int)(&DAT_00ea4474)[DAT_00eaac28 * 0x1276] < 1) {
    iVar5 = (&DAT_00ea446c)[DAT_00eaac28 * 0x1276];
    if (iVar5 < 5) {
      local_c = local_c + 2;
    }
    else if (iVar5 < 10) {
      if (uVar4 == 0) {
        uVar4 = 1;
        goto LAB_0046e5b2;
      }
    }
    else if (iVar5 < 0xf) {
      local_c = local_c + -1;
      if (uVar4 == 0) {
        uVar4 = 1;
        goto LAB_0046e5b2;
      }
    }
    else if (iVar5 < 0x14) {
      local_c = local_c + -2;
      if (uVar4 < 2) {
        uVar4 = 2;
LAB_0046e5b2: ;
        local_10 = 5;
      }
    }
    else {
      local_c = local_c + -3;
      if (uVar4 < 3) {
        uVar4 = 3;
        goto LAB_0046e5b2;
      }
    }
  }
  else {
    if (uVar4 == 0) {
      uVar4 = 1;
      local_10 = 10;
    }
    local_c = local_c + -1;
  }
  iVar2 = ((int)(&DAT_00ea5e18)[DAT_00eaac28 * 0x1276] / 0xc -
          (&DAT_00ea447c)[DAT_00eaac28 * 0x1276]) - iVar2;
  if (iVar2 < 6) {
    if (iVar2 < 1) {
      if (iVar2 < -10) {
        if (uVar4 < 3) {
          uVar4 = 3;
          local_10 = 6;
        }
        local_c = local_c + -3;
      }
      else {
        if (iVar2 < -5) {
          local_c = local_c + -2;
          if (1 < uVar4) goto LAB_0046e637;
          uVar4 = 2;
        }
        else {
          if ((-1 < iVar2) || (local_c = local_c + -1, uVar4 != 0)) goto LAB_0046e637;
          uVar4 = 1;
        }
        local_10 = 6;
      }
    }
    else {
      local_c = local_c + 1;
    }
  }
  else {
    local_c = local_c + 2;
  }
LAB_0046e637: ;
  iVar2 = FUN_004cde60(*(undefined4 *)(&DAT_00ea3f78 + iVar3),*(undefined4 *)(&DAT_00ea1894 + iVar3)
                      );
  iVar3 = FUN_004cde60(*(undefined4 *)(&DAT_00ea3f7c + DAT_00eaac28 * 0x49d8),
                       *(undefined4 *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8));
  if (0x5f < iVar2) {
    if (uVar4 == 0) {
      uVar4 = 1;
      local_10 = 7;
    }
    local_c = local_c + -1;
  }
  if (10 < iVar3) {
    local_c = local_c + 2;
  }
  if (((&DAT_00ea5d58)[DAT_00eaac28 * 0x1276] != 0) && (local_c = local_c + -1, uVar4 == 0)) {
    uVar4 = 1;
    local_10 = 8;
  }
  if (0 < (int)(&DAT_00ea5be8)[DAT_00eaac28 * 0x1276]) {
    local_c = local_c + 1;
  }
  cVar1 = FUN_00451b10();
  iVar2 = DAT_00eaac28;
  if (cVar1 != '\0') {
    local_c = local_c + 1;
  }
  iVar3 = DAT_00eaac28 * 0x1276;
  if ((local_c == 0) || (((int)(&DAT_00ea5b40)[iVar3] < 1 && (DAT_0078d7bc == DAT_007848d8)))) {
    (&DAT_00ea5ca8)[DAT_00eaac28 * 0x1276] = 0;
    (&DAT_00ea5b54)[iVar2 * 0x1276] = 2;
    return;
  }
  if (local_c < 1) {
    (&DAT_00ea5b54)[DAT_00eaac28 * 0x1276] = 1;
    (&DAT_00ea5ca8)[iVar2 * 0x1276] = local_10;
    return;
  }
  (&DAT_00ea5b54)[DAT_00eaac28 * 0x1276] = 0;
  iVar5 = (&DAT_00ea5f18)[iVar2 * 0x1276];
  if ((&DAT_00ea5b40)[iVar3] + local_c <= iVar5) {
    if (uVar4 == 0) {
      (&DAT_00ea5ca8)[iVar2 * 0x1276] = 0;
      return;
    }
    (&DAT_00ea5ca8)[iVar2 * 0x1276] = local_10;
    return;
  }
  if ((int)(&DAT_00ea5b40)[iVar2 * 0x1276] <= iVar5) {
    (&DAT_00ea5b54)[iVar2 * 0x1276] =
         ((iVar5 <= (int)(&DAT_00ea5b40)[iVar2 * 0x1276]) - 1 & 0xfffffffe) + 2;
    (&DAT_00ea5ca8)[iVar2 * 0x1276] = 3;
    return;
  }
  (&DAT_00ea5b54)[iVar2 * 0x1276] = 1;
  (&DAT_00ea5ca8)[iVar2 * 0x1276] = 3;
  return;
}

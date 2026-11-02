#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea18a8 (*(undefined4 *)0x00ea18a8u)
#define DAT_00ea3f98 (*(undefined4 *)0x00ea3f98u)
#define DAT_00ea3f9c (*(undefined4 *)0x00ea3f9cu)
#define DAT_00ea3fac (*(undefined4 *)0x00ea3facu)
#define DAT_00ea3fb0 (*(undefined4 *)0x00ea3fb0u)
#define DAT_00ea5c80 (*(undefined4 *)0x00ea5c80u)
#define DAT_00ea5ea4 (*(undefined4 *)0x00ea5ea4u)
#define DAT_00ea5eb4 (*(undefined4 *)0x00ea5eb4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004e2970();

void FUN_004e2fb0(int param_1)

{
  int iVar1;
  int iVar2;
  
  if (DAT_00605a3c == 1) {
    return;
  }
  iVar2 = DAT_00eaac28 * 0x49d8;
  if (*(int *)(&DAT_00ea1894 + iVar2) < 0x14) {
    return;
  }
  if ((DAT_00e38ebc == 0) && (DAT_00e38ea8 < '\x02')) {
    iVar1 = (&DAT_00ea5c80)[DAT_00eaac28 * 0x1276];
    if (iVar1 == 0) {
      return;
    }
    if (iVar1 == 1) {
      return;
    }
    if (iVar1 == 3) {
      return;
    }
    if (iVar1 == 5) {
      return;
    }
  }
  if (*(int *)(&DAT_00ea3fb0 + iVar2) == 0) {
    if (3 < (int)((&DAT_00ea5eb4)[DAT_00eaac28 * 0x1276] + (&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276]))
    {
      FUN_004e2970(0xd8,0,0);
      return;
    }
    if (*(int *)(&DAT_00ea3fac + iVar2) < 5) {
      if ((*(int *)(&DAT_00ea3f98 + iVar2) != 0) || (*(int *)(&DAT_00ea18a8 + iVar2) < 1)) {
        if (param_1 != 0) {
          return;
        }
        FUN_004e2970(0x7b,0,0);
        return;
      }
      if (*(int *)(&DAT_00ea3f9c + iVar2) < 0x50) {
        iVar2 = (&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] + 0x7c;
        goto LAB_004e308f;
      }
    }
    if (param_1 == 0) {
      FUN_004e2970(0x7a,0,0);
      return;
    }
  }
  else {
    if (*(int *)(&DAT_00ea3fb0 + iVar2) != 1) {
      return;
    }
    if ((*(int *)(&DAT_00ea3f98 + iVar2) != 0) || (*(int *)(&DAT_00ea18a8 + iVar2) < 1)) {
      if (param_1 != 0) {
        return;
      }
      FUN_004e2970(0x82,0,0);
      return;
    }
    iVar2 = (&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] + 0x83;
LAB_004e308f: ;
    if (iVar2 != 0) {
      FUN_004e2970(iVar2,0,0);
    }
  }
  return;
}

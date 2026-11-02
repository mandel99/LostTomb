#include "restore_types.h"
#define DAT_00ea1880 (*(undefined4 *)0x00ea1880u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea3f58 (*(undefined4 *)0x00ea3f58u)
#define DAT_00ea3f5c (*(undefined4 *)0x00ea3f5cu)
#define DAT_00ea3f9c (*(undefined4 *)0x00ea3f9cu)
#define DAT_00ea3fa4 (*(undefined4 *)0x00ea3fa4u)
#define DAT_00ea3fa8 (*(undefined4 *)0x00ea3fa8u)
#define DAT_00ea5ea4 (*(undefined4 *)0x00ea5ea4u)
#define DAT_00ea5eb4 (*(undefined4 *)0x00ea5eb4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004cde40();
extern void FUN_004e2fb0();

void FUN_004e8f80(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  
  param_1 = param_1 * 0x49d8;
  iVar1 = *(int *)(&DAT_00ea1880 + param_1);
  iVar2 = *(int *)(&DAT_00ea3f9c + param_1);
  if (iVar1 < 0x47) {
    if (iVar1 < 0x3d) {
      if (iVar1 < 0x32) {
        if (iVar1 < 0x29) {
          if (iVar1 < 0x1f) {
            *(uint *)(&DAT_00ea3f9c + param_1) = ((iVar1 < 0x15) - 1 & 0x19) - 0x32;
          }
          else {
            *(undefined4 *)(&DAT_00ea3f9c + param_1) = 0xfffffff6;
          }
        }
        else {
          *(undefined4 *)(&DAT_00ea3f9c + param_1) = 0;
        }
      }
      else {
        *(undefined4 *)(&DAT_00ea3f9c + param_1) = 0x32;
      }
    }
    else {
      *(undefined4 *)(&DAT_00ea3f9c + param_1) = 0x4b;
    }
  }
  else {
    *(undefined4 *)(&DAT_00ea3f9c + param_1) = 100;
  }
  iVar1 = DAT_00eaac28;
  *(undefined4 *)(&DAT_00ea3f5c + param_1) = 0;
  *(undefined4 *)(&DAT_00ea3f58 + param_1) = 0;
  if (199999 < *(int *)(&DAT_00ea1894 + iVar1 * 0x49d8)) {
    *(undefined4 *)(&DAT_00ea3f9c + param_1) = 0;
    return;
  }
  if ((3 < (int)((&DAT_00ea5eb4)[iVar1 * 0x1276] + (&DAT_00ea5ea4)[iVar1 * 0x1276])) &&
     (0 < *(int *)(&DAT_00ea3f9c + param_1))) {
    *(undefined4 *)(&DAT_00ea3f9c + param_1) = 0;
    return;
  }
  iVar1 = *(int *)(&DAT_00ea3f9c + param_1);
  if (iVar1 < 1) {
    if (iVar1 < 0) {
      if (*(int *)(&DAT_00ea3fa4 + param_1) != 0) {
        *(int *)(&DAT_00ea3fa4 + param_1) = *(int *)(&DAT_00ea3fa4 + param_1) + -1;
        return;
      }
      if (*(int *)(&DAT_00ea1894 + param_1) < 0x65) {
        return;
      }
      uVar3 = FUN_004cde40(0xc,-iVar1);
      *(undefined4 *)(&DAT_00ea3f5c + param_1) = uVar3;
      *(undefined4 *)(&DAT_00ea3fa8 + param_1) = 2;
    }
  }
  else {
    if (*(int *)(&DAT_00ea3fa8 + param_1) != 0) {
      *(int *)(&DAT_00ea3fa8 + param_1) = *(int *)(&DAT_00ea3fa8 + param_1) + -1;
      return;
    }
    uVar3 = FUN_004cde40(0xc,iVar1);
    *(undefined4 *)(&DAT_00ea3f58 + param_1) = uVar3;
    *(undefined4 *)(&DAT_00ea3fa4 + param_1) = 2;
  }
  if (iVar2 != *(int *)(&DAT_00ea3f9c + param_1)) {
    FUN_004e2fb0(0);
  }
  return;
}

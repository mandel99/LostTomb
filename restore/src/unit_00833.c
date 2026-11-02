#include "restore_types.h"
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00e951c5 (*(char *)0x00e951c5u)
#define DAT_00e951c6 (*(char *)0x00e951c6u)
#define DAT_00e951c7 (*(char *)0x00e951c7u)
#define DAT_00e951c8 (*(undefined *)0x00e951c8u)
#define DAT_00e951c9 (*(char *)0x00e951c9u)
#define DAT_00e951ca (*(char *)0x00e951cau)
#define DAT_00e951cb (*(char *)0x00e951cbu)
#define DAT_00e951cc (*(undefined *)0x00e951ccu)
#define DAT_00e951cd (*(char *)0x00e951cdu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea3f58 (*(undefined4 *)0x00ea3f58u)
#define DAT_00ea3f5c (*(undefined4 *)0x00ea3f5cu)
#define DAT_00ea3f60 (*(undefined4 *)0x00ea3f60u)
#define DAT_00ea3f64 (*(undefined4 *)0x00ea3f64u)
#define DAT_00ea3f84 (*(undefined4 *)0x00ea3f84u)
#define DAT_00ea3f90 (*(undefined4 *)0x00ea3f90u)
#define DAT_00ea3f94 (*(undefined4 *)0x00ea3f94u)
#define DAT_00ea3f98 (*(undefined4 *)0x00ea3f98u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00458880();
extern void FUN_00458950();
extern void FUN_00458b60();
extern void FUN_004e0cf0();
extern void FUN_004e8650();
extern void FUN_004e8720();
extern void FUN_004e87a0();
extern void FUN_004e8f80();

void FUN_00458540(void)

{
  int iVar1;
  int iVar2;
  
  FUN_004e8f80(DAT_00eaac28);
  iVar1 = DAT_00eaac28 * 0x49d8;
  *(undefined4 *)(&DAT_00ea3f90 + iVar1) = 0;
  *(undefined4 *)(&DAT_00ea3f94 + iVar1) = 0;
  *(undefined4 *)(&DAT_00ea3f98 + iVar1) = 0;
  iVar2 = *(int *)(&DAT_00ea3f58 + iVar1);
  if (0 < iVar2) {
    if (iVar2 < 4) {
      iVar2 = iVar2 + *(int *)(&DAT_00ea3f64 + iVar1);
      if (iVar2 < 4) {
        *(int *)(&DAT_00ea3f64 + iVar1) = iVar2;
      }
      else {
        FUN_00458950(iVar2);
        *(undefined4 *)(&DAT_00ea3f64 + DAT_00eaac28 * 0x49d8) = 0;
      }
    }
    else {
      FUN_00458950(iVar2);
    }
  }
  iVar1 = DAT_00eaac28 * 0x49d8;
  iVar2 = *(int *)(&DAT_00ea3f5c + iVar1);
  if (0 < iVar2) {
    if (iVar2 < 4) {
      iVar2 = iVar2 + *(int *)(&DAT_00ea3f60 + iVar1);
      if (iVar2 < 4) {
        *(int *)(&DAT_00ea3f60 + iVar1) = iVar2;
      }
      else {
        FUN_00458b60(iVar2);
        *(undefined4 *)(&DAT_00ea3f60 + DAT_00eaac28 * 0x49d8) = 0;
      }
    }
    else {
      FUN_00458b60(iVar2);
    }
  }
  iVar2 = DAT_00eaac28 * 0x49d8;
  *(undefined4 *)(&DAT_00ea3f58 + iVar2) = 0;
  *(undefined4 *)(&DAT_00ea3f5c + iVar2) = 0;
  if (*(int *)(&DAT_00ea3f84 + iVar2) != 0) {
    FUN_004e8650();
    FUN_004e8720();
    FUN_004e87a0(DAT_00eaac28);
  }
  FUN_00458880();
  if ((499 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8)) && (DAT_00e951c5 == '\0')) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x65,0,0,0,0);
    DAT_00e951c5 = '\x01';
  }
  if ((999 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8)) && (DAT_00e951c6 == '\0')) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x66,0,0,0,0);
    DAT_00e951c6 = '\x01';
  }
  if ((1999 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8)) && (DAT_00e951c7 == '\0')) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x67,0,0,0,0);
    DAT_00e951c7 = '\x01';
  }
  if ((2999 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8)) && (DAT_00e951c8 == '\0')) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x68,0,0,0,0);
    DAT_00e951c8 = '\x01';
  }
  if ((4999 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8)) && (DAT_00e951c9 == '\0')) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x69,0,0,0,0);
    DAT_00e951c9 = '\x01';
  }
  if ((9999 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8)) && (DAT_00e951ca == '\0')) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x6a,0,0,0,0);
    DAT_00e951ca = '\x01';
  }
  if ((14999 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8)) && (DAT_00e951cb == '\0')) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x6b,0,0,0,0);
    DAT_00e951cb = '\x01';
  }
  if ((19999 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8)) && (DAT_00e951cc == '\0')) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x6c,0,0,0,0);
    DAT_00e951cc = '\x01';
  }
  if ((24999 < *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8)) && (DAT_00e951cd == '\0')) {
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x6d,0,0,0,0);
    DAT_00e951cd = '\x01';
  }
  return;
}

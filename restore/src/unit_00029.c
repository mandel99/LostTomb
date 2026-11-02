#include "restore_types.h"
#define DAT_0060595c (*(undefined4 *)0x0060595cu)
#define DAT_00605960 (*(undefined4 *)0x00605960u)
#define DAT_00605964 (*(undefined4 *)0x00605964u)
#define DAT_00605968 (*(undefined4 *)0x00605968u)
#define DAT_0060596c (*(undefined4 *)0x0060596cu)
#define DAT_0060597c (*(undefined *)0x0060597cu)
#define DAT_00605980 (*(undefined1 *)0x00605980u)
#define DAT_00605a10 (*(undefined4 *)0x00605a10u)
#define DAT_00605a14 (*(undefined4 *)0x00605a14u)
#define DAT_00605a4c (*(undefined4 *)0x00605a4cu)
#define DAT_00bca2bc (*(undefined *)0x00bca2bcu)
#define DAT_00bca2c0 (*(undefined *)0x00bca2c0u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define _DAT_00bca2b8 (*(undefined4 *)0x00bca2b8u)
extern void FUN_00444270();
extern void FUN_004de160();

void FUN_004035f0(void)

{
  short sVar1;
  int iVar2;
  int iVar3;
  
  if (DAT_0060596c == 2) {
    if (*(undefined4 *)(DAT_00605960) < 0) {
      iVar3 = -1;
      *(undefined4 *)(DAT_00605960) = (short)DAT_0060597c;
    }
    else {
LAB_0040362a: ;
      iVar3 = 1;
      if (DAT_0060596c == 1) {
        *(undefined4 *)(DAT_00605964) = (char)DAT_0060597c;
      }
      else if (DAT_0060596c == 2) {
        *(undefined4 *)(DAT_00605960) = (short)DAT_0060597c;
      }
      else if (DAT_0060596c == 4) goto LAB_00403657;
    }
  }
  else {
    if ((DAT_0060596c != 4) || (-1 < *(undefined4 *)(DAT_0060595c))) goto LAB_0040362a;
    iVar3 = -1;
LAB_00403657: ;
    *(undefined4 *)(DAT_0060595c) = DAT_0060597c;
  }
  if (DAT_0060596c == 1) {
    if ((*(undefined4 *)(DAT_00605964) != '\0') && (iVar3 == -1)) {
      *(undefined4 *)(DAT_00605964) = -*(undefined4 *)(DAT_00605964);
      goto LAB_00403684;
    }
  }
  else {
LAB_00403684: ;
    if (DAT_0060596c == 2) {
      sVar1 = *(undefined4 *)(DAT_00605960);
      if (((-1 < sVar1) || (iVar3 != 1)) && ((sVar1 < 1 || (iVar3 != -1)))) goto LAB_004036d3;
      *(undefined4 *)(DAT_00605960) = -sVar1;
    }
    if ((DAT_0060596c == 4) &&
       (((iVar2 = *(undefined4 *)(DAT_0060595c), iVar2 < 0 && (iVar3 == 1)) || ((0 < iVar2 && (iVar3 == -1)))))) {
      *(undefined4 *)(DAT_0060595c) = -iVar2;
    }
  }
LAB_004036d3: ;
  if (DAT_00605968 == 1) {
    DAT_00605a14 = DAT_00605a14 + -1;
    if ((DAT_00605a14 < 0) || (0x21 < DAT_00605a14)) goto LAB_00403729;
    _DAT_00bca2b8 = DAT_00605a14;
    FUN_004de160(DAT_00605a14);
    DAT_00bca2bc = 0x6d;
    DAT_00bca2c0 = 0xda;
  }
  if (DAT_00605968 == 2) {
    FUN_00444270(DAT_00605a10 + -1);
  }
LAB_00403729: ;
  if (DAT_00605a4c == (code *)0x0) {
    DAT_00e38dca = 1;
    DAT_00e38dda = DAT_00605980;
    return;
  }
  ((code *)DAT_00605a4c)();
  DAT_00605a4c = (code *)0x0;
  DAT_00e38dda = DAT_00605980;
  DAT_00e38dca = 1;
  return;
}

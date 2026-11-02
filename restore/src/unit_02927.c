#include "restore_types.h"
#define DAT_00604e80 (*(undefined *)0x00604e80u)
#define DAT_00604f10 (*(undefined *)0x00604f10u)
#define DAT_00604f14 (*(undefined *)0x00604f14u)
#define DAT_00604f18 (*(undefined *)0x00604f18u)
#define DAT_00604f20 (*(undefined *)0x00604f20u)
#define DAT_00604f24 (*(undefined *)0x00604f24u)
#define DAT_00604f28 (*(undefined *)0x00604f28u)
#define DAT_0060532c (*(undefined4 *)0x0060532cu)
#define DAT_00605330 (*(undefined4 *)0x00605330u)
#define DAT_00605360 (*(undefined4 *)0x00605360u)
#define DAT_00605364 (*(undefined4 *)0x00605364u)

void FUN_00566fcd(int param_1,int param_2,uint param_3,int param_4,int param_5,int param_6,
                 int param_7,int param_8,int param_9,int param_10,int param_11)

{
  int iVar1;
  int iVar2;
  
  if (param_2 == 1) {
    if ((param_3 & 3) == 0) {
      iVar1 = *(int *)(&DAT_0060532c + param_4 * 4);
    }
    else {
      iVar1 = *(int *)(&DAT_00605360 + param_4 * 4);
    }
    iVar2 = (int)(param_3 * 0x16d + -0x63db + iVar1 + 1 + ((int)(param_3 - 1) >> 2)) % 7;
    if (iVar2 < param_6) {
      iVar1 = iVar1 + -6 + (param_5 * 7 - iVar2) + param_6;
    }
    else {
      iVar1 = iVar1 + 1 + (param_5 * 7 - iVar2) + param_6;
    }
    if (param_5 == 5) {
      if ((param_3 & 3) == 0) {
        iVar2 = *(int *)(&DAT_00605330 + param_4 * 4);
      }
      else {
        iVar2 = *(int *)(&DAT_00605364 + param_4 * 4);
      }
      if (iVar2 < iVar1) {
        iVar1 = iVar1 + -7;
      }
    }
  }
  else {
    if ((param_3 & 3) == 0) {
      iVar1 = *(int *)(&DAT_0060532c + param_4 * 4);
    }
    else {
      iVar1 = *(int *)(&DAT_00605360 + param_4 * 4);
    }
    iVar1 = iVar1 + param_7;
  }
  if (param_1 == 1) {
    DAT_00604f10 = param_3;
    DAT_00604f18 = ((param_8 * 0x3c + param_9) * 0x3c + param_10) * 1000 + param_11;
    DAT_00604f14 = iVar1;
  }
  else {
    DAT_00604f28 = ((param_8 * 0x3c + param_9) * 0x3c + DAT_00604e80 + param_10) * 1000 + param_11;
    if (DAT_00604f28 < 0) {
      DAT_00604f28 = DAT_00604f28 + 86400000;
      DAT_00604f24 = iVar1 + -1;
    }
    else {
      DAT_00604f24 = iVar1;
      if (86399999 < DAT_00604f28) {
        DAT_00604f28 = DAT_00604f28 + -86400000;
        DAT_00604f24 = iVar1 + 1;
      }
    }
    DAT_00604f20 = param_3;
  }
  return;
}

#include "restore_types.h"
#define DAT_0078d83c (*(undefined *)0x0078d83cu)
#define DAT_0078d840 (*(undefined *)0x0078d840u)
#define DAT_0078e178 (*(undefined4 *)0x0078e178u)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_007999f6 (*(undefined4 *)0x007999f6u)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_00799a06 (*(undefined4 *)0x00799a06u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00ea3f90 (*(undefined4 *)0x00ea3f90u)
#define DAT_00ea3f98 (*(undefined4 *)0x00ea3f98u)
#define DAT_00ea3fac (*(undefined4 *)0x00ea3facu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00458d00();

void FUN_00458950(int param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  int iVar5;
  
  DAT_0078d840 = 0;
  iVar3 = param_1;
  if (0 < DAT_0078d83c) {
    psVar4 = &DAT_0078e178;
    do {
      if (param_1 < 1) {
        if (DAT_0078d840 < DAT_0078d83c) goto LAB_004589c0;
        break;
      }
      sVar1 = *psVar4;
      DAT_0078d840 = DAT_0078d840 + 1;
      psVar4 = psVar4 + 1;
      if (((&DAT_00799a06)[sVar1 * 0x84] != 0) &&
         ((&DAT_00bcd606)[(short)(&DAT_00799a06)[sVar1 * 0x84] * 0x184] != '\x01')) {
        (&DAT_00799a06)[sVar1 * 0x84] = 0;
      }
    } while (DAT_0078d840 < DAT_0078d83c);
  }
  goto LAB_00458a2c;
  while( true ) {
    iVar2 = (int)(short)(&DAT_0078e178)[DAT_0078d840];
    DAT_0078d840 = DAT_0078d840 + 1;
    if (((0 < (short)(&DAT_007999f8)[iVar2 * 0x84]) && ((&DAT_007999f4)[iVar2 * 0x84] == 0)) &&
       ((&DAT_00799a06)[iVar2 * 0x84] == 0)) {
      if (iVar3 < 5) {
        FUN_00458d00(iVar2,iVar3);
        iVar3 = 0;
      }
      else {
        FUN_00458d00(iVar2,4);
        iVar3 = iVar3 + -4;
      }
    }
    if (DAT_0078d83c <= DAT_0078d840) break;
LAB_004589c0: ;
    if (iVar3 < 1) break;
  }
LAB_00458a2c: ;
  DAT_0078d840 = 0;
  if (0 < DAT_0078d83c) {
    do {
      if (iVar3 < 1) break;
      iVar2 = (int)(short)(&DAT_0078e178)[DAT_0078d840];
      DAT_0078d840 = DAT_0078d840 + 1;
      if (((0 < (short)(&DAT_007999f8)[iVar2 * 0x84]) && (7 < (short)(&DAT_007999f6)[iVar2 * 0x84]))
         && ((&DAT_00799a06)[iVar2 * 0x84] == 0)) {
        if (iVar3 < 5) {
          FUN_00458d00(iVar2,iVar3);
          iVar3 = 0;
        }
        else {
          FUN_00458d00(iVar2,4);
          iVar3 = iVar3 + -4;
        }
      }
    } while (DAT_0078d840 < DAT_0078d83c);
  }
  DAT_0078d840 = 0;
  if (0 < DAT_0078d83c) {
    do {
      if (iVar3 < 1) break;
      iVar2 = (int)(short)(&DAT_0078e178)[DAT_0078d840];
      DAT_0078d840 = DAT_0078d840 + 1;
      if (((0 < (short)(&DAT_007999f8)[iVar2 * 0x84]) && (0 < (short)(&DAT_007999f6)[iVar2 * 0x84]))
         && ((&DAT_00799a06)[iVar2 * 0x84] == 0)) {
        iVar5 = (int)(short)(&DAT_007999f6)[iVar2 * 0x84];
        if (iVar5 < iVar3) {
          FUN_00458d00(iVar2,iVar5);
          iVar3 = iVar3 - iVar5;
        }
        else {
          FUN_00458d00(iVar2,iVar3);
          iVar3 = 0;
        }
      }
    } while (DAT_0078d840 < DAT_0078d83c);
  }
  iVar2 = DAT_00eaac28 * 0x49d8;
  iVar5 = *(int *)(&DAT_00ea3f90 + iVar2) + (param_1 - iVar3);
  *(int *)(&DAT_00ea3f90 + iVar2) = iVar5;
  *(int *)(&DAT_00ea3fac + iVar2) = *(int *)(&DAT_00ea3fac + iVar2) + iVar5;
  if ((iVar3 != 0) && (param_1 == iVar3)) {
    *(int *)(&DAT_00ea3f98 + iVar2) = *(int *)(&DAT_00ea3f98 + iVar2) + iVar3;
  }
  return;
}

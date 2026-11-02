#include "restore_types.h"
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea448c (*(undefined4 *)0x00ea448cu)
#define DAT_00ea44c0 (*(undefined4 *)0x00ea44c0u)
#define DAT_00ea44d8 (*(undefined4 *)0x00ea44d8u)
#define DAT_00ea44e0 (*(undefined4 *)0x00ea44e0u)
#define DAT_00ea44e8 (*(undefined4 *)0x00ea44e8u)
#define DAT_00ea44f8 (*(undefined4 *)0x00ea44f8u)
#define DAT_00ea4500 (*(undefined4 *)0x00ea4500u)
#define DAT_00ea4514 (*(undefined4 *)0x00ea4514u)
#define DAT_00ea451c (*(undefined4 *)0x00ea451cu)
#define DAT_00ea452c (*(undefined4 *)0x00ea452cu)
#define DAT_00ea5cbc (*(undefined4 *)0x00ea5cbcu)
#define DAT_00ea5cc4 (*(undefined4 *)0x00ea5cc4u)
#define DAT_00ea5cc8 (*(undefined4 *)0x00ea5cc8u)
#define DAT_00ea5d54 (*(undefined4 *)0x00ea5d54u)
#define DAT_00ea5d58 (*(undefined4 *)0x00ea5d58u)
#define DAT_00ea5d5c (*(undefined4 *)0x00ea5d5cu)
#define DAT_00ea601c (*(undefined4 *)0x00ea601cu)
#define DAT_00ea6020 (*(undefined4 *)0x00ea6020u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_0045a7b0();
extern int FUN_004cde40();

void FUN_0045a5d0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar1 = DAT_00eaac28;
  iVar3 = DAT_00eaac28 * 0x49d8;
  (&DAT_00ea5cc4)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5cc8)[DAT_00eaac28 * 0x1276];
  iVar2 = 0;
  (&DAT_00ea5cc8)[iVar1 * 0x1276] = 0;
  *(undefined4 *)(&DAT_00ea601c + iVar3) = *(undefined4 *)(&DAT_00ea6020 + iVar3);
  *(undefined4 *)(&DAT_00ea6020 + iVar3) = 0;
  (&DAT_00ea5d58)[iVar1 * 0x1276] = 0;
  iVar5 = (&DAT_00ea44c0)[iVar1 * 0x1276] + (&DAT_00ea44d8)[iVar1 * 0x1276] +
          *(int *)(&DAT_00ea601c + iVar3) + (&DAT_00ea5cc4)[iVar1 * 0x1276];
  iVar4 = (&DAT_00ea4500)[iVar1 * 0x1276] + (&DAT_00ea44e8)[iVar1 * 0x1276] +
          (&DAT_00ea44f8)[iVar1 * 0x1276] + (&DAT_00ea448c)[iVar1 * 0x1276] +
          (&DAT_00ea44e0)[iVar1 * 0x1276];
  if ((&DAT_00ea5cbc)[iVar1 * 0x1276] == 10) {
    (&DAT_00ea5d58)[iVar1 * 0x1276] = 0;
    (&DAT_00ea5d5c)[iVar1 * 0x1276] = 0;
  }
  else if (*(int *)(&DAT_00ea187c + iVar3) < 1) {
    (&DAT_00ea5d58)[iVar1 * 0x1276] = 1;
    (&DAT_00ea5d5c)[iVar1 * 0x1276] = (&DAT_00ea5d5c)[iVar1 * 0x1276] + 1;
  }
  else {
    (&DAT_00ea5d5c)[iVar1 * 0x1276] = 0;
    if (iVar4 < iVar5) {
      iVar2 = *(int *)(&DAT_00ea1894 + iVar3);
      if (iVar2 < 0x1389) {
        if (iVar2 < 0xbb9) {
          if (iVar2 < 0x7d1) {
            if (iVar2 < 0x3e9) {
              if (iVar2 < 0x1f5) {
                (&DAT_00ea5d54)[iVar1 * 0x1276] = 0x32;
              }
              else {
                (&DAT_00ea5d54)[iVar1 * 0x1276] = 0x96;
              }
            }
            else {
              (&DAT_00ea5d54)[iVar1 * 0x1276] = 0xe1;
            }
          }
          else {
            (&DAT_00ea5d54)[iVar1 * 0x1276] = 300;
          }
        }
        else {
          (&DAT_00ea5d54)[iVar1 * 0x1276] = 400;
        }
      }
      else {
        (&DAT_00ea5d54)[iVar1 * 0x1276] = 500;
      }
      iVar2 = FUN_004cde40(iVar5 - iVar4,0x19);
      iVar3 = DAT_00eaac28 * 0x49d8;
      if (iVar2 <= (int)(&DAT_00ea5d54)[DAT_00eaac28 * 0x1276]) goto LAB_0045a768;
    }
    else {
      if (2000 < *(int *)(&DAT_00ea1894 + iVar3)) {
        (&DAT_00ea5d54)[iVar1 * 0x1276] = 200;
        goto LAB_0045a768;
      }
      if (1000 < *(int *)(&DAT_00ea1894 + iVar3)) {
        (&DAT_00ea5d54)[iVar1 * 0x1276] = 100;
        goto LAB_0045a768;
      }
    }
  }
  *(int *)((int)&DAT_00ea5d54 + iVar3) = iVar2;
LAB_0045a768: ;
  *(int *)(&DAT_00ea187c + iVar3) =
       *(int *)(&DAT_00ea187c + iVar3) - *(int *)((int)&DAT_00ea5d54 + iVar3);
  iVar1 = *(int *)((int)&DAT_00ea5d54 + iVar3);
  FUN_0045a7b0();
  iVar2 = DAT_00eaac28;
  (&DAT_00ea452c)[DAT_00eaac28 * 0x1276] = *(undefined4 *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8);
  (&DAT_00ea4514)[iVar2 * 0x1276] = iVar5;
  (&DAT_00ea451c)[iVar2 * 0x1276] = iVar4 + iVar1;
  return;
}

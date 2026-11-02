#include "restore_types.h"
#define DAT_007999d9 (*(undefined4 *)0x007999d9u)
#define DAT_00bcb9cc (*(undefined4 *)0x00bcb9ccu)
#define DAT_00bcb9ce (*(undefined4 *)0x00bcb9ceu)
#define DAT_00bcb9f6 (*(undefined4 *)0x00bcb9f6u)
#define DAT_00bcb9f8 (*(undefined4 *)0x00bcb9f8u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00bcd65b (*(undefined4 *)0x00bcd65bu)
#define DAT_00ea3fe8 (*(undefined4 *)0x00ea3fe8u)
#define DAT_00ea3fea (*(undefined4 *)0x00ea3feau)
#define DAT_00ea3ff0 (*(undefined4 *)0x00ea3ff0u)
#define DAT_00ea3ff2 (*(undefined4 *)0x00ea3ff2u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004b4800();
extern void FUN_004b7c80();
extern bool FUN_00518400();

void FUN_00472790(int param_1,int param_2,int param_3,int param_4)

{
  short sVar1;
  short sVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar4 = param_2;
  iVar7 = param_2 * 0x90;
  sVar1 = *(short *)(&DAT_00bcb9ce + iVar7);
  if (0 < param_1) {
    param_2 = param_1;
    do {
      sVar2 = *(short *)(&DAT_00ea3fea + DAT_00eaac28 * 0x49d8);
      sVar3 = *(short *)(&DAT_00ea3fe8 + DAT_00eaac28 * 0x49d8);
      iVar5 = FUN_00518400((int)sVar3,(int)sVar2,(int)*(short *)(&DAT_00bcb9f6 + iVar7),
                           (int)*(short *)(&DAT_00bcb9f8 + iVar7));
      iVar6 = (int)sVar2;
      iVar8 = (int)sVar3;
      if (iVar5 == 0) {
        sVar2 = *(short *)(&DAT_00ea3ff0 + DAT_00eaac28 * 0x49d8);
        sVar3 = *(short *)(&DAT_00ea3ff2 + DAT_00eaac28 * 0x49d8);
        iVar5 = FUN_00518400((int)sVar2,(int)sVar3,(int)*(short *)(&DAT_00bcb9f6 + iVar7),
                             (int)*(short *)(&DAT_00bcb9f8 + iVar7));
        iVar6 = (int)sVar3;
        iVar8 = (int)sVar2;
        if (iVar5 == 0) {
          iVar6 = param_4;
          iVar8 = param_3;
        }
      }
      iVar6 = FUN_004b4800((&DAT_007999d9)[sVar1 * 0x108],(int)*(short *)(&DAT_00bcb9cc + iVar7),
                           iVar8,iVar6,0);
      param_2 = param_2 + -1;
      (&DAT_00bcd658)[iVar6 * 0xc2] = (short)iVar4;
      (&DAT_00bcd65b)[iVar6 * 0x184] = 1;
      (&DAT_00bcd630)[iVar6 * 0x184] = 0x10;
    } while (param_2 != 0);
  }
  FUN_004b7c80();
  return;
}

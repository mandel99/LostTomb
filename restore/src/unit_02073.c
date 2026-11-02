#include "restore_types.h"
#define LAB_004eae20 (*(undefined1 *)0x004eae20u)
#define DAT_005e4cd0 (*(undefined4 *)0x005e4cd0u)
#define DAT_005e4cd4 (*(undefined4 *)0x005e4cd4u)
#define DAT_00784b94 (*(undefined *)0x00784b94u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd663 (*(undefined4 *)0x00bcd663u)
#define _DAT_00784cb0 (*(undefined4 *)0x00784cb0u)
extern int FUN_004ea9c0();
extern int FUN_004ec0f0();
extern void FUN_0056233e();

undefined4 FUN_004eaec0(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  DWORD DVar5;
  uint uVar6;
  int iVar7;
  undefined1 local_4 [4];
  
  iVar2 = param_1;
  iVar7 = param_1 * 0x184;
  cVar3 = FUN_004ea9c0((&DAT_00bcd602)[iVar7]);
  if (cVar3 != '\0') {
    if ((&DAT_00bcd606)[iVar7] != '\0') {
      cVar3 = (&DAT_00bcd630)[iVar7];
      if ((((cVar3 != '\f') && (cVar3 != '\x04')) && (cVar3 != '\x05')) && (cVar3 != '\x15')) {
        cVar3 = (&DAT_00bcd663)[iVar7];
        uVar6 = _DAT_00784cb0 & 0xff;
        (&DAT_00bcd663)[iVar7] = cVar3 + 1U;
        iVar4 = ((int)DAT_00784b94 + uVar6 * 2) * 0x30;
        if (*(int *)(&DAT_005e4cd0 + iVar4) <= (int)(uint)(byte)(cVar3 + 1U)) {
          uVar1 = *(undefined4 *)(&DAT_005e4cd4 + iVar4);
          (&DAT_00bcd663)[iVar7] = 0;
          DVar5 = timeGetTime();
          FUN_0056233e(DVar5);
          iVar4 = FUN_004ec0f0((int)(short)(&DAT_00bcd60c)[iVar2 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[iVar2 * 0xc2],local_4,&param_1,uVar1,
                               iVar2,&LAB_004eae20);
          if (iVar4 != 0) {
            *(undefined2 *)(&DAT_00bcd61c + iVar7) = (&DAT_00bcd60c)[iVar4 * 0xc2];
            *(undefined2 *)(&DAT_00bcd61e + iVar7) = (&DAT_00bcd60e)[iVar4 * 0xc2];
            (&DAT_00bcd630)[iVar7] = 0x15;
            *(short *)(&DAT_00bcd62e + iVar7) = (short)uVar1 * 0x12;
            (&DAT_00bcd656)[iVar2 * 0xc2] = (short)iVar4;
            return 1;
          }
        }
      }
    }
  }
  return 0;
}

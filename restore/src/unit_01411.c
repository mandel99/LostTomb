#include "restore_types.h"
#define DAT_007090e4 (*(undefined4 *)0x007090e4u)
#define DAT_007090e8 (*(undefined4 *)0x007090e8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd662 (*(undefined4 *)0x00bcd662u)
#define DAT_00bcd663 (*(undefined4 *)0x00bcd663u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern int FUN_004b45c0();
extern int FUN_004bcf40();
extern undefined1 FUN_0051cd80();

void FUN_004aed10(int param_1)

{
  char cVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined1 uVar4;
  int iVar5;
  int iVar6;
  
  iVar6 = param_1 * 0x184;
  cVar1 = (&DAT_00bcd663)[iVar6];
  (&DAT_00bcd663)[iVar6] = cVar1 + 1U;
  if (*(int *)(&DAT_007090e8 + (uint)(byte)(&DAT_00bcd602)[iVar6] * 0x38) <
      (int)(uint)(byte)(cVar1 + 1U)) {
    (&DAT_00bcd663)[iVar6] = 0;
    iVar5 = FUN_004bcf40(param_1,*(undefined4 *)
                                  (&DAT_007090e4 + (uint)(byte)(&DAT_00bcd602)[iVar6] * 0x38));
    uVar3 = _DAT_00d3b4e4;
    uVar2 = _DAT_00d3b4e0;
    if (iVar5 == 0) {
      (&DAT_00bcd662)[iVar6] = 0;
    }
    else {
      (&DAT_00bcd662)[iVar6] = 1;
      uVar4 = FUN_0051cd80((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                           (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],uVar3,uVar2);
      (&DAT_00bcd609)[iVar6] = uVar4;
    }
  }
  if ((&DAT_00bcd662)[iVar6] != '\0') {
    if ((&DAT_00bcd662)[iVar6] == '\x01') {
      FUN_004b45c0(param_1,(int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                   (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],_DAT_00d3b4e4,_DAT_00d3b4e0,0x47);
    }
    cVar1 = (&DAT_00bcd662)[iVar6];
    (&DAT_00bcd662)[iVar6] = cVar1 + 1U;
    if (100 < (byte)(cVar1 + 1U)) {
      (&DAT_00bcd662)[iVar6] = 0;
    }
  }
  return;
}

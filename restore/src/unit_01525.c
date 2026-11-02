#include "restore_types.h"
#define DAT_007090d0 (*(undefined4 *)0x007090d0u)
#define DAT_007090d4 (*(undefined4 *)0x007090d4u)
#define DAT_007090d8 (*(undefined4 *)0x007090d8u)
#define DAT_00bcba42 (*(undefined4 *)0x00bcba42u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd607 (*(undefined4 *)0x00bcd607u)
#define DAT_00bcd615 (*(undefined4 *)0x00bcd615u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00bcd72c (*(undefined4 *)0x00bcd72cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004b5050();
extern void FUN_004b5210();
extern void FUN_004b7ab0();
extern undefined4 FUN_004ea9a0();
extern int FUN_004ea9c0();

void FUN_004bc4a0(int param_1,int param_2)

{
  short sVar1;
  short sVar2;
  byte bVar3;
  char cVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  iVar7 = param_1 * 0x184;
  sVar1 = (&DAT_00bcd658)[param_1 * 0xc2];
  iVar6 = param_2 * 0x184;
  sVar2 = (&DAT_00bcd658)[param_2 * 0xc2];
  if (param_1 == param_2) {
    return;
  }
  if ((byte)(&DAT_00bcd607)[iVar6] != DAT_00eaac28) {
    iVar5 = *(int *)(&DAT_007090d4 + (uint)(byte)(&DAT_00bcd602)[iVar7] * 0x38);
    if ((5 < *(int *)(&DAT_00bcd72c + iVar7)) && (*(int *)(&DAT_00bcd72c + iVar7) < 0x14)) {
      iVar5 = iVar5 << 2;
    }
    if ((iVar5 == 0) ||
       (iVar5 = ((0x1e - *(int *)(&DAT_007090d8 + (uint)(byte)(&DAT_00bcd602)[iVar7] * 0x38)) / 0x14
                ) * iVar5, iVar5 < 0)) {
      iVar5 = 0;
    }
    bVar3 = (&DAT_00bcd615)[iVar6] + (char)iVar5;
    (&DAT_00bcd615)[iVar6] = bVar3;
    if ((int)(uint)bVar3 <= *(int *)(&DAT_007090d0 + (uint)(byte)(&DAT_00bcd602)[iVar6] * 0x38)) {
      FUN_004b5210(param_1);
      return;
    }
    cVar4 = FUN_004ea9c0((uint)(byte)(&DAT_00bcd602)[iVar6]);
    if ((((cVar4 == '\0') && (cVar4 = FUN_004ea9a0((&DAT_00bcd602)[iVar6]), cVar4 == '\0')) &&
        ((&DAT_00bcd602)[iVar6] != '\x17')) && ((byte)(&DAT_00bcd607)[iVar7] == DAT_00eaac28)) {
      iVar7 = *(short *)(&DAT_00bcba42 + sVar1 * 0x90) + 1;
      if (99 < iVar7) {
        iVar7 = 100;
      }
      *(short *)(&DAT_00bcba42 + sVar1 * 0x90) = (short)iVar7;
    }
    (&DAT_00bcd630)[iVar6] = 4;
    *(undefined2 *)(&DAT_00bcd62e + iVar6) = 0;
    FUN_004b5050(param_2);
    FUN_004b7ab0((int)sVar2);
    return;
  }
  return;
}

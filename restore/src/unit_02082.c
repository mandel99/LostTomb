#include "restore_types.h"
#define LAB_004eae20 (*(undefined1 *)0x004eae20u)
#define DAT_005e4cd4 (*(undefined4 *)0x005e4cd4u)
#define DAT_00784b94 (*(undefined *)0x00784b94u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define _DAT_00784cb0 (*(undefined4 *)0x00784cb0u)
extern int FUN_004ea9c0();
extern int FUN_004ec0f0();
extern void FUN_004ec7c0();

void FUN_004ec990(int param_1)

{
  int iVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  undefined1 local_4 [4];
  
  iVar1 = param_1;
  iVar4 = param_1 * 0x184;
  cVar2 = FUN_004ea9c0((&DAT_00bcd602)[iVar4]);
  if ((((cVar2 != '\0') && ((&DAT_00bcd606)[iVar4] != '\0')) && ((&DAT_00bcd630)[iVar4] != '\f')) &&
     ((&DAT_00bcd630)[iVar4] != '\x04')) {
    iVar3 = FUN_004ec0f0((int)(short)(&DAT_00bcd60c)[iVar1 * 0xc2],
                         (int)(short)(&DAT_00bcd60e)[iVar1 * 0xc2],local_4,&param_1,
                         *(int *)(&DAT_005e4cd4 +
                                 ((int)DAT_00784b94 + (_DAT_00784cb0 & 0xff) * 2) * 0x30) << 1,iVar1
                         ,&LAB_004eae20);
    if (iVar3 != 0) {
      *(undefined2 *)(&DAT_00bcd61c + iVar4) = (&DAT_00bcd60c)[iVar3 * 0xc2];
      *(undefined2 *)(&DAT_00bcd61e + iVar4) = (&DAT_00bcd60e)[iVar3 * 0xc2];
      (&DAT_00bcd656)[iVar1 * 0xc2] = (short)iVar3;
      return;
    }
    FUN_004ec7c0(iVar1);
  }
  return;
}

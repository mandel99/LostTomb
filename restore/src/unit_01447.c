#include "restore_types.h"
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd5f0 (*(undefined *)0x00bcd5f0u)
#define DAT_00bcd5f8 (*(undefined4 *)0x00bcd5f8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd604 (*(undefined4 *)0x00bcd604u)
#define DAT_00bcd605 (*(undefined4 *)0x00bcd605u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd607 (*(undefined4 *)0x00bcd607u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd610 (*(undefined4 *)0x00bcd610u)
#define DAT_00bcd612 (*(undefined4 *)0x00bcd612u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd631 (*(undefined4 *)0x00bcd631u)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd674 (*(undefined4 *)0x00bcd674u)
#define DAT_00bcd67e (*(undefined4 *)0x00bcd67eu)
#define DAT_00bcd68a (*(undefined4 *)0x00bcd68au)
#define DAT_00bcd72a (*(undefined4 *)0x00bcd72au)
#define DAT_00bcd78a (*(undefined4 *)0x00bcd78au)
#define DAT_00d056fc (*(undefined *)0x00d056fcu)
extern void FUN_00444b30();
extern undefined4 FUN_004b47a0();
extern void FUN_004b52f0();
extern void FUN_004b5af0();
extern uint FUN_00562348();

int FUN_004b4800(undefined1 param_1,int param_2,int param_3,int param_4,undefined1 param_5)

{
  char cVar1;
  byte bVar2;
  undefined2 uVar3;
  int iVar4;
  int iVar5;
  char *pcVar6;
  int iVar7;
  undefined4 *puVar8;
  
  iVar5 = 1;
  pcVar6 = &DAT_00bcd78a;
  do {
    if (*pcVar6 == '\0') {
      cVar1 = FUN_004b47a0(iVar5);
      if (cVar1 == '\0') {
        iVar7 = iVar5 * 0x184;
        puVar8 = (undefined4 *)(&DAT_00bcd5f8 + iVar7);
        for (iVar4 = 0x61; iVar4 != 0; iVar4 = iVar4 + -1) {
          *puVar8 = 0;
          puVar8 = puVar8 + 1;
        }
        (&DAT_00bcd606)[iVar7] = 1;
        (&DAT_00bcd607)[iVar7] = param_1;
        (&DAT_00bcd602)[iVar7] = (char)param_2;
        (&DAT_00bcd604)[iVar7] = 0;
        (&DAT_00bcd605)[iVar7] = 1;
        *(undefined2 *)(&DAT_00bcd72a + iVar7) = 0xffff;
        (&DAT_00bcd67e)[iVar5 * 0xc2] = (short)DAT_00d056fc;
        (&DAT_00bcd609)[iVar7] = param_5;
        DAT_00d056fc = DAT_00d056fc + 1;
        uVar3 = (undefined2)param_3;
        *(undefined2 *)(&DAT_00bcd624 + iVar7) = uVar3;
        *(undefined2 *)(&DAT_00bcd61c + iVar7) = uVar3;
        *(undefined2 *)(&DAT_00bcd610 + iVar7) = uVar3;
        (&DAT_00bcd60c)[iVar5 * 0xc2] = uVar3;
        uVar3 = (undefined2)param_4;
        *(undefined2 *)(&DAT_00bcd626 + iVar7) = uVar3;
        *(undefined2 *)(&DAT_00bcd61e + iVar7) = uVar3;
        *(undefined2 *)(&DAT_00bcd612 + iVar7) = uVar3;
        (&DAT_00bcd60e)[iVar5 * 0xc2] = uVar3;
        *(int *)(&DAT_00bcd618 + iVar7) = DAT_00bca2d8 + param_4 * 0xe4 + param_3;
        (&DAT_00bcd642)[iVar5 * 0xc2] = (&DAT_00bcd60c)[iVar5 * 0xc2] * 0xf;
        (&DAT_00bcd644)[iVar5 * 0xc2] = (&DAT_00bcd60e)[iVar5 * 0xc2] * 0xf;
        (&DAT_00bcd631)[iVar7] = 0xf;
        (&DAT_00bcd68a)[iVar7] = 0;
        bVar2 = FUN_00562348();
        *(byte *)(iVar7 + 0xbcd676) = bVar2 & 3;
        (&DAT_00bcd674)[iVar7] = bVar2 & 3;
        FUN_004b5af0(iVar5);
        FUN_004b52f0(iVar5);
        if ((param_2 == 0x13) || (param_2 == 0x14)) {
          FUN_00444b30(iVar5);
        }
        if (DAT_00bcd5f0 < iVar5) {
          DAT_00bcd5f0 = iVar5;
        }
        return iVar5;
      }
    }
    pcVar6 = pcVar6 + 0x184;
    iVar5 = iVar5 + 1;
  } while ((int)pcVar6 < 0xc8ad46);
  return 0;
}

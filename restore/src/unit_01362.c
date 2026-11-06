#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_009784dc (*(undefined4 *)0x009784dcu)
#define DAT_00bca2d8 (*(uint32_t *)0x00bca2d8u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd608 (*(undefined4 *)0x00bcd608u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd610 (*(undefined4 *)0x00bcd610u)
#define DAT_00bcd612 (*(undefined4 *)0x00bcd612u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd631 (*(undefined4 *)0x00bcd631u)
#define DAT_00bcd642 (*(undefined4 *)0x00bcd642u)
#define DAT_00bcd644 (*(undefined4 *)0x00bcd644u)
#define DAT_00bcd714 (*(undefined4 *)0x00bcd714u)
#define DAT_00bcd794 (*(undefined4 *)0x00bcd794u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_00480180();
extern int FUN_00492360();
extern int FUN_0049b3c0();
extern int FUN_004a34b0();
extern int FUN_004a3540();
extern void FUN_004b52f0();

void FUN_004a38a0(int param_1,int param_2)

{
  int iVar1;
  undefined2 uVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  undefined2 *puVar7;
  int local_4;
  
  iVar1 = *(int *)(&DAT_00bcd714 + param_1 * 0x184);
  local_4 = FUN_004a34b0(param_1);
  if (0 < local_4) {
    piVar4 = &DAT_009784dc + iVar1 * 0xb;
    do {
      iVar1 = *piVar4;
      *piVar4 = -1;
      iVar6 = iVar1 * 0x184;
      if (((&DAT_00bcd606)[iVar6] == '\x01') && ((&DAT_00bcd630)[iVar6] == '\x06')) {
        FUN_00480180((int)*(short *)((int)&DAT_007999dc + param_2 * 0x108 + 2),
                     (int)*(short *)(&DAT_007999e0 + param_2 * 0x108),2);
        iVar3 = _DAT_00d3b4e4;
        iVar5 = _DAT_00d3b4e0;
        uVar2 = (undefined2)_DAT_00d3b4e4;
        *(undefined2 *)(&DAT_00bcd610 + iVar6) = uVar2;
        (&DAT_00bcd60c)[iVar1 * 0xc2] = uVar2;
        uVar2 = (undefined2)iVar5;
        *(undefined2 *)(&DAT_00bcd612 + iVar6) = uVar2;
        (&DAT_00bcd60e)[iVar1 * 0xc2] = uVar2;
        *(int *)(&DAT_00bcd618 + iVar6) = DAT_00bca2d8 + iVar5 * 0xe4 + iVar3;
        (&DAT_00bcd642)[iVar1 * 0xc2] = (&DAT_00bcd60c)[iVar1 * 0xc2] * 0xf;
        (&DAT_00bcd644)[iVar1 * 0xc2] = (&DAT_00bcd60e)[iVar1 * 0xc2] * 0xf;
        (&DAT_00bcd630)[iVar6] = (&DAT_00bcd608)[iVar6];
        (&DAT_00bcd631)[iVar6] = 0xf;
        FUN_004b52f0(iVar1);
        iVar5 = 1;
        puVar7 = &DAT_00bcd794;
        do {
          switch(*(undefined1 *)(puVar7 + -7)) {
          case 0x15: ;
            iVar3 = FUN_00492360(iVar5);
            break;
default: ;
            goto switchD_004a39d4_caseD_16;
          case 0x42: ;
            iVar3 = FUN_0049b3c0(iVar5);
            break;
          case 0x56: ;
          case 0x60: ;
            iVar3 = FUN_004a3540(iVar5);
          }
          if (iVar3 == iVar1) {
            uVar2 = (&DAT_00bcd60c)[iVar1 * 0xc2];
            *puVar7 = uVar2;
            puVar7[-2] = uVar2;
            uVar2 = (&DAT_00bcd60e)[iVar1 * 0xc2];
            puVar7[1] = uVar2;
            puVar7[-1] = uVar2;
            *(int *)(puVar7 + 4) =
                 DAT_00bca2d8 + (short)(&DAT_00bcd60c)[iVar1 * 0xc2] +
                 (short)(&DAT_00bcd60e)[iVar1 * 0xc2] * 0xe4;
            puVar7[0x19] = puVar7[-2] * 0xf;
            puVar7[0x1a] = puVar7[-1] * 0xf;
            (&DAT_00bcd631)[iVar6] = 0xf;
            FUN_004b52f0(iVar5);
          }
switchD_004a39d4_caseD_16: ;
          puVar7 = puVar7 + 0xc2;
          iVar5 = iVar5 + 1;
        } while ((int)puVar7 < 0xc8ad50);
      }
      piVar4 = piVar4 + 1;
      local_4 = local_4 + -1;
    } while (local_4 != 0);
  }
  return;
}

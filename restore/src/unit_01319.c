#include "restore_types.h"
#define DAT_005d2c48 (*(undefined4 *)0x005d2c48u)
#define DAT_00784e78 (*(undefined4 *)0x00784e78u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
extern uint FUN_004ea930();
extern void FUN_005468d0();

void FUN_00492fc0(void)

{
  short sVar1;
  char cVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  short *psVar8;
  
  iVar7 = 0;
  iVar5 = 0;
  iVar3 = 0;
  piVar6 = (int *)&DAT_005d2c48;
  piVar4 = &DAT_00784e78;
  do {
    switch((int)(iVar3)) {
    case 1: ;
    case 8: ;
    case 10: ;
    case 0xd: ;
    case 0xf: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1a: ;
    case 0x1c: ;
    case 0x1e: ;
      if ((*piVar4 != 0) && (iVar7 < *piVar6)) {
        iVar5 = iVar3;
        iVar7 = *piVar6;
      }
    }
    piVar4 = piVar4 + 1;
    iVar3 = iVar3 + 1;
    piVar6 = piVar6 + 2;
  } while ((int)piVar4 < 0x784f08);
  if (0 < iVar5) {
    (&DAT_00784e78)[iVar5] = (&DAT_00784e78)[iVar5] + -1;
  }
  cVar2 = FUN_004ea930();
  if ((cVar2 == '\0') && (iVar5 = 1, 1 < DAT_0078d790)) {
    psVar8 = &DAT_00799af0;
    do {
      sVar1 = *psVar8;
      if ((((sVar1 == 0xe5) || (sVar1 == 0xea)) || (sVar1 == 0xeb)) || (sVar1 == 0xec)) {
        FUN_005468d0(iVar5);
      }
      iVar5 = iVar5 + 1;
      psVar8 = psVar8 + 0x84;
    } while (iVar5 < DAT_0078d790);
  }
  return;
}

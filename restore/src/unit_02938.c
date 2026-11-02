#include "restore_types.h"
#define DAT_00605148 (*(undefined *)0x00605148u)
#define DAT_0060514c (*(undefined *)0x0060514cu)
#define DAT_00605154 (*(undefined *)0x00605154u)
#define DAT_01266594 (*(undefined *)0x01266594u)
extern int * FUN_005678f9();

LONG FUN_005677b8(undefined4 param_1,_EXCEPTION_POINTERS *param_2)

{
  code *pcVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  LONG LVar5;
  int iVar6;
  undefined4 *puVar7;
  
  piVar4 = (int *)FUN_005678f9(param_1);
  uVar3 = DAT_01266594;
  if ((piVar4 == (int *)0x0) || (pcVar1 = (code *)piVar4[2], pcVar1 == (code *)0x0)) {
    LVar5 = UnhandledExceptionFilter(param_2);
  }
  else if (pcVar1 == (code *)0x5) {
    piVar4[2] = 0;
    LVar5 = 1;
  }
  else {
    if (pcVar1 != (code *)0x1) {
      DAT_01266594 = param_2;
      if (piVar4[1] == 8) {
        if (DAT_00605148 < DAT_0060514c + DAT_00605148) {
          iVar6 = (DAT_0060514c + DAT_00605148) - DAT_00605148;
          puVar7 = (undefined4 *)(DAT_00605148 * 0xc + 0x6050d8);
          do {
            *puVar7 = 0;
            puVar7 = puVar7 + 3;
            iVar6 = iVar6 + -1;
          } while (iVar6 != 0);
        }
        uVar2 = DAT_00605154;
        iVar6 = *piVar4;
        if (iVar6 == -0x3fffff72) {
          DAT_00605154 = 0x83;
        }
        else if (iVar6 == -0x3fffff70) {
          DAT_00605154 = 0x81;
        }
        else if (iVar6 == -0x3fffff6f) {
          DAT_00605154 = 0x84;
        }
        else if (iVar6 == -0x3fffff6d) {
          DAT_00605154 = 0x85;
        }
        else if (iVar6 == -0x3fffff73) {
          DAT_00605154 = 0x82;
        }
        else if (iVar6 == -0x3fffff71) {
          DAT_00605154 = 0x86;
        }
        else if (iVar6 == -0x3fffff6e) {
          DAT_00605154 = 0x8a;
        }
        (*pcVar1)(8,DAT_00605154);
        DAT_00605154 = uVar2;
      }
      else {
        piVar4[2] = 0;
        (*pcVar1)(piVar4[1]);
      }
    }
    LVar5 = -1;
    DAT_01266594 = (_EXCEPTION_POINTERS *)uVar3;
  }
  return LVar5;
}

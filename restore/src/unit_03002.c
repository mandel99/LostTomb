#include "restore_types.h"
#define DAT_00605148 (*(undefined *)0x00605148u)
#define DAT_0060514c (*(undefined *)0x0060514cu)
#define DAT_00605154 (*(undefined *)0x00605154u)
#define DAT_01266594 (*(undefined *)0x01266594u)
#define DAT_012666f4 (*(undefined4 *)0x012666f4u)
#define DAT_012666f8 (*(undefined4 *)0x012666f8u)
#define DAT_012666fc (*(undefined4 *)0x012666fcu)
#define DAT_01266700 (*(undefined4 *)0x01266700u)
extern void FUN_005646fe();
extern undefined4 * FUN_00569dc7();

undefined4 FUN_00569c9a(int param_1)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  code *pcVar6;
  
  iVar2 = param_1;
  if (param_1 == 2) {
    puVar3 = &DAT_012666f4;
    pcVar6 = DAT_012666f4;
  }
  else if (((param_1 == 4) || (param_1 == 8)) || (param_1 == 0xb)) {
    iVar4 = FUN_00569dc7(param_1);
    pcVar6 = *(code **)(iVar4 + 8);
    puVar3 = (undefined4 *)(iVar4 + 8);
  }
  else if (param_1 == 0xf) {
    puVar3 = &DAT_01266700;
    pcVar6 = DAT_01266700;
  }
  else if (param_1 == 0x15) {
    puVar3 = &DAT_012666f8;
    pcVar6 = DAT_012666f8;
  }
  else {
    if (param_1 != 0x16) {
      return 0xffffffff;
    }
    puVar3 = &DAT_012666fc;
    pcVar6 = DAT_012666fc;
  }
  iVar1 = DAT_01266594;
  iVar4 = DAT_00605154;
  if (pcVar6 == (code *)0x1) {
    return 0;
  }
  if (pcVar6 == (code *)0x0) {
                    /* WARNING: Subroutine does not return */
    FUN_005646fe(3);
  }
  if (((param_1 == 8) || (param_1 == 0xb)) || (iVar5 = param_1, param_1 == 4)) {
    DAT_01266594 = 0;
    iVar5 = iVar1;
    if (param_1 == 8) {
      DAT_00605154 = 0x8c;
      param_1 = iVar4;
      goto LAB_00569d5e;
    }
LAB_00569d8a: ;
    *puVar3 = 0;
    if (iVar2 != 8) {
      (*pcVar6)(iVar2);
      if ((iVar2 != 0xb) && (iVar2 != 4)) {
        return 0;
      }
      goto LAB_00569dad;
    }
  }
  else {
LAB_00569d5e: ;
    if (iVar2 != 8) goto LAB_00569d8a;
    if (DAT_00605148 < DAT_0060514c + DAT_00605148) {
      iVar4 = (DAT_0060514c + DAT_00605148) - DAT_00605148;
      puVar3 = (undefined4 *)(DAT_00605148 * 0xc + 0x6050d8);
      do {
        *puVar3 = 0;
        puVar3 = puVar3 + 3;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
    }
  }
  (*pcVar6)(8,DAT_00605154);
LAB_00569dad: ;
  if (iVar2 == 8) {
    DAT_00605154 = param_1;
  }
  DAT_01266594 = iVar5;
  return 0;
}

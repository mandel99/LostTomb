#include "restore_types.h"
#define PTR_FUN_00604804 (*(unsigned char * *)0x00604804u)
#define PTR_LAB_00604808 (*(unsigned char * *)0x00604808u)

void FUN_005605c0(void)

{
  if (PTR_FUN_00604804 != (undefined *)0x0) {
    (*(code *)PTR_FUN_00604804)();
  }
                    /* WARNING: Could not recover jumptable at 0x005605d4. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)PTR_LAB_00604808)();
  return;
}

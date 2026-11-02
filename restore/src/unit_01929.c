#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00e950d4 (*(undefined *)0x00e950d4u)
extern undefined4 FUN_00480180();
extern undefined4 FUN_004802c0();
extern void FUN_004e2970();

void FUN_004e2b10(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  if ((((((((DAT_00605a3c != 1) && (param_1 != 0x29)) && (param_1 != 0x2a)) &&
         ((param_1 != 0x2b && (param_1 != 0x5c)))) &&
        ((param_1 != 0x39 && ((param_1 != 0x3a && (param_1 != 0xaa)))))) &&
       ((param_1 != 0xab &&
        ((((((param_1 != 200 && (param_1 != 0xc9)) && (param_1 != 0xca)) &&
           (((param_1 != 0x38 && (param_1 != 10)) &&
            ((param_1 != 0x28 && ((param_1 != 0x2c && (param_1 != 0x2d)))))))) && (param_1 != 0xd3))
         && (((param_1 != 0xd4 && (param_1 != 0xb7)) && (param_1 != 0xd2)))))))) &&
      (((param_1 != 0xe3 && (param_1 != 0xe1)) &&
       (((param_1 != 0xe6 && ((param_1 != 0xe4 && (param_1 != 0xde)))) && (param_1 != 0xcf)))))) &&
     (((((param_1 != 0xd0 && (param_1 != 0xd5)) && (param_1 != 0xe5)) &&
       ((param_1 != 0xea && (param_1 != 0xec)))) &&
      ((param_1 != 0xeb && (iVar1 = FUN_00480180(param_2,param_3,param_4), iVar1 == 0)))))) {
    if (((param_1 == 0x48) || (param_1 == 0xc1)) &&
       (iVar1 = FUN_00480180(param_2,param_3,3), iVar1 != 0)) {
      return;
    }
    if ((param_1 < 0x41) || (0x45 < param_1)) {
      if ((0x8b < param_1) &&
         ((param_1 < 0x96 && (iVar1 = FUN_004802c0(param_2,param_3), iVar1 != 0)))) {
        return;
      }
      if (DAT_00e950d4 < 3) {
        if ((0x8b < param_1) && (param_1 < 0x96)) {
          FUN_004e2970(0xcf,0,0);
          return;
        }
        FUN_004e2970(0xb,0,0);
      }
    }
  }
  return;
}

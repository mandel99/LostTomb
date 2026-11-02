#include "restore_types.h"
extern int FUN_00401f30();
extern int FUN_0040f070();
extern int FUN_004178e0();
extern int FUN_00420350();
extern int FUN_004e0c00();
extern int FUN_004e5c00();
extern int FUN_004e6e10();
extern undefined4 FUN_004f01e0();
extern int FUN_00542a80();

void FUN_004bc140(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = FUN_004f01e0(param_1,param_2,param_3);
  if (iVar1 == 0) {
    iVar1 = FUN_00420350(param_1,param_2,param_3);
    if (iVar1 == 0) {
      iVar1 = FUN_004e5c00(param_1,param_2,param_3);
      if (iVar1 == 0) {
        iVar1 = FUN_004178e0(param_1,param_2,param_3);
        if (iVar1 == 0) {
          iVar1 = FUN_004e6e10(param_1,param_2,param_3);
          if (iVar1 == 0) {
            iVar1 = FUN_00542a80(param_1,param_2,param_3);
            if (iVar1 == 0) {
              iVar1 = FUN_0040f070(param_1,param_2,param_3);
              if (iVar1 == 0) {
                iVar1 = FUN_00401f30(param_1,param_2,param_3);
                if (iVar1 == 0) {
                  FUN_004e0c00(param_1,param_2,param_3);
                }
              }
            }
          }
        }
      }
    }
  }
  return;
}

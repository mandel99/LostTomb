#include "restore_types.h"
#include "crt_translation_guard.h"
#include "crt_eh3_frames.h"
extern void FUN_00563be1();
extern undefined4 FUN_00563d10();
extern void FUN_00563e46();

void FUN_00563c95(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4,
                 undefined4 param_5,int param_6,int param_7,undefined4 *param_8,undefined4 param_9,
                 int param_10)

{
  int iVar1;
  
  if (param_7 != 0) {
    FUN_00563e46(param_1,param_2,param_6,param_7);
  }
  if (param_10 == 0) {
    param_10 = param_2;
  }
  restore_crt_unwind_exception((uint32_t)param_10,(EXCEPTION_RECORD *)(uintptr_t)param_1);
  FUN_00563be1(param_2,param_4,param_5,*param_8);
  *(int *)(param_2 + 8) = param_8[1] + 1;
  iVar1 = FUN_00563d10(param_1,param_2,param_3,param_5,*(undefined4 *)(param_6 + 0xc),param_9,0x100)
  ;
  if (iVar1 != 0) {
    restore_crt_jump_to_catch((uint32_t)iVar1,(uint32_t)param_2);
  }
  return;
}

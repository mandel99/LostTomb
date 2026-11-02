#include "restore_types.h"
#define DAT_006044b0 (*(undefined *)0x006044b0u)
extern bool FUN_0055d480();
extern bool FUN_0055d4c0();
extern int FUN_0055d590();
extern int FUN_0055d630();
extern longlong FUN_0056133c();

void FUN_0055d4f0(void)

{
  int iVar1;
  int in_ECX;
  float10 fVar2;
  float10 extraout_ST0;
  float10 extraout_ST1;
  undefined4 local_8;
  
  fVar2 = (float10)*(float *)(in_ECX + 0x50) * (float10)*(float *)(in_ECX + 0x4c) *
          (float10)0.00016666666;
  if (DAT_006044b0 != 0) {
    iVar1 = FUN_0056133c();
    fVar2 = (extraout_ST0 / (float10)iVar1) * extraout_ST1;
  }
  local_8 = (float)fVar2;
  if (fVar2 < (float10)0.0) {
    iVar1 = FUN_0055d4c0(local_8);
    if (iVar1 != 0) {
      FUN_0055d630();
    }
  }
  else {
    iVar1 = FUN_0055d480(local_8);
    if (iVar1 != 0) {
      FUN_0055d590();
      return;
    }
  }
  return;
}

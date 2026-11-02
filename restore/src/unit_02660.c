#include "restore_types.h"

void FUN_0055d290(short param_1)

{
  int iVar1;
  int in_ECX;
  
  iVar1 = *(int *)(in_ECX + 0x44);
  if (iVar1 != 0) {
    if (param_1 < 0) {
      do {
        param_1 = param_1 + *(short *)(iVar1 + 2);
      } while (param_1 < 0);
    }
    for (; (int)(uint)*(ushort *)(iVar1 + 2) <= (int)param_1;
        param_1 = (short)((int)param_1 % (int)(uint)*(ushort *)(iVar1 + 2))) {
    }
    *(float *)(in_ECX + 0x48) = (float)(int)param_1;
  }
  return;
}

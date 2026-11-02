#include "restore_types.h"
extern longlong FUN_0056133c();

int FUN_00445d10(int param_1,int param_2,int param_3)

{
  float fVar1;
  undefined2 uVar2;
  byte *in_ECX;
  int iVar3;
  int iVar4;
  
  iVar3 = 0;
  fVar1 = restore_x87_sqrt((float)((int)*(short *)(in_ECX + param_2 * 6 + 10) -
                      (int)*(short *)(in_ECX + param_1 * 6 + 10)) *
               (float)((int)*(short *)(in_ECX + param_2 * 6 + 10) -
                      (int)*(short *)(in_ECX + param_1 * 6 + 10)) +
               (float)((int)*(short *)(in_ECX + param_2 * 6 + 8) -
                      (int)*(short *)(in_ECX + param_1 * 6 + 8)) *
               (float)((int)*(short *)(in_ECX + param_2 * 6 + 8) -
                      (int)*(short *)(in_ECX + param_1 * 6 + 8)));
  if (fVar1 == 0.0) {
    return 0;
  }
  if (*in_ECX != 0) {
    param_2 = 0;
    if (0.0 <= fVar1) {
      iVar4 = param_3 * 6;
      do {
        uVar2 = FUN_0056133c();
        *(undefined2 *)(iVar4 + *(int *)(in_ECX + 4)) = uVar2;
        uVar2 = FUN_0056133c();
        *(undefined2 *)(iVar4 + 2 + *(int *)(in_ECX + 4)) = uVar2;
        *(char *)(iVar4 + 4 + *(int *)(in_ECX + 4)) = (char)param_1;
        param_2 = param_2 + (uint)*in_ECX;
        iVar3 = iVar3 + 1;
        iVar4 = iVar4 + 6;
      } while ((float)(int)param_2 <= fVar1);
    }
    return iVar3;
  }
  return 0;
}

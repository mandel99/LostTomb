#include "restore_types.h"

int FUN_0055d590(void)

{
  float fVar1;
  float fVar2;
  short sVar3;
  int in_ECX;
  int iVar4;
  
  iVar4 = 0;
  fVar2 = (float)*(ushort *)(*(int *)(in_ECX + 0x44) + 2);
  fVar1 = *(float *)(in_ECX + 0x48);
  while ((fVar2 <= fVar1 && (iVar4 == 0))) {
    fVar1 = *(float *)(in_ECX + 0x48) - fVar2;
    *(float *)(in_ECX + 0x48) = fVar1;
    if ((*(short *)(in_ECX + 0x78) != 0) &&
       (sVar3 = *(short *)(in_ECX + 0x78) + -1, *(short *)(in_ECX + 0x78) = sVar3, sVar3 == 0)) {
      if (fVar2 <= fVar1) {
        *(float *)(in_ECX + 0x48) = fVar2 - 1.0;
      }
      iVar4 = 1;
      *(ushort *)(in_ECX + 0x78) =
           (ushort)(*(byte **)(in_ECX + 0x40))[3] << ((**(byte **)(in_ECX + 0x40) & 7) == 4);
    }
    fVar1 = *(float *)(in_ECX + 0x48);
  }
  return iVar4;
}

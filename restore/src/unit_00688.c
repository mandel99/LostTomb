#include "restore_types.h"

uint FUN_00448e30(int param_1)

{
  uint in_EAX;
  uint *puVar1;
  short *psVar2;
  uint uVar3;
  uint *in_ECX;
  int iVar4;
  int iVar5;
  uint *puVar6;
  uint *puVar7;
  
  if ((-1 < param_1) && (in_EAX = *in_ECX, param_1 < (int)in_EAX)) {
    if (param_1 < (int)(in_EAX - 1)) {
      puVar1 = in_ECX + param_1 * 0x1f + 1;
      iVar5 = param_1;
      do {
        puVar6 = puVar1 + 0x1f;
        puVar7 = puVar1;
        for (iVar4 = 0x1f; iVar4 != 0; iVar4 = iVar4 + -1) {
          *puVar7 = *puVar6;
          puVar6 = puVar6 + 1;
          puVar7 = puVar7 + 1;
        }
        *(short *)puVar1 = (short)iVar5;
        iVar5 = iVar5 + 1;
        puVar1 = puVar1 + 0x1f;
      } while (iVar5 < (int)(*in_ECX - 1));
    }
    uVar3 = *in_ECX;
    *in_ECX = uVar3 - 1;
    iVar5 = 0;
    if (0 < (int)(uVar3 - 1)) {
      psVar2 = (short *)((int)in_ECX + 0x2a);
      do {
        if (param_1 < psVar2[-1]) {
          psVar2[-1] = psVar2[-1] + -1;
        }
        if (param_1 < *psVar2) {
          *psVar2 = *psVar2 + -1;
        }
        if (param_1 < psVar2[0x16]) {
          psVar2[0x16] = psVar2[0x16] + -1;
        }
        if (param_1 < psVar2[0x17]) {
          psVar2[0x17] = psVar2[0x17] + -1;
        }
        iVar5 = iVar5 + 1;
        psVar2 = psVar2 + 0x3e;
      } while (iVar5 < (int)*in_ECX);
    }
    uVar3 = in_ECX[0x122c];
    if ((int)*in_ECX <= (int)uVar3) {
      uVar3 = uVar3 - 1;
      in_ECX[0x122c] = uVar3;
      if ((int)uVar3 < 0) {
        in_ECX[0x122c] = 0;
      }
    }
    if ((int)*in_ECX <= (int)in_ECX[0x122b]) {
      in_ECX[0x122b] = 0xffffffff;
    }
    return CONCAT31((int3)(uVar3 >> 8),1);
  }
  return in_EAX & 0xffffff00;
}

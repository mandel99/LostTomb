#include "restore_types.h"

uint FUN_004ddbd0(int param_1,int param_2,undefined1 *param_3)

{
  uint uVar1;
  short *psVar2;
  int in_ECX;
  int iVar3;
  
  uVar1 = 0;
  if (*(byte *)(in_ECX + 0x141) != 0) {
    psVar2 = (short *)(in_ECX + 10);
    do {
      if ((((param_1 + -8 <= (int)psVar2[-1]) && ((int)psVar2[-1] <= param_1 + 8)) &&
          (param_2 + -8 <= (int)*psVar2)) && ((int)*psVar2 <= param_2 + 8)) {
        *param_3 = 1;
        return uVar1;
      }
      uVar1 = uVar1 + 1;
      psVar2 = psVar2 + 3;
    } while ((int)uVar1 < (int)(uint)*(byte *)(in_ECX + 0x141));
  }
  iVar3 = 0;
  *param_3 = 0;
  if (*(int *)(in_ECX + 0x134) < 1) {
    return 0xffffffff;
  }
  psVar2 = *(short **)(in_ECX + 4);
  while ((((int)*psVar2 < param_1 + -8 || (param_1 + 8 < (int)*psVar2)) ||
         (((int)psVar2[1] < param_2 + -8 || (param_2 + 8 < (int)psVar2[1]))))) {
    iVar3 = iVar3 + 1;
    psVar2 = psVar2 + 3;
    if (*(int *)(in_ECX + 0x134) <= iVar3) {
      return 0xffffffff;
    }
  }
  return (uint)*(byte *)(*(short **)(in_ECX + 4) + iVar3 * 3 + 2);
}

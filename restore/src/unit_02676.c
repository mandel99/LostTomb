#include "restore_types.h"
extern void FUN_0055d9c0();

uint FUN_0055d840(void)

{
  undefined4 uVar1;
  uint in_EAX;
  uint uVar2;
  uint *puVar3;
  int in_ECX;
  float10 fVar4;
  undefined1 auStack_8 [8];
  
  if (*(int *)(in_ECX + 0x38) == 0) {
    return in_EAX & 0xffff0000;
  }
  uVar2 = 0;
  if (*(int *)(*(int *)(in_ECX + 0x38) + 8) != 0) {
    uVar1 = *(undefined4 *)(in_ECX + 0x40);
    fVar4 = (float10)(*(code *)**(undefined4 **)(in_ECX + 4))();
    puVar3 = (uint *)(FUN_0055d9c0(auStack_8,uVar1,(float)fVar4), restore_unresolved_return(0x0055d9c0u));
    uVar2 = *puVar3;
    if (puVar3[1] != 0) {
      *(uint *)(in_ECX + 0x34) = *(uint *)(in_ECX + 0x34) | 1;
      return uVar2;
    }
    *(uint *)(in_ECX + 0x34) = *(uint *)(in_ECX + 0x34) & 0xfffffffe;
  }
  return uVar2;
}

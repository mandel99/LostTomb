#include "restore_types.h"
#define DAT_012652f8 (*(undefined *)0x012652f8u)
#define _DAT_012652f4 (*(undefined4 *)0x012652f4u)
extern void FUN_0055db90();
extern void FUN_00561363();
extern void FUN_00561a2b();

void FUN_0055d8b0(void)

{
  int iVar1;
  int in_ECX;
  
  if (*(undefined4 **)(in_ECX + 0x114) != (undefined4 *)0x0) {
    (**(code **)**(undefined4 **)(in_ECX + 0x114))(3);
    *(undefined4 *)(in_ECX + 0x114) = 0;
  }
  iVar1 = *(int *)(in_ECX + 0x118);
  if (iVar1 != 0) {
    FUN_00561a2b(*(undefined4 *)(iVar1 + 8));
    FUN_00561a2b(iVar1);
    *(undefined4 *)(in_ECX + 0x118) = 0;
  }
  iVar1 = *(int *)(in_ECX + 0x11c);
  if (iVar1 != 0) {
    FUN_00561363(iVar1,0x18,*(undefined4 *)(iVar1 + -4),FUN_0055db90);
    FUN_00561a2b(iVar1 + -4);
    *(undefined4 *)(in_ECX + 0x11c) = 0;
  }
  iVar1 = *(int *)(in_ECX + 0x120);
  if (iVar1 != 0) {
    FUN_00561a2b(*(undefined4 *)(iVar1 + 8));
    FUN_00561a2b(iVar1);
    *(undefined4 *)(in_ECX + 0x120) = 0;
  }
  if (DAT_012652f8 != 0) {
    FUN_00561a2b(DAT_012652f8);
    DAT_012652f8 = 0;
    _DAT_012652f4 = 0;
  }
  return;
}

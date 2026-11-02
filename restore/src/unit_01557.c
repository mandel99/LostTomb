#include "restore_types.h"
#include "city_prepare.h"

void FUN_004be0f0(void *world)

{
  undefined4 *puVar1;
  undefined4 *in_ECX=(undefined4 *)world;
  int iVar2;
  undefined4 *puVar3;
  
  puVar3 = in_ECX;
  for (iVar2 = 0xcb10; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  iVar2 = 0;
  puVar3 = in_ECX + 0xcb1a;
  do {
    puVar1 = puVar3 + -1;
    FUN_004bd6a0((RestoreWorldRegion *)puVar1);
    iVar2 = iVar2 + 1;
    *puVar3 = 0;
    puVar3[1] = iVar2;
    puVar3[2] = in_ECX;
    puVar3[5] = 0;
    puVar3 = puVar3 + 7;
    *puVar1 = 0;
  } while (iVar2 < 0x1e);
  in_ECX[0xcbeb] = 0x1e;
  in_ECX[0xcbec] = 0;
  *(undefined1 *)(in_ECX + 0xcbef) = 0;
  return;
}

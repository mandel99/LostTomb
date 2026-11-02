#include "restore_types.h"
#define DAT_00bca848 (*(undefined *)0x00bca848u)
#define DAT_00bca850 (*(undefined4 *)0x00bca850u)
#define DAT_00d05710 (*(undefined1 *)0x00d05710u)

void FUN_00489b40(void)

{
  BOOL BVar1;
  int iVar2;
  undefined4 *puVar3;
  
  BVar1 = QueryPerformanceFrequency((LARGE_INTEGER *)&DAT_00bca848);
  DAT_00d05710 = BVar1 != 0;
  puVar3 = &DAT_00bca850;
  for (iVar2 = 0x370; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar3 = 0;
    puVar3 = puVar3 + 1;
  }
  return;
}

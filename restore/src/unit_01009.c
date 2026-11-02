#include "restore_types.h"
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d0 (*(int32_t *)0x00bca2d0u)
#define DAT_00bca2d4 (*(int32_t *)0x00bca2d4u)
#define DAT_00bca2e8 (*(int32_t *)0x00bca2e8u)
#define DAT_00bca2ec (*(int32_t *)0x00bca2ecu)
#define DAT_00bca2f0 (*(int32_t *)0x00bca2f0u)
#define DAT_00bca2f4 (*(int32_t *)0x00bca2f4u)
#define DAT_00bca2f8 (*(int32_t *)0x00bca2f8u)
#define DAT_00bca2fc (*(int32_t *)0x00bca2fcu)

void FUN_0046f7b0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int local_4;
  
  iVar5 = (0xe4 - DAT_00bca2d0) / 2;
  iVar1 = (0xe4 - DAT_00bca2d4) / 2;
  DAT_00bca2f0 = iVar5 + -1 + DAT_00bca2d0;
  DAT_00bca2f4 = iVar1 + -1 + DAT_00bca2d4;
  DAT_00bca2f8 = (DAT_00bca2f0 + iVar5) / 2;
  DAT_00bca2fc = (DAT_00bca2f4 + iVar1) / 2;
  DAT_00bca2e8 = iVar5;
  DAT_00bca2ec = iVar1;
  if (iVar1 <= DAT_00bca2f4) {
    iVar6 = iVar1 * 4;
    local_4 = iVar1 * 0xe4;
    iVar3 = (DAT_00bca2fc - iVar1) + iVar5;
    iVar2 = iVar1;
    do {
      iVar4 = iVar3;
      if (DAT_00bca2fc < iVar2) {
        iVar4 = (iVar5 - DAT_00bca2fc) + -1 + iVar2;
      }
      *(int *)((int)&DAT_00bc9f24 + iVar6 + iVar1 * -4) = iVar4 - iVar5;
      *(int *)((int)&DAT_00bc9b94 + iVar6 + iVar1 * -4) = ((DAT_00bca2f8 * 2 - iVar4) + 2) - iVar5;
      iVar4 = iVar4 + local_4;
      iVar7 = iVar6 + iVar1 * -4;
      iVar2 = iVar2 + 1;
      iVar6 = iVar6 + 4;
      local_4 = local_4 + 0xe4;
      *(int *)((int)&DAT_00bc9804 + iVar7) = iVar4;
      iVar3 = iVar3 + -1;
    } while (iVar2 <= DAT_00bca2f4);
  }
  return;
}

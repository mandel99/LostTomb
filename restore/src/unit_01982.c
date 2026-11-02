#include "restore_types.h"
#define DAT_00586b08 (*(undefined4 *)0x00586b08u)
#define DAT_0058fe48 (*(undefined4 *)0x0058fe48u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#include "audio_state.h"

void FUN_004e5b90(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  int iVar5;
  
  iVar5 = 0;
  for (iVar3 = 0; iVar3 < (int)((-(uint)((&DAT_007999e8)[param_1 * 0x84] != 0xcf) & 8) + 10);
      iVar3 = iVar3 + 1) {
    puVar4 = (undefined4 *)&DAT_00586b08;
    if ((&DAT_007999e8)[param_1 * 0x84] != 0xcf) {
      puVar4 = &DAT_0058fe48;
    }
    *(undefined2 *)(*(int *)(iVar5 + 8 + (int)puVar4) + 0x12) = 0;
    FUN_0041fca0(*(void **)(iVar5+8+(uintptr_t)puVar4),3);
    iVar1 = iVar5 + 8;
    iVar2 = iVar5 + 0xc;
    iVar5 = iVar5 + 0x10;
    *(undefined4 *)(*(int *)(iVar1 + (int)puVar4) + 0xc) = *(undefined4 *)(iVar2 + (int)puVar4);
  }
  return;
}

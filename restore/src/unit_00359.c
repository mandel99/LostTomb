#include "restore_types.h"
#define PTR_DAT_005b0060 (*(unsigned char * *)0x005b0060u)
#include "audio_state.h"

void FUN_00421010(void)

{
  undefined **ppuVar1;
  undefined *puVar2;
  undefined **ppuVar3;
  
  ppuVar3 = &PTR_DAT_005b0060;
  do {
    *(undefined2 *)(*ppuVar3 + 0x12) = 0;
    FUN_0041fca0(*ppuVar3,3);
    puVar2 = *ppuVar3;
    ppuVar1 = ppuVar3 + 1;
    ppuVar3 = ppuVar3 + 4;
    *(undefined **)(puVar2 + 0xc) = *ppuVar1;
  } while ((int)ppuVar3 < 0x5b0150);
  return;
}

#include "restore_types.h"
#define PTR_DAT_005c2750 (*(unsigned char * *)0x005c2750u)
#include "audio_state.h"

void FUN_0041ff90(void)

{
  undefined **ppuVar1;
  undefined *puVar2;
  undefined **ppuVar3;
  
  ppuVar3 = &PTR_DAT_005c2750;
  do {
    *(undefined2 *)(*ppuVar3 + 0x12) = 0;
    FUN_0041fca0(*ppuVar3,3);
    puVar2 = *ppuVar3;
    ppuVar1 = ppuVar3 + 1;
    ppuVar3 = ppuVar3 + 4;
    *(undefined **)(puVar2 + 0xc) = *ppuVar1;
  } while ((int)ppuVar3 < 0x5c28b0);
  return;
}

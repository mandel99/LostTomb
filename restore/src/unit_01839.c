#include "restore_types.h"
#define DAT_005c28ac (*(undefined4 *)0x005c28acu)
#define DAT_00e94dac (*(int32_t *)0x00e94dacu)
#define DAT_00e94db0 (*(int32_t *)0x00e94db0u)
extern int FUN_004d5f80();

void FUN_004d5e70(void)

{
  uint uVar1;
  int iVar2;
  short *psVar3;
  bool bVar4;
  
  uVar1 = FUN_004d5f80(0x1c);
  iVar2 = (int)(uVar1 + ((int)uVar1 >> 0x1f & 0xfU)) >> 4;
  uVar1 = uVar1 & 0x8000000f;
  bVar4 = uVar1 == 0;
  if ((int)uVar1 < 0) {
    bVar4 = (uVar1 - 1 | 0xfffffff0) == 0xffffffff;
  }
  if (!bVar4) {
    iVar2 = iVar2 + 1;
  }
  psVar3 = &DAT_005c28ac;
  DAT_00e94db0 = iVar2;
  do {
    *psVar3 = ((short)iVar2 + 7) * 0x10;
    psVar3 = psVar3 + 0xe;
  } while ((int)psVar3 < 0x5c2bf4);
  DAT_00e94dac = iVar2 << 4;
  return;
}

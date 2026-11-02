#include "restore_types.h"
#include "sprite_libraries.h"
#define DAT_00709a34 (*(undefined4 *)0x00709a34u)
#define DAT_007848fc (*(undefined *)0x007848fcu)
#define PTR_s_HYKSOS_005def84 (*(unsigned char * *)0x005def84u)

void FUN_004bc720(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  int iVar3;
  
  iVar3 = (int)DAT_007848fc;
  if (0xb < iVar3) {
    iVar3 = 0xb;
    DAT_007848fc = 0xb;
  }
  FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(&PTR_s_HYKSOS_005def84)[iVar3],2);
  puVar1 = &DAT_00709a34;
  puVar2 = (undefined4 *)(iVar3 * 0x118 + 0x7083e4);
  do {
    puVar1[0xe] = puVar2[-0xe];
    *puVar1 = *puVar2;
    puVar1[0x1c] = puVar2[0xe];
    puVar1[700] = puVar2[0x1c];
    puVar1[0x2ae] = puVar2[0x2a];
    puVar1 = puVar1 + 1;
    puVar2 = puVar2 + 1;
  } while ((int)puVar1 < 0x709a6c);
  return;
}

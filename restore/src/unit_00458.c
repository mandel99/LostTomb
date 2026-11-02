#include "restore_types.h"
#define DAT_0070e494 (*(undefined4 *)0x0070e494u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

void FUN_004304c0(uint param_1)

{
  int iVar1;
  uint *puVar2;
  uint *puVar3;
  int iVar4;
  
  _DAT_00d3b4e0 = 0;
  _DAT_00d3b4e4 = 0;
  iVar4 = 0;
  puVar3 = &DAT_0070e494;
  do {
    iVar1 = 0;
    puVar2 = puVar3;
    do {
      if ((*puVar2 < 0xfff0000) && (*puVar2 == param_1)) {
        _DAT_00d3b4e0 = iVar4;
        _DAT_00d3b4e4 = iVar1;
        return;
      }
      iVar1 = iVar1 + 1;
      puVar2 = puVar2 + 1;
    } while (iVar1 < 0xe7);
    puVar3 = puVar3 + 0xe7;
    iVar4 = iVar4 + 1;
    if (0x77560f < (int)puVar3) {
      return;
    }
  } while( true );
}

#include "restore_types.h"
#define DAT_00acf85c (*(uint8_t *)0x00acf85cu)
#define DAT_00d3b1c4 (*(uint32_t *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(uint32_t *)0x00d3b374u)
#define DAT_00d3b380 (*(uint32_t *)0x00d3b380u)
#define DAT_00d3b384 (*(uint32_t *)0x00d3b384u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)

void FUN_0046fc80(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int local_8;
  int local_4;
  
  iVar6 = 0;
  local_4 = 0xe4;
  iVar4 = 0xe4;
  while( true ) {
    do {
      (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
      DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
      if (99 < DAT_00d3b1c4) {
        DAT_00d3b1c4 = 0;
      }
      local_8 = 0x1f;
      do {
        uVar1 = DAT_00d3b384 >> 4;
        uVar2 = DAT_00d3b384 & 1;
        uVar3 = DAT_00d3b380 >> 4;
        uVar5 = DAT_00d3b380 & 1;
        DAT_00d3b384 = DAT_00d3b384 >> 1;
        DAT_00d3b380 = DAT_00d3b380 >> 1;
        if ((uVar1 & 1) != uVar2) {
          DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
        }
        if ((uVar3 & 1) != uVar5) {
          DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
        }
        local_8 = local_8 + -1;
      } while (local_8 != 0);
      _DAT_00d3b378 = DAT_00d3b384 & 7;
      _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
      _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
      DAT_00d3b374 = DAT_00d3b384 & 0x7f;
      (&DAT_00acf85c)[iVar6] = (char)(DAT_00d3b384 & 0x7fff);
      iVar6 = iVar6 + 1;
      iVar4 = iVar4 + -1;
      _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
      _DAT_00d3b36c = DAT_00d3b380 & 7;
    } while (iVar4 != 0);
    local_4 = local_4 + -1;
    if (local_4 == 0) break;
    iVar4 = 0xe4;
  }
  return;
}

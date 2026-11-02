#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00a76aec (*(undefined4 *)0x00a76aecu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ea5f1c (*(undefined4 *)0x00ea5f1cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

undefined4 FUN_004802c0(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  uint *puVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *local_6c;
  int local_68;
  int local_64;
  int local_60 [8];
  int local_40 [16];
  
  local_40[9] = 0xfffffffe;
  local_40[0xc] = 0xfffffffe;
  local_6c = local_60;
  iVar5 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  local_40[2] = 1;
  local_40[7] = 1;
  local_40[0] = 0;
  local_40[1] = 0xffffffff;
  local_40[3] = 0;
  local_40[4] = 0xffffffff;
  local_40[5] = 0;
  local_40[6] = 0;
  local_40[8] = 0;
  local_40[10] = 2;
  local_40[0xb] = 0;
  local_40[0xd] = 0;
  local_40[0xe] = 0;
  local_40[0xf] = 2;
  local_68 = 0xc;
  iVar2 = 0;
  local_64 = iVar5;
  do {
    iVar1 = *local_6c;
    if (iVar1 == 0) break;
    if (((((&DAT_00af598c)[iVar1 + iVar5] & 8) == 0) ||
        (((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + (iVar1 + iVar5) * 2) * 0x84] != 0x8a &&
         ((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + (iVar1 + iVar5) * 2) * 0x84] != 0x88)))) &&
       (((&DAT_00af598c)[iVar1 + iVar5] & 0x40) != 0)) {
      iVar4 = 0;
      puVar3 = &DAT_00ea5f1c + DAT_00eaac28 * 0x1276;
      do {
        iVar6 = iVar4;
        if (*puVar3 == (uint)(byte)(&DAT_00a76aec)[iVar5 + iVar1]) break;
        iVar4 = iVar4 + 1;
        puVar3 = puVar3 + 2;
        iVar6 = 0xb;
      } while (iVar4 < 10);
      if (iVar6 < local_68) {
        local_68 = iVar6;
        local_64 = iVar2;
      }
    }
    iVar2 = iVar2 + 1;
    local_6c = local_6c + 1;
  } while (iVar2 < 8);
  if (0xb < local_68) {
    return 0;
  }
  _DAT_00d3b4e4 = local_40[local_64 * 2] + param_1;
  _DAT_00d3b4e0 = local_40[local_64 * 2 + 1] + param_2;
  return 1;
}

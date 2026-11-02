#include "restore_types.h"
#define DAT_005cc9e0 (*(undefined4 *)0x005cc9e0u)
#define DAT_005cce98 (*(undefined4 *)0x005cce98u)
#define DAT_005cce9c (*(undefined4 *)0x005cce9cu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00a76aec (*(undefined4 *)0x00a76aecu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ea5f1c (*(undefined4 *)0x00ea5f1cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

undefined4 FUN_00480180(int param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  uint *puVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int local_c;
  int local_8;
  int local_4;
  
  local_8 = 0xc;
  local_c = 0;
  iVar5 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  piVar6 = (int *)(&DAT_005cc9e0 + param_3 * 0x60);
  local_4 = iVar5;
  do {
    iVar1 = *piVar6;
    if (iVar1 == 0) break;
    uVar2 = (&DAT_00af598c)[iVar1 + iVar5];
    if (((((uVar2 & 8) == 0) ||
         (((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + (iVar1 + iVar5) * 2) * 0x84] != 0x8a &&
          ((&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + (iVar1 + iVar5) * 2) * 0x84] != 0x88)))) &&
        ((uVar2 & 0x40) != 0)) && ((uVar2 & 4) == 0)) {
      iVar4 = 0;
      puVar3 = &DAT_00ea5f1c + DAT_00eaac28 * 0x1276;
      do {
        iVar7 = iVar4;
        if (*puVar3 == (uint)(byte)(&DAT_00a76aec)[iVar5 + iVar1]) break;
        iVar4 = iVar4 + 1;
        puVar3 = puVar3 + 2;
        iVar7 = 0xb;
      } while (iVar4 < 10);
      if (iVar7 < local_8) {
        local_4 = local_c;
        local_8 = iVar7;
      }
    }
    piVar6 = piVar6 + 1;
    local_c = local_c + 1;
  } while (local_c < 0x18);
  if (0xb < local_8) {
    return 0;
  }
  iVar5 = (local_4 + param_3 * 0x18) * 8;
  _DAT_00d3b4e4 = *(int *)(&DAT_005cce98 + iVar5) + param_1;
  _DAT_00d3b4e0 = *(int *)(&DAT_005cce9c + iVar5) + param_2;
  return 1;
}

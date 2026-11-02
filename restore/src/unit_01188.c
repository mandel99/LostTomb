#include "restore_types.h"
#define DAT_005cc9e0 (*(undefined4 *)0x005cc9e0u)
#define DAT_005cce98 (*(undefined4 *)0x005cce98u)
#define DAT_005cce9c (*(undefined4 *)0x005cce9cu)
#define DAT_00a76aec (*(undefined4 *)0x00a76aecu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00ea5f1c (*(undefined4 *)0x00ea5f1cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

int FUN_00481060(int param_1,int param_2,int *param_3)

{
  uint *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int local_c;
  int local_8;
  
  iVar5 = (int)param_3;
  iVar7 = 0;
  local_c = 0xc;
  iVar9 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  iVar3 = (int)param_3 * 0x60;
  param_3 = (int *)(&DAT_005cc9e0 + iVar3);
  local_8 = iVar9;
  do {
    iVar8 = *param_3;
    if (iVar8 == 0) break;
    if (((((&DAT_00af598c)[iVar8 + iVar9] & 0x40) != 0) &&
        (((&DAT_00af598c)[iVar8 + iVar9] & 4) == 0)) && (0 < (int)(&DAT_00eed760)[iVar8 + iVar9])) {
      iVar4 = 0;
      puVar1 = &DAT_00ea5f1c + DAT_00eaac28 * 0x1276;
      do {
        iVar6 = iVar4;
        if (*puVar1 == (uint)(byte)(&DAT_00a76aec)[iVar9 + iVar8]) break;
        iVar4 = iVar4 + 1;
        puVar1 = puVar1 + 2;
        iVar6 = 0xb;
      } while (iVar4 < 10);
      if (iVar6 < local_c) {
        local_c = iVar6;
        local_8 = iVar7;
      }
    }
    iVar7 = iVar7 + 1;
    param_3 = param_3 + 1;
  } while (iVar7 < 0x18);
  if (local_c < 0xc) {
    local_8 = local_8 + iVar5 * 0x18;
    _DAT_00d3b4e4 = *(int *)(&DAT_005cce98 + local_8 * 8) + param_1;
    _DAT_00d3b4e0 = *(int *)(&DAT_005cce9c + local_8 * 8) + param_2;
    return *(int *)(&DAT_005cc9e0 + local_8 * 4) + 1 + iVar9;
  }
  iVar8 = -1;
  local_8 = 100000;
  local_c = 0;
  piVar2 = (int *)(&DAT_005cce9c + iVar5 * 0xc0);
  iVar5 = param_1;
  iVar7 = param_2;
  param_3 = (int *)(&DAT_005cc9e0 + iVar3);
  do {
    if (*param_3 == 0) break;
    iVar4 = *param_3 + iVar9;
    iVar3 = (&DAT_00eed760)[iVar4];
    if ((0 < iVar3) && (iVar3 < local_8)) {
      iVar5 = piVar2[-1] + param_1;
      iVar7 = *piVar2 + param_2;
      iVar8 = iVar4;
      local_8 = iVar3;
    }
    local_c = local_c + 1;
    param_3 = param_3 + 1;
    piVar2 = piVar2 + 2;
  } while (local_c < 0x18);
  if (iVar8 < 0) {
    return 0;
  }
  _DAT_00d3b4e4 = iVar5;
  _DAT_00d3b4e0 = iVar7;
  return iVar8 + 1;
}

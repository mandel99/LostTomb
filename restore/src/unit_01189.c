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

int FUN_00481210(int param_1,int param_2,int param_3)

{
  int iVar1;
  uint *puVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  int iVar9;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  local_c = 0xc;
  iVar9 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  local_18 = 0;
  local_14 = 0;
  local_8 = iVar9;
  do {
    iVar7 = 0;
    piVar4 = (int *)(&DAT_005cc9e0 + param_3 * 0x60);
    do {
      if (*piVar4 == 0) break;
      iVar1 = *piVar4 + local_14;
      iVar3 = iVar1 + iVar9;
      if (((*(byte *)(&DAT_00af598c + iVar3) & 0x40) != 0) && (0 < (int)(&DAT_00eed760)[iVar3])) {
        iVar3 = 0;
        puVar2 = &DAT_00ea5f1c + DAT_00eaac28 * 0x1276;
        do {
          iVar5 = iVar3;
          if (*puVar2 == (uint)(byte)(&DAT_00a76aec)[iVar9 + iVar1]) break;
          iVar3 = iVar3 + 1;
          puVar2 = puVar2 + 2;
          iVar5 = 0xb;
        } while (iVar3 < 10);
        if (iVar5 < local_c) {
          local_18 = local_14;
          local_c = iVar5;
          local_8 = iVar7;
        }
      }
      iVar7 = iVar7 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar7 < 0x18);
    local_14 = local_14 + 3;
  } while (local_14 < 7);
  if (local_c < 0xc) {
    local_8 = local_8 + param_3 * 0x18;
    _DAT_00d3b4e4 = *(int *)(&DAT_005cce98 + local_8 * 8) + local_18 + param_1;
    _DAT_00d3b4e0 = *(int *)(&DAT_005cce9c + local_8 * 8) + param_2;
    return *(int *)(&DAT_005cc9e0 + local_8 * 4) + local_18 + 1 + iVar9;
  }
  local_8 = 100000;
  local_10 = -1;
  local_18 = 0;
  iVar1 = 0;
  iVar7 = param_1;
  iVar3 = param_2;
  do {
    piVar4 = (int *)(&DAT_005cce9c + param_3 * 0xc0);
    local_c = 0;
    piVar8 = (int *)(&DAT_005cc9e0 + param_3 * 0x60);
    do {
      if (*piVar8 == 0) break;
      iVar6 = *piVar8 + iVar1 + iVar9;
      iVar5 = (&DAT_00eed760)[iVar6];
      if ((0 < iVar5) && (iVar5 < local_8)) {
        iVar7 = piVar4[-1] + iVar1 + param_1;
        iVar3 = *piVar4 + param_2;
        local_18 = iVar1;
        local_10 = iVar6;
        local_8 = iVar5;
      }
      piVar8 = piVar8 + 1;
      local_c = local_c + 1;
      piVar4 = piVar4 + 2;
    } while (local_c < 0x18);
    iVar1 = iVar1 + 3;
    if (6 < iVar1) {
      if (local_10 < 0) {
        return 0;
      }
      _DAT_00d3b4e0 = iVar3;
      _DAT_00d3b4e4 = iVar7 + local_18;
      return local_10 + 1 + local_18;
    }
  } while( true );
}

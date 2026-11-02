#include "restore_types.h"
#define DAT_01266960 (*(uint32_t *)0x01266960u)
#define _DAT_01266430 (*(undefined4 *)0x01266430u)
extern DWORD FUN_005662e3();

int FUN_0056637d(char *param_1)

{
  uint uVar1;
  uint uVar2;
  byte bVar3;
  undefined4 *puVar4;
  char *pcVar5;
  int iVar6;
  char *pcVar7;
  char *pcVar8;
  int iVar9;
  int local_c;
  int local_8;
  
  puVar4 = (undefined4 *)param_1;
  uVar1 = *(uint *)((int)param_1 + 0x10);
  if (*(int *)((int)param_1 + 4) < 0) {
    *(undefined4 *)((int)param_1 + 4) = 0;
  }
  local_8 = FUN_005662e3(uVar1,0,1);
  if (local_8 < 0) {
LAB_00566407: ;
    local_c = -1;
  }
  else {
    uVar2 = *(uint *)((int)param_1 + 0xc);
    if ((uVar2 & 0x108) == 0) {
      return local_8 - *(int *)((int)param_1 + 4);
    }
    pcVar5 = *(char **)param_1;
    pcVar7 = *(char **)((int)param_1 + 8);
    local_c = (int)pcVar5 - (int)pcVar7;
    if ((uVar2 & 3) == 0) {
      if ((uVar2 & 0x80) == 0) {
        _DAT_01266430 = 0x16;
        goto LAB_00566407;
      }
    }
    else {
      pcVar8 = pcVar7;
      if ((*(byte *)((&DAT_01266960)[(int)uVar1 >> 5] + 4 + (uVar1 & 0x1f) * 8) & 0x80) != 0) {
        for (; pcVar8 < pcVar5; pcVar8 = pcVar8 + 1) {
          if (*pcVar8 == '\n') {
            local_c = local_c + 1;
          }
        }
      }
    }
    if (local_8 != 0) {
      if ((*(byte *)((int)param_1 + 0xc) & 1) != 0) {
        if (*(int *)((int)param_1 + 4) == 0) {
          local_c = 0;
        }
        else {
          pcVar5 = pcVar5 + (*(int *)((int)param_1 + 4) - (int)pcVar7);
          iVar9 = (uVar1 & 0x1f) * 8;
          if ((*(byte *)(iVar9 + 4 + (&DAT_01266960)[(int)uVar1 >> 5]) & 0x80) != 0) {
            iVar6 = FUN_005662e3(uVar1,0,2);
            if (iVar6 == local_8) {
              pcVar7 = *(char **)((int)param_1 + 8);
              pcVar8 = pcVar7 + (int)pcVar5;
              param_1 = pcVar5;
              for (; pcVar7 < pcVar8; pcVar7 = pcVar7 + 1) {
                if (*pcVar7 == '\n') {
                  param_1 = param_1 + 1;
                }
              }
              bVar3 = *(byte *)((int)puVar4 + 0xd) & 0x20;
            }
            else {
              FUN_005662e3(uVar1,local_8,0);
              pcVar7 = (char *)0x200;
              if ((((char *)0x200 < pcVar5) || ((*(uint *)((int)param_1 + 0xc) & 8) == 0)) ||
                 ((*(uint *)((int)param_1 + 0xc) & 0x400) != 0)) {
                pcVar7 = *(char **)((int)param_1 + 0x18);
              }
              bVar3 = *(byte *)(iVar9 + 4 + (&DAT_01266960)[(int)uVar1 >> 5]) & 4;
              param_1 = pcVar7;
            }
            pcVar5 = param_1;
            if (bVar3 != 0) {
              pcVar5 = param_1 + 1;
            }
          }
          param_1 = pcVar5;
          local_8 = local_8 - (int)param_1;
        }
      }
      local_c = local_c + local_8;
    }
  }
  return local_c;
}

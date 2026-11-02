#include "restore_types.h"

int FUN_0055c650(int *param_1,byte *param_2)

{
  int iVar1;
  byte *pbVar2;
  ushort *puVar3;
  byte *pbVar4;
  int iVar5;
  ushort *puVar6;
  byte *pbVar7;
  short sVar8;
  short local_12;
  byte *local_10;
  uint local_c;
  
  iVar1 = 1;
  puVar6 = (ushort *)((int)param_1 + ((uint)param_2[1] * 5 + (uint)*param_2 * 4) * 2 + 0xdc8);
  local_c = (uint)*puVar6;
  puVar3 = (ushort *)((int)param_1 + local_c * 2 + 0x49d0);
  if ((byte *)(uint)*puVar3 < param_2 + (-(int)param_1 - param_1[6]) + -0x27cb) {
    do {
      puVar3 = puVar3 + 1;
      local_c = local_c + 1;
    } while ((byte *)(uint)*puVar3 < param_2 + (-(int)param_1 - param_1[6]) + -0x27cb);
    *puVar6 = (ushort)local_c;
  }
  pbVar4 = param_2 + -1;
  puVar6 = (ushort *)((int)param_1 + local_c * 2 + 0x49d0);
  pbVar7 = (byte *)(*puVar6 + 0x27cc + (int)param_1);
  if (pbVar4 <= pbVar7) {
    return 0;
  }
  do {
    if ((pbVar7[iVar1 + -1] == param_2[iVar1 + -1]) && (*pbVar7 == *param_2)) {
      pbVar7 = pbVar7 + 1;
      iVar5 = 2;
      pbVar2 = param_2;
      do {
        pbVar7 = pbVar7 + 1;
        if (*pbVar7 != pbVar2[2]) break;
        iVar5 = iVar5 + 1;
        pbVar2 = pbVar2 + 1;
      } while (iVar5 < 0x204);
      if ((iVar1 <= iVar5) &&
         (*param_1 = (int)(param_2 + (iVar5 - (int)pbVar7) + -1), iVar1 = iVar5, 10 < iVar5)) {
        if (iVar5 == 0x204) {
          *param_1 = *param_1 + -1;
          return 0x204;
        }
        if (pbVar4 <= (byte *)((int)param_1 +
                              *(ushort *)((int)param_1 + local_c * 2 + 0x49d2) + 0x27cc)) {
          return iVar5;
        }
        sVar8 = 0;
        *(undefined2 *)(param_1 + 0x26f) = 0xffff;
        local_12 = 1;
        *(undefined2 *)((int)param_1 + 0x9be) = 0;
        do {
          if ((param_2[local_12] == param_2[sVar8]) ||
             (sVar8 = *(short *)((int)param_1 + sVar8 * 2 + 0x9bc), sVar8 == -1)) {
            local_12 = local_12 + 1;
            sVar8 = sVar8 + 1;
            *(short *)((int)param_1 + local_12 * 2 + 0x9bc) = sVar8;
          }
        } while (local_12 < iVar5);
        local_10 = (byte *)((uint)*(ushort *)((int)param_1 + local_c * 2 + 0x49d0) + iVar5 + 0x27cc
                           + (int)param_1);
        do {
          iVar5 = (int)*(short *)((int)param_1 + iVar5 * 2 + 0x9bc);
          if (iVar5 == -1) {
            iVar5 = 0;
          }
          puVar6 = (ushort *)((int)param_1 + local_c * 2 + 0x49d0);
          do {
            puVar6 = puVar6 + 1;
            local_c = local_c + 1;
            pbVar7 = (byte *)(*puVar6 + 0x27cc + (int)param_1);
            if (pbVar4 <= pbVar7) {
              return iVar1;
            }
          } while (pbVar7 + iVar5 < local_10);
          if (pbVar7[iVar1 + -2] == param_2[iVar1 + -2]) {
            if (pbVar7 + iVar5 != local_10) {
              iVar5 = 0;
              local_10 = pbVar7;
            }
          }
          else {
            puVar6 = (ushort *)((int)param_1 + local_c * 2 + 0x49d0);
            do {
              puVar6 = puVar6 + 1;
              local_c = local_c + 1;
              pbVar7 = (byte *)(*puVar6 + 0x27cc + (int)param_1);
              if (pbVar4 <= pbVar7) {
                return iVar1;
              }
            } while ((pbVar7[iVar1 + -2] != param_2[iVar1 + -2]) || (*param_2 != *pbVar7));
            iVar5 = 2;
            local_10 = pbVar7 + 2;
          }
          if (param_2[iVar5] == *local_10) {
            do {
              iVar5 = iVar5 + 1;
              if (0x203 < iVar5) break;
              local_10 = local_10 + 1;
            } while (param_2[iVar5] == *local_10);
          }
          if ((iVar1 <= iVar5) && (*param_1 = (int)(param_2 + (-1 - (int)pbVar7)), iVar1 < iVar5)) {
            if (iVar5 == 0x204) {
              return 0x204;
            }
            do {
              if ((param_2[local_12] == param_2[sVar8]) ||
                 (sVar8 = *(short *)((int)param_1 + sVar8 * 2 + 0x9bc), sVar8 == -1)) {
                local_12 = local_12 + 1;
                sVar8 = sVar8 + 1;
                *(short *)((int)param_1 + local_12 * 2 + 0x9bc) = sVar8;
              }
              iVar1 = iVar5;
            } while (local_12 < iVar5);
          }
        } while( true );
      }
    }
    puVar6 = puVar6 + 1;
    local_c = local_c + 1;
    pbVar7 = (byte *)(*puVar6 + 0x27cc + (int)param_1);
    if (pbVar4 <= pbVar7) {
      if (iVar1 < 2) {
        iVar1 = 0;
      }
      return iVar1;
    }
  } while( true );
}

#include "restore_types.h"
extern undefined4 FUN_00407f90();
extern longlong FUN_0056133c();
extern void * FUN_00561a1d();
extern void FUN_00561a2b();

undefined4 FUN_004dd930(int param_1)

{
  float fVar1;
  byte bVar2;
  undefined2 uVar3;
  void *pvVar4;
  byte *in_ECX;
  int iVar5;
  int iVar6;
  byte *pbVar7;
  int iVar8;
  int iVar9;
  int local_50;
  int local_4c;
  int local_44;
  undefined1 auStack_34 [52];
  
  if (param_1 < 0) {
    return 0;
  }
  if (param_1 < (int)(uint)in_ECX[0x141]) {
    bVar2 = in_ECX[0x141] - 1;
    in_ECX[0x141] = bVar2;
    if (param_1 < (int)(uint)bVar2) {
      pbVar7 = in_ECX + param_1 * 6 + 8;
      do {
        param_1 = param_1 + 1;
        *(undefined4 *)pbVar7 = *(undefined4 *)(pbVar7 + 6);
        *(undefined2 *)(pbVar7 + 4) = *(undefined2 *)(pbVar7 + 10);
        pbVar7 = pbVar7 + 6;
      } while (param_1 < (int)(uint)in_ECX[0x141]);
    }
    iVar9 = 1;
    bVar2 = in_ECX[0x141];
    iVar8 = 0;
    if (1 < bVar2) {
      pbVar7 = in_ECX + 8;
      do {
        param_1 = (int)*(short *)pbVar7 - (int)*(short *)(pbVar7 + 6);
        iVar5 = (int)*(short *)(pbVar7 + 6) - (int)*(short *)pbVar7;
        if (param_1 <= iVar5) {
          param_1 = iVar5;
        }
        iVar5 = (int)*(short *)(pbVar7 + 2) - (int)*(short *)(pbVar7 + 8);
        iVar6 = (int)*(short *)(pbVar7 + 8) - (int)*(short *)(pbVar7 + 2);
        if (iVar5 <= iVar6) {
          iVar5 = iVar6;
        }
        if (iVar5 < param_1) {
          iVar8 = iVar8 + 1 + param_1;
          auStack_34[iVar9] = 1;
        }
        else {
          iVar8 = iVar8 + 1 + iVar5;
          auStack_34[iVar9] = 0;
        }
        iVar9 = iVar9 + 1;
        pbVar7 = pbVar7 + 6;
      } while (iVar9 < (int)(uint)bVar2);
    }
    if ((*(int *)(in_ECX + 0x138) < iVar8) &&
       (iVar9 = iVar8 + 0x32, *(int *)(in_ECX + 0x138) < iVar9)) {
      if (*(int *)(in_ECX + 4) != 0) {
        FUN_00561a2b(*(int *)(in_ECX + 4));
        in_ECX[4] = 0;
        in_ECX[5] = 0;
        in_ECX[6] = 0;
        in_ECX[7] = 0;
      }
      pvVar4 = FUN_00561a1d(iVar9 * 6);
      *(void **)(in_ECX + 4) = pvVar4;
      *(int *)(in_ECX + 0x138) = iVar9;
    }
    iVar9 = 0;
    *(int *)(in_ECX + 0x134) = iVar8;
    local_44 = 0;
    local_4c = 1;
    if (1 < in_ECX[0x141]) {
      pbVar7 = in_ECX + 10;
      do {
        if (*(int *)(in_ECX + 0x134) < local_44) {
          return 0;
        }
        iVar9 = 0;
        fVar1 = restore_x87_sqrt((float)((int)*(short *)(pbVar7 + 6) - (int)*(short *)pbVar7) *
                     (float)((int)*(short *)(pbVar7 + 6) - (int)*(short *)pbVar7) +
                     (float)((int)*(short *)(pbVar7 + 4) - (int)*(short *)(pbVar7 + -2)) *
                     (float)((int)*(short *)(pbVar7 + 4) - (int)*(short *)(pbVar7 + -2)));
        if (fVar1 != 0.0) {
          if (*in_ECX == 0) {
            iVar9 = 0;
          }
          else {
            local_50 = 0;
            if (0.0 <= fVar1) {
              iVar8 = local_44 * 6;
              do {
                uVar3 = FUN_0056133c();
                *(undefined2 *)(iVar8 + *(int *)(in_ECX + 4)) = uVar3;
                uVar3 = FUN_0056133c();
                *(undefined2 *)(iVar8 + 2 + *(int *)(in_ECX + 4)) = uVar3;
                *(char *)(iVar8 + 4 + *(int *)(in_ECX + 4)) = (char)local_4c + -1;
                local_50 = local_50 + (uint)*in_ECX;
                iVar9 = iVar9 + 1;
                iVar8 = iVar8 + 6;
              } while ((float)local_50 <= fVar1);
            }
          }
        }
        iVar9 = local_44 + iVar9;
        local_4c = local_4c + 1;
        pbVar7 = pbVar7 + 6;
        local_44 = iVar9;
      } while (local_4c < (int)(uint)in_ECX[0x141]);
    }
    *(int *)(in_ECX + 0x134) = iVar9;
    FUN_00407f90();
    return 1;
  }
  return 0;
}

#include "restore_types.h"
extern void FUN_005621bd();
extern void FUN_0056220b();

void FUN_00562069(uint *param_1,uint *param_2,uint param_3,code *param_4)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  uint local_fc [30];
  uint local_84 [30];
  uint local_c;
  int local_8;
  
  if ((param_2 < 2) || (param_3 == 0)) {
    return;
  }
  local_8 = 0;
  iVar4 = (int)param_2 - 1;
  param_2 = local_fc;
  uVar5 = iVar4 * param_3 + (int)param_1;
  uVar3 = (uint)param_1;
  param_1 = local_84;
LAB_005620a8: ;
  uVar2 = (uVar5 - uVar3) / param_3 + 1;
  if (8 < uVar2) {
    FUN_0056220b((uVar2 >> 1) * param_3 + uVar3,uVar3,param_3);
    uVar2 = param_3 + uVar5;
    local_c = uVar3;
LAB_005620ff: ;
    local_c = local_c + param_3;
    if (local_c <= uVar5) goto code_r0x0056210c;
    goto LAB_00562117;
  }
  FUN_005621bd(uVar3,uVar5,param_3,param_4);
  goto LAB_005620c7;
code_r0x0056210c: ;
  iVar4 = (*param_4)(local_c,uVar3);
  if (iVar4 < 1) goto LAB_005620ff;
LAB_00562117: ;
  do {
    uVar2 = uVar2 - param_3;
    if (uVar2 <= uVar3) break;
    iVar4 = (*param_4)(uVar2,uVar3);
  } while (-1 < iVar4);
  if (local_c <= uVar2) {
    FUN_0056220b(local_c,uVar2,param_3);
    goto LAB_005620ff;
  }
  FUN_0056220b(uVar3,uVar2,param_3);
  uVar1 = local_c;
  if ((int)((uVar2 - uVar3) + -1) < (int)(uVar5 - local_c)) {
    if (local_c < uVar5) {
      local_8 = local_8 + 1;
      *param_2 = local_c;
      *param_1 = uVar5;
      param_1 = param_1 + 1;
      param_2 = param_2 + 1;
    }
    if (param_3 + uVar3 < uVar2) {
      uVar5 = uVar2 - param_3;
      goto LAB_005620a8;
    }
  }
  else {
    if (uVar3 + param_3 < uVar2) {
      local_8 = local_8 + 1;
      *param_2 = uVar3;
      *param_1 = uVar2 - param_3;
      param_1 = param_1 + 1;
      param_2 = param_2 + 1;
    }
    uVar3 = uVar1;
    if (uVar1 < uVar5) goto LAB_005620a8;
  }
LAB_005620c7: ;
  local_8 = local_8 + -1;
  param_2 = param_2 + -1;
  param_1 = param_1 + -1;
  if (local_8 < 0) {
    return;
  }
  uVar5 = *param_1;
  uVar3 = *param_2;
  goto LAB_005620a8;
}

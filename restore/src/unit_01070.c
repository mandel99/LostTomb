#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
extern char FUN_00477690();
extern undefined4 FUN_00477eb0();

int FUN_004781a0(int param_1,uint param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  char cVar4;
  int iVar5;
  int *piVar6;
  int local_28;
  int local_24;
  int local_20 [8];
  
  uVar3 = param_2;
  local_24 = 0;
  cVar4 = FUN_00477eb0(param_1,param_2,1);
  if (cVar4 == '\0') {
    param_2 = 0;
    local_20[0] = -0xe4;
    local_20[1] = 1;
    local_20[2] = 0xe4;
    local_20[3] = 0xffffffff;
    local_20[4] = 0xffffff1d;
    local_20[5] = 0xe5;
    local_20[6] = 0xe3;
    local_20[7] = 0xffffff1b;
    piVar6 = local_20;
    local_28 = 8;
    do {
      iVar2 = *piVar6;
      iVar1 = iVar2 + param_1;
      iVar5 = FUN_00477690(uVar3,iVar1);
      if (((uVar3 & (&DAT_00af598c)[iVar1]) != 0) && ((int)param_2 < 8 - iVar5)) {
        param_2 = 8 - iVar5;
        local_24 = iVar2;
      }
      piVar6 = piVar6 + 1;
      local_28 = local_28 + -1;
    } while (local_28 != 0);
    return local_24;
  }
  return 0;
}

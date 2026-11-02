#include "restore_types.h"
#define DAT_005df818 (*(undefined4 *)0x005df818u)
extern undefined4 FUN_0047c880();

undefined4 FUN_004bff60(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *local_c;
  int local_8;
  
  local_c = (int *)&DAT_005df818;
  do {
    if (param_3 == 0) {
      iVar6 = local_c[1];
      iVar4 = *local_c;
LAB_004bff9e: ;
      iVar4 = iVar4 + param_2;
      iVar6 = iVar6 + param_1;
    }
    else {
      iVar4 = param_2;
      iVar6 = param_1;
      if (param_3 == 1) {
        iVar6 = *local_c;
        iVar4 = local_c[1];
        goto LAB_004bff9e;
      }
    }
    iVar1 = local_c[-1];
    local_8 = 0;
    iVar5 = iVar1;
    if (0 < iVar1) {
      do {
        iVar3 = 0;
        iVar5 = iVar4;
        do {
          if ((((char)local_c[-2] == '\0') ||
              (iVar2 = FUN_0047c880(iVar6,iVar5,1,1,0x40,0,0), iVar2 != 0)) &&
             (iVar2 = FUN_0047c880(iVar6,iVar5,1,1,0xffffffff,0,0), iVar2 == 0)) {
            return 0;
          }
          iVar3 = iVar3 + 1;
          iVar5 = iVar5 + 1;
        } while (iVar3 < iVar1);
        iVar5 = local_8 + 1;
        iVar6 = iVar6 + 1;
        local_8 = iVar5;
      } while (iVar5 < iVar1);
    }
    local_c = local_c + 4;
    if (0x5df857 < (int)local_c) {
      return CONCAT31((int3)((uint)iVar5 >> 8),1);
    }
  } while( true );
}

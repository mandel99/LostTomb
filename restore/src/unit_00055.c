#include "restore_types.h"
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
extern undefined4 FUN_004069e0();

int FUN_00406a30(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  int local_c;
  int local_8;
  int local_4;
  
  iVar3 = 100000;
  local_4 = 0;
  local_c = 200;
  local_8 = 1;
  psVar4 = &DAT_00799af0;
  do {
    if (((((char)psVar4[-8] == '\x01') && (*psVar4 == 0x4a)) && (psVar4[0x18] != 0)) &&
       (iVar1 = FUN_004069e0(local_8), iVar1 != -1)) {
      if (iVar1 < local_c) {
        iVar3 = ((int)psVar4[-5] - (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2]) *
                ((int)psVar4[-5] - (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2]) +
                ((int)psVar4[-4] - (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]) *
                ((int)psVar4[-4] - (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]);
        local_c = iVar1;
        local_4 = local_8;
      }
      else if ((iVar1 == local_c) &&
              (iVar2 = ((int)psVar4[-5] - (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2]) *
                       ((int)psVar4[-5] - (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2]) +
                       ((int)psVar4[-4] - (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]) *
                       ((int)psVar4[-4] - (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2]),
              iVar2 < iVar3)) {
        local_4 = local_8;
        iVar3 = iVar2;
        local_c = iVar1;
      }
    }
    psVar4 = psVar4 + 0x84;
    local_8 = local_8 + 1;
  } while ((int)psVar4 < 0x89b6e8);
  return local_4;
}

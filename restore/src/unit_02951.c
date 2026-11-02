#include "restore_types.h"
extern float10 FUN_00568448();

float10 FUN_00568471(uint param_1,uint param_2,int *param_3)

{
  ushort uVar1;
  int iVar2;
  bool bVar3;
  int iVar4;
  float10 fVar5;
  double local_c;
  
  if ((double)CONCAT17(RESTORE_SLICE1(param_2,3),CONCAT16(RESTORE_SLICE1(param_2,2),CONCAT24((ushort)param_2,param_1))) ==
      0.0) {
    iVar4 = 0;
    local_c = 0.0;
  }
  else if (((param_2 & 0x7ff00000) == 0) && (((param_2 & 0xfffff) != 0 || (param_1 != 0)))) {
    iVar4 = -0x3fd;
    if (0.0 <= (double)CONCAT17(RESTORE_SLICE1(param_2,3),
                                CONCAT16(RESTORE_SLICE1(param_2,2),CONCAT24((ushort)param_2,param_1)))) {
      bVar3 = false;
    }
    else {
      bVar3 = true;
    }
    while ((RESTORE_SLICE1(param_2,2) & 0x10) == 0) {
      iVar2 = CONCAT13(RESTORE_SLICE1(param_2,3),CONCAT12(RESTORE_SLICE1(param_2,2),(ushort)param_2)) << 1;
      RESTORE_SLICE2(param_2,0) = (ushort)iVar2;
      RESTORE_SLICE1(param_2,2) = (byte)((uint)iVar2 >> 0x10);
      RESTORE_SLICE1(param_2,3) = (byte)((uint)iVar2 >> 0x18);
      if ((param_1 & 0x80000000) != 0) {
        RESTORE_SLICE2(param_2,0) = (ushort)param_2 | 1;
      }
      param_1 = param_1 << 1;
      iVar4 = iVar4 + -1;
    }
    uVar1 = CONCAT11(RESTORE_SLICE1(param_2,3),RESTORE_SLICE1(param_2,2)) & 0xffef;
    RESTORE_SLICE1(param_2,2) = (byte)uVar1;
    RESTORE_SLICE1(param_2,3) = (byte)(uVar1 >> 8);
    if (bVar3) {
      RESTORE_SLICE1(param_2,3) = RESTORE_SLICE1(param_2,3) | 0x80;
    }
    fVar5 = (float10)FUN_00568448(CONCAT17(RESTORE_SLICE1(param_2,3),
                                           CONCAT16(RESTORE_SLICE1(param_2,2),CONCAT24((ushort)param_2,param_1))
                                          ),0);
    local_c = (double)fVar5;
  }
  else {
    fVar5 = (float10)FUN_00568448(CONCAT17(RESTORE_SLICE1(param_2,3),
                                           CONCAT16(RESTORE_SLICE1(param_2,2),CONCAT24((ushort)param_2,param_1))
                                          ),0);
    local_c = (double)fVar5;
    iVar4 = (short)((ushort)(param_2 >> 0x14) & 0x7ff) + -0x3fe;
  }
  *param_3 = iVar4;
  return (float10)local_c;
}

#include "restore_types.h"
#define _DAT_006052c0 (*(undefined4 *)0x006052c0u)
#define _DAT_006052d0 (*(undefined4 *)0x006052d0u)
extern float10 FUN_00568471();
extern void FUN_00568572();

bool FUN_0056820b(uint param_1,double *param_2,uint param_3)

{
  double dVar1;
  bool bVar2;
  uint uVar3;
  bool bVar4;
  float10 fVar5;
  undefined8 local_10;
  int local_8;
  
  uVar3 = param_1 & 0x1f;
  bVar2 = true;
  if (((param_1 & 8) != 0) && ((param_3 & 1) != 0)) {
    FUN_00568572(1);
    uVar3 = param_1 & 0x17;
    goto LAB_00568400;
  }
  if (((param_1 & 4) != 0) && ((param_3 & 4) != 0)) {
    FUN_00568572(4);
    uVar3 = param_1 & 0x1b;
    goto LAB_00568400;
  }
  if (((param_1 & 1) == 0) || ((param_3 & 8) == 0)) {
    if (((param_1 & 2) != 0) && ((param_3 & 0x10) != 0)) {
      bVar4 = (param_1 & 0x10) != 0;
      if (*param_2 != 0.0) {
        fVar5 = (float10)FUN_00568471(*param_2,&local_8);
        local_8 = local_8 + -0x600;
        if (local_8 < -0x432) {
          local_10 = 0.0;
          bVar4 = bVar2;
        }
        else {
          local_10 = (double)(ulonglong)
                             (SUB87((double)fVar5,0) & 0xfffffffffffff | 0x10000000000000);
          if (local_8 < -0x3fd) {
            local_8 = -0x3fd - local_8;
            do {
              if ((((ulonglong)local_10 & 1) != 0) && (!bVar4)) {
                bVar4 = bVar2;
              }
              uVar3 = (uint)local_10 >> 1;
              if (((ulonglong)local_10 & 0x100000000) != 0) {
                RESTORE_SLICE1(local_10,3) = (byte)((ulonglong)local_10 >> 0x18) >> 1;
                RESTORE_SLICE3(local_10,0) = (undefined3)uVar3;
                RESTORE_SLICE4(local_10,0) = CONCAT13(RESTORE_SLICE1(local_10,3),(undefined3)local_10) | 0x80000000;
                uVar3 = (uint)local_10;
              }
              RESTORE_SLICE4(local_10,0) = uVar3;
              local_10 = (double)CONCAT44(RESTORE_SLICE4(local_10,4) >> 1,(uint)local_10);
              local_8 = local_8 + -1;
            } while (local_8 != 0);
          }
          if ((double)fVar5 < 0.0) {
            local_10 = -local_10;
          }
        }
        *param_2 = local_10;
        bVar2 = bVar4;
      }
      if (bVar2) {
        FUN_00568572(0x10);
      }
      uVar3 = param_1 & 0x1d;
    }
    goto LAB_00568400;
  }
  FUN_00568572(8);
  uVar3 = param_3 & 0xc00;
  dVar1 = _DAT_006052c0;
  if (uVar3 == 0) {
    if (*param_2 <= 0.0) {
      dVar1 = -_DAT_006052c0;
    }
LAB_00568320: ;
    *param_2 = dVar1;
  }
  else {
    if (uVar3 == 0x400) {
      dVar1 = _DAT_006052d0;
      if (*param_2 <= 0.0) {
        dVar1 = -_DAT_006052c0;
      }
      goto LAB_00568320;
    }
    if (uVar3 == 0x800) {
      if (*param_2 <= 0.0) {
        dVar1 = -_DAT_006052d0;
      }
      goto LAB_00568320;
    }
    if (uVar3 == 0xc00) {
      dVar1 = _DAT_006052d0;
      if (*param_2 <= 0.0) {
        dVar1 = -_DAT_006052d0;
      }
      goto LAB_00568320;
    }
  }
  uVar3 = param_1 & 0x1e;
LAB_00568400: ;
  if (((param_1 & 0x10) != 0) && ((param_3 & 0x20) != 0)) {
    FUN_00568572(0x20);
    uVar3 = uVar3 & 0xffffffef;
  }
  return uVar3 == 0;
}

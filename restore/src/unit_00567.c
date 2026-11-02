#include "restore_types.h"
#define DAT_005cfcf0 (*(undefined4 *)0x005cfcf0u)
#define DAT_005d1184 (*(undefined *)0x005d1184u)
#define DAT_005d1188 (*(undefined *)0x005d1188u)
#define DAT_0070e120 (*(undefined *)0x0070e120u)
#define DAT_00799a9c (*(undefined4 *)0x00799a9cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
#define DAT_00f56be4 (*(undefined *)0x00f56be4u)
extern int FUN_0043d010();
extern int FUN_0047cbd0();
extern int FUN_0047cc70();
extern undefined4 FUN_00486a30();
extern undefined4 FUN_00486cf0();
extern void FUN_004f0ba0();
extern int FUN_0053e590();
extern undefined4 FUN_0053f250();
extern int FUN_0053f4f0();

uint FUN_0043d030(int param_1,int param_2,int param_3,uint param_4,int param_5,int param_6)

{
  int iVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  undefined1 local_39;
  int local_38;
  int local_30;
  int local_2c;
  undefined4 local_28;
  undefined4 local_24;
  uint local_20;
  uint local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  undefined4 local_8;
  int local_4;
  
  local_1c = param_4;
  local_18 = param_5;
  iVar4 = param_6 * 2;
  param_4 = param_4 & 0xffffff00;
  local_38 = 0;
  local_39 = 0;
  local_8 = 0;
  local_30 = 0;
  if (-1 < iVar4) {
    local_2c = 0;
    local_4 = iVar4;
    do {
      param_6 = -local_38;
      if (-local_38 == local_38 || param_6 < local_38) {
        local_14 = param_2 + param_3 * 0xe4;
        local_20 = param_1 + 4U & 0x80000007;
        iVar4 = local_38;
        if ((int)local_20 < 0) {
          local_20 = (local_20 - 1 | 0xfffffff8) + 1;
        }
        do {
          param_4 = local_1c;
          param_5 = local_18;
          iVar3 = param_6;
          iVar6 = local_2c;
          if (((param_1 != 0) && (iVar3 = local_30, iVar6 = iVar4, param_1 != 2)) &&
             (iVar3 = iVar4, iVar6 = local_30, param_1 != 4)) {
            iVar3 = local_2c;
            iVar6 = param_6;
          }
          iVar6 = iVar6 + param_3;
          iVar1 = iVar3 + param_2;
          FUN_004f0ba0(iVar1,iVar6,&param_4,&param_5);
          local_10 = local_14 + DAT_00bca2d8;
          iVar8 = DAT_00bca2d8 + iVar6 * 0xe4 + iVar3 + param_2;
          local_c = FUN_0047cc70(iVar1,iVar6,1,0);
          sVar2 = *(short *)(&DAT_00b4e6fc + iVar8 * 2);
          iVar3 = FUN_00486cf0(iVar8);
          uVar5 = local_20;
          if (((iVar3 != 0) && ((&DAT_00799a9c)[sVar2 * 0x42] == -1)) &&
             ((iVar8 != local_10 &&
              (*(short *)(&DAT_00b4e6fc + iVar8 * 2) != *(short *)(&DAT_00b4e6fc + local_10 * 2)))))
          {
            FUN_004f0ba0(iVar1,iVar6,&local_24,&local_28);
            uVar5 = local_20;
            DAT_0070e120 = 0x1fe3;
            iVar8 = (&DAT_00f55bb4)[DAT_005d1184 * 2] + DAT_005d1188;
            iVar3 = FUN_0043d010(local_20);
            iVar8 = iVar8 + iVar3;
            FUN_0053e590(iVar8,local_24,local_28);
            FUN_0053f250(iVar8,local_24,local_28);
            DAT_0070e120 = 0;
            local_39 = 1;
          }
          iVar3 = FUN_0047cbd0(iVar1,iVar6,0);
          if (((iVar3 * 2 - uVar5 == 0) && (local_c == 0)) &&
             (iVar3 = FUN_00486a30(iVar1,iVar6,param_2,param_3,2,iVar3 * 2,uVar5,0,&local_8),
             iVar3 != 0)) {
            piVar7 = &DAT_005cfcf0;
            do {
              FUN_0053f4f0(DAT_00f56be4,*piVar7 + param_4,piVar7[1] + param_5,0x1fe3);
              piVar7 = piVar7 + 2;
            } while ((int)piVar7 < 0x5cfd10);
            local_39 = 1;
          }
          param_6 = param_6 + 1;
          iVar4 = iVar4 + -1;
        } while (param_6 <= local_38);
        param_4 = CONCAT31((int3)((uint)param_6 >> 8),local_39);
        iVar4 = local_4;
      }
      local_38 = local_38 + 1;
      if (5 < local_38) {
        local_38 = 5;
      }
      local_30 = local_30 + 1;
      local_2c = local_2c + -1;
    } while (local_30 <= iVar4);
  }
  return param_4;
}

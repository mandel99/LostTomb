#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_005cfcf0 (*(undefined4 *)0x005cfcf0u)
#define DAT_005cfcf4 (*(undefined4 *)0x005cfcf4u)
#define DAT_005e44b4 (*(undefined4 *)0x005e44b4u)
#define DAT_005e44cc (*(undefined4 *)0x005e44ccu)
#define DAT_005e44d0 (*(undefined4 *)0x005e44d0u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
#define DAT_00bca300 (*(undefined *)0x00bca300u)
#define DAT_00bca304 (*(undefined *)0x00bca304u)
#define DAT_00bca308 (*(undefined *)0x00bca308u)
#define DAT_00f56be4 (*(undefined *)0x00f56be4u)
extern bool FUN_0046f9a0();
extern void FUN_004f0ba0();
extern int FUN_0053f4f0();

void FUN_004e5490(int param_1,int param_2)

{
  int iVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int *in_ECX;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  int *piVar13;
  int local_48;
  int local_40;
  int *local_3c;
  int *local_38;
  int local_34;
  int local_14;
  int local_10;
  int local_c;
  int iStack_8;
  int local_4;
  
  iVar1 = DAT_0070e44c;
  iVar11 = DAT_00bca2e0 / 2;
  iVar3 = DAT_0070e45c + 1;
  FUN_004f0ba0(DAT_00bca304,DAT_00bca308,&local_14,&local_10);
  iVar5 = DAT_00bca300 - DAT_00bca2d8;
  local_34 = 0;
  if (0 < param_2) {
    piVar12 = (int *)(param_1 + 0x24 + iVar11 * 4);
    piVar13 = (int *)(param_1 + 8);
    do {
      iVar11 = piVar13[-1] + iVar5 / 0xe4;
      iVar7 = piVar13[-2] + iVar5 % 0xe4;
      DAT_00bc9768 = DAT_00bca2d8 + iVar11 * 0xe4 + iVar7;
      param_1 = 0;
      if (0 < *piVar13) {
        local_48 = 0;
        local_38 = (int *)&DAT_005e44d0;
        local_4 = iVar11;
        do {
          iVar9 = 0;
          if (0 < *piVar13) {
            local_c = param_1 + iVar11;
            local_40 = 0;
            local_3c = local_38;
            do {
              iVar10 = piVar12[-4] + local_14;
              iVar11 = *piVar12 + DAT_0070e44c + local_10;
              cVar2 = FUN_0046f9a0(DAT_00bca2e8 + iVar9 + iVar7,local_c + DAT_00bca2ec,3);
              if (cVar2 != '\0') goto LAB_004e56f0;
              cVar2 = (**(code **)(*in_ECX + 0x10))
                                ((&DAT_00af598c)
                                 [(&DAT_005cc8c0)[(local_48 + iVar9) * 2] + DAT_00bc9768],local_34);
              iVar6 = 0;
              iStack_8 = (-(uint)(cVar2 != '\0') & 0xffff2780) + 0xf863;
              switch((int)(DAT_00bca2e0)) {
              case 0: ;
                iVar6 = *local_3c;
                break;
              case 2: ;
                iVar4 = *piVar13 + -1;
                iVar8 = iVar4 * iVar3;
                iVar6 = *(int *)(&DAT_005e44cc + ((*piVar13 - iVar9) + local_48) * 4);
                goto LAB_004e56b8;
              case 4: ;
                iVar6 = *(int *)(&DAT_005e44b4 + ((*piVar13 * 7 - local_40) - param_1) * 2);
                iVar11 = iVar11 + (*piVar13 + -1) * iVar1 * -2;
                break;
              case 6: ;
                iVar4 = *piVar13 + -1;
                iVar6 = *(int *)(&DAT_005e44d0 + (iVar9 + ((*piVar13 - param_1) + -1) * 6) * 4);
                iVar8 = -(iVar4 * iVar3);
LAB_004e56b8: ;
                iVar10 = iVar10 + iVar8;
                iVar11 = iVar11 - iVar4 * iVar1;
              }
              FUN_0053f4f0(DAT_00f56be4,(&DAT_005cfcf0)[iVar6 * 2] + iVar10,
                           ((&DAT_005cfcf4)[iVar6 * 2] - DAT_0070e44c) + iVar11,iStack_8);
LAB_004e56f0: ;
              iVar9 = iVar9 + 1;
              local_40 = local_40 + 6;
              local_3c = local_3c + 6;
              iVar11 = local_4;
            } while (iVar9 < *piVar13);
          }
          param_1 = param_1 + 1;
          local_38 = local_38 + 1;
          local_48 = local_48 + 6;
        } while (param_1 < *piVar13);
      }
      local_34 = local_34 + 1;
      piVar12 = piVar12 + 0xd;
      piVar13 = piVar13 + 0xd;
    } while (local_34 < param_2);
  }
  return;
}

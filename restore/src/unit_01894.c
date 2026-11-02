#include "restore_types.h"
#define DAT_005cfcd4 (*(undefined4 *)0x005cfcd4u)
#define DAT_005cfcf0 (*(undefined4 *)0x005cfcf0u)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca300 (*(undefined *)0x00bca300u)
#define DAT_00bca304 (*(undefined *)0x00bca304u)
#define DAT_00bca308 (*(undefined *)0x00bca308u)
#define DAT_00f56be4 (*(undefined *)0x00f56be4u)
extern void FUN_004ded50();
#include "city_footprint_validation.h"
extern void FUN_004f0ba0();
extern int FUN_0053f4f0();

void FUN_004debe0(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  int iVar9;
  int local_58;
  int local_50;
  int *local_4c;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  int local_38;
  undefined1 local_34 [8];
  int local_2c;
  int aiStack_20 [4];
  int aiStack_10 [4];
  
  cVar1 = FUN_004def00(DAT_00bca300);
  local_40 = (-(uint)(cVar1 != '\0') & 0xffff2780) + 0xf863;
  iVar6 = DAT_00bca2e0 / 2;
  FUN_004f0ba0(DAT_00bca304,DAT_00bca308,&local_50,&local_48);
  local_4c = aiStack_10 + iVar6;
  iVar5 = DAT_0070e45c + 1;
  local_3c = DAT_0070e44c;
  local_58 = 0;
  iVar9 = DAT_0070e44c;
  local_38 = iVar5;
  do {
    FUN_004ded50(local_34,local_58,DAT_00bca2e0,DAT_00bca2e0);
    iVar3 = aiStack_20[iVar6] + local_50;
    iVar7 = *local_4c + DAT_0070e44c + local_48;
    iVar4 = *(int *)(&DAT_005cfcd4 + local_2c * 4);
    if (1 < local_2c) {
      if (DAT_00bca2e0 == 2) {
        iVar2 = (local_2c + -1) * iVar5;
LAB_004decdf: ;
        iVar3 = iVar3 + iVar2;
        iVar7 = iVar7 - (local_2c + -1) * iVar9;
      }
      else if (DAT_00bca2e0 == 4) {
        iVar7 = iVar7 + (local_2c + -1) * iVar9 * -2;
      }
      else if (DAT_00bca2e0 == 6) {
        iVar2 = -((local_2c + -1) * iVar5);
        goto LAB_004decdf;
      }
    }
    local_44 = iVar4;
    if (0 < iVar4) {
      piVar8 = &DAT_005cfcf0;
      do {
        FUN_0053f4f0(DAT_00f56be4,*piVar8 + iVar3,(piVar8[1] - DAT_0070e44c) + iVar7,local_40);
        piVar8 = piVar8 + 2;
        iVar4 = iVar4 + -1;
        iVar5 = local_38;
        iVar9 = local_3c;
      } while (iVar4 != 0);
    }
    local_58 = local_58 + 1;
    if (0x73 < local_58) {
      return;
    }
  } while( true );
}

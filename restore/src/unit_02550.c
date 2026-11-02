#include "restore_types.h"
#define DAT_005cfcd4 (*(undefined4 *)0x005cfcd4u)
#define DAT_005cfcf0 (*(undefined4 *)0x005cfcf0u)
#define DAT_00601de0 (*(undefined *)0x00601de0u)
#define DAT_00601dfc (*(undefined4 *)0x00601dfcu)
#define DAT_0070e44c (*(undefined *)0x0070e44cu)
#define DAT_0070e45c (*(undefined *)0x0070e45cu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bca300 (*(undefined *)0x00bca300u)
#define DAT_00bca304 (*(undefined *)0x00bca304u)
#define DAT_00bca308 (*(undefined *)0x00bca308u)
#define DAT_00f56be4 (*(undefined *)0x00f56be4u)
extern void FUN_004f0ba0();
extern int FUN_0053f4f0();
extern uint FUN_00541ae0();

void FUN_00541c00(void)

{
  int iVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int *piVar10;
  int *local_20;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  cVar2 = FUN_00541ae0(DAT_00bca300);
  local_c = (-(uint)(cVar2 != '\0') & 0xffff2780) + 0xf863;
  iVar7 = DAT_00bca2e0 / 2;
  FUN_004f0ba0(DAT_00bca304,DAT_00bca308,&local_18,&local_14);
  piVar9 = &DAT_00601dfc + iVar7;
  local_20 = (int *)&DAT_00601de0;
  iVar7 = DAT_0070e45c + 1;
  local_8 = DAT_0070e44c;
  iVar4 = DAT_0070e44c;
  local_4 = iVar7;
  do {
    iVar5 = piVar9[-4] + local_18;
    iVar8 = DAT_0070e44c + local_14 + *piVar9;
    iVar1 = *local_20;
    iVar6 = *(int *)(&DAT_005cfcd4 + iVar1 * 4);
    if (1 < iVar1) {
      if (DAT_00bca2e0 == 2) {
        iVar3 = (iVar1 + -1) * iVar7;
LAB_00541cdb: ;
        iVar5 = iVar5 + iVar3;
        iVar8 = iVar8 - (iVar1 + -1) * iVar4;
      }
      else if (DAT_00bca2e0 == 4) {
        iVar8 = iVar8 + (iVar1 + -1) * iVar4 * -2;
      }
      else if (DAT_00bca2e0 == 6) {
        iVar3 = -((iVar1 + -1) * iVar7);
        goto LAB_00541cdb;
      }
    }
    if (0 < iVar6) {
      piVar10 = &DAT_005cfcf0;
      local_10 = iVar6;
      do {
        FUN_0053f4f0(DAT_00f56be4,*piVar10 + iVar5,(piVar10[1] - DAT_0070e44c) + iVar8,local_c);
        piVar10 = piVar10 + 2;
        iVar6 = iVar6 + -1;
        iVar4 = local_8;
        iVar7 = local_4;
      } while (iVar6 != 0);
    }
    piVar9 = piVar9 + 0xd;
    local_20 = local_20 + 0xd;
    if (0x60322f < (int)local_20) {
      return;
    }
  } while( true );
}

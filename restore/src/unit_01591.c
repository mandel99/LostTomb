#include "restore_types.h"
#define DAT_005e00cc (*(undefined4 *)0x005e00ccu)
#define DAT_005e00d0 (*(undefined4 *)0x005e00d0u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bc9714 (*(undefined *)0x00bc9714u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
extern uint FUN_00470900();

undefined4 FUN_004c3730(int param_1,int param_2)

{
  uint uVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int local_10;
  int local_c;
  
  iVar3 = (DAT_00bc9714 + (DAT_00bca2e0 / 2) * 4) * 8;
  iVar6 = *(int *)(&DAT_005e00cc + iVar3) + param_1;
  param_2 = *(int *)(&DAT_005e00d0 + iVar3) + param_2;
  local_c = 0;
  iVar3 = iVar6;
  param_1 = param_2;
  do {
    iVar11 = iVar6 - param_2;
    local_10 = 0;
    iVar9 = param_2 - iVar6;
    iVar8 = iVar6;
    iVar10 = param_2;
    do {
      iVar4 = iVar10;
      iVar5 = iVar3;
      if (DAT_00bca2e0 == 0) {
        iVar7 = DAT_00bc9714 * 2;
        if (iVar7 == 0) {
LAB_004c3837: ;
          iVar4 = param_1;
          iVar5 = iVar8;
        }
        else if (iVar7 != 2) {
          if (iVar7 == 4) goto LAB_004c3812;
LAB_004c382c: ;
          iVar4 = iVar9 + iVar8;
          iVar5 = param_1 + iVar11;
        }
      }
      else if (DAT_00bca2e0 == 2) {
        iVar7 = DAT_00bc9714 * 2;
        if (iVar7 == 0) {
LAB_004c37df: ;
          iVar4 = iVar9 + iVar8;
          iVar5 = param_1 + iVar11;
        }
        else {
          if (iVar7 == 2) goto LAB_004c3837;
          if (iVar7 != 4) goto LAB_004c3812;
        }
      }
      else {
        iVar7 = DAT_00bc9714 * 2;
        if (DAT_00bca2e0 == 4) {
          if (iVar7 == 0) goto LAB_004c3812;
          if (iVar7 == 2) goto LAB_004c382c;
          if (iVar7 == 4) goto LAB_004c3837;
        }
        else if (iVar7 != 0) {
          if (iVar7 != 2) {
            if (iVar7 != 4) goto LAB_004c3837;
            goto LAB_004c37df;
          }
LAB_004c3812: ;
          iVar4 = iVar3 + iVar9;
          iVar5 = iVar11 + iVar10;
        }
      }
      iVar7 = DAT_00bca2d8 + iVar4 * 0xe4 + iVar5;
      uVar1 = (&DAT_00af598c)[iVar7];
      if (*(short *)(&DAT_00b350dc + iVar7 * 2) != 0) {
        return 0xffffffff;
      }
      cVar2 = FUN_00470900(iVar5,iVar4,iVar5,iVar4);
      if (cVar2 != '\0') {
        return 0xfffffffe;
      }
      if ((uVar1 & 0xeeffd77f) != 0) {
        return 0xffffffff;
      }
      local_10 = local_10 + 1;
      iVar10 = iVar10 + -1;
      iVar8 = iVar8 + 1;
    } while (local_10 < 0xd);
    local_c = local_c + 1;
    iVar3 = iVar3 + -1;
    param_1 = param_1 + 1;
    if (6 < local_c) {
      return 1;
    }
  } while( true );
}

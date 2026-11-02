#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca304 (*(undefined *)0x00bca304u)
#define DAT_00bca308 (*(undefined *)0x00bca308u)
#define DAT_00d05720 (*(undefined4 *)0x00d05720u)
extern uint FUN_0043f250();
extern undefined1 FUN_0043f3a0();

undefined4 FUN_0043f410(int param_1)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  int iVar4;
  int iVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int local_18;
  int local_14;
  
  iVar5 = DAT_00bca308;
  iVar4 = DAT_00bca304;
  bVar2 = true;
  bVar3 = true;
  bVar1 = false;
  iVar8 = 5;
  if (param_1 == 0x1f) {
    iVar8 = 2;
  }
  else if (param_1 == 0x1e) {
    iVar8 = 3;
  }
  else if (param_1 == 0x21) {
    iVar8 = 4;
  }
  else if (param_1 == 0xd1) {
    iVar8 = 5;
  }
  local_18 = DAT_00bca308;
  local_14 = 0;
  if (DAT_00bca308 < iVar8 + DAT_00bca308) {
    iVar11 = DAT_00bca304 + DAT_00bca308 * 0xe4;
    do {
      param_1 = 0;
      iVar9 = iVar4;
      iVar10 = iVar11;
      if (iVar4 < iVar8 + iVar4) {
        do {
          cVar6 = FUN_0043f3a0(param_1,local_14,iVar8);
          if ((cVar6 != '\0') &&
             (cVar6 = FUN_0043f250(iVar9,local_18,iVar4,iVar5,iVar8), cVar6 != '\0')) {
            bVar1 = true;
          }
          iVar7 = DAT_00bca2d8 + iVar10;
          if ((*(int *)(&DAT_00d05720 + iVar7 * 4) != 0) &&
             (*(char *)(*(int *)(&DAT_00d05720 + iVar7 * 4) + 0x18) == '\0')) {
            bVar2 = false;
          }
          if ((((&DAT_00af598c)[iVar7] & 0xeeffd77f) != 0) && (((&DAT_00af598c)[iVar7] & 0x40) == 0)
             ) {
            bVar2 = false;
          }
          if (0 < *(short *)(&DAT_00b4e6fc + iVar7 * 2)) {
            bVar3 = false;
          }
          iVar9 = iVar9 + 1;
          iVar10 = iVar10 + 1;
          param_1 = param_1 + 1;
        } while (iVar9 < iVar8 + iVar4);
      }
      local_18 = local_18 + 1;
      iVar11 = iVar11 + 0xe4;
      local_14 = local_14 + 1;
    } while (local_18 < iVar8 + iVar5);
    if (((bVar1) && (bVar2)) && (bVar3)) {
      return 1;
    }
  }
  return 0;
}

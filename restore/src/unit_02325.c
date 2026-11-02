#include "restore_types.h"
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00d05720 (*(undefined4 *)0x00d05720u)
#define DAT_00eed3d0 (*(undefined4 *)0x00eed3d0u)
#define DAT_00eed75c (*(undefined4 *)0x00eed75cu)
#define DAT_00eed764 (*(undefined4 *)0x00eed764u)
#define DAT_00eedaf0 (*(undefined4 *)0x00eedaf0u)
#define DAT_00f56c34 (*(undefined *)0x00f56c34u)
extern undefined4 FUN_0051c390();

undefined4 FUN_0051c510(int param_1)

{
  bool bVar1;
  undefined4 uVar2;
  int iVar3;
  
  if (((&DAT_00af598c)[param_1] & 0x100) != 0) {
    uVar2 = FUN_0051c390(param_1);
    return uVar2;
  }
  if (*(int *)(&DAT_00d05720 + param_1 * 4) != 0) {
    if (*(char *)(*(int *)(&DAT_00d05720 + param_1 * 4) + 0x18) != '\0') {
      return 1;
    }
    iVar3 = (&DAT_00b7482c)[param_1] - DAT_00f56c34;
    if ((iVar3 == 0x34) || (iVar3 == 0x30)) {
      bVar1 = false;
    }
    else {
      if ((iVar3 != 0x36) && (iVar3 != 0x32)) {
        return 0;
      }
      bVar1 = true;
    }
    if ((DAT_00bca2e0 == 2) || (DAT_00bca2e0 == 6)) {
      bVar1 = (bool)(bVar1 ^ 1);
    }
    if (bVar1) {
      if (((((&DAT_00af55fc)[param_1 * 4] & 0x40) == 0) &&
          (*(int *)(&DAT_00eed3d0 + param_1 * 4) < 1)) &&
         (((&DAT_00af5d1c)[param_1 * 4] & 0x40) == 0)) {
        if (0 < *(int *)(&DAT_00eedaf0 + param_1 * 4)) {
          return 0;
        }
        return 1;
      }
    }
    else if ((((((&DAT_00af5990)[param_1 * 4] & 0x40) == 0) &&
              (*(int *)(&DAT_00eed764 + param_1 * 4) < 1)) &&
             (((&DAT_00af5988)[param_1 * 4] & 0x40) == 0)) &&
            (*(int *)(&DAT_00eed75c + param_1 * 4) < 1)) {
      return 1;
    }
    return 0;
  }
  return 1;
}

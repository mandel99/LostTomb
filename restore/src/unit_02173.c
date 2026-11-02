#include "restore_types.h"
#define DAT_00570070 (*(undefined4 *)0x00570070u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f3878 (*(undefined4 *)0x005f3878u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
extern undefined4 FUN_004ef100();

undefined4 FUN_004f5d10(int param_1)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  
  if (DAT_005e4ef4 != 5) {
    iVar3 = 0;
    bVar1 = *(byte *)(&DAT_00799a2c + param_1 * 0x84);
    if (0 < *(int *)(&DAT_00570070 + (uint)bVar1 * 4)) {
      do {
        iVar2 = FUN_004ef100(*(int *)(&DAT_005f3878 +
                                     ((uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 0x30 + iVar3
                                     ) * 0x14) + param_1);
        if (iVar2 != 0) {
          return 1;
        }
        iVar3 = iVar3 + 1;
      } while (iVar3 < *(int *)(&DAT_00570070 + (uint)bVar1 * 4));
    }
  }
  return 0;
}

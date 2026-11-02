#include "restore_types.h"
#define DAT_00570070 (*(undefined4 *)0x00570070u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f3878 (*(undefined4 *)0x005f3878u)
#define DAT_005f387c (*(undefined4 *)0x005f387cu)
#define DAT_005f3880 (*(undefined4 *)0x005f3880u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)

undefined4 FUN_004ef100(int param_1)

{
  uint uVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  
  if (DAT_005e4ef4 != 5) {
    iVar2 = param_1 * 0x108;
    uVar1 = (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84);
    if ((*(char *)((int)&DAT_00799a2c + iVar2 + 1) != '\x01') ||
       (*(char *)((int)&DAT_00799a2c +
                 (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x108 + 1) != '\x01')) {
      iVar4 = 0;
      if (0 < *(int *)(&DAT_00570070 + uVar1 * 4)) {
        piVar3 = (int *)(&DAT_005f3878 + uVar1 * 0x3c0);
        do {
          if ((int)(short)(&DAT_007999ea)[param_1 * 0x84] == *piVar3) {
            iVar4 = (uVar1 * 0x30 + iVar4) * 0x14;
            if (((0 < *(int *)(&DAT_005f387c + iVar4)) &&
                (*(int *)(&DAT_005f387c + iVar4) <=
                 (int)((uint)*(byte *)(&DAT_00799a2e + param_1 * 0x84) +
                      (uint)*(byte *)((int)&DAT_00799a2e + iVar2 + 1) * 6))) &&
               (*(char *)((int)&DAT_00799a30 + iVar2 + 1) == '\0')) {
              return 1;
            }
            if (*(int *)(&DAT_005f3880 + iVar4) < 1) {
              return 0;
            }
            if ((int)((uint)*(byte *)(&DAT_00799a2e + param_1 * 0x84) +
                     (uint)*(byte *)((int)&DAT_00799a2e + iVar2 + 1) * 6) <
                *(int *)(&DAT_005f3880 + iVar4)) {
              return 0;
            }
            if (*(char *)((int)&DAT_00799a30 + iVar2 + 2) != '\0') {
              return 0;
            }
            return 2;
          }
          iVar4 = iVar4 + 1;
          piVar3 = piVar3 + 5;
        } while (iVar4 < *(int *)(&DAT_00570070 + uVar1 * 4));
      }
    }
  }
  return 0;
}

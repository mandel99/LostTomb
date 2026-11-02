#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005e68d8 (*(undefined4 *)0x005e68d8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)

int FUN_004efd90(int param_1)

{
  byte bVar1;
  uint3 uVar2;
  uint uVar3;
  
  bVar1 = *(byte *)((int)&DAT_00799a2e + param_1 * 0x108 + 1);
  uVar2 = (uint3)((uint)(param_1 * 0x108) >> 8);
  if (bVar1 < *(byte *)(&DAT_00799a30 + param_1 * 0x42)) {
    if (DAT_005e4ef4 == 1) {
      uVar3 = (-(uint)((&DAT_005e68d8)
                       [(uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 6 + (uint)bVar1] != '\0')
              & 0xfffffffd) + 6;
    }
    else if ((DAT_005e4ef4 != 5) || (uVar3 = 1, (&DAT_007999ea)[param_1 * 0x84] != 0)) {
      uVar3 = 6;
    }
    if ((*(byte *)(&DAT_00799a2e + param_1 * 0x84) != uVar3) ||
       (bVar1 + 1 != (uint)*(byte *)(&DAT_00799a30 + param_1 * 0x42))) {
      return CONCAT31(uVar2,1);
    }
  }
  return (uint)uVar2 << 8;
}

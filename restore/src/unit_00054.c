#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_009783b0 (*(undefined4 *)0x009783b0u)

undefined4 FUN_004069e0(int param_1)

{
  byte bVar1;
  
  if ((&DAT_007999e8)[param_1 * 0x84] != 0x4a) {
    return 0xffffffff;
  }
  if ((&DAT_007999d8)[param_1 * 0x108] != '\x01') {
    return 0xffffffff;
  }
  bVar1 = *(byte *)((int)&DAT_00799aa0 + param_1 * 0x108 + 3);
  if (bVar1 == 0) {
    return 0xffffffff;
  }
  return *(undefined4 *)(&DAT_009783b0 + (uint)bVar1 * 0x1c);
}

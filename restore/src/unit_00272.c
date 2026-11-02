#include "restore_types.h"
#define DAT_0057c3cc (*(undefined4 *)0x0057c3ccu)
#define DAT_00ea5d10 (*(undefined4 *)0x00ea5d10u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

undefined4 FUN_0041d010(int param_1)

{
  if ((-1 < param_1) && (param_1 < 0x22)) {
    *(int *)(&DAT_00ea5d10 + DAT_00eaac28 * 0x24ec) =
         *(int *)(&DAT_00ea5d10 + DAT_00eaac28 * 0x24ec) + 1;
    (**(code **)(&DAT_0057c3cc + param_1 * 0xc))();
    return 1;
  }
  return 0;
}

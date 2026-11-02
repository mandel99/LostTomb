#include "restore_types.h"
#define DAT_00af55f8 (*(undefined4 *)0x00af55f8u)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5600 (*(undefined4 *)0x00af5600u)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d18 (*(undefined4 *)0x00af5d18u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00af5d20 (*(undefined4 *)0x00af5d20u)

undefined4 FUN_00477eb0(int param_1,uint param_2,char param_3)

{
  unsigned int offset=(unsigned int)param_1 * 4u;
  uint mask_55f8=*(uint *)(0x00af55f8u+offset);
  uint mask_55fc=*(uint *)(0x00af55fcu+offset);
  uint mask_5600=*(uint *)(0x00af5600u+offset);
  uint mask_5988=*(uint *)(0x00af5988u+offset);
  uint mask_5990=*(uint *)(0x00af5990u+offset);
  uint mask_5d18=*(uint *)(0x00af5d18u+offset);
  uint mask_5d1c=*(uint *)(0x00af5d1cu+offset);
  uint mask_5d20=*(uint *)(0x00af5d20u+offset);
  if (param_3 != '\0') {
    if ((param_2 & mask_55f8 & mask_5d20) != 0) {
      return 0;
    }
    if ((param_2 & mask_5600 & mask_5d18) != 0) {
      return 0;
    }
  }
  if (((((param_2 &
         (mask_55f8 | mask_5988 | mask_55fc)) != 0) &&
       ((param_2 &
        (mask_5600 | mask_5990 | mask_55fc)) != 0)) &&
      ((param_2 &
       (mask_5d18 | mask_5d1c | mask_5988)) != 0)) &&
     ((param_2 &
      (mask_5d20 | mask_5d1c | mask_5990)) != 0)) {
    return 0;
  }
  return 1;
}

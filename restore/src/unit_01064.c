#include "restore_types.h"
#define DAT_00af55f8 (*(undefined4 *)0x00af55f8u)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5600 (*(undefined4 *)0x00af5600u)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d18 (*(undefined4 *)0x00af5d18u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00af5d20 (*(undefined4 *)0x00af5d20u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

undefined4 FUN_00477d40(int param_1,int param_2)

{
  param_1 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  if (((((&DAT_00af598c)[param_1] & 4) != 0) && (((&DAT_00af598c)[param_1] & 0x4000000) != 0)) &&
     (((((*(uint *)(&DAT_00af55fc + param_1 * 4) & 0x4000000) == 0 ||
        ((((*(uint *)(&DAT_00af5600 + param_1 * 4) & 0x4000000) == 0 ||
          ((*(uint *)(&DAT_00af5990 + param_1 * 4) & 0x4000000) == 0)) ||
         ((*(uint *)(&DAT_00af5d20 + param_1 * 4) & 0x4000000) == 0)))) ||
       ((((*(uint *)(&DAT_00af5d1c + param_1 * 4) & 0x4000000) == 0 ||
         ((*(uint *)(&DAT_00af5d18 + param_1 * 4) & 0x4000000) == 0)) ||
        ((*(uint *)(&DAT_00af5988 + param_1 * 4) & 0x4000000) == 0)))) ||
      ((*(uint *)(&DAT_00af55f8 + param_1 * 4) & 0x4000000) == 0)))) {
    return 1;
  }
  return 0;
}

#include "restore_types.h"
#include "city_ui_controllers.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005c7538 (*(undefined1 *)0x005c7538u)
#define DAT_005c755c (*(undefined1 *)0x005c755cu)
#define DAT_005c7580 (*(undefined1 *)0x005c7580u)
#define DAT_005c75a4 (*(undefined1 *)0x005c75a4u)
#define DAT_00605a3c (*(int32_t *)0x00605a3cu)
#define DAT_00bc968c (*(int32_t *)0x00bc968cu)
#define DAT_00bc9694 (*(int32_t *)0x00bc9694u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38ebc (*(int32_t *)0x00e38ebcu)
#define DAT_00e951d8 (*(int32_t *)0x00e951d8u)
#define DAT_00e951ec (*(int32_t *)0x00e951ecu)
#define DAT_00f53da0 (*(int32_t *)0x00f53da0u)
#define DAT_00f53f64 (*(int32_t *)0x00f53f64u)
#define DAT_00f53f78 (*(int32_t *)0x00f53f78u)
#define DAT_00f56f84 (*(int32_t *)0x00f56f84u)
extern void FUN_004cce90();

void FUN_0051f680(int param_1)

{
  if ((((DAT_00605a3c != 1) && (DAT_00e38dda == '\x01')) && (DAT_00f53f78 == 0)) &&
     ((DAT_00e951ec != DAT_00f53da0 || (param_1 != 0)))) {
    DAT_00f53da0 = DAT_00e951ec;
    if ((DAT_00bc968c == 0) || (DAT_005c7538 = 1, DAT_00bc9694 == 0)) {
      DAT_005c7538 = 0;
    }
    DAT_005c755c = 0 < DAT_00e951ec;
    DAT_005c7580 = 0 < DAT_00e951d8;
    DAT_005c75a4 = DAT_00e38ebc == 0;
    if (DAT_00e951ec != 0) {
      FUN_004cce90(DAT_00e951ec,0x40,&DAT_0057c048,DAT_00f53f64 + 0x30,0x1c4,0x20,DAT_00f56f84,0);
      FUN_004cce90(DAT_00e951ec,0x40,&DAT_0057c048,DAT_00f53f64 + 0x2f,0x1c5,0x20,DAT_00f56f84,
                   0xffff);
    }
  }
  return;
}

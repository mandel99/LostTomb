#include "restore_types.h"
#include "city_ui_controllers.h"
#define DAT_005c7300 (*(undefined4 *)0x005c7300u)
#define DAT_005c7348 (*(undefined4 *)0x005c7348u)
#define DAT_005c7538 (*(undefined1 *)0x005c7538u)
#define DAT_005c75b0 (*(undefined4 *)0x005c75b0u)
#define DAT_00bc968c (*(int32_t *)0x00bc968cu)
#define DAT_00bc9694 (*(int32_t *)0x00bc9694u)
#define DAT_00f53f60 (*(int32_t *)0x00f53f60u)
#define DAT_00f53f64 (*(int32_t *)0x00f53f64u)
#define DAT_00f53f78 (*(int32_t *)0x00f53f78u)
#define PTR_DAT_005c7370 (*(unsigned char * *)0x005c7370u)
#define PTR_DAT_005c7980 (*(unsigned char * *)0x005c7980u)
extern void FUN_00425730();

void FUN_0051ec90(void)

{
  if ((DAT_00bc968c == 0) || (DAT_005c7538 = 1, DAT_00bc9694 == 0)) {
    DAT_005c7538 = 0;
  }
  if (DAT_00f53f78 == 0) {
    FUN_00425730(DAT_00f53f64,0x1e,&DAT_005c7300,2);
    FUN_00425730(DAT_00f53f64,0x1e,&PTR_DAT_005c7370,0x10);
    FUN_00425730(DAT_00f53f64,0x1e,&PTR_DAT_005c7980,2);
    return;
  }
  if (DAT_00f53f78 == 1) {
    FUN_00425730(DAT_00f53f60,0x1e,&DAT_005c7348,1);
    FUN_00425730(DAT_00f53f60,0x1e,&DAT_005c75b0,0xc);
  }
  return;
}

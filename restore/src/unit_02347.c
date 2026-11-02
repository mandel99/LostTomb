#include "restore_types.h"
#include "city_ui_controllers.h"
#define DAT_007056ec (*(int32_t *)0x007056ecu)
#define DAT_00f53f78 (*(int32_t *)0x00f53f78u)
#define DAT_00f567c4 (*(int32_t *)0x00f567c4u)
extern int FUN_0053e590();

void FUN_0051edd0(int param_1,undefined4 param_2)

{
  if ((param_1 != 0) || (DAT_00f53f78 == 0)) {
    if (DAT_007056ec == 1) {
      FUN_0053e590(DAT_00f567c4 + 1,param_2,0xd3);
      return;
    }
    if (DAT_007056ec == 3) {
      FUN_0053e590(DAT_00f567c4 + 5,param_2,0xd3);
      return;
    }
    if (DAT_007056ec == 2) {
      FUN_0053e590(DAT_00f567c4 + 9,param_2,0xd3);
      return;
    }
    if (DAT_007056ec == 0xc) {
      FUN_0053e590(DAT_00f567c4 + 2,param_2,0xd3);
      return;
    }
    if (DAT_007056ec == 10) {
      FUN_0053e590(DAT_00f567c4 + 6,param_2,0xd3);
      return;
    }
    if (DAT_007056ec == 4) {
      FUN_0053e590(DAT_00f567c4 + 10,param_2,0xd3);
      return;
    }
    if (DAT_007056ec == 8) {
      FUN_0053e590(DAT_00f567c4 + 3,param_2,0xd3);
      return;
    }
    if (DAT_007056ec == 6) {
      FUN_0053e590(DAT_00f567c4 + 7,param_2,0xd3);
      return;
    }
    if (DAT_007056ec == 7) {
      FUN_0053e590(DAT_00f567c4 + 0xb,param_2,0xd3);
      return;
    }
    if (DAT_007056ec == 5) {
      FUN_0053e590(DAT_00f567c4 + 4,param_2,0xd3);
      return;
    }
    if (DAT_007056ec == 9) {
      FUN_0053e590(DAT_00f567c4 + 8,param_2,0xd3);
      return;
    }
    if (DAT_007056ec == 0xb) {
      FUN_0053e590(DAT_00f567c4 + 0xc,param_2,0xd3);
      return;
    }
    FUN_0053e590(DAT_00f567c4,param_2,0xd3);
  }
  return;
}

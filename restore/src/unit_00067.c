#include "restore_types.h"
#define DAT_0077591c (*(undefined *)0x0077591cu)
#define DAT_00775928 (*(undefined *)0x00775928u)
#define DAT_0077f749 (*(undefined4 *)0x0077f749u)
#define DAT_0077f74c (*(undefined4 *)0x0077f74cu)
#define DAT_0077f74e (*(undefined4 *)0x0077f74eu)
#define DAT_0077f75c (*(undefined4 *)0x0077f75cu)
#define DAT_0077f75e (*(undefined4 *)0x0077f75eu)
#define DAT_00784cb4 (*(undefined *)0x00784cb4u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define _DAT_005d2a2c (*(undefined4 *)0x005d2a2cu)

void FUN_00407c10(int param_1,short param_2)

{
  if (((DAT_00e38dda == '\x15') && (DAT_00775928 == 2)) &&
     ((&DAT_0077f749)[DAT_0077591c * 0x62] != '\0')) {
    if (DAT_00784cb4 == 0) {
      if (param_1 == 0) {
        _DAT_005d2a2c = 0;
        (&DAT_0077f74e)[DAT_0077591c * 0x31] = (&DAT_0077f74e)[DAT_0077591c * 0x31] - param_2;
        return;
      }
      if (param_1 == 2) {
        _DAT_005d2a2c = 0;
        (&DAT_0077f74c)[DAT_0077591c * 0x31] = (&DAT_0077f74c)[DAT_0077591c * 0x31] + param_2;
        return;
      }
      if (param_1 == 4) {
        _DAT_005d2a2c = 0;
        (&DAT_0077f74e)[DAT_0077591c * 0x31] = (&DAT_0077f74e)[DAT_0077591c * 0x31] + param_2;
        return;
      }
      if (param_1 == 6) {
        (&DAT_0077f74c)[DAT_0077591c * 0x31] = (&DAT_0077f74c)[DAT_0077591c * 0x31] - param_2;
      }
    }
    else {
      if (param_1 == 0) {
        _DAT_005d2a2c = 0;
        (&DAT_0077f75e)[DAT_0077591c * 0x31] = (&DAT_0077f75e)[DAT_0077591c * 0x31] - param_2;
        return;
      }
      if (param_1 == 2) {
        _DAT_005d2a2c = 0;
        (&DAT_0077f75c)[DAT_0077591c * 0x31] = (&DAT_0077f75c)[DAT_0077591c * 0x31] + param_2;
        return;
      }
      if (param_1 == 4) {
        _DAT_005d2a2c = 0;
        (&DAT_0077f75e)[DAT_0077591c * 0x31] = (&DAT_0077f75e)[DAT_0077591c * 0x31] + param_2;
        return;
      }
      if (param_1 == 6) {
        _DAT_005d2a2c = 0;
        (&DAT_0077f75c)[DAT_0077591c * 0x31] = (&DAT_0077f75c)[DAT_0077591c * 0x31] - param_2;
        return;
      }
    }
    _DAT_005d2a2c = 0;
  }
  return;
}

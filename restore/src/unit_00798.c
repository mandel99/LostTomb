#include "restore_types.h"
#define DAT_005da3d4 (*(undefined4 *)0x005da3d4u)
#define DAT_0078d588 (*(undefined *)0x0078d588u)
#define DAT_0078d7c4 (*(undefined *)0x0078d7c4u)
#define DAT_0078d7e4 (*(undefined *)0x0078d7e4u)
#define DAT_0078d838 (*(undefined *)0x0078d838u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
extern void FUN_00454b40();
extern void FUN_00454d70();
extern void FUN_0046b740();
extern void FUN_00517370();
extern void FUN_00517660();

void FUN_004528b0(void)

{
  FUN_00454d70();
  DAT_0078d588 = 0;
  DAT_0078d838 = 1;
  do {
    if ((((&DAT_007999d8)[DAT_0078d838 * 0x108] == '\x01') &&
        (9 < (short)(&DAT_007999e8)[DAT_0078d838 * 0x84])) &&
       ((short)(&DAT_007999e8)[DAT_0078d838 * 0x84] < 0x1e)) {
      FUN_0046b740(DAT_0078d838);
      DAT_0078d7e4 = 0;
      (**(code **)(&DAT_005da3d4 + (short)(&DAT_007999e8)[DAT_0078d838 * 0x84] * 4))();
      if (DAT_0078d7c4 == 0) {
        FUN_00454b40(DAT_0078d838);
      }
      if (DAT_0078d7c4 == 7) {
        FUN_00454b40(DAT_0078d838);
      }
    }
    DAT_0078d838 = DAT_0078d838 + 1;
  } while (DAT_0078d838 < 4000);
  if (DAT_0078d588 != 0) {
    FUN_00517370();
    FUN_00517660();
    return;
  }
  return;
}

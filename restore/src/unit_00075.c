#include "restore_types.h"
#define DAT_00579638 (*(undefined4 *)0x00579638u)
#define DAT_005d2e88 (*(undefined *)0x005d2e88u)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_007027c6 (*(undefined4 *)0x007027c6u)
#define DAT_00775920 (*(undefined *)0x00775920u)
#define DAT_00775928 (*(undefined *)0x00775928u)
#define DAT_0078d828 (*(undefined *)0x0078d828u)
#define DAT_00e20764 (*(uint32_t *)0x00e20764u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38e68 (*(undefined *)0x00e38e68u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00f53fdc (*(undefined *)0x00f53fdcu)
#define _DAT_00d3b17c (*(undefined4 *)0x00d3b17cu)
extern void FUN_00417ae0();
extern void FUN_00442800();
extern void FUN_004cc110();
extern void FUN_004cc160();
extern void FUN_004e2970();
extern int FUN_00538cd0();

void FUN_00409fa0(void)

{
  DAT_005d2e88 = 0xffffffff;
  DAT_00f53fdc = 0;
  if (DAT_00605a3c == 1) {
    if (DAT_00e38e68 == 1) {
      FUN_004cc110(5,0x7b);
      FUN_004cc160(5,0x7c);
      FUN_00538cd0(DAT_00e20768,DAT_00e20764,0);
      DAT_00e38dca = 1;
      return;
    }
    DAT_00e38dda = 0x15;
    DAT_00775928 = 4;
    DAT_00775920 = 1;
    FUN_00442800();
    DAT_00e38dca = 1;
    return;
  }
  if ((DAT_00e38ebc == 0) && (DAT_0078d828 != 0)) {
    FUN_004e2970(0xd,0,0);
    return;
  }
  DAT_00e38dda = 0x14;
  if ((DAT_00e38ebc == 0) && ((DAT_007027c6 == '\0' && (DAT_00e38ea8 == DAT_00579638)))) {
    DAT_00e38dda = 1;
    FUN_00417ae0(0xe);
    _DAT_00d3b17c = 0x14;
  }
  DAT_00e38dca = 1;
  return;
}

#include "restore_types.h"
#define DAT_0070e460 (*(undefined *)0x0070e460u)
#define DAT_0070e464 (*(undefined *)0x0070e464u)
#define DAT_0070e494 (*(undefined4 *)0x0070e494u)
#define DAT_00bca2bc (*(undefined *)0x00bca2bcu)
#define DAT_00bca2c0 (*(undefined *)0x00bca2c0u)
#define DAT_00e94e04 (*(undefined *)0x00e94e04u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern void FUN_004304c0();
extern void FUN_00430530();

void FUN_00430b20(void)

{
  int iVar1;
  
  iVar1 = (&DAT_0070e494)
          [DAT_0070e464 / 2 + (DAT_0070e460 / 2 + DAT_00bca2c0) * 0xe7 + DAT_00bca2bc];
  DAT_00e94e04 = DAT_00e94e04 + 2;
  if (6 < DAT_00e94e04) {
    DAT_00e94e04 = 0;
  }
  FUN_00430530(DAT_00e94e04,0);
  if (iVar1 < 0xfff0000) {
    FUN_004304c0(iVar1);
    DAT_00bca2bc = _DAT_00d3b4e4 - DAT_0070e464 / 2;
    DAT_00bca2c0 = _DAT_00d3b4e0 - DAT_0070e460 / 2;
    if ((DAT_00e94e04 == 4) || (DAT_00e94e04 == 0)) {
      DAT_00bca2bc = DAT_00bca2bc + 1;
    }
  }
  return;
}

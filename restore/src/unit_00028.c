#include "restore_types.h"
#define DAT_0060596c (*(undefined4 *)0x0060596cu)
#define DAT_00605978 (*(undefined *)0x00605978u)
#define DAT_0060597c (*(undefined *)0x0060597cu)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
extern undefined4 FUN_0047d350();

void FUN_004035a0(void)

{
  if (DAT_00605978 < (int)((-(uint)(DAT_0060596c != 1) & 2) + 3)) {
    DAT_00e38dca = 1;
    DAT_0060597c = DAT_00d3b198 + DAT_0060597c * 10;
    DAT_00605978 = DAT_00605978 + 1;
    FUN_0047d350(4);
  }
  return;
}

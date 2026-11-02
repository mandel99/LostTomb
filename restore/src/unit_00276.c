#include "restore_types.h"
#define DAT_0057c560 (*(undefined4 *)0x0057c560u)
#define DAT_007027e0 (*(char *)0x007027e0u)
#define DAT_007027e4 (*(undefined *)0x007027e4u)
#define DAT_00e20998 (*(undefined *)0x00e20998u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
extern void FUN_0041d3f0();
extern int FUN_004261d0();
extern int FUN_00426450();
extern void FUN_004cc500();
extern void FUN_004e2970();

void FUN_0041d310(void)

{
  int iVar1;
  
  FUN_004cc500(7);
  DAT_007027e4 = FUN_00426450(DAT_00e38da8 + 0x32,DAT_00e38da4 + 0xf6,&DAT_0057c560,1);
  iVar1 = FUN_004261d0(DAT_00e38da8 + 0x32,DAT_00e38da4 + 0xf6,&DAT_0057c560,1);
  if ((iVar1 == 0) && (DAT_00e20998 != 0)) {
    if (DAT_007027e0 == '\0') {
      FUN_004e2970(0xd9,0,0);
    }
    DAT_007027e0 = 1;
    FUN_0041d3f0();
    return;
  }
  return;
}

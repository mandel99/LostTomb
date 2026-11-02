#include "restore_types.h"
#define DAT_007027e8 (*(undefined1 *)0x007027e8u)
#define DAT_007027ec (*(undefined4 *)0x007027ecu)
#define DAT_00e38dda (*(char *)0x00e38ddau)
extern undefined4 FUN_0041d010();
extern int FUN_0041d050();

void FUN_0041d3f0(void)

{
  undefined4 uVar1;
  
  DAT_00e38dda = DAT_007027e8;
  uVar1 = FUN_0041d050(&DAT_007027ec);
  FUN_0041d010(uVar1);
  return;
}

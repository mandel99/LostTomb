#include "restore_types.h"
#define DAT_007027e4 (*(undefined *)0x007027e4u)
#define DAT_007027ec (*(undefined4 *)0x007027ecu)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
extern void FUN_004cc540();
extern void FUN_004cc770();
extern void FUN_004ccbb0();
extern void FUN_00536820();
extern void FUN_00536a30();
extern void FUN_00536c30();
extern void FUN_00537140();

void FUN_0041d210(void)

{
  bool bVar1;
  
  FUN_00536820(DAT_00e38da8 + 10,DAT_00e38da4 + 0xa0,0x18,8);
  bVar1 = DAT_007027e4 == 1;
  FUN_00536c30(0,DAT_00e38da8 + 0x32,DAT_00e38da4 + 0xf6,0x96,0x1e,bVar1);
  FUN_004ccbb0(0xbf,1,DAT_00e38da8 + 0x37,DAT_00e38da4 + 0x100,DAT_00f56f8c,bVar1);
  FUN_00536a30(DAT_00e38da8 + 0x2a,DAT_00e38da4 + 0xd0,0x14,2);
  FUN_004cc540();
  FUN_004cc770(&DAT_007027ec,DAT_00e38da8 + 0x3a,DAT_00e38da4 + 0xd8,DAT_00f56f9c,0);
  FUN_00537140(DAT_00e38da8 + 0x3a,DAT_00e38da4 + 0xd9,0xffff);
  return;
}

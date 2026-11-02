#include "restore_types.h"
#define DAT_006fdc9c (*(undefined *)0x006fdc9cu)
#define DAT_006fdca0 (*(undefined *)0x006fdca0u)
#define DAT_006fdd20 (*(undefined4 *)0x006fdd20u)
#define DAT_006fdd24 (*(undefined4 *)0x006fdd24u)
#define DAT_006ff608 (*(undefined *)0x006ff608u)
#define DAT_00d3b568 (*(undefined *)0x00d3b568u)
#define DAT_00d3b56c (*(undefined *)0x00d3b56cu)
#define DAT_00d3b570 (*(undefined *)0x00d3b570u)
#define DAT_00d3b574 (*(undefined *)0x00d3b574u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00e38eab (*(char *)0x00e38eabu)
#define DAT_00f55c14 (*(undefined *)0x00f55c14u)
#define DAT_00f55c1c (*(undefined *)0x00f55c1cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_007027a4 (*(undefined4 *)0x007027a4u)
extern void FUN_004ccbb0();
extern void FUN_004ccfd0();
extern int FUN_0053e590();

void FUN_0041aaa0(void)

{
  int iVar1;
  
  iVar1 = (int)DAT_00e38eab;
  if (((-1 < iVar1) && (iVar1 < DAT_006ff608)) && (iVar1 = iVar1 * 0x40, iVar1 != -0x6fdd08)) {
    DAT_006fdca0 = DAT_00e38da8;
    DAT_006fdc9c = DAT_00e38da4;
    DAT_00d3b56c = (0x400 - DAT_00e38db0) / 2;
    DAT_00d3b568 = (0x300 - DAT_00e38dac) / 2;
    DAT_00d3b574 = DAT_00e38db0;
    DAT_00d3b570 = DAT_00e38dac;
    FUN_0053e590(DAT_00f55c14,0,0);
    FUN_0053e590(*(int *)(&DAT_006fdd24 + iVar1) + DAT_00f55c1c,DAT_006fdca0,DAT_006fdc9c);
    FUN_004ccbb0(0x90,*(undefined4 *)(&DAT_006fdd20 + iVar1),DAT_006fdca0 + 0x14,
                 DAT_006fdc9c + 0x19a,DAT_00f56fb4,0);
    if ((char)_DAT_007027a4 == '\0') {
      FUN_004ccfd0(0x90,0,DAT_006fdca0 + 0x14,DAT_006fdc9c + 0x1b8,0x230,DAT_00f56f8c,0);
      return;
    }
    FUN_004ccfd0(0x90,(int)*(short *)(&DAT_006fdd24 + (_DAT_007027a4 & 0xff) * 0xc + iVar1),
                 DAT_006fdca0 + 0x14,DAT_006fdc9c + 0x1b8,0x230,DAT_00f56f8c,0);
  }
  return;
}

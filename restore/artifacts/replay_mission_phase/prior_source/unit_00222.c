#include "restore_types.h"
#define DAT_007027a8 (*(undefined *)0x007027a8u)
#define DAT_00d3b124 (*(undefined *)0x00d3b124u)
#define DAT_00d3b130 (*(undefined *)0x00d3b130u)
#define DAT_00d3b138 (*(undefined *)0x00d3b138u)
extern int FUN_00418140();
extern void FUN_0041aa20();

void FUN_004181e0(void)

{
  FUN_0041aa20(0,0x35,1000);
  DAT_007027a8 = 0;
  DAT_00d3b130 = 0;
  DAT_00d3b138 = 0;
  DAT_00d3b124 = FUN_00418140();
  return;
}

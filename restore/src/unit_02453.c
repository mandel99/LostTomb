#include "restore_types.h"
#define DAT_00d3b550 (*(undefined *)0x00d3b550u)
#define DAT_00d3b554 (*(undefined *)0x00d3b554u)
#define DAT_00d3b558 (*(undefined *)0x00d3b558u)
#define DAT_00d3b55c (*(undefined *)0x00d3b55cu)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)

void FUN_005370d0(void)

{
  DAT_00d3b558 = DAT_00e38db0;
  DAT_00d3b55c = 0;
  DAT_00d3b554 = 0;
  DAT_00d3b550 = DAT_00e38dac;
  return;
}

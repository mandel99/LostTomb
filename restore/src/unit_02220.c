#include "restore_types.h"
#define DAT_005f76e8 (*(undefined4 *)0x005f76e8u)
#define DAT_0070e3e0 (*(undefined *)0x0070e3e0u)
#define DAT_0070e3e4 (*(undefined *)0x0070e3e4u)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00d3b5bc (*(undefined *)0x00d3b5bcu)
#define DAT_00d3b5be (*(undefined *)0x00d3b5beu)
#define DAT_00d3b5dc (*(undefined *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(undefined *)0x00d3b5e0u)
#define DAT_00e38d94 (*(undefined *)0x00e38d94u)
#define DAT_00e92e1c (*(uint32_t *)0x00e92e1cu)
#define DAT_00eadd24 (*(undefined4 *)0x00eadd24u)
#define DAT_00eadd40 (*(undefined *)0x00eadd40u)
#define DAT_00f567e4 (*(undefined *)0x00f567e4u)
#define DAT_010d6a0e (*(undefined4 *)0x010d6a0eu)
#define DAT_0126517c (*(undefined *)0x0126517cu)
extern void FUN_0040f9f0();
extern void FUN_004dddc0();
extern void FUN_004fa330();

void FUN_004fa3c0(void)

{
  short sVar1;
  int iVar2;
  int iVar3;
  
  if (0 < DAT_00eadd40) {
    iVar2 = DAT_00f567e4 + 8;
    if (DAT_0126517c <= iVar2) {
      iVar2 = 0;
    }
    iVar3 = 0;
    sVar1 = (&DAT_010d6a0e)[iVar2 * 0x20];
    if (0 < DAT_00eadd40) {
      do {
        FUN_004fa330(*(undefined4 *)(&DAT_00bcd618 + (&DAT_00eadd24)[iVar3] * 0x184),
                     (&DAT_00eadd24)[iVar3]);
        DAT_00d3b5e0 = DAT_0070e3e4;
        DAT_00d3b5dc = DAT_0070e3e0;
        DAT_00d3b5be = 0xc;
        DAT_00d3b5bc = 0x30;
        if (DAT_0070e3e4 < 0) {
          DAT_00d3b5e0 = 0;
        }
        if (DAT_0070e3e0 < sVar1) {
          DAT_00d3b5dc = (int)sVar1;
        }
        FUN_0040f9f0((&DAT_005f76e8)[iVar3] + DAT_00e92e1c,DAT_00e38d94 + -0x60,0);
        iVar3 = iVar3 + 1;
      } while (iVar3 < DAT_00eadd40);
    }
    FUN_004dddc0();
  }
  return;
}

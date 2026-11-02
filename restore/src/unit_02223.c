#include "restore_types.h"
#define DAT_005f76e8 (*(undefined4 *)0x005f76e8u)
#define DAT_00d3b5bc (*(undefined *)0x00d3b5bcu)
#define DAT_00d3b5be (*(undefined *)0x00d3b5beu)
#define DAT_00d3b5dc (*(undefined *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(undefined *)0x00d3b5e0u)
#define DAT_00e38d94 (*(undefined *)0x00e38d94u)
#define DAT_00e92e1c (*(uint32_t *)0x00e92e1cu)
#define DAT_00eadce4 (*(undefined *)0x00eadce4u)
#define DAT_00eadd24 (*(undefined4 *)0x00eadd24u)
#define DAT_00eadd40 (*(undefined *)0x00eadd40u)
#define DAT_00eadd48 (*(undefined *)0x00eadd48u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
extern void FUN_0040f9a0();
extern void FUN_004d6250();
extern void FUN_004d63d0();
extern void FUN_004d65b0();
extern void FUN_004fab20();

void FUN_004fa780(void)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  FUN_004d65b0(DAT_00eadd90 + 0x10,DAT_00eadd8c + 0x28,DAT_00eadd88 + -2,0xc,DAT_00eadd90,
               DAT_00eadd8c);
  if (0 < DAT_00eadd40) {
    iVar3 = 0;
    if (0 < DAT_00eadd40) {
      iVar2 = 0;
      piVar1 = &DAT_005f76e8;
      do {
        FUN_004d63d0(0,iVar2 + 0x19 + DAT_00eadd90,DAT_00eadd8c + 0x2d,0x34,0x34,
                     iVar3 == DAT_00eadce4,DAT_00eadd90,DAT_00eadd8c);
        DAT_00d3b5dc = DAT_00eadd8c + 0x2f;
        DAT_00d3b5e0 = iVar2 + 0x1b + DAT_00eadd90;
        DAT_00d3b5be = 0xc;
        DAT_00d3b5bc = 0x30;
        FUN_0040f9a0(*piVar1 + DAT_00e92e1c,DAT_00e38d94 + -0x60,0);
        iVar3 = iVar3 + 1;
        iVar2 = iVar2 + 0x3c;
        piVar1 = piVar1 + 1;
      } while (iVar3 < DAT_00eadd40);
    }
    FUN_004fab20((&DAT_00eadd24)[DAT_00eadce4]);
    DAT_00eadd48 = 1;
    return;
  }
  FUN_004d6250(0x46,0,DAT_00eadd90,DAT_00eadd8c + 0x78,DAT_00eadd88 << 4,DAT_00f56f94,0,DAT_00eadd90
               ,DAT_00eadd8c);
  DAT_00eadd48 = 1;
  return;
}

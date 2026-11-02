#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern undefined4 FUN_0047fe60();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004fa3c0();
extern void FUN_004fa780();
extern void FUN_00536820();

void FUN_0050efb0(void)

{
  int iVar1;
  
  FUN_004fa3c0();
  FUN_00536820(DAT_00eadd90,DAT_00eadd8c,DAT_00eadd88,DAT_00eadd84);
  FUN_004d6250(0x80,0,DAT_00eadd90,DAT_00eadd8c + 8,DAT_00eadd88 << 4,DAT_00f56fb4,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d6250(0xd,1,DAT_00eadd90,DAT_00eadd84 * 0x10 + -0x16 + DAT_00eadd8c,DAT_00eadd88 << 4,
               DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  FUN_004fa780();
  iVar1 = FUN_0047fe60((int)*(short *)((int)&DAT_007999dc + DAT_00eadd6c * 0x108 + 2),
                       (int)*(short *)(&DAT_007999e0 + DAT_00eadd6c * 0x108),1,2);
  if (iVar1 == 0) {
    FUN_004d6160(0x80,2,DAT_00eadd90 + 0x20,DAT_00eadd84 * 0x10 + -0x71 + DAT_00eadd8c,
                 (DAT_00eadd88 + -4) * 0x10,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
    return;
  }
  FUN_004d6160(0x80,1,DAT_00eadd90 + 0x20,DAT_00eadd84 * 0x10 + -0x71 + DAT_00eadd8c,
               (DAT_00eadd88 + -4) * 0x10,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  return;
}

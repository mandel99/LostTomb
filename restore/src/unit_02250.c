#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00ea4508 (*(undefined4 *)0x00ea4508u)
#define DAT_00ea450c (*(undefined4 *)0x00ea450cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d6860();

void FUN_00503e10(void)

{
  FUN_004d63d0(0,DAT_00eadd90 + 0x28,DAT_00eadd8c + 0x3d,400,0x19,DAT_007056c8 == 1,DAT_00eadd90,
               DAT_00eadd8c);
  DAT_00e20778 = 0;
  FUN_004d62f0(0x67,(&DAT_00ea450c)[DAT_00eaac28 * 0x1276] + 3,DAT_00eadd90 + 0x2c,
               DAT_00eadd8c + 0x41,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c);
  FUN_004d6860((&DAT_00ea4508)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,
               DAT_00e20778 + 0x2c + DAT_00eadd90,DAT_00eadd8c + 0x41,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d62f0(0x67,2,DAT_00e20778 + 0x2c + DAT_00eadd90,DAT_00eadd8c + 0x41,DAT_00f56f8c,0,
               DAT_00eadd90,DAT_00eadd8c);
  return;
}

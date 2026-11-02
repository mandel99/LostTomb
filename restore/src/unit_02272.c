#include "restore_types.h"
#define DAT_007848e2 (*(undefined *)0x007848e2u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00ea5d60 (*(undefined4 *)0x00ea5d60u)
#define DAT_00ea5d64 (*(undefined4 *)0x00ea5d64u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f55c5c (*(undefined *)0x00f55c5cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004cf090();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00516080(void)

{
  short *psVar1;
  int iVar2;
  int iVar3;
  
  FUN_00536820(0x4e,0x4e,0x22,0x14);
  iVar3 = 0;
  psVar1 = &DAT_007848e2;
  iVar2 = 100;
  do {
    if (*psVar1 == 0) {
      if (iVar3 == (&DAT_00ea5d60)[DAT_00eaac28 * 0x1276]) {
        (&DAT_00ea5d60)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5d60)[DAT_00eaac28 * 0x1276] + 1;
      }
      FUN_0053e590(DAT_00f55c5c + 0x10 + iVar3,iVar2,0x7e);
      FUN_004cf090(iVar2 + 5,0x83,0x48,0x52,0);
    }
    else if (iVar3 == (&DAT_00ea5d60)[DAT_00eaac28 * 0x1276]) {
      FUN_004d63d0(1,iVar2 + -4,0x7a,0x5a,100,1,DAT_00eadd90,DAT_00eadd8c);
      FUN_0053e590(DAT_00f55c5c + 0x15 + iVar3,iVar2,0x7e);
    }
    else {
      FUN_0053e590(DAT_00f55c5c + 0x10 + iVar3,iVar2,0x7e);
    }
    iVar2 = iVar2 + 100;
    iVar3 = iVar3 + 1;
    psVar1 = psVar1 + 1;
  } while (iVar2 < 600);
  if ((int)(&DAT_00ea5d60)[DAT_00eaac28 * 0x1276] < 5) {
    FUN_004d6250(0x3a,(&DAT_00ea5d60)[DAT_00eaac28 * 0x1276] + 0x19,0x4e,0x5a,0x220,DAT_00f56fb4,0,
                 DAT_00eadd90,DAT_00eadd8c);
  }
  DAT_00e20778 = 0;
  FUN_004d62f0(0x3a,(&DAT_00ea5d64)[DAT_00eaac28 * 0x1276] + 0x1e,0xd2,0x160,DAT_00f56f8c,0,
               DAT_00eadd90,DAT_00eadd8c);
  return;
}

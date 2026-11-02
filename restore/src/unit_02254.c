#include "restore_types.h"
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
extern int FUN_00488080();
extern bool FUN_004890f0();
extern int FUN_00489240();
extern uint FUN_004894a0();
extern void FUN_004d62f0();

void FUN_00505c40(undefined4 param_1)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  
  iVar2 = FUN_00489240(DAT_00eadd6c);
  if (-1 < iVar2) {
    DAT_00e20778 = 0;
    cVar1 = FUN_004894a0(DAT_00eadd6c);
    if (cVar1 == '\0') {
      uVar3 = 0xe;
    }
    else {
      uVar3 = 2;
      param_1 = 0xb1;
    }
    FUN_004d62f0(param_1,uVar3,DAT_00eadd90 + 0x20 + DAT_00e20778,
                 DAT_00eadd84 * 0x10 + -0x9e + DAT_00eadd8c,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c
                );
    FUN_004d62f0(0xa0,iVar2,DAT_00eadd90 + 0x20 + DAT_00e20778,
                 DAT_00eadd84 * 0x10 + -0x9e + DAT_00eadd8c,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c
                );
  }
  cVar1 = FUN_00488080(DAT_00eadd6c);
  if (cVar1 != '\0') {
    DAT_00e20778 = 0;
    cVar1 = FUN_004890f0(DAT_00eadd6c);
    if (cVar1 != '\0') {
      FUN_004d62f0(0xb1,0,DAT_00eadd90 + 0x20 + DAT_00e20778,
                   DAT_00eadd84 * 0x10 + -0x8f + DAT_00eadd8c,DAT_00f56f8c,0,DAT_00eadd90,
                   DAT_00eadd8c);
      return;
    }
    FUN_004d62f0(0xb1,1,DAT_00eadd90 + 0x20 + DAT_00e20778,
                 DAT_00eadd84 * 0x10 + -0x8f + DAT_00eadd8c,DAT_00f56f8c,0,DAT_00eadd90,DAT_00eadd8c
                );
  }
  return;
}

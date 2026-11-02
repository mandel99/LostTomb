#include "restore_types.h"
#define DAT_00d3b5dc (*(undefined *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(undefined *)0x00d3b5e0u)
#define DAT_00eadcc4 (*(undefined *)0x00eadcc4u)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f55cdc (*(undefined *)0x00f55cdcu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00eadd54 (*(undefined4 *)0x00eadd54u)
extern bool FUN_004a4300();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern char FUN_0050fc10();
extern char FUN_0050fc70();
extern void FUN_0050fe50();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_0050fca0(void)

{
  undefined4 uVar1;
  char cVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  
  uVar1 = DAT_00eadcc4;
  _DAT_00eadd54 = 0x16d;
  FUN_00536820(DAT_00eadd90,DAT_00eadd8c,DAT_00eadd88,DAT_00eadd84);
  FUN_004d6250(0xb8,1,DAT_00eadd90,DAT_00eadd8c + 10,DAT_00eadd88 << 4,DAT_00f56fb4,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d62f0(0xb8,2,DAT_00eadd90 + 100,DAT_00eadd8c + 0x3c,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  uVar3 = FUN_0050fc10(uVar1);
  FUN_004d62f0(0xb8,uVar3,DAT_00eadd90 + 300,DAT_00eadd8c + 0x3c,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d62f0(0xb8,0x1b,DAT_00eadd90 + 100,DAT_00eadd8c + 0x50,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  uVar3 = FUN_0050fc70(uVar1);
  FUN_004d62f0(0xb8,uVar3,DAT_00eadd90 + 300,DAT_00eadd8c + 0x50,DAT_00f56f8c,0,DAT_00eadd90,
               DAT_00eadd8c);
  iVar5 = 0;
  do {
    DAT_00d3b5dc = DAT_00eadd8c + 0x8d;
    DAT_00d3b5e0 = iVar5 * 0x55 + 0x15 + DAT_00eadd90;
    if (iVar5 == 4) {
      cVar2 = FUN_004a4300(uVar1);
      if (cVar2 == '\0') goto LAB_0050fe1c;
      iVar4 = DAT_00f55cdc + 5;
    }
    else {
LAB_0050fe1c: ;
      iVar4 = DAT_00f55cdc + iVar5;
    }
    FUN_0053e590(iVar4,DAT_00d3b5e0,DAT_00d3b5dc);
    iVar5 = iVar5 + 1;
    if (4 < iVar5) {
      FUN_0050fe50(uVar1);
      return;
    }
  } while( true );
}

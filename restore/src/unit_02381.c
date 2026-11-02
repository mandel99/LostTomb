#include "restore_types.h"
#define DAT_005d0320 (*(undefined4 *)0x005d0320u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea450c (*(undefined4 *)0x00ea450cu)
#define DAT_00ea5cbc (*(undefined4 *)0x00ea5cbcu)
#define DAT_00ea5d44 (*(undefined4 *)0x00ea5d44u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
#define PTR_DAT_005e03ec (*(unsigned char * *)0x005e03ecu)
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00524e20(void)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  
  FUN_00536820(DAT_00f53f0c + 0x80,DAT_00f53f08 + 0x20,0x18,0x19);
  FUN_0053e590(DAT_00f571fc + 0x15,DAT_00f53f0c + 0x90,DAT_00f53f08 + 0x30);
  FUN_004d6250(0x34,0xf,DAT_00f53f0c + 0x90,DAT_00f53f08 + 0x30,0x170,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d65b0(DAT_00f53f0c + 0x90,DAT_00f53f08 + 0x50,0x16,0xf,DAT_00e38da8,DAT_00e38da4);
  iVar4 = 0;
  puVar2 = &DAT_005d0320;
  iVar3 = 4;
  do {
    DAT_00e20778 = 0;
    uVar5 = DAT_00f56fa4;
    if (DAT_007056c8 != iVar3 + -2) {
      uVar5 = DAT_00f56f9c;
    }
    FUN_004d62f0(0x34,iVar3,DAT_00f53f0c + 0xb0,iVar4 + 0x5a + DAT_00f53f08,uVar5,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860(*puVar2,0x40,&PTR_DAT_005e03ec,DAT_00e20778 + 0xb0 + DAT_00f53f0c,
                 iVar4 + 0x5a + DAT_00f53f08,uVar5,0,DAT_00e38da8,DAT_00e38da4);
    iVar4 = iVar4 + 0x14;
    puVar2 = puVar2 + 1;
    iVar1 = iVar3 + -3;
    iVar3 = iVar3 + 1;
  } while (iVar1 < 0xb);
  if ((int)(&DAT_00ea5d44)[DAT_00eaac28 * 0x1276] < 1) {
    if ((int)(&DAT_00ea5cbc)[DAT_00eaac28 * 0x1276] < (int)(&DAT_00ea450c)[DAT_00eaac28 * 0x1276]) {
      uVar5 = 0x47;
    }
    else {
      uVar5 = 0x4c;
    }
  }
  else {
    uVar5 = 0x4d;
  }
  FUN_004d6160(0x34,uVar5,DAT_00f53f0c + 0x98,DAT_00f53f08 + 0x150,0x150,DAT_00f56f8c,0,DAT_00e38da8
               ,DAT_00e38da4);
  FUN_004d63d0(0,DAT_00f53f0c + 0xf0,DAT_00f53f08 + 0x18b,0xa0,0x14,DAT_007056c8 == 1,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0xd,4,DAT_00f53f0c + 0xb0,DAT_00f53f08 + 400,0x120,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  return;
}

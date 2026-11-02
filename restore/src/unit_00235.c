#include "restore_types.h"
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e2079a (*(undefined1 *)0x00e2079au)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dd9 (*(undefined1 *)0x00e38dd9u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ea9 (*(undefined1 *)0x00e38ea9u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00f53db4 (*(undefined *)0x00f53db4u)
#define DAT_00f53db8 (*(undefined *)0x00f53db8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
extern void FUN_004ccb30();
extern undefined4 FUN_004ea880();
extern void FUN_00536710();

void FUN_00419db0(void)

{
  undefined4 uVar1;
  int iVar2;
  
  uVar1 = DAT_00f56f8c;
  DAT_00f53db8 = DAT_00e38da8 + 0x30;
  DAT_00f53db4 = DAT_00e38da4 + 0x80;
  if (DAT_00e38dd9 == '\x01') {
    DAT_00e2079a = DAT_007056c8 == 1;
    FUN_00536710(6,DAT_00e38da8 + 0x50,DAT_00e38da4 + 0xf0,0x1e,DAT_00e2079a);
    FUN_004ccb30(0x3e,3,DAT_00f53db8 + 0x20,DAT_00f53db4 + 0x76,0x1e0,uVar1,0);
    if ((('\x01' < DAT_00e38ea8) || (DAT_00e38ebc != 0)) || (DAT_00e38ea9 != '\0')) {
      DAT_00e2079a = DAT_007056c8 == 2;
      FUN_00536710(6,DAT_00f53db8 + 0x20,DAT_00f53db4 + 0x90,0x1e,DAT_00e2079a);
      FUN_004ccb30(0x3e,4,DAT_00f53db8 + 0x20,DAT_00f53db4 + 0x96,0x1e0,uVar1,0);
      DAT_00e2079a = DAT_007056c8 == 3;
      FUN_00536710(6,DAT_00f53db8 + 0x20,DAT_00f53db4 + 0xb0,0x1e,DAT_00e2079a);
      FUN_004ccb30(0x3e,5,DAT_00f53db8 + 0x20,DAT_00f53db4 + 0xb6,0x1e0,uVar1,0);
      return;
    }
  }
  else {
    iVar2 = FUN_004ea880();
    if (iVar2 == 1) {
      DAT_00e2079a = DAT_007056c8 == 1;
      FUN_00536710(6,DAT_00f53db8 + 0x40,DAT_00f53db4 + 0x60,0x1a,DAT_00e2079a);
      FUN_004ccb30(0x3e,6,DAT_00f53db8 + 0x40,DAT_00f53db4 + 0x66,0x1a0,DAT_00f56f8c,0);
      return;
    }
    iVar2 = FUN_004ea880();
    if (iVar2 == 2) {
      DAT_00e2079a = DAT_007056c8 == 1;
      FUN_00536710(6,DAT_00f53db8 + 0x40,DAT_00f53db4 + 0x60,9,DAT_00e2079a);
      FUN_004ccb30(0x3e,6,DAT_00f53db8 + 0x40,DAT_00f53db4 + 0x66,0x90,DAT_00f56f8c,0);
      DAT_00e2079a = DAT_007056c8 == 2;
      FUN_00536710(6,DAT_00f53db8 + 0x160,DAT_00f53db4 + 0x60,9,DAT_00e2079a);
      FUN_004ccb30(0x3e,0x25,DAT_00f53db8 + 0x160,DAT_00f53db4 + 0x66,0x90,DAT_00f56f8c,0);
      return;
    }
    DAT_00e2079a = DAT_007056c8 == 1;
    FUN_00536710(6,DAT_00f53db8 + 0x20,DAT_00f53db4 + 0x60,9,DAT_00e2079a);
    FUN_004ccb30(0x3e,0x28,DAT_00f53db8 + 0x20,DAT_00f53db4 + 0x66,0x90,DAT_00f56f8c,0);
    DAT_00e2079a = DAT_007056c8 == 2;
    FUN_00536710(6,DAT_00f53db8 + 0xd0,DAT_00f53db4 + 0x60,9,DAT_00e2079a);
    FUN_004ccb30(0x3e,6,DAT_00f53db8 + 0xd0,DAT_00f53db4 + 0x66,0x90,DAT_00f56f8c,0);
    DAT_00e2079a = DAT_007056c8 == 3;
    FUN_00536710(6,DAT_00f53db8 + 0x180,DAT_00f53db4 + 0x60,9,DAT_00e2079a);
    FUN_004ccb30(0x3e,0x25,DAT_00f53db8 + 0x180,DAT_00f53db4 + 0x66,0x90,DAT_00f56f8c,0);
  }
  return;
}

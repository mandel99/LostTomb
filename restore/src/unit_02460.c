#include "restore_types.h"
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
#define DAT_007056b0 (*(undefined *)0x007056b0u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea41ec (*(undefined4 *)0x00ea41ecu)
#define DAT_00ea41f4 (*(undefined4 *)0x00ea41f4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d6860();

void FUN_00537a50(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  iVar1 = *(int *)(&DAT_00ea41ec + DAT_00eaac28 * 0x49d8);
  iVar3 = (&DAT_00ea41f4)[DAT_00eaac28 * 0x1276];
  switch((int)(iVar1)) {
  case 0x18: ;
  case 0x19: ;
  case 0x1a: ;
  case 0x1e: ;
  case 0x23: ;
    break;
default: ;
    if (((iVar1 != 0x1c) && (iVar1 != 10)) && (iVar1 != 0x24)) {
      iVar3 = iVar3 * 100;
    }
  }
  DAT_00e20778 = 0;
  FUN_004d62f0(199,4,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0xd5,DAT_00f56f9c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6860(iVar3,0x40,&DAT_007027d4,DAT_00f53f0c + 0x80 + DAT_00e20778,DAT_00f53f08 + 0xd5,
               DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
  uVar2 = DAT_00f56fa4;
  if (DAT_007056c8 != 1) {
    uVar2 = DAT_00f56f9c;
  }
  FUN_004d62f0(199,5,DAT_00f53f0c + 0x152,DAT_00f53f08 + 0xd5,uVar2,0,DAT_00e38da8,DAT_00e38da4);
  uVar2 = DAT_00f56fa4;
  if (DAT_007056b0 != 10) {
    uVar2 = DAT_00f56f9c;
  }
  FUN_004d62f0(199,8,DAT_00f53f0c + 0x152,DAT_00f53f08 + 0xee,uVar2,0,DAT_00e38da8,DAT_00e38da4);
  uVar2 = DAT_00f56fa4;
  if (DAT_007056b0 != 0xb) {
    uVar2 = DAT_00f56f9c;
  }
  FUN_004d62f0(199,9,DAT_00f53f0c + 0x152,DAT_00f53f08 + 0x107,uVar2,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(199,6,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0x155,0xf0,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d63d0(1,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0x150,0xf0,0x14,DAT_007056c8 == 2,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d63d0(1,DAT_00f53f0c + 400,DAT_00f53f08 + 0x150,0xa0,0x14,DAT_007056c8 == 3,DAT_00e38da8,
               DAT_00e38da4);
  return;
}
